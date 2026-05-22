import fs from "node:fs";
import path from "node:path";
import { truncateToWidth } from "@earendil-works/pi-tui";

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

function clamp(value, min, max) {
  return Math.max(min, Math.min(max, value));
}

function formatPercent(value) {
  return value == null || !Number.isFinite(value) ? "--" : `${value.toFixed(2)}%`;
}

function formatRatio(part, total) {
  if (!Number.isFinite(part) || !Number.isFinite(total) || total <= 0) return `${part}/--`;
  return `${part}/${total}`;
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
  let standaloneDecompUnits = 0;

  for (const line of fs.readFileSync(ldFile, "utf8").split(/\r?\n/)) {
    const match = OBJECT_RE.exec(line);
    if (!match) continue;
    const objPath = match[1];
    if (seen.has(objPath) || EXCLUDE.has(objPath)) continue;
    seen.add(objPath);
    totalUnits += 1;
    if (objPath.startsWith("build/src/")) cUnits += 1;
    if (objPath.startsWith("build/src/decomp/")) standaloneDecompUnits += 1;
  }

  const nonCUnits = totalUnits - cUnits;
  return {
    cUnits,
    nonCUnits,
    totalUnits,
    standaloneDecompUnits,
    cUnitsPercent: safePercent(cUnits, totalUnits),
    fileMtimeMs: fs.statSync(ldFile).mtimeMs,
  };
}

function readDecompCoverage(repoRoot, report, units) {
  const decompDir = path.join(repoRoot, "src", "decomp");
  if (!fs.existsSync(decompDir)) return null;

  const decompFiles = fs.readdirSync(decompDir).filter((name) => name.endsWith(".c")).length;
  const standaloneFiles = units?.standaloneDecompUnits ?? 0;
  const includedStubFiles = Math.max(0, decompFiles - standaloneFiles);
  const totalFunctions = report?.totalFunctions ?? null;

  return {
    decompFiles,
    standaloneFiles,
    includedStubFiles,
    decompFilesPercent: safePercent(decompFiles, totalFunctions),
    totalFunctions,
  };
}

function buildPlainLines(snapshot) {
  const lines = [];

  if (snapshot.report) {
    const reportPrefix = snapshot.reportFresh ? "match progress" : "match progress (cached)";
    lines.push(
      `${reportPrefix}: ${formatPercent(snapshot.report.matchedFunctionsPercent)} fn (${snapshot.report.matchedFunctions}/${snapshot.report.totalFunctions}) · ${formatPercent(snapshot.report.matchedCodePercent)} code`,
    );
  } else {
    lines.push("match progress: unavailable (run Docker make report)");
  }

  if (snapshot.decomp) {
    const percentText = formatPercent(snapshot.decomp.decompFilesPercent);
    const ratio = formatRatio(snapshot.decomp.decompFiles, snapshot.decomp.totalFunctions);
    lines.push(
      `decomp files: ${percentText} of fn total (${ratio}) · ${snapshot.decomp.standaloneFiles} standalone · ${snapshot.decomp.includedStubFiles} included_stub`,
    );
  } else {
    lines.push("decomp files: unavailable");
  }

  if (snapshot.units) {
    lines.push(
      `linked C TUs: ${formatPercent(snapshot.units.cUnitsPercent)} (${snapshot.units.cUnits}/${snapshot.units.totalUnits}) · non-C units: ${snapshot.units.nonCUnits}`,
    );
  } else {
    lines.push("linked C TUs: unavailable");
  }

  return lines;
}

function buildSignature(snapshot) {
  return JSON.stringify({
    report: snapshot.report,
    reportFresh: snapshot.reportFresh,
    units: snapshot.units,
    decomp: snapshot.decomp,
  });
}

function makeBar(theme, percent, color, width = 14) {
  if (percent == null || !Number.isFinite(percent)) {
    return theme.fg("dim", "░".repeat(width));
  }

  const filled = clamp(Math.round((percent / 100) * width), 0, width);
  return theme.fg(color, "█".repeat(filled)) + theme.fg("dim", "░".repeat(width - filled));
}

