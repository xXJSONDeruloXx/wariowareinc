import fs from "node:fs";
import path from "node:path";

const WIDGET_KEY = "warioware-progress";
const REFRESH_INTERVAL_MS = 15000;
const LD_SCRIPT = "wariowareinc.ld";
const REPORT_PATH = path.join("build", "report.json");
const OBJECT_RE = /^\s*(build\/\S+\.o)\(/;
const EXCLUDE = new Set([
  "build/asm/rom_header.s.o",
  "build/asm/boot.s.o",
  "build/asm/math.s.o",
  "build/asm/lib_sprite_080efc88.s.o",
]);

function safePercent(part, total) {
  if (!Number.isFinite(part) || !Number.isFinite(total) || total <= 0) return null;
  return (part / total) * 100;
}

function formatPercent(value) {
  return value == null || !Number.isFinite(value) ? "--" : `${value.toFixed(2)}%`;
}

function findRepoRoot(startDir) {
  let current = path.resolve(startDir);
  while (true) {
    if (fs.existsSync(path.join(current, LD_SCRIPT))) return current;
    const parent = path.dirname(current);
    if (parent === current) return path.resolve(startDir);
    current = parent;
  }
}

function readReport(repoRoot) {
  const reportFile = path.join(repoRoot, REPORT_PATH);
  if (!fs.existsSync(reportFile)) return null;

  const report = JSON.parse(fs.readFileSync(reportFile, "utf8"));
  const measures = report?.measures;
  if (!measures) return null;

  return {
    matchedFunctions: Number(measures.matched_functions ?? 0),
    totalFunctions: Number(measures.total_functions ?? 0),
    matchedFunctionsPercent: Number(measures.matched_functions_percent ?? 0),
    matchedCodePercent: Number(measures.matched_code_percent ?? 0),
    fileMtimeMs: fs.statSync(reportFile).mtimeMs,
  };
}

function readUnitCoverage(repoRoot) {
  const ldFile = path.join(repoRoot, LD_SCRIPT);
  if (!fs.existsSync(ldFile)) return null;

  const seen = new Set();
  let totalUnits = 0;
  let cUnits = 0;

  for (const line of fs.readFileSync(ldFile, "utf8").split(/\r?\n/)) {
    const match = OBJECT_RE.exec(line);
    if (!match) continue;
    const objPath = match[1];
    if (seen.has(objPath) || EXCLUDE.has(objPath)) continue;
    seen.add(objPath);
    totalUnits += 1;
    if (objPath.startsWith("build/src/")) cUnits += 1;
  }

  const asmUnits = totalUnits - cUnits;
  return {
    cUnits,
    asmUnits,
    totalUnits,
    cUnitsPercent: safePercent(cUnits, totalUnits),
    fileMtimeMs: fs.statSync(ldFile).mtimeMs,
  };
}

function buildLines(snapshot) {
  const lines = [];
  const reportLine = snapshot.report
    ? `decomp report: ${formatPercent(snapshot.report.matchedFunctionsPercent)} fn (${snapshot.report.matchedFunctions}/${snapshot.report.totalFunctions}) · ${formatPercent(snapshot.report.matchedCodePercent)} code`
    : "decomp report: unavailable (run make report)";
  lines.push(reportLine);

  if (snapshot.units) {
    lines.push(
      `C units: ${formatPercent(snapshot.units.cUnitsPercent)} (${snapshot.units.cUnits}/${snapshot.units.totalUnits}) · asm stubs: ${snapshot.units.asmUnits}`,
    );
  } else {
    lines.push("C units: unavailable");
  }

  return lines;
}

export default function wariowareProgressWidget(pi) {
  let latestCtx;
  let refreshTimer;
  let lastSignature = "";

  const applyWidget = (ctx, snapshot) => {
    if (!ctx.hasUI) return;
    const lines = buildLines(snapshot);
    const signature = JSON.stringify(lines);
    if (signature === lastSignature) return;
    lastSignature = signature;
    ctx.ui.setWidget(WIDGET_KEY, lines);
  };

  const refresh = async (ctx = latestCtx) => {
    if (!ctx) return;
    latestCtx = ctx;

    const repoRoot = findRepoRoot(ctx.cwd);
    const snapshot = {
      report: readReport(repoRoot),
      units: readUnitCoverage(repoRoot),
    };

    applyWidget(ctx, snapshot);
  };

  const ensureTimer = () => {
    if (refreshTimer) return;
    refreshTimer = setInterval(() => {
      void refresh();
    }, REFRESH_INTERVAL_MS);
  };

  pi.registerCommand("decomp-progress", {
    description: "Show current warioware decomp progress metrics",
    handler: async (_args, ctx) => {
      const repoRoot = findRepoRoot(ctx.cwd);
      const report = readReport(repoRoot);
      const units = readUnitCoverage(repoRoot);
      const lines = buildLines({ report, units });
      ctx.ui.notify(lines.join(" | "), "info");
    },
  });

  pi.on("session_start", async (_event, ctx) => {
    latestCtx = ctx;
    ensureTimer();
    await refresh(ctx);
  });

  pi.on("agent_end", async (_event, ctx) => {
    latestCtx = ctx;
    await refresh(ctx);
  });

  pi.on("model_select", async (_event, ctx) => {
    latestCtx = ctx;
    await refresh(ctx);
  });

  pi.on("session_shutdown", async () => {
    if (refreshTimer) {
      clearInterval(refreshTimer);
      refreshTimer = undefined;
    }
  });
}