function buildMetricLine(theme, width, options) {
  const {
    label,
    labelColor,
    percent,
    barColor,
    ratio,
    detail,
    unavailableText,
  } = options;

  const labelText = theme.fg(labelColor, theme.bold(label.padEnd(6)));
  if (percent == null && unavailableText) {
    return truncateToWidth(
      `${labelText} ${theme.fg("warning", unavailableText)}`,
      width,
    );
  }

  const percentText = theme.fg(labelColor, formatPercent(percent).padStart(7));
  const bar = makeBar(theme, percent, barColor ?? labelColor);
  let line = `${labelText} ${percentText} ${bar}`;

  if (ratio) line += ` ${theme.fg("muted", ratio)}`;
  if (detail) line += ` ${theme.fg("dim", "·")} ${theme.fg("dim", detail)}`;

  return truncateToWidth(line, width);
}

function buildStyledLines(snapshot, theme, width) {
  const lines = [];
  const headerParts = [theme.fg("accent", theme.bold("◆ WarioWare Decomp"))];
  if (snapshot.report && !snapshot.reportFresh) {
    headerParts.push(theme.fg("warning", "cached report"));
  }
  lines.push(truncateToWidth(headerParts.join(` ${theme.fg("dim", "·")} `), width));

  lines.push(
    buildMetricLine(theme, width, {
      label: "match",
      labelColor: "success",
      barColor: "success",
      percent: snapshot.report?.matchedFunctionsPercent ?? null,
      ratio: snapshot.report ? `${snapshot.report.matchedFunctions}/${snapshot.report.totalFunctions} fn` : null,
      detail: snapshot.report ? `${formatPercent(snapshot.report.matchedCodePercent)} code` : null,
      unavailableText: "run Docker make report",
    }),
  );

  lines.push(
    buildMetricLine(theme, width, {
      label: "files",
      labelColor: "accent",
      barColor: "accent",
      percent: snapshot.decomp?.decompFilesPercent ?? null,
      ratio: snapshot.decomp ? `${formatRatio(snapshot.decomp.decompFiles, snapshot.decomp.totalFunctions)} fn` : null,
      detail: snapshot.decomp
        ? `${snapshot.decomp.standaloneFiles} standalone · ${snapshot.decomp.includedStubFiles} included`
        : null,
    }),
  );

  lines.push(
    buildMetricLine(theme, width, {
      label: "linked",
      labelColor: "muted",
      barColor: "warning",
      percent: snapshot.units?.cUnitsPercent ?? null,
      ratio: snapshot.units ? `${snapshot.units.cUnits}/${snapshot.units.totalUnits} TU` : null,
      detail: snapshot.units ? `${snapshot.units.nonCUnits} non-C` : null,
    }),
  );

  return lines;
}

export default function wariowareProgressWidget(pi) {
  let latestCtx;
  let refreshTimer;
  let lastSignature = "";
  let lastReport = null;

  const applyWidget = (ctx, snapshot) => {
    if (!ctx.hasUI) return;
    const signature = buildSignature(snapshot);
    if (signature === lastSignature) return;
    lastSignature = signature;

    ctx.ui.setWidget(WIDGET_KEY, (_tui, theme) => ({
      render(width) {
        return buildStyledLines(snapshot, theme, width);
      },
      invalidate() {},
    }));
  };

  const refresh = async (ctx = latestCtx) => {
    if (!ctx) return;
    latestCtx = ctx;

    const repoRoot = findRepoRoot(ctx.cwd);
    const freshReport = readReport(repoRoot);
    if (freshReport) lastReport = freshReport;

    const units = readUnitCoverage(repoRoot);
    const report = freshReport ?? lastReport;
    const snapshot = {
      report,
      reportFresh: !!freshReport,
      units,
      decomp: readDecompCoverage(repoRoot, report, units),
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
      const freshReport = readReport(repoRoot);
      if (freshReport) lastReport = freshReport;
      const report = freshReport ?? lastReport;
      const units = readUnitCoverage(repoRoot);
      const decomp = readDecompCoverage(repoRoot, report, units);
      const lines = buildPlainLines({ report, reportFresh: !!freshReport, units, decomp });
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
