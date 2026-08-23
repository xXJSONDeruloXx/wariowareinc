# Decomp tooling feedback log

Use this file to record where the current decomp tools helped, where they missed integration risk, and what manual workaround was needed. The goal is to improve future automation without reducing current verification rigor or tool scope.

### Related docs
- `docs/windows-tooling-notes.md` — Windows/MSYS2/Docker path issues and fixes (added 2025-06-26)
- `.pi/extensions/warioware-decomp-loop.js` — loop prompt includes a "Documentation discipline" section that instructs the AI to record tooling issues as they're encountered

## Tooling hardening — no cheap-shot layout matches (2026-08-07)
- The sister Conker workflow confirmed the right separation: provenance records
  the candidate, hashes, diff, compiler run, and branch, while the compiler and
  final ROM identity decide acceptance. WarioWare already had the no-asm audit;
  it now also classifies layout access.
- `tools/check_decomp_policy.py` and `tools/audit_decomp_source.py` retain raw
  pointer/offset lines as evidence, but reject new offset-heavy scalar-cast
  blobs unless the source exposes a named struct/field model. Bounded accesses
  and typed overlays remain available for partially recovered GBA records.
- `decomp_cycle.py isolate/apply-batch` and the Git hooks enforce the same
  layout-quality result, so a candidate cannot pass the cheap isolated path and
  later bypass the source-quality check at apply or push. The test suite grew
  from 27 to 29 cases and covers both the rejection and named-overlay paths.
- Round 84's `func_080165D4` was rewritten from an exact raw-byte-pointer
  spelling to a named `SceneState` overlay; it remained **100% exact** in a
  fresh isolated compile. `func_08016BF0` already used a named graphics
  register overlay. m2c/asmlift remain candidate aids only: asmlift's BF0
  output was a generic pointer skeleton and was not admitted.

## Round 90 — included-stub direct-field shaping and full-context proof (2026-08-07)
- Refreshing the local Mizuchi index was useful for discovery, but its remaining
  queue is dominated by included stubs. Those conversions can improve source
  coverage without changing the matched-function report, as happened for both
  `func_0800C218` and `func_080147B0`; future 30% work must prioritize functions
  that are currently mismatching in the linked report.
- For `func_080147B0`, six readable local-overlay/order variants missed the
  target register and constant shape. The seventh spelling used the existing
  typed `gMainMenu` fields directly and reproduced the instruction sequence;
  the isolated comparison still showed a small candidate-origin relocation
  metadata gap, so it was not promoted by an instruction-only waiver.
- The guarded full-context transaction caught a real same-TU integration issue:
  the first candidate declared `func_08011698` as `s32`, conflicting with the
  host's existing `u32` declaration. The transaction rolled back and verified
  the baseline before the declaration was corrected. The rerun passed the
  Docker ROM gate and report/objdiff refresh with the expected SHA-1.
- This round is a useful no-cheap-shot proof: the accepted function has zero
  instruction asm, compiler register pins, barriers, non-mapped volatile
  accesses, raw pointer accesses, and numeric pointer-offset lines. The
  candidate's `gMainMenu` accesses are named fields, not fragile byte-pointer
  offsets. The six readable near misses remain in the receipts rather than
  being forced.

## Round 91 — standalone exact screen with decompiler cross-checks (2026-08-07)
- The queue audit separated source-coverage work from report progress: after
  Round 90 all linked C functions were exact, so the next useful candidates
  had to come from the remaining asm-only units. One 13-entry manifest kept
  discovery cheap and left the full Docker cost for the four exact winners.
- m2c produced usable skeletons for all six families probed. For the two
  `LDM R4!` bitmap walkers it emitted `s32 *var_r4` with `var_r4 += 4`; that
  expression would advance 16 bytes in C, not the target's 4-byte post-increment.
  Reading the instruction and changing the source to a typed `const u32 *`
  with `cursor++` produced exact ordinary C. This is a durable reminder that
  decompiler output is a hypothesis, not a semantic or byte-match authority.
- asmlift was useful as a diagnostic boundary: it explicitly declined both
  walkers because `ldm` has an unmodelled multi-register effect, and its
  project-header scoring failed for the scene/leaf candidates. None of its
  output was admitted. The isolation receipt selected four m2c/manual C forms:
  two typed sentinel walkers, one inverted threshold branch, and one named
  record overlay.
- The `func_08016A60`/`08016A7C` named-field variants remained at **5.5** and
  **6.23077** isolated gaps, while `func_08003FB8` remained at **40.5**. Their
  readable C sources and best receipts are retained as evidence; no barrier,
  volatile, register pin, raw numeric offset, or asm escape was added to close
  any of them.
- The exact-only batch passed the strict source audit, report/objdiff refresh,
  and clean Docker ROM gate. This moved **1691 → 1695** matched functions and
  **1231 → 1235** linked C units with unchanged ROM SHA-1.

## Round 92 — named table/ABI wrapper screen (2026-08-07)
- The short asm-only queue produced a useful **9-candidate / 3-exact / 6-near-miss** screen. m2c recovered the call/key, byte-table, sound/store, stack-forwarding, and fixed-point shapes quickly; the isolated compiler pass was enough to select exact ordinary-C spellings without paying a full ROM build for each permutation.
- The `func_08022070` m2c-style raw pointer loop was not accepted as-is. A named root overlay plus a padded 0x20-byte entry record and `entry++` preserved the byte stride while making the recovered layout explicit. The `s8` source parameter caused the first named spelling to miss; the `s32` parameter with an explicit byte store matched exactly.
- The `func_080DF224` wrapper demonstrates that the declared C ABI remains part of the evidence: `u16 arg2` reproduced the target's explicit halfword normalization, while a widened `u32` parameter with a call-site cast did not. The accepted form contains no asm or pointer-layout trick.
- asmlift again helped define boundaries rather than supply admitted code. It failed project-header scoring for the call/key and sound wrappers, declined the stack-forwarder because local stack address-taking is unsupported, and emitted diagnostic table/fixed-point forms that did not match. None of those generated forms entered the repository.
- The six near misses remain in `.nearmiss/`, `.decomp-runs/round-92-isolation.json`, and `tools/attempts.tsv`; no barrier, volatile, register pin, inline asm, or offset-heavy substitute was added to close them. The exact-only transaction passed the strict source audit, report/objdiff refresh, and clean Docker ROM gate, moving **1695 → 1698** matched functions and **1235 → 1238** linked C units with unchanged ROM SHA-1.

## Round 93 — named-record sibling screen (2026-08-07)
- The short asm-only queue was screened as a **13-entry / 5-exact / 8-near-miss** batch, with a second isolation pass for four legitimate source-shape repairs. The full-ROM cost was paid once for the five exact winners, not per candidate. m2c supplied usable skeletons for the record fields, two-entry stride, fixed-point helper, and random-range wrapper; asmlift remained a diagnostic boundary because its project-header compilation failed for this group.
- `func_08089148` became a named 0x40-byte entry walk rather than a scalar byte-pointer increment. `func_080B39F0` and `func_080CF440`/`080CF6C0` use named packed overlays. The fixed-point siblings required loading the target field into a local before the sign correction so agbcc preserved the target load order; this was an ordinary-C source-shape fix, not a register pin.
- `func_0801CB24` matched only with the target neighborhood's `s32 get_random_range(u16)` ABI declaration. The repository implementation/header declares the helper as `u16`; existing exact target-neighborhood decomp units use the same widened caller declaration, so this is recorded as an ABI provenance point rather than hidden in a cast or asm. It remains a follow-up candidate for a future project-wide prototype cleanup.
- The eight near misses remain evidence-only: the allocation and scene-store leaves differ only in their natural interworking epilogue register, the record mask's constant spelling still folds differently, and the first random-range/fixed-point spellings differ in return normalization or load order. No volatile, barrier, register pin, inline asm, or opaque offset blob was added. The exact-only transaction passed source audit, report/objdiff refresh, and the clean Docker ROM gate, moving **1698 → 1703** matched functions and **1238 → 1243** linked C units with unchanged ROM SHA-1.

## Round 94 — allocator and flag-table sibling screen (2026-08-07)
- This pass used a sibling/manual screen of **13** ordinary-C candidates and found **1 exact / 12 near miss**. `func_0800C73C` matched with a named five-halfword allocation record and direct typed stores; its full-ROM transaction passed unchanged.
- The sentinel pointer-list candidate exposed a useful constant-shape trap: writing `(u16)-1` caused agbcc to choose a literal-pool load instead of the target's `MOVS #1; RSBS` sequence. A signed local improved the hypothesis but still did not match, so the function remains evidence-only.
- The three 0x20-byte flag-table sibling pairs were kept as named entry/root overlays, but ordinary C still chose different register ownership and folded masks (`0xFE`/`0xFB`/etc.) instead of the target's `MOVS`+`RSBS` forms. This confirms the layout guard does not falsely promote readable pointer work; no register pin, barrier, volatile, or asm escape was added.
- asmlift was not run for this screen because the earlier rounds had already established the same project-context boundary for this header-heavy family; the candidates were selected from the assembly and existing sibling source evidence. The durable evidence is in `.decomp-runs/round-94-source-audit.json`, `round-94-isolation.json`, `round-94-apply.json`, `.nearmiss/`, and `tools/attempts.tsv`.

## Round 88 — included-stub host-TU lifecycle (2026-08-07)
- The next queue was predominantly included stubs, so the lifecycle was
  exercised against full host objects rather than treating a standalone
  isolated object as sufficient. `decomp_cycle.py` imported the host object's
  section-relative symbols for comparison, then `apply-batch` replaced the
  host asm include with a guarded `src/decomp` C include and paid one clean
  full-ROM gate. This path accepted `func_0800C4E0` with `wariowareinc.gba: OK`
  and unchanged SHA-1.
- `func_0800C4E0`'s first real-C spelling scored **91.7%** because the fifth
  stack argument's `u16` truncation happened too late. Moving that conversion
  into an explicit local before the coordinate call produced an exact
  ordinary-C candidate. The final source was then made semantically honest by
  returning the typed `void *` result from `func_0800C430`; a fresh comparison
  against the exact current host object and a clean full build both passed.
- `func_08011864` was a useful negative test. Five named-overlay/switch/goto
  variants were compiled, and the best reached only a **0.74074** isolated
  gap. The residual `CMP #1; BLO` versus `CMP #0; BEQ` difference is the
  documented compiler optimization trap, so the candidate stayed in the
  near-miss ledger. No register pin, barrier, volatile, or asm escape was
  introduced to close it.
- The generic target assembler cannot consume the raw `.mizuchi-asm` copy of
  an embedded stub without the host's `gba.inc` macro context. The host-object
  comparison plus the final ROM gate is therefore the correct evidence path
  for included stubs; the receipts record both the host source and include
  replacement. The 33-test suite, strict audit, policy scan, report, and
  objdiff refresh passed.

## Round 89 — refreshed included-stub ABI screen (2026-08-07)
- The locally built Mizuchi index was refreshed before screening. Its supported
  unmatched-function queue exposed the remaining included-stub candidates;
  data labels were kept out of the function screen. m2c supplied useful
  semantic skeletons, while the isolation/audit receipts—not generated text
  alone—determined whether a spelling could proceed.
- `func_0800C218` demonstrated the useful pure-C ABI pattern for this host TU:
  preserve the helper's argument order with a unique typed function pointer,
  keep all seven call parameters `s16` at the local call boundary, and use
  named `u32` temporaries for the source `u16` truncations. The v7 and v8
  spellings were both exact in host-TU isolation; v7 was chosen because it
  returns the helper's real `void *` value. The full transaction passed without
  any asm, register pin, barrier, volatile shaping, or opaque offset layout.
- `func_0800C3AC` stayed at a best **13.046875** isolated gap across its
  readable variants. `func_0800DE84` reached **0.34884** with equal instruction
  sequences but literal-pool/relocation metadata differences; it was retained
  as evidence rather than promoted through a metadata waiver. This keeps the
  byte-match policy stricter than an instruction-only near miss.
- The c218 apply receipt (`.decomp-runs/round-89-c218-apply.json`) records the
  clean Docker result, report refresh, and unchanged ROM SHA-1. The round's
  candidate audits, isolation receipts, near-miss sources, and attempt-ledger
  rows are committed together so later runs can reproduce the decision.

## Round 87 — named-overlay standalone screen (2026-08-07)
- The screen evaluated **12** fresh standalone candidates in one isolation
  container and classified **8 exact / 4 near miss**. Only the eight exact
  inputs entered `apply-batch`; the full-context Docker transaction ran once
  for that exact subset and passed `wariowareinc.gba: OK` with unchanged ROM
  SHA-1. The report advanced **1683 → 1691** functions, **1223 → 1231** linked
  C TUs, and **1407 → 1415** decomp files.
- m2c supplied useful semantic skeletons. The accepted forms were then
  manually reshaped into named input/output, scene, scene-data, table, and
  sprite-record fields with explicit padding. asmlift was not needed for this
  screen, and no generated decompiler output was admitted as source without
  the compiler comparison.
- The strict source/layout receipt reports zero instruction asm, empty
  barriers, compiler register pins, non-mapped volatile accesses, raw pointer
  accesses, and numeric pointer-offset lines in all eight accepted files.
  The one `func_080D70EC` handler alias is an API parameter; it is not used as
  a scalar pointer to reach hidden record fields. This is the intended
  boundary for partially recovered records: named fields and visible gaps are
  acceptable evidence, while an offset-heavy `u8 *` blob is not.
- The four rejected seeds were retained rather than padded or forced:
  `func_080D906C` (**39.9375**), `func_08040AAC` (**8.066666**),
  `func_080B2450` (**7.117645**), and `func_08082BB0` (**0.125**). Their
  candidate sources, normalized comparison results, and attempt rows remain
  available in `.nearmiss/`, `.decomp-runs/round-87-isolation-v1.json`, and
  `tools/attempts.tsv`.
- The single exact-only full gate was the useful cost improvement: discovery
  and near-miss recording stayed in the shared screen, while only the
  accepted batch paid the clean full-ROM build/report cost. The 33-test tools
  suite, strict audit, policy scan, report, and objdiff refresh all passed.

## Round 85 — six-function strict-C screen (2026-08-07)
- The shared screen covered **15** candidate entries and classified **6 exact / 9 near miss**. Only the six exact entries entered `apply-batch`, so the expensive full-context Docker build ran once for the accepted subset rather than once per spelling. The gate passed `wariowareinc.gba: OK`, preserving SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3` and advancing **1676 → 1682** functions, **1216 → 1222** linked C TUs, and **1400 → 1406** decomp files.
- m2c was the fastest semantic source for the sprite/gameplay/scene wrappers. asmlift was still useful as a diagnostic boundary, but it declined stack-pointer/data shapes or hit project-compile failures; no asmlift-generated C was admitted. The compiler isolation receipt remained the authority for exactness.
- The layout-quality guard worked as intended. A named `GraphicsInitRegisters` overlay recovered the `0x48` halfword while preserving the known `0x4C` field, and an inline named `SceneVariableRoot` lvalue preserved the target's post-call global reload. Both passed `--strict-layout`; neither is a large raw pointer-offset stand-in.
- The DMA siblings are a valuable negative result: after reducing the source/destination conversions to bounded ordinary C, the candidates passed policy but still emitted a signed branch and changed register/literal order. The mask-clear helper likewise folded `MOVS #2; RSBS` into `0xFE`. These remain near-miss receipts; no mismatched prototype, barrier, volatile trick, or instruction asm was used to manufacture a match.
- Post-apply `make report`, `gen_objdiff.py`, the strict source audit, policy scan, and all **29** unit tests passed. The six accepted files contain zero instruction asm, barriers, register pins, or opaque layouts. Receipts: `.decomp-runs/round-85-isolation-v1.json` through `round-85-isolation-v14.json`, `round-85-source-audit.json`, and `round-85-apply.json`.

## Round 84 — overlay-only exact batch (2026-08-07)
- The five-candidate title/main-menu screen initially had **2 exact / 2
  compile errors / 1 near miss**. After header/prototype repairs it had **2
  exact / 3 near miss**. The exact subset was then re-screened after the
  layout-quality policy commit and remained **2 exact / 0 rejected**.
- `func_080165D4` first matched with four explicit scene offsets. Replacing
  that opaque byte-pointer view with a named `SceneState` overlay preserved the
  exact object bytes, proving the stricter source shape did not cost progress.
  `func_08016BF0` was exact through a named register overlay from the start.
- The transactional apply passed the full Docker gate from **11:49:15 to
  11:50:29 UTC**, with `wariowareinc.gba: OK` and unchanged SHA-1
  `3f556448d290fa5406d6ed367fee16cc02387ad3`. Fresh metrics advanced **1674
  → 1676** functions, **1214 → 1216** linked C units, **1398 → 1400** decomp
  files, and **75056 / 993772 → 75264 / 993780** matched code.
- m2c supplied usable dispatcher/register skeletons. asmlift declined the
  loop-heavy dispatcher and generated a generic pointer BF0 skeleton; neither
  generated output was accepted. The final sources are pure named-field C with
  no asm, pins, barriers, or raw pointer-offset evidence.

## Round 83 — strict ordinary-C scene/main-menu fan-in (2026-08-07)
- One shared isolation container screened four m2c/manual candidates. v1 found **1 exact / 2 compile errors / 1 exact**; adding `graphics.h` for the transitive `struct Animation` declaration produced v2 with **3 exact / 1 near miss**. The remaining `func_08016CBC` miss differed only in its ordinary-C stack frame (`SUB/ADD SP,#8` versus the target's `#0x10`); modeling the local subscene pointer array as four entries repaired the frame without asm, barriers, or register pins. v3 reached **4 exact / 0 rejected**.
- m2c was useful for recovering the beatscript bootstrap's argument/stack shape and the state-dispatch skeletons. asmlift again served as a project-context diagnostic and did not produce an admitted source. The compiler isolation receipt, not decompiler output, selected the final spellings.
- The full-context exact-only transaction passed the Docker ROM gate and advanced **1670 → 1674** matched functions, **1210 → 1214** linked C TUs, and **1394 → 1398** decomp files. `make report`, `gen_objdiff.py`, strict source audit, policy checks, and the 27-tool-test suite remain required post-apply checks; ROM SHA-1 stayed `3f556448d290fa5406d6ed367fee16cc02387ad3` with `wariowareinc.gba: OK`.
- The strict audit recorded no asm, barriers, or register pins. Two accepted bodies use explicit packed scene offsets, and the audit preserves those accesses as reviewable provenance. This is the desired boundary: the C is readable and compiler-independent, while unresolved struct layout remains visible instead of being hidden in a wrapper.

## Tooling maintenance — named-symbol manifest addresses (2026-08-06)
- `decomp_cycle.py` originally derived every standalone source and linker path
  from a `func_XXXXXXXX`/`asm_XXXXXXXX` function name. That rejected valid raw
  symbols such as `set_soundplayer_pitch` before isolation could begin.
- Manifests now accept an explicit eight-digit `address` override, validate it,
  and preserve the descriptive symbol for target-object entry selection. A
  regression test covers the derived source and linker paths.
- This is discovery plumbing only: the candidate still has to pass isolated
  objdiff and the transactional full-ROM gate. It does not relax exact-only
  admission.

## Round 71 — named-symbol and scene-wrapper fan-in (2026-08-06)
- m2c supplied compact skeletons for two named sound-player wrappers, two
  scene/sprite wrappers, and a heap-record cleanup helper. The first screen
  used one Docker isolation invocation and classified **0 exact / 4 near miss /
  1 compile error**; the repaired screen reached **2 exact / 3 near miss**.
- The explicit-address manifest support was necessary for the exported
  `set_soundplayer_pitch`/`set_soundplayer_volume` symbols. The screen also
  exposed that the sprite wrapper needed the handler global loaded before the
  scene-data global, while the cleanup candidate needed a local `extern`
  declaration to avoid the project's conflicting heap-header prototype.
- `func_0800E800` and `func_08004EAC` were the only candidates admitted. The
  apply reused the v3 receipt and ran one clean full Docker ROM/report gate;
  it passed with the unchanged SHA-1. The three near misses remain durable
  seeds rather than source changes.

### Round 71 boundary follow-up
- The two named sound-player candidates compiled to complete `.text` sections
  that were byte-identical to their raw targets, but linked objdiff inferred
  the target function size only through a local branch label. A separate
  Docker audit recorded exact 28-byte/24-byte sections and matching SHA-256
  hashes in `round-71-boundary-audit.json`.
- Only after that audit did the narrow `--force` path admit them. The full
  linked ROM gate passed, confirming that the metadata exception did not hide
  a relocation or integration mismatch. This remains distinct from an
  ordinary compiler near miss, which stays evidence-only.

## Round 73 — exact scene-wrapper apply and literal-pool near miss (2026-08-06)
- The live loop now cleanly separates the cheap discovery/screen phase from integration: one isolation Docker invocation screened three candidates, then the exact two-entry manifest paid for one full-context Docker build/report gate. The gate passed with the unchanged ROM SHA-1 and advanced **1610 → 1612** matched functions, **1150 → 1152** linked C units, and **1334 → 1336** decomp files.
- The exact candidates were callers of already-converted `func_08007000`; isolation alone was not treated as sufficient. The full-context transaction confirmed that both ordinary-C scene-data wrappers remain byte-identical when linked with the converted callee.
- `func_080047D4` is a useful negative result: the numeric absolute-address C spelling reproduced all seven instructions but omitted the target's absolute table-address literal-pool word. The recorded **28.57143** gap is therefore a real data-byte mismatch, not a target-symbol boundary artifact, and correctly stayed outside `apply-batch`.
- The cycle automatically updated `.nearmiss/func_080047D4.json` and `tools/attempts.tsv`, while `.decomp-runs/round-73-isolation-v1.json` and `round-73-apply.json` preserve candidate hashes and the full verification command. This is the desired provenance shape for the next repair pass.

## Round 79 — scene animation helpers (2026-08-07)
- The sibling loop selected two linked scene animation helpers. Six shared isolation passes converged from **0/2 exact/near miss** to **2 exact / 0 rejected**; the exact-only transaction then passed one full Docker ROM/report gate and advanced **1660 → 1662** matched functions, **1200 → 1202** linked C units, and **1384 → 1386** decomp files.
- m2c supplied the actionable scene-field and bitmask-loop skeletons. asmlift was still useful as a boundary signal but hit project-context compile errors on both candidates; it did not produce an accepted spelling.
- The durable compiler-shaping lesson is to preserve handler-before-scene load order and keep the countdown as a widened `u32` so the target's store precedes its `LSLS #16` zero test. The initial `func_0801646C` spelling used compiler-only hard-register declarations, but those were not needed: the no-pin ordinary-local candidate was exact in isolation and in the full ROM.
- The v1–v5 near misses, one compile error, and the final exact v6 receipt remain committed provenance. The accepted sources contain no instruction-bearing or volatile inline asm; ROM SHA-1 stayed `3f556448d290fa5406d6ed367fee16cc02387ad3` with `wariowareinc.gba: OK`.

## Round 79 follow-up — strict real-C audit (2026-08-07)
- Inspection of Conker's `no-asm-pin` guideline exposed a quality gap: our older policy rejected instruction-bearing asm but still admitted compiler register pins and empty barriers. The Git policy, pre-push range check, transactional cycle, Pi guard, and new `tools/audit_decomp_source.py` now reject all four asm escape forms for new candidates.
- The six pins were removed from `func_0801646C`. Its no-pin candidate had the same SHA-256 as the final tracked source and matched the converted target at **100%** in isolated objdiff; a clean Docker verification then emitted `wariowareinc.gba: OK` with ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Two readable struct-layout experiments were deliberately not accepted: a named `MainMenuSceneData` view scored **73.59524%**, and a hand-laid packed animation view scored **80.52381%**. Both are preserved as near-miss evidence. The final raw scene-pointer form is real C; the audit reports its pointer casts/offsets for future struct recovery instead of hiding them or weakening byte matching.
- The strict source-audit test suite passes **27 tests**. The candidate lifecycle now makes “byte-exact” and “strict ordinary C” independent gates, so a future pinned or wrapped candidate cannot enter the ROM transaction by accident.

## Round 78 — counted save-unlock family and aggregate (2026-08-07)
- The sibling loop selected two counted save-unlock predicates and their aggregate caller. One isolation Docker invocation classified **3 exact / 0 rejected**, and one exact-only transactional full Docker ROM/report gate advanced **1657 → 1660** matched functions, **1197 → 1200** linked C units, and **1381 → 1384** decomp files.
- m2c provided the actionable do-while/count and repeated-call skeletons. asmlift was still valuable as a diagnostic: it declined both post-loop predicates because of the pre-update loop value and declined the aggregate because of the trailing alignment halfword. Those are tool coverage limits, not acceptance exceptions.
- The full gate is especially informative here: the aggregate caller was screened against original leaf objects and then linked successfully with the newly converted leaf callees. This validates keeping caller conversion behind the full-context gate without needlessly rejecting safe callers.
- The accepted sources are ordinary C with no instruction-bearing or volatile inline asm. Full verification retained ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3` with `wariowareinc.gba: OK`.

## Round 77 — richer stage-unlock predicates (2026-08-07)
- The sibling mining loop selected nine richer save-unlock predicates. One isolation Docker invocation classified **9 exact / 0 rejected**, and one exact-only transactional full Docker ROM/report gate advanced **1648 → 1657** matched functions, **1188 → 1197** linked C units, and **1372 → 1381** decomp files.
- m2c and the globally installed `@asmlift/cli` were both useful for recovering and cross-checking the semantic skeletons. The compiler-facing repairs were explicit: preserve `((0 - temp) | temp) >> 31` for nonzero-to-one count normalization, keep `u32 count` and `if (count > 1)` for the threshold branches, and use a temporary `s32 ready` for the `func_08008AA4(...) > 0xE` path.
- No near-miss repair was needed in this slice. The accepted candidates are ordinary C with no instruction-bearing or volatile inline asm, and the full gate retained ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3` with `wariowareinc.gba: OK`.
- The cost profile is now predictable: one shared isolation container for a sibling batch, then one expensive full-context Docker transaction for exact winners only. The two Round 77 receipts preserve candidate hashes, results, and the gate command.

## Round 76 — stage-unlock wrapper fan-in (2026-08-06)
- The sibling mining loop selected fifteen unmatched save-unlock wrappers. A single isolation Docker invocation classified **15 exact / 0 rejected**, and one exact-only transactional full Docker ROM/report gate advanced **1633 → 1648** matched functions, **1173 → 1188** linked C units, and **1357 → 1372** decomp files.
- m2c and the globally installed `@asmlift/cli` both produced useful semantic skeletons; asmlift omitted literal/alignment data in its normalized diagnostic output, so the project candidate still required the existing C header context and explicit shifted constants. The isolated compiler result—not either decompiler's confidence—remained the admission authority.
- The durable family rule is to express the target's nested `save_is_stage_unlocked == 0` and progress-check branches as nested C `if` statements, use a scalar return type to preserve `POP {R1}; BX R1`, and spell large flag returns as `0x80 << N` so agbcc materializes the same shift pair.
- The accepted sources contain ordinary C only; no inline asm was introduced. The baseline, isolation, and apply receipts preserve the complete provenance, and full verification retained ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3` with `wariowareinc.gba: OK`.

## Round 75 — task/scene/sprite wrapper fan-in and ABI repair (2026-08-06)
- The current loop handled twelve compact candidates with one cheap v1 isolation container, one repaired v2 isolation container, and one transactional full Docker ROM/report gate. The first pass classified **9 exact / 2 near miss / 1 compile error**; the exact-only v2 manifest classified **12 exact / 0 rejected** and the full gate advanced **1621 → 1633** matched functions, **1161 → 1173** linked C units, and **1345 → 1357** decomp files.
- m2c-style skeletons supplied the useful semantic shapes for the wrappers. asmlift remained useful as a comparison/diagnostic path and project-context probe, but it was not the deciding source of the final spellings; isolated compiler diffs and sibling patterns were.
- The two v1 near misses exposed a reusable ABI lesson: a function can ignore its middle logical parameter in C while still needing a placeholder parameter so the target pointer arrives in R2. Adding that unused `s32` parameter made `func_0806A958` and `func_0806A97C` exact without any asm. The compile error was repaired by adding the narrow `scenes.h` context for `gCurrentSceneData`.
- This is now a practical cost envelope: discovery/repair stays in short isolated Docker passes, while only the exact v2 manifest pays for the expensive full-context build/report. The receipt chain (`round-75-isolation-v1.json`, `round-75-isolation-v2.json`, `round-75-apply.json`) plus `.nearmiss/` and `tools/attempts.tsv` preserves every rejected spelling and its reason.
- The accepted sources contain ordinary C only. No instruction-bearing or volatile inline asm entered the batch. Full verification preserved ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3` and emitted `wariowareinc.gba: OK`.

## Round 74 — wrapper fan-in and compiler-shape repair (2026-08-06)
- The two-stage screen/apply lifecycle handled a useful nine-function slice: the first isolation container classified **5 exact / 2 compile errors / 2 near misses**, and one repaired isolation container reached **9 exact / 0 rejected**. The exact-only apply then paid for one full Docker build/report gate and advanced **1612 → 1621** matched functions, **1152 → 1161** linked C units, and **1336 → 1345** decomp files.
- m2c supplied the actionable semantic skeletons for all nine wrappers. asmlift was useful as a comparison/diagnostic path but mostly stopped at project-header/prototype or literal-data boundaries; the successful repairs came from the isolated diff and existing project sibling patterns.
- The two compile errors were integration context, not code-generation blockers: `func_08022650` needed a narrow direct declaration instead of pulling in a header with an `-Werror` warning, and `func_080733AC` needed `scenes.h` for `gCurrentSceneData`.
- The two near misses were compiler-shape issues. `func_08039A44` needed register-bound ordinary-C locals for the target's R1 base/R2 value/R0 mask reuse, while `func_0801A688` needed an empty `"+r"` dependency to keep the numeric table base live in R4 across `get_random_range`.
- The full gate passed with `wariowareinc.gba: OK` and unchanged ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. The v1 near-miss records and both isolation receipts remain durable provenance; no non-exact candidate entered the transaction.

## Round 72 — gameplay/data fan-in and boundary audit (2026-08-06)
- The five-pass screen made the current loop practical: one Docker isolation invocation per revision, immutable candidate hashes in each receipt, near-miss records retained automatically, and one fresh full-context Docker gate for the selected three-function batch. The final screen was **2 exact / 1 near miss**.
- m2c/asmlift-style semantic skeletons were useful for discovery, but the successful spellings required project-aware repair: a numeric absolute pointer for `func_08003DE0`, a widened `u32` callee plus `R4` pin and empty compiler dependency for `func_080E1F48`, and `src/audio.h` for `func_08023494`.
- The linked objdiff near miss for `func_08003DE0` was metadata-only. A complete linked `.text` audit recorded equal 20-byte sections and SHA-256 `d479c6a815c77122b2ce066889fd20bbcb86355f9174a64085fd1d1148d38f39` in `round-72-boundary-audit.json`; only then did the force path admit it. This reinforces that the boundary exception needs its own receipt and must remain coupled to the ROM gate.
- The new C contains no instruction-bearing or volatile inline asm. The `asm(\"\" : \"+r\"(r4))` in `func_080E1F48` is an empty compiler metadata constraint; it emits no instruction and is covered by the existing policy. `func_08003DE0`'s `.nearmiss` record correctly remains provenance even after the final audit.
- Timing remains favorable for small fan-in: five cheap isolated passes plus one full Docker transaction, rather than a full ROM build for every permutation. The expensive step is now deliberately reserved for the selected exact/audited subset.

## Round 33 — sprite, scene, music-table, and graphics fan-in (2026-08-05)
- m2c supplied ten compact standalone skeletons. The first screen found **5 exact / 4 near miss / 1 compile error**; register-bound variants recovered the three music-table siblings and the graphics-buffer store, producing a combined **10 exact** manifest.
- `func_0800C7A4`, `func_0800CE6C`, `func_08016688`, `func_08018534`, `func_08019644`, and `func_080C477C` matched directly through typed sprite/scene headers, explicit field offsets, or signed division. The music siblings required a base in `R4` and an offset/address accumulator in `R0`; `func_0804E290` required the same two-register accumulator to prevent folding `gGraphicsBuffer + 0x54` into the literal symbol.
- The rejected first spellings remain in `.nearmiss/` and `tools/attempts.tsv`; the best near misses show only the three-operand add or literal-folding differences. No instruction-bearing or volatile inline asm was added.
- The combined exact-only screen and one transactional full-ROM gate passed. Report progress is **1464 → 1474** matched functions, **1004 → 1014 C TUs**, and ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.

## Round 34 — local-label pure leaves (2026-08-05)
- The existing m2c/probe ledger supplied readable C for `func_080039EC` and `func_08008058`. A fresh one-container isolation pass reproduced their instruction streams, but linked-ELF objdiff inferred each target symbol only through its first internal local label, reporting **33.333332%** and **0.0%** symbol matches despite the candidate epilogues being present in the full object.
- This is distinct from an ordinary compiler near miss: the target assembly has `glabel` followed by local branch labels, so the comparison symbol size is truncated. The evidence is preserved in `.decomp-runs/20260805T-round-34-isolation.json`, `.nearmiss/`, and `tools/attempts.tsv`; no instruction-bearing or volatile inline asm was introduced.
- The rollback-capable `apply-batch --force` path was used as a narrow metadata exception, and the full-context gate—not the misleading inferred symbol score—decided acceptance. It passed `wariowareinc.gba: OK` with unchanged SHA-1 and moved the report **1474 → 1476** matched functions and **1014 → 1016** linked C TUs.
- Durable rule: keep normal standalone admission exact-only; only use the force path when the receipt identifies a known target-symbol-boundary defect, the full object bytes are explained, and the transactional full-ROM gate passes. Do not use it to waive a compiler-generated instruction difference.

## Round 35 — scene/runtime-buffer sibling screen (2026-08-05)
- m2c supplied eleven compact skeletons spanning scene-variable masks, scene-data setters, a runtime byte table, a graphics-buffer indexed transfer, and two scene wrappers. One Docker isolation invocation classified **3 exact / 5 near miss / 3 compile error**.
- The exact candidates were `func_0801002C`, `func_08010308`, and `func_080F3C60`. The two scene wrappers matched directly as ordinary C when their prototypes and absolute table arrays were made explicit; the four-byte runtime buffer setter matched with an `extern u8 D_030068F0[]` array. The linker preflight required the canonical `D_030068F0` assignment, which was added before the transaction.
- The rejected mask candidates demonstrate the existing constant-folding trap (`MOVS #0x3D; RSBS` or `MOVS #2; RSBS` becoming a literal mask), while the graphics indexed-transfer candidate changed load/add register allocation. Compile errors were missing `gCurrentSceneData` declarations in candidates that used the wrong header. All rejected hypotheses remain in `.nearmiss/`, `.decomp-runs/`, and `tools/attempts.tsv`; no non-exact source entered the ROM transaction.
- The exact-only subset passed one transactional full Docker ROM/report gate. Report progress is **1476 → 1479** matched functions and **1016 → 1019 C TUs**; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.

## Rounds 36–44 — register-allocation follow-up (2026-08-05)
- The Round 35 near misses were revisited through eight small variant screens. Register-bound result masks closed `func_0801BEA8`, `func_0801AF18`, and `func_0801B3E4`; an ordered `variable`/`scene` pointer spelling closed `func_080DA0B0`; the `R0` offset/`R1` base accumulator closed `func_08082934`; and a two-operand `offset += base` form closed `func_080F1574`.
- `func_08016A60` and `func_08016A7C` were deliberately not forced. Their pointer/argument allocations can be reproduced, but after the argument's `MOVS #1; ANDS` sequence agbcc folds the later `mask = -2/-3` materialization into `SUB #3/#4`; all tested ordinary-C forms retained that mismatch. The near-miss receipts and candidate seeds remain durable, and no inline asm was added.
- One consolidated six-entry exact isolation receipt passed one transactional full Docker ROM/report gate. Report progress is **1479 → 1485** matched functions and **1019 → 1025 C TUs**; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. Receipts: `.decomp-runs/20260805T-round-36-isolation.json`, `.decomp-runs/20260805T-round-37-isolation.json`, `.decomp-runs/20260805T-round-38-isolation.json`, `.decomp-runs/20260805T-round-39-isolation.json`, `.decomp-runs/20260805T-round-40-isolation.json`, `.decomp-runs/20260805T-round-41-isolation.json`, `.decomp-runs/20260805T-round-42-isolation.json`, `.decomp-runs/20260805T-round-43-isolation.json`, `.decomp-runs/20260805T-round-44-isolation.json`, and `.decomp-runs/20260805T-round-44-apply.json`.

## Round 45 — wrapper/runtime fan-in and exact-only promotion (2026-08-05)
- m2c supplied ten compact candidates spanning five `gCurrentKeys` test wrappers, a scheduler callback, and the `D_030068E8` runtime-table family. The first one-container screen found **7 exact / 3 near miss**; a small register/order follow-up closed `func_080F0E14`, leaving **8 exact / 2 near miss** for the final manifest.
- The two-load runtime helper required more than direct pointer indexing: bind the table address to `R4`, load its first pointee into `R3` before shifting `R0`, then reload the pointee into `R1` after the first store. This reproduced both target loads and the literal-pool placement without instruction-bearing asm.
- The two retained bit setters reproduce the base load, scaled offset, argument mask, byte load, and OR exactly. agbcc still sees the preceding `R2 = 1` value and canonicalizes `mask = -5/-9` as `SUB R2,#6/#10`; the target instead materializes `MOVS R2,#5/#9; RSBS R2,R2,#0`. The near-miss diff and all candidate variants remain in `.nearmiss/`, `.decomp-runs/`, and `tools/attempts.tsv`.
- The exact-only eight-entry `apply-batch` ran one full Docker ROM/report gate and passed with SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. Report progress is **1485 → 1493** matched functions and **1025 → 1033 C TUs**. m2c plus the one-container lifecycle was sufficient for this wrapper-heavy batch; asmlift was not needed, and no non-exact candidate entered the transaction.

## Round 32 — scene-table and audio wrapper fan-in (2026-08-05)
- m2c supplied seven compact standalone skeletons. One Docker isolation invocation classified **6 exact / 1 near miss**; the exact subset was selected by candidate hash and reused for one transactional full-ROM gate.
- The three scene-table siblings matched as ordinary C when the candidates used `scenes.h`, raw `*(u32 *)((u8 *)gCurrentSceneData + 8)`, and `u8 D_083A98xx[]` table addresses. The audio siblings matched with an explicit non-void return for `func_0800C7FC`, a declared `u16` key read in `func_0801E918`, and a typed data-symbol pointer for `func_08024494`.
- The remaining `func_0800CDB0` hypothesis is retained in `.nearmiss/`: using a local absolute base preserved the target `+2` field offset, but agbcc still folded the target's `MOVS #3; RSBS` mask into `MOVS #0xFD`. No inline asm was added.
- Full-context integration caught three linker-map omissions before mutation (`D_083A98B8`, `D_083A98D8`, `D_083FC594`); adding canonical assignments mirrored from `include/undefined_syms.inc` allowed the six-entry transaction to pass. Report progress is **1458 → 1464** matched functions, **998 → 1004 C TUs**, and ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.

## Round 0805 — standalone wrapper fan-in (2026-08-05)
- m2c supplied usable C skeletons for eight short standalone candidates. One isolation Docker invocation found exact spellings for `func_0800D23C` and `func_08019A8C`; asmlift was useful as a diagnostic on the simpler data helpers but declined several project-global wrapper candidates. The six rejected candidates remain in `.nearmiss/`, `.decomp-runs/`, and `tools/attempts.tsv`; no non-exact C or assembly move entered the ROM transaction.
- The first apply attempt caught two lifecycle defects before source acceptance: `git status --porcelain` was parsed after a destructive whitespace trim, and `apply-batch` treated unrelated near misses in a larger receipt as batch failures. The fixes preserve the status column, select receipt results by candidate SHA-256, and add regression coverage; the tooling suite now passes 18 tests.
- The corrected run reused the exact screen receipt and applied only the two exact entries. One full Docker ROM/report gate passed with `wariowareinc.gba: OK`, SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`, and report progress from **1401** to **1403** matched functions. This validates the intended expensive-step budget: one isolation container for the candidate fan-in and one full build for the accepted subset.

## Round 30-b — standalone wrapper sibling fan-in (2026-08-05)
- m2c supplied useful skeletons for 14 small standalone functions. One Docker isolation invocation split them into **10 exact**, **3 near-miss**, and **1 compile-error** results. The exact subset was selected by candidate identity from the immutable receipt and applied without `--force`.
- The exact winners were ordinary C field/callback wrappers and unsigned loop siblings. `u32` loop counters reproduced the target `BLS` branches for `func_08017668` and `func_080A8A3C`; direct byte/halfword pointer offsets reproduced the scene-data and sprite-visible siblings. `func_080043A0` required the raw-object fallback because target symbol metadata prevented normalized linked comparison, but the full ROM gate confirmed it.
- The three near misses remain quarantined: `func_0805627C` adds an extra sign/zero-normalization round trip when the local is declared `s16`; `func_080A002C` loads the music-player global after argument normalization instead of before it; and `func_080F5FF4` models the callee's stack arguments as a five-parameter prototype, changing the stack frame and callee-saved registers. `func_08003228` did not compile because `src/lib_sprite.h` lacks the `sprite_handler_set_global_pause` declaration; the candidate was not applied.
- One transactional full Docker build passed `wariowareinc.gba: OK`, preserving SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. The report advanced from **1407** to **1417** matched functions. This confirms that one-container fan-in materially reduces the expensive iteration loop while retaining exact-only standalone acceptance.

## Round 30-c — near-miss shaping follow-up (2026-08-05)
- A focused seven-spelling screen recovered three previously rejected standalone candidates. `func_08003228` only needed the missing `sprite_handler_set_global_pause` prototype; `func_0805627C` needed a widened `s32` parameter followed by an explicit `(s16)` cast to produce the target `LSLS`/`ASRS` pair without a second normalization; and `func_080A002C` needed `register u16 value asm("r1")` so the music-player global loads before the compiler normalizes the argument.
- The old-style `func_080F56EC()` declaration improved `func_080F5FF4` from a 100% object-layout mismatch to a 10.5% near miss, but the remaining stack/callee-save difference is not accepted. It remains in `.nearmiss/` with the new evidence and no source change.
- The three exact candidates reused one isolation receipt and passed one transactional full Docker build. Report progress is **1417 → 1420** matched functions; ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.

## Round 31 — DMA, heap-copy, and sprite sibling fan-in (2026-08-05)
- The repo-local m2c adapter supplied compact skeletons for two related screens. The wrapper screen produced **8 exact / 4 near miss** results; the DMA/sprite screen produced **11 exact / 1 near miss**. A combined exact-only manifest kept all nineteen accepted entries tied to their candidate hashes and receipts.
- Register-bound C destinations recovered the two heap-copy siblings: a non-void no-return function must keep the allocation in `R0` while copying, otherwise agbcc moves the destination to `R2` to preserve a conceptual return value. A local byte-pointer base, rather than direct global-plus-offset syntax, preserved the target's `MOVS #0x1E; LDRSH` sequence in `func_0800CD94`.
- The DMA family matched directly through `src/code_08000f10.h`, but the first linker preflight found four `D_0300XXXX` symbols and `D_083FD264` missing from `undefined_syms.ld` even though `include/undefined_syms.inc` defined them. Adding those canonical absolute assignments let all eleven DMA/sprite candidates pass without force.
- `func_0801E6F8` remains evidence-only: the generated C semantics were correct, but the target's `MOVS #2; RSBS R0,R0,#0` mask shape was folded to a single `MOVS #0xFE`. No inline asm was added to rescue it. The accepted 19-entry transaction passed with report progress **1439 → 1458** and unchanged ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.

## Round 30-d — wrapper and sprite sibling fan-in (2026-08-05)
- m2c supplied the initial skeletons for nineteen short standalone wrappers, field stores, sprite-visible siblings, and one arithmetic helper. The first one-container screen found **11 exact**, **6 near misses**, and **2 compile errors**; all rejected candidates were recorded in `.nearmiss/`, `.decomp-runs/`, and `tools/attempts.tsv`.
- Small ordinary-C ABI variants closed the remaining gaps. `func_080042F4` keeps six incoming parameters but forwards only the four registers used by the target; `func_080043B8` models unused stack slots so its two loaded values land at the target's `SP+0x14`/`SP+0x18`; and non-void declarations without an explicit return reproduce the target `POP {R1}; BX R1` epilogue. A staged multiply in `func_080F2FFC` preserves the target's left-to-right register reuse, while register-bound locals preserve the global literal-load order in `func_0801E44C` and `func_08007FC0`. These are register-allocation declarations, not instruction-bearing asm.
- The two sprite-ID candidates initially failed only because `src/lib_sprite.h` did not declare `sprite_id_set_visible`; explicit matching prototypes fixed the compile errors. `func_0801E44C` also exposed the existing map asymmetry: `D_0300490E` was present in `include/undefined_syms.inc` but absent from `undefined_syms.ld`. The linker preflight caught this before the full build, and the canonical `0x0300490E` definition was added.
- The final exact-only 19-entry manifest passed one reused-receipt transaction and one clean full Docker ROM/report gate. Report progress is **1420 → 1439** matched functions, **979 C / 5708 asm-only** units, and the ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`. `asmlift` was not needed for this wrapper-heavy screen; m2c plus the automated receipt/variant loop was faster, while the asmlift adapter remains available for less direct semantic/control-flow candidates.

## Round 25-a — included-stub relocation false near misses (2026-08-05)
- m2c supplied usable semantic skeletons for four main-menu task wrappers: `func_080122FC`, `func_0801312C`, `func_080148EC`, and `func_08014C9C`. The candidates use ordinary C calls, callback pointers, and host-TU include guards; no instruction-bearing inline asm was added.
- Normalized linked-ELF isolation scored the four at **99.59–99.72%** instead of exact because external `BL` relocation records remained different in the isolated candidate object. Direct object disassembly showed the instructions and host-TU layout were exact, so the candidates were retained for a full-context research apply rather than recorded as ordinary exact winners.
- The forced transaction did not weaken the acceptance gate: it still ran the clean full Docker build/report step and passed `wariowareinc.gba: OK`, preserving SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. This is evidence that, for included stubs, final host-TU bytes and the full-ROM gate outrank relocation-only isolated scores; standalone candidates should remain exact-only.
- The same run exposed that standalone `D_XXXXXXXX` map validation is inappropriate for included stubs. `d7f9d29` now defers those references to the host TU and adds regression coverage; the lifecycle/tool test suite passed **21 tests**. The four conversions increased matched code but not matched-function count, so the 30% push should prioritize standalone linker entries.

## Round 0805b — linker preflight and rollback (2026-08-05)
- One eight-candidate isolation pass found six exact spellings. Four were safe to apply because they were pure helpers or called still-ASM callees; two exact callers of already-converted C helpers (`func_0800C9A4`, `func_080CAAEC`) were intentionally retained as evidence only.
- The first four-entry transaction failed at the final link, not at objdiff: `D_083A98D0` was present in `include/undefined_syms.inc` but absent from `undefined_syms.ld`. The transaction restored every source/ASM/linker path and the rollback verifier rebuilt the exact baseline SHA-1. Adding the canonical linker symbol and rerunning the immutable screen made the same four-entry transaction pass.
- `tools/decomp_cycle.py` now preflights candidate `D_XXXXXXXX` references against `undefined_syms.ld`, with a regression test, so this class of failure is reported before an expensive full-ROM build. The accepted rerun advanced the report from **1403** to **1407** matched functions and kept SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.

## Lifecycle hardening and included-stub fan-in (2026-08-05)
- The first included-stub probe found a real normalization gap: m2c dropped labels transported inside C-string asm (`_0800BFB6: \\n\\`), so it reported an undefined branch target. `tools/asmlift_warioware.py` now strips that transport suffix before matching labels/directives, with two unit regressions.
- Included-stub isolation also needs the host TU's section-relative symbol map. Without it, a correct standalone candidate produced artificial `BL 0` differences against the whole `bitmap_font.c.o`. `tools/decomp_cycle.py` now imports the target TU's text/data symbol offsets for the candidate link while keeping the entry function at the comparison origin.
- `tools/decomp_permute.py` ports Conker's safe fan-in shape: a directory of independent m2c/asmlift/manual spellings is compiled in one Docker isolation invocation, exact winners are listed by content hash, and `accept` delegates to the exact-only transactional ROM gate. The two-variant `func_0800BF7C` exercise found one exact m2c-shaped spelling and one 87.878784%-matching unsigned-coordinate near miss in one invocation; the earlier 99.393936% result was correctly identified as an unresolved-host-symbol artifact and disappeared after the host map fix.
- Receipts now record manifest and candidate/target SHA-256 identities. `.nearmiss/FUNC.json` retains a bounded eight-entry history instead of silently losing prior attempts when a better seed arrives. This keeps the Conker-style provenance trail without allowing diff payloads to grow unbounded.
- Apply transactions ignore only generated evidence paths (`.decomp-runs/`, `.nearmiss/`, `.mizuchi-tmp/`, and `tools/attempts.tsv`) when checking cleanliness; any unrelated source, linker, or tool edit remains a hard stop. This closes the loop between durable recording and the next exact apply without weakening the ROM gate.
- The Pi `apply_conversion` frontend was a bypass: it could mutate before isolated comparison and accept `verify:false`. It now hands a temporary manifest to `decomp_cycle.py apply`, so all frontends share exact-only isolation, candidate hashes, full-ROM verification, and rollback receipts.

## Included-stub full-context gate and provenance follow-up (2026-08-05)
- The first end-to-end `func_0800BF7C` acceptance caught an integration bug that isolated objdiff could not see: included-stub candidates are screened as standalone objects, but the normal Makefile also compiles `src/decomp/*.c` independently while the host TU includes the same file. Applying the raw candidate caused a duplicate definition. The full Docker gate failed, the transaction restored every touched path, and the rollback verifier rebuilt the exact baseline ROM.
- `decomp_cycle.py apply` now adds the repository's `#if __INCLUDE_LEVEL__ > 0` wrapper for included stubs and records `source_transform` in the receipt. The corrected candidate passed the same full-context gate with `wariowareinc.gba: OK` and unchanged SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- `decomp_permute.py screen` now snapshots all raw candidate files into `.decomp-runs/`; acceptance uses the immutable snapshot rather than an ignored scratch path. This makes a committed run receipt independently auditable.
- Acceptance also reuses a fresh, hash- and commit-validated isolation receipt, removing the redundant second isolation Docker invocation. Stale receipts fail closed, so this optimization does not weaken exact-only admission.

## Batch 180 lifecycle correction and pure-leaf screen (2026-08-05)
- The first ten-candidate screen exposed a false near-miss source: isolation compiled agbcc output without the Makefile's appended `.text` / `.align 2, 0` / `.note.GNU-stack` tail. For functions ending two bytes off a four-byte boundary, the linker supplied a Thumb NOP (`0xC046`) while the real build supplies zero padding. `tools/decomp_cycle.py` now reproduces the Makefile tail, and a regression test locks that contract in.
- After the correction, eight of ten candidates were exact in one Docker isolation invocation. The accepted set was `func_080F1F9C`, `func_080F28F8`, `func_080F2C50`, `func_08035ACC`, `func_08003014`, `func_0803F224`, `func_0803F26C`, and `func_0806754C`. The C candidates contain no instruction-bearing asm; `func_080F28F8` uses a register-bound C local (`asm("r0")`) for allocation shaping, not an asm instruction block.
- `func_08003014` has a legacy target object whose `glabel`/`.thumb_func` ordering leaves the linked target symbol as `NOTYPE`; objdiff cannot load that normalized ELF and reports a symbol-bounds error. The cycle now falls back to raw-object comparison only in that linked-ELF failure case, records `comparison: raw_object_fallback` and the loader error in the receipt, and still requires the transactional full-ROM gate. The raw comparison was byte-exact and the full ROM gate passed.
- `func_08008058` and `func_080039EC` were intentionally held back: objdiff/report identifies internal local labels as separate target functions, so a single C function would be semantically right but would not be a clean one-function acceptance. This is evidence for a future split-symbol-aware candidate mode, not permission to force the conversion.
- `apply-batch` then performed one full Docker ROM/report gate for the eight exact candidates; the result was `wariowareinc.gba: OK`, SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`, and report progress `1401 / 5960`. The isolation and apply receipts are committed with the batch.

## Automated candidate cycle and Git gates (2026-08-05)
- The old isolated frontend launched `tools/mizuchi/compile-in-docker.sh` once per candidate. `tools/decomp_cycle.py isolate` now accepts a manifest and compiles all candidates plus any temporary target objects in one `devkitpro/devkitarm:latest` container, then runs host-side objdiff for each result. This keeps the slow full build out of C-shape iteration while preserving the project compiler.
- The first two-entry exercise matched `func_080D74F4` exactly and classified the preserved `func_08002038` ordinary-C spelling as a near miss with one Docker invocation. The full diff is stored in `.decomp-runs/20260805T183342Z-isolation.json`; the near miss remains rejected and its prior full-ROM byte score is not compared with the isolated objdiff gap.
- `tools/decomp_cycle.py apply` is the guarded transaction layer: it refuses non-exact isolation in normal mode, moves standalone or included-stub files mechanically, runs a clean Docker ROM/report gate, and restores the exact pre-apply snapshot plus a fresh baseline build when the ROM differs. The forced audit of `func_08002038` produced `.decomp-runs/20260805T183726Z-apply-func_08002038.json`; rollback rebuilt SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3` and `cmp` passed.
- `.githooks/pre-commit` now performs only the fast policy and staged/unstaged ROM-scope checks; `.githooks/pre-push` checks the complete pushed range, rejects dirty ROM-affecting paths, and owns the Docker SHA gate. This removes one duplicate full compile from the normal commit-then-push loop without weakening the remote invariant. Tooling/docs-only pushes still skip Docker, while `tools/check_decomp_policy.py` independently blocks new non-empty inline asm and verification-gate edits. This complements, rather than replaces, `.pi/extensions/warioware-decomp-guard.js`.
- The remaining gap is candidate generation: the cycle evaluates explicitly supplied permutations but does not invent C AST variants. m2c/asmlift can continue producing those candidate files; the manifest is the safe fan-in point for testing them.
- A read-only Luna worker reproduced the same lifecycle from a fresh context: five unit tests passed, the exact/near-miss split was correct, the receipt reported one Docker invocation, and the worker made no repository edits. This is a suitable small validation task for future delegated workers.

## m2c/asmlift adapter — strict re-hoist (2026-08-04)
- The restored `tools/asmlift_warioware.py` adapter finds an unconverted Splat assembly source, normalizes its address-commented Thumb syntax, runs the sibling Mizuchi `m2c`, and can pass an optional candidate through the globally installed `asmlift` executable.
- `tools/asmlift-compile.sh` supplies project headers and reuses the Docker compiler bridge, so generated candidates can be tested in the same agbcc context as normal decompilation units.
- On `func_08003D28`, m2c provided the correct mask-operation skeleton and asmlift was useful as a second attempted synthesis path, but asmlift declined the candidate because its generated type context did not compile against this project. The accepted spelling still required manual register pins and an empty barrier for the `MOVS`/`LSLS`/`RSBS` instruction shape.
- On `func_0800DAD8`, asmlift produced the exact accepted C spelling on its first project-header-aware attempt, including the signed index cast and 48-byte stride. The adapter therefore moved this function from source discovery through candidate synthesis quickly; the strict build and isolated object diff remained the acceptance gates.
- On `func_08015F80`, m2c's compact condition and asmlift's nested early-return spelling were both useful, with the latter matching the original branch layout exactly. This is a good adapter-assisted pattern for small control-flow leaves: compare both generators, then retain the candidate whose structure preserves the target branches.
- On `func_0803FED0`, the adapter exposed the global-halfword semantics but initially folded the non-zero offset into the literal address. An empty barrier on a local absolute pointer recovered the original base literal plus `[base, #2]` load; isolated objdiff and the strict ROM build both confirmed the workaround.
- On `func_080F26BC`, m2c correctly identified the raw record offsets, while the first C spelling lost the target's three-register add and loaded the base too late. Register pins plus `r3 = (u8 *)((u32)r1 + (u32)r3)` recovered `ADDS R3,R1,R3`; this was exact in isolation and in the strict ROM.
- On `func_080F2F68`, m2c surfaced a non-obvious ABI gap: the function uses its third incoming argument from `R2`, so a two-parameter C spelling sign-extended `R1` instead. Modeling the unused middle parameter made the call setup exact without inline instruction shims.
- On `func_080F2F78`, the same adapter insight transferred directly to the signed-byte sibling: only the shift width changed from 16 to 24, and the strict object/ROM gates confirmed the family spelling.
- On `func_08073650`, m2c identified the two-call wrapper but the first isolated C object carried an alignment NOP after the function instead of the source's explicit zero halfword. A `.text`-section `u16` padding symbol fixed the bytes; objdiff then matched all five instructions and the strict ROM stayed identical. Standalone reports count that padding symbol as an additional matched text symbol.
- On `func_08002024`, the full target and candidate `.text` sections were identical, but objdiff inferred the target function size only through the first internal local label and reported a misleading 40% symbol match. When target asm lacks an explicit function end before a local branch label, compare the complete section bytes and require the strict linked ROM gate.
- On `func_08007E8C`, the m2c wrapper was straightforward and the compiler naturally reproduced the source's alignment halfword before the `0x7FFFFFFF` literal. The full section and strict ROM matched; the report's total-function recount shifted by one because of the target literal-pool/local-symbol layout, so metrics must be copied from the fresh report rather than inferred by arithmetic.
- Strict workflow lesson: adapters are candidate generators and diagnostics only. Every adopted function still needs an isolated object diff plus a clean `NONMATCHING=0` Docker build with `wariowareinc.gba: OK`; this first re-hoist preserved the baseline ROM SHA-1 exactly.

## Guard/loop hardening — non-empty inline asm ban and unstoppable loop
- Tools that helped:
  - Recent chunks proved the naked-asm guard works, but also showed weaker agents can still hide meaningful instruction sequences inside non-empty `asm volatile` blocks.
  - The loop already had a deprecated no-progress compatibility field on `decomp_chunk_done`, but no-progress/text-marker/no-signal paths were not equally robust about keeping the stream moving.
- Tooling implemented:
  - `.pi/extensions/warioware-decomp-guard.js` now blocks new non-empty statement-form inline asm in `src/decomp/*.c` payloads and file edits. Empty asm barriers/clobbers remain allowed; register-pinning declarations like `register u32 r0 asm("r0")` are not treated as inline asm statements.
  - `.pi/extensions/warioware-decomp-loop.js` now has no blocked loop state: no-progress chunks, the legacy text marker, and missing completion signals all compact and advance immediately. No-progress means "try a different candidate next," not "stop the stream."
- Follow-up idea:
  - If accepted legacy non-empty inline asm files are edited later, convert them to pure C/empty-barrier C instead of preserving the instruction shim.

## Batch 147 — same-TU unprototyped callee call shaping
- Tools that helped:
  - `compile_and_view_asm` made the last-mile differences explicit: first the `sp+0xA` pointer was in `r3` instead of `r2`, then the generated coordinate load was `LDRH; LSLS; ASRS` instead of indexed `LDRSH`.
  - `apply_conversion` handled the bitmap_font include-shim and clean Docker ROM verification once the isolated match was perfect.
- Tooling/workflow gap:
  - The candidate did not surface that the already-converted same-TU callee prototype for `func_0800C110` would influence caller-side truncation/sign-extension.
- Manual workaround:
  - Use narrow asm only for the helper-call setup and two indexed `LDRSH` loads, keep the rest of the wrapper in C, and declare the callee old-style (`extern void *func_0800C110();`) so agbcc does not re-truncate already-shaped register arguments.
- Desired tooling improvement:
  - For included-stub callers of converted same-TU functions, show both the current callee prototype and whether an old-style no-prototype declaration may be needed to preserve original register argument shaping.

## Batch 146 — same-TU callee prototype shaping
- Tools that helped:
  - `compile_and_view_asm` quickly showed `func_0800C0BC` was a perfect match if `func_0800C080`'s second parameter was treated as signed, and that the only pure-C miss under the old prototype was `LDRH` vs `LDRSH`.
  - `apply_conversion` handled the include-shim, asm move, clean Docker build, and ROM identity check after the manual same-TU prototype adjustment.
- Tooling/workflow gap:
  - `preflight_candidate` did not flag that `func_0800C0BC` calls an already-converted included-stub callee in the same TU, so the necessary prototype compatibility check was manual.
- Manual workaround:
  - Update the already-converted callee signature from `u16` to `s16` only after preserving its own byte shape with an explicit `(u16)` cast in the callee body, then apply the caller and let the clean ROM build prove the linked result.
- Desired tooling improvement:
  - When an included-stub candidate calls a converted same-TU function, surface the current callee prototype and note whether signedness may affect caller load instructions (`LDRH` vs `LDRSH`).

## Batch 109 — linked included-stub mini-batch
- Tools that helped:
  - `compile_and_view_asm` correctly proved isolated instruction matches for `func_08014E88` and `func_080152A0`.
  - `apply_conversion` safely performed the include-shim edits, asm moves, clean Docker builds, and ROM identity checks for each function.
- Tooling/workflow gap:
  - Chunk 29 initially misdiagnosed several candidates as callee-risk because the worktree had an unrelated manual edit in `src/decomp/asm_08012c64.c` from a failed `func_08014374` experiment. `apply_conversion` restored its own attempted edits, but it cannot know whether unrelated pre-existing manual edits are accidental.
  - `query_candidates` and `preflight_candidate` are useful but do not express linked-batch opportunities or dirty-worktree risk.
- Manual workaround:
  - Run `git status --short` before and after failed `apply_conversion` attempts.
  - Revert unrelated edits before retrying a 100% isolated match.
  - For linked included stubs, apply the callee first and verify ROM, then apply the caller.
- Desired tooling improvement:
  - Add a pre-apply clean-worktree warning or at least list unrelated dirty files before running the Docker build.
  - Add a candidate-family mode that suggests tiny callee-before-caller batches instead of only one-function candidates.

## Batch 133 — real-C-first / naked-asm follow-up
- Tools that helped:
  - `compile_and_view_asm` made it obvious when a candidate was only failing on register allocation or epilogue shape, which is exactly the point where a real-C pass is worth trying.
  - `make report` + `tools/gen_objdiff.py` confirmed the post-chunk baseline after the accepted `func_08001D5C` included-stub conversion.
- Tooling/workflow gap:
  - The loop prompt previously did not explicitly reserve time for revisiting existing naked asm files with a real-C attempt after the primary chunk work was done.
  - Some candidates that look like simple C rewrites still collapse into return-shape or scratch-register problems, so the prompt needs to steer agents toward "real C first, naked asm last" instead of treating naked asm as a default fallback.
- Manual workaround:
  - I updated `docs/decomp-agent-workflow.md`, `docs/decomp-pattern-library.md`, and the decomp-loop prompt to make the real-C-first rule explicit and to reserve spare capacity for revisting existing naked asm files.
- Desired tooling improvement:
  - Surface a lightweight reminder in chunk prompts when the candidate queue is thin: try one real-C conversion pass on an existing naked file before reaching for a new naked file or another manual asm wrapper.

## Batch 134 — task-wrapper real-C shape
- Tools that helped:
  - `compile_and_view_asm` confirmed the last-mile difference on `func_0800C110` was just epilogue shape and one missing return, not the stack layout.
  - `apply_conversion` preserved the host-TU include order correctly for the included-stub conversion.
- Tooling/workflow gap:
  - Task-launcher wrappers with on-stack structs are easy to under-specify when the return type is left as `void`; that hides the correct `POP {R1}; BX R1` epilogue.
- Manual workaround:
  - Use the `start_new_task` return value directly in the wrapper when the asm returns via `bx r1`, and keep the task arguments in a local struct so the compiler emits the same `strh` sequence.
- Desired tooling improvement:
  - Add a small reminder in the loop prompt for stack-struct wrappers: if the asm returns a value, try `return start_new_task(...)` before falling back to void/naked variants.

## Batch 135 — anti-ASM-cop-out guardrails
- Tools that helped:
  - `vcc_recall` surfaced the earlier prompt edits and the exact toxic failure mode: a near-miss compile result leading straight to naked asm.
  - The existing naked-asm real-C docs already had the right ingredients; the missing piece was a stricter loop prompt that treats naked asm as maintenance, not a fallback.
- Tooling/workflow gap:
  - The loop prompt still made the naked-asm maintenance pass feel optional and low-friction, which is too easy for weaker agents to abuse when a candidate is only a register-allocation mismatch away from perfect.
  - Included-stub real-C conversions can fail late on host-TU symbol collisions if helper typedef names are reused across files.
- Manual workaround:
  - Tightened the loop prompt and workflow docs to say: keep iterating on C when a candidate is close; never turn a near-miss into naked asm progress. Existing naked files are maintenance-only targets for removal.
  - Added a reminder to use unique helper typedef names (or anonymous structs) inside included-stub decomp files.
- Tooling implemented:
  - Added `.pi/extensions/warioware-decomp-guard.js` to block `compile_and_view_asm` / `apply_conversion` C payloads, `write` / `edit` changes, suspicious `bash` writes, and `git add` / `git commit` attempts that introduce or preserve naked/original asm in changed `src/decomp/*.c` files.
  - Added `decomp_guard_check` so agents can explicitly scan changed `src/decomp` files before committing.
- Desired tooling improvement:
  - Consider a lint/pass that flags duplicate helper typedef names across `src/decomp/*.c` includes before `apply_conversion` runs.

## Batch 135 cleanup — legacy wrapper burn-down
- Tools that helped:
  - The guard forced the right behavior: edited legacy files had to become real C before commit, and `decomp_guard_check` verified no changed `src/decomp` files still contained naked/original asm.
  - Full Docker ROM builds caught semantically-correct but byte-different rewrites (`& 1` vs shift pair, unpinned copy registers, header return-type ripple).
- Tooling/workflow gap:
  - Already-converted legacy files often cannot use `compile_and_view_asm` because their target asm object has moved; for those, clean Docker ROM builds remain the reliable verifier.
  - `make -j4` can pick up stale dependency state after failed include-shim experiments; removing `build/` before verification avoids false missing-source errors.
- Manual workaround:
  - Burned down 7 legacy wrappers to real C and shrank the guard exception list to the 7 wrappers that still need real-C work.
- Desired tooling improvement:
  - Add first-class support for comparing an already-converted `src/decomp/*.c` against its `asm/converted/*.s` target so legacy wrapper cleanup can iterate without full ROM builds.

## Batch 136 cleanup — reducing legacy quarantine to one file
- Tools that helped:
  - Full ROM verification caught operand-order differences (`r0 + r3` vs `r3 + r0`) and caller-side codegen ripples from changing helper return prototypes.
  - The guard's changed-file scan stayed useful while shrinking the legacy allowlist; converted files now need no legacy exception.
- Remaining blocker:
  - `func_0800BEC0` is the only naked/whole-function wrapper left. Pure C still hits the documented `CMP #1/BGE` vs `CMP #0/BGT` optimizer trap.
- Follow-up idea:
  - Decide whether a very small branch/compare inline-asm workaround is acceptable for `func_0800BEC0`, or keep it quarantined until a pure-C spelling is found.

## Legacy inline-asm reshaping pass (2026-08-04)
- The repo-local m2c/asmlift adapter was not needed for this maintenance pass; direct source/target instruction comparison plus the Docker agbcc build was the faster path for these already-understood wrappers. m2c/asmlift remains useful for discovering and synthesizing new functions, as recorded in the earlier re-hoist section.
- The repeatable solution was an ABI-shaping function-pointer typedef: widen `s16`/`s8`/`u16` helper parameters to `s32`/`u32` at the local call site so already-shaped ABI registers are not re-truncated or swapped. This converted the old non-empty BL shims to ordinary C calls across 25 included-stub files.
- Register-pinned C recovered the two indexed `LDRSH` loads and the exact operand order in `func_08012DCC`; ordinary C indirect dispatch reproduced `_call_via_r0`. A first `func_08012DCC` spelling missed by one byte, so the strict ROM gate caught and corrected the addition order.
- The same strict gate rejected pure-C attempts for the two-operand in-place `ADD` forms in `func_080141C8` and `func_08014DFC`, and the known `func_0800BEC0` compare trap remains. Those shims were restored rather than weakening the byte-matching rule.
- Metrics were unchanged at this pass's starting point because the 25 files were already C-linked included stubs: **1362 / 5960**, **6.4803877%** matched code, and **902 C / 5785 asm-only** objdiff units. The final clean Docker build still reported `wariowareinc.gba: OK`; the six legacy files listed there were then resolved by Batches 160–164 (real C) and Batch 165 (the honest asm-only SVC exception).

## Batch 160 — condition-code barrier for two-operand ADD (2026-08-04)
- A small isolated compiler probe showed that `asm volatile("" : "+r"(r2) : : "cc")` immediately before ordinary `r2 += 2` forces the exact Thumb `ADDS R2,#2`; the same empty barrier is allowed by the strengthened guard because it contains no instruction text.
- `func_080141C8` matched in the linked `main_menu.c` object and passed a clean `NONMATCHING=0` Docker build. This is a useful new shaping rule, while the analogous `R5 += 4` case remains unresolved.
- The maintenance count is now **26 reshaped files**, with five non-empty inline-asm files remaining.

## Batch 161 — callee-saved register ADD shaping (2026-08-04)
- The condition-code barrier pattern transferred unchanged from `R2` to callee-saved `R5`: ordinary `r5 += 4` emits `ADDS R5,#4` after `asm volatile("" : "+r"(r5) : : "cc")`.
- `func_08014DFC` passed the linked object check and clean Docker ROM gate. The maintenance count is now **27 reshaped files**, with four non-empty inline-asm files remaining.

## Batch 162 — compiler-generated STMIA (2026-08-04)
- A small pointer-loop probe showed that agbcc emits `STMIA` when the store pointer is a `u32 *` and the source uses `*r2++ = r1`; this was more effective than manually pinning an integer address and trying to model the store as a byte/word assignment.
- `func_08015A4C` matched through its literal pool and passed the clean Docker ROM gate. The maintenance count is now **28 reshaped files**, with three non-empty inline-asm files remaining.

## Batch 163 — pure-C generated-coordinate wrapper (2026-08-04)
- An isolated pure-C probe showed that non-volatile `s16` locals are important: `volatile` forces `LDRH` plus explicit sign-extension, while ordinary locals let agbcc select the target indexed `LDRSH` form. A typed `func_08006F84(s16, s16 *, s16 *)` declaration also reproduced the target `SB = SP+0xA` setup.
- `func_0800C15C` matched in the integrated bitmap-font object and passed the clean Docker ROM gate. The maintenance count is now **29 reshaped files**, with two non-empty inline-asm files remaining.

## Batch 164 — pure-C range-dispatch shaping (2026-08-04)
- The documented direct-comparison trap was not a fundamental blocker for a byte-valued switch. An ordinary `switch` with cases 1–3 and 4 still gave the wrong lower-bound-first tree; adding a redundant `case -10` that shares the default target makes agbcc retain the target upper-bound-first range dispatch without any non-empty inline asm. The case is unreachable because the switch value is loaded from a `u8` byte.
- `func_0800BEC0` matched byte-for-byte in the integrated bitmap-font object, including its literal-pool padding, and passed the clean Docker ROM gate. The maintenance count is now **30 reshaped files**, with only the BIOS `svc #6` wrapper still containing non-empty inline asm.
- This is a compiler-shaping workaround, so keep the value-domain justification beside the switch; do not generalize the redundant case to a signed value whose negative range is reachable.

## Batch 165 — standalone BIOS SVC exception (2026-08-04)
- The final legacy non-empty inline-asm file was `func_080EE61C`, whose exact target body is only `SVC #6; BX LR` (the GBA BIOS signed-division service).
- Direct C `a0 / a1` is not an exact substitute under the bundled agbcc: it emits a call to `__divsi3`. Probes for the usual ARM SVC/SWI builtins and attributes did not produce an SVC instruction; the installed devkitARM `libgba` has an exact `Div.o`, but that is also external assembly and would not turn this function into real C.
- m2c/asmlift therefore cannot solve this particular compiler/ISA boundary. The honest project form is a standalone `asm/asm_080ee61c.s` source with the original two instructions, while all `src/decomp` C files remain free of non-empty instruction asm.
- The clean Docker ROM gate passed with `wariowareinc.gba: OK`; both ROMs hash to `3f556448d290fa5406d6ed367fee16cc02387ad3`. The report moves from **1362 / 5960** to **1361 / 5960** and from **902 C / 5785 asm-only** to **901 C / 5786 asm-only** solely because objdiff no longer classifies this function as C-produced output.
- Durable rule: do not trade an honest asm-only BIOS exception for a fake C wrapper or a non-empty inline-asm shim merely to preserve the decompilation counter.

## Batch 166 — compiler-level BIOS SVC lowering (2026-08-04)
- The remaining wrapper is now real C. A target-specific `__builtin_swi_div()` was added to the bundled agbcc front end and expanded through a Thumb `define_insn` backend pattern; the source function itself contains no instruction-bearing inline asm.
- The builtin intentionally takes no source arguments: the BIOS service consumes the function's incoming `r0`/`r1` ABI values directly and returns its `r0` result. Passing the C parameters through an ordinary builtin call caused old agbcc to create stack home stores, so the fixed-ABI spelling is important.
- The isolated and integrated object both disassemble to exactly `SVC #6; BX LR` (`df06 4770`). The clean Docker ROM gate passed, with both ROMs at SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Reproducibility lesson: the compiler change is tracked in `tools/agbcc-swi.patch`, and `.github/workflows/report.yaml` applies it after cloning agbcc. This keeps the project C source honest without relying on a machine-local compiler binary or source-level asm escape.
- Tool assessment: m2c/asmlift correctly exposed the signed-division semantics, but the decisive step was extending the target compiler; adapters cannot repair a missing ISA lowering by themselves.

## Batch 167 — shifted scene-data accumulator siblings (2026-08-05)
- Candidate selection worked best by mining the report for tiny unmatched standalone objects and then grouping identical instruction families. `func_080B36B0`, `func_080C9BFC`, `func_080DA1A4`, and `func_080E1A80` share the same seven-instruction body with only the destination offset changing.
- The existing real-C pattern from `func_080C9520` transferred directly: cast both bases to `u8 *`, load the scene-data halfword at `0x16`, shift by three, and add it to the `u32` destination field. No register pins or barriers were needed.
- The first linker attempt exposed an important integration rule: replacing a linker-script entry is not enough while the old assembly file remains in `SFILES`; the original source must also move to `asm/converted/` so its object is no longer included in the global object list.
- All four linked C units matched 100%, and the clean Docker ROM gate passed with the baseline SHA-1 unchanged. This batch adds **4 matched functions**, moving the report to **1366 / 5960** and **22.919462%**.

## Batch 168 — large global-offset beatscript stores (2026-08-05)
- `func_0800CAA4` and `func_0800CAB8` were semantically simple halfword stores, but direct C pointer arithmetic caused agbcc to fold the large offset into the `gBeatscriptScene` relocation, producing only `LDR; STRH; BX` instead of the target’s separate global/offset loads and `ADDS`.
- Register-pinned C (`r1` base, `r2` offset) plus an empty compiler barrier restored the exact target instruction order and literal-pool layout. This remains source-level real C; the barrier contains no instruction text.
- The first full-build attempt also reinforced the integration rule that converted assembly must move to `asm/converted/`; leaving it in `SFILES` creates duplicate symbols even when its linker-script entry is replaced.
- Both linked units matched 100%, the strict ROM gate passed, and the report advanced to **1368 / 5960** and **22.953020%**.
## Batch 169 — graphics-buffer ordering (2026-08-05)
- m2c-style semantic translation was straightforward, but the first C spelling hoisted the literal load before the target address add in `func_0805CB5C`, causing a ROM mismatch despite similar isolated code.
- A statement-level empty barrier after `r0 += 0x54` restored the target order. The final four objects matched and the clean ROM gate passed.
- Lesson: for tiny register-sensitive stores, inspect the linked bytes—not just semantic output or a disassembly that omits literal-pool ordering.
## Batch 170 — nested scene-variable pointer siblings (2026-08-05)
- The two helpers share the same `gCurrentSceneVariable -> +0xC` pointer chain but differ in whether the final byte is zeroed or set to one.
- Register-pinned C pointers plus empty barriers reproduced both 20-byte objects exactly. This is a useful companion pattern for small state-field helpers; ordinary typed struct access is more likely to alter register allocation.
## Batch 171 — indexed large-offset store (2026-08-05)
- The earlier large-global-offset store pattern transferred to an indexed word store: pin the base in `r2`, shift the index before introducing the offset literal, then preserve `ADDS R2,R3` followed by `ADDS R0,R2`.
- The real-C object matched exactly and the clean ROM gate passed. This confirms the pattern extends beyond fixed halfword stores when the compiler-visible operand order is controlled.
## Batch 172 — explicit byte serialization (2026-08-05)
- Ordinary C pointer increments and explicit shifts were sufficient to reproduce both the unrolled writer and reader; no register pins or barriers were needed.
- The report confirms both new units at 100%, and the strict ROM gate passed. This is a useful easy family because the compiler's byte-width operations naturally select the target `STRB`/`LDRB` forms.
## Batch 173 — paired accumulator (2026-08-05)
- A direct C spelling of two offset-based `u32` accumulations reproduced the target's load/add/store sequence without pins or barriers.
- This is a low-risk family worth prioritizing when the source and destination offsets are regular and the compiler can retain the same base register.
## Batch 174 — scene-variable flag setter (2026-08-05)
- The established large-offset global pattern transferred directly with ordinary C pointer arithmetic; no register pins or barriers are needed for this setter.
- The object and full ROM matched exactly, confirming this is a productive family for nearby scene-state helpers.

## Conker tooling audit (2026-08-05)
- Ported the useful part of Conker's workflow: append-only attempt provenance plus keep-best near-miss seeds. The WarioWare recorder captures Git/ROM hashes and structured diff evidence while leaving source and acceptance state untouched.
- Deliberately did not port Conker's N64/IDO-specific harness, Transmuter, dashboard, or orchestrator. WarioWare already has a Docker/devkitARM gate and Mizuchi candidate tooling; those components would add unrelated compiler assumptions.

## First WarioWare near-miss record (2026-08-05)
- `func_08002038` was tested as ordinary C and failed the strict ROM gate with eight differing bytes in its 18-byte function window. The candidate was reverted, while `.nearmiss/func_08002038.json`, `.nearmiss/func_08002038.full.c`, and `tools/attempts.tsv` preserve the candidate, hashes, command, and localized diff.
- The restored baseline passed the clean Docker gate and matched SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.

## Batch 175 — normalized isolation and transactional batches (2026-08-05)
- The first paired graphics-buffer probe exposed a false raw-object near miss: both candidates emitted the exact 20 linked bytes, but the target assembly resolved `gGraphicsBuffer` through `gba.inc` while agbcc left the candidate literal as a relocation/common symbol. Raw objdiff therefore reported **88.888885%** and a missing literal-pool word even though the final linked bytes were identical.
- `tools/decomp_cycle.py isolate` now links the target and candidate objects at address zero inside the same Docker invocation. It extracts only the target object's absolute symbols as `--defsym` values, resolves candidate relocations/common symbols, and compares the resulting ELFs. Receipts identify this as `comparison: linked_elf`; raw-object comparison remains a fallback when normalization cannot be produced.
- `apply-batch` now applies every exact manifest entry under one rollback snapshot and runs one clean Docker ROM/report gate for the whole batch. The accepted `func_080A2524` + `func_080EE608` batch used one isolation container and one full build, passed the ROM SHA-1 gate, and advanced the report to **1381 / 5960**.
- `m2c` was useful for the semantic field-level hypothesis. `asmlift` correctly declined these Splat-disassembled GBA sources because its ARM frontend expects compiler-emitted GNU assembly; this is a tool-selection signal, not a candidate failure.
- The cycle unit suite now covers the linker normalization script and transaction path helpers (**7 tests**). Keep full ROM verification as the acceptance gate even when linked isolation is exact, because caller/callee and linker-order effects remain outside a single-function probe.

## Batch 176 — sibling batch throughput (2026-08-05)
- Candidate mining found four unmatched standalone functions sharing the Batch 175 graphics-buffer clear sequence. Three ended in `func_0800CDB0(1)` and one in `func_0800418C()`; existing C siblings supplied the exact prototypes and source spelling.
- One `isolate` invocation scored all four candidates at **100.0%** with `comparison: linked_elf`, and one `apply-batch` full-ROM transaction accepted all four. This confirms the new loop can safely amortize the Docker startup/build cost across a small linked family.
- The report advanced from **1381** to **1385** matched functions with the baseline ROM SHA-1 unchanged. Keep batches sibling-rich but semantically narrow; the four functions had identical clear ordering and only a known final call difference.

## Batch 177 — exact offset screen with preserved near misses (2026-08-05)
- A six-candidate isolation screen found two exact real-C candidates: `func_0808BD98` (large scene-variable halfword store) and `func_080AAA40` (indexed scene-variable halfword store). The other four remained isolated near misses and were not applied.
- `func_080AAA40` confirms that the manifest candidate can preserve a split `index << 1` plus `0x83 << 2` computation when the base load and additions are written as separate C statements.
- The exact pair passed one `apply-batch` transaction and moved the report from **1385** to **1387** matched functions. This is the intended loop behavior: collect several probes in one container, accept only exact results, and retain the rejected hypotheses for later shaping rather than mixing them into the ROM transaction.

## Batch 178 — near-miss shaping loop (2026-08-05)
- The preserved near-miss receipt localized four mismatches to codegen choices: wrong accumulator destination (`func_080C4A48`), repeated zero/literal handling (`func_080195E4`), register-role allocation (`func_080DF440`), and constant hoisting/reload collapse (`func_080EC308`).
- Register-pinned readable C solved `func_080C4A48` and `func_080EC308` in the next isolation pass. `func_080DF440` still has a near miss and `func_080195E4` still has literal-pool/code-order drift; neither was applied.
- The two exact variants passed one `apply-batch` transaction and advanced the report from **1387** to **1389** with the ROM SHA-1 unchanged. This validates the intended provenance loop: use the recorded localized diff to select the next C permutation, then re-score before any full build.

## Batch 179 — wrapper/reload shaping (2026-08-05)
- The next isolation manifest combined four focused permutations: a non-void callback wrapper, delayed zero initialization, pinned scene-variable reloads, and a call-then-clear wrapper. All four reached **100.0%** in one normalized linked-ELF pass.
- The full transaction accepted all four and advanced the report from **1389** to **1393** without ROM drift. The most useful feedback was the isolated epilogue mismatch on `func_0809C47C`: changing only the declaration from `void` to non-void corrected `POP {R0}` to the target `POP {R1}`.

## Batch 195 — runtime-table ordering screen (2026-08-05)
- The eight-entry screen amortized candidate compilation correctly: one Docker isolation invocation produced two exact results and six recorded near misses. The exact-only `apply-batch` reused the same receipt and paid for one full Docker ROM gate.
- `func_080F0DFC` and `func_080F2358` converted cleanly to real C and advanced the report from **1493** to **1495** with the baseline ROM SHA-1 unchanged. The source files use register-bound C locals only; there is no instruction-bearing or volatile inline asm.
- The six rejected runtime-table siblings exposed a reusable ordering gap that isolated objdiff made obvious: independent C locals allowed the `arg0+0x18` base load to move before the target's `LSLS/LSRS #24` normalization. `func_080F253C` also demonstrated the known constant-folding trap (`MOVS #2; RSBS` became `SUB #3`), and `func_080F2558` exposed a register-role mismatch around the field/literal loads.
- The cycle and provenance tooling helped with admission and evidence but did not synthesize a fix for the ordering gap. The next attempt should test only a small barrier/dependency permutation family, keep every non-exact result in `.nearmiss/`, and continue to reject instruction-bearing asm.

## Batch 196 — ordering repair follow-up (2026-08-05)
- A six-entry follow-up screen tested the smallest documented repair: an empty memory barrier after argument normalization, plus the R0/R3 register-role correction for the word-field sibling. Five candidates became exact in the same isolated Docker pass; `func_080F253C` remained a near miss solely because the compiler still canonicalizes the target `MOVS #2; RSBS` mask into `SUB #3`.
- The five-entry exact-only transaction passed one full Docker ROM gate and advanced the report from **1495** to **1500**. The cycle's receipt reuse avoided a second isolation container, while the final push hook still supplied the remote full-ROM invariant.
- This validates the workflow's role split: m2c/manual shaping supplied the semantic candidates, the isolated cycle localized the codegen gap, and the strict ROM gate admitted only the exact subgroup. The empty barriers are compiler metadata only; no instruction-bearing or volatile inline asm was added.

## Batch 197 — leaf screen and adapter comparison (2026-08-05)
- m2c supplied accurate semantic skeletons for `func_08003FB8`, `func_08006CC8`, `func_08006EE0`, `func_080F1B5C`, and `func_080F1FB4`. asmlift was useful as a comparison, but its generic pointer signatures were wrong for the project-global graphics/D03000528 candidates; it did not replace project-aware manual shaping.
- The isolated diff on `func_080F1FB4` directly exposed a control-flow-layout mismatch: the first candidate emitted `BHI` to the constant-return path, while the target uses `BLS` to a later constant-return block. Reordering the C early return closed that gap exactly.
- `func_080F1B5C` and `func_080F1FB4` passed one combined exact-only transaction and advanced the report **1500 → 1502**. The three global-buffer candidates remain evidence-only because their instruction bodies were accompanied by literal-pool width/symbol-boundary differences; strict admission correctly kept them out.

## Batch 198 — branch/epilogue shaping (2026-08-05)
- m2c/asmlift exposed three useful candidate skeletons. The isolated cycle then separated semantic correctness from codegen shape: `func_08016F60` needed the `!= 0` source branch, while `func_080F2C68` needed a widened return type to preserve the target's already-normalized counter register.
- `func_080F282C` matched after using the absolute `0x03000E78` address value instead of adding a new undefined-symbol map entry; the empty barrier kept input normalization before the global load and emits no instructions.
- The final three-entry exact-only apply passed the full Docker gate and advanced the report **1502 → 1505**. This was another case where m2c was directionally correct, asmlift was useful for comparison, and the isolated diff identified the minimal manual C-shape change.

## Batch 199 — register-copy and constant-folding follow-up (2026-08-05)
- The five-entry screen found exact initial spellings for `func_0801B174` and `func_08062488`; `func_0801C2D4` became exact after widening its byte argument to preserve raw ABI bits; and `func_0801F698` became exact after an empty `"+r"` output constraint prevented R3/R0 coalescing. The accepted four-entry transaction advanced the report **1505 → 1509**.
- Round 50 confirmed the RSBS blocker is not fixed by merely writing the first bit-mask as a direct expression: `func_080F253C`, `func_080F2598`, and `func_080F25B8` all still become `SUB` sequences. This is now a deliberate blocked family, not an unrecorded failed attempt.
- `func_080047D4` remains a useful literal-pool research seed: its instructions are close, but the candidate's absolute address pool and alignment shift the inferred symbol boundary. Strict admission correctly withheld it.

## Batch 200 — scene-state sibling sweep (2026-08-05)
- A five-entry m2c/manual screen for scene-state zero/setter helpers produced **5 exact / 0 near miss** results in one Docker isolation invocation. Existing register/offset patterns transferred directly, including the non-sequential reloads in `func_080D2768` and `func_080D286C`.
- The exact-only transaction passed one full Docker ROM gate and advanced the report **1509 → 1514**. This is a high-throughput family: the candidate generator needed no asmlift-specific repair and no inline instruction shim; the isolated cycle plus strict gate handled admission.

## Batch 201 — beatscript/runtime screen and symbol-boundary feedback (2026-08-05)
- Three isolated passes over the same five candidates were cheaper than full builds and localized the useful fixes: a destination-register reassignment, an empty output constraint that blocks a mask fold, and explicit absolute-symbol/operand-order shaping. The final screen classified **3 exact / 2 near miss**, and one exact-only full-ROM transaction accepted the three winners.
- The two retained misses demonstrate different limits. `func_08004770` has the right semantic byte-pair predicate but ordinary C does not retain its unusual leaf `PUSH {LR}` / `POP {R1}; BX R1` ABI shape. `func_08006148` has matching instruction/pool bytes through the return, but linked-ELF symbol inference counts the candidate's compiler literal pool inside the function while the target's local pool label ends the symbol earlier. The strict standalone path correctly withheld both rather than using `--force`.
- The runtime-table winner exposed a workflow detail: `include/undefined_syms.inc` can already know a `D_0300XXXX` address while `undefined_syms.ld` does not. The apply preflight caught this before mutation; adding the three canonical linker assignments and rerunning with the intentional map edit explicitly acknowledged let the full gate verify the integrated result.
- The accepted C contains no instruction-bearing or volatile inline asm. The only asm syntax is an empty `"+r"` compiler constraint, which contributes no bytes. m2c/manual shaping supplied the final candidates; asmlift was not needed for this small direct-wrapper family, and the isolated cycle plus ROM gate remained the deciding tools.

## Batch 202 — graphics/scene/DMA screen and metadata-only force (2026-08-05)
- m2c and asmlift both supplied useful semantic skeletons for the five candidates, but neither adapter was sufficient as the final source: project headers, absolute symbol declarations, register-bound locals, and manual statement ordering were required. The isolated diff then made the repairs cheap: moving `r1 = 0` before the first store fixed `func_080186AC`, while delaying the DMA table-symbol assignment until after the first store fixed the instruction order in `func_08002620` and `func_0800774C`.
- The final Round 54 evidence split was **3 exact / 2 near miss**. The two DMA near misses had instruction-identical bodies and fully accounted pool bytes; their only gap was compiler-versus-target function-symbol coverage over the literal pool. A first attempt to reuse the v2 receipt with `--force` was correctly rejected because receipt reuse requires exact entries. Re-running a fresh screen and then applying with `--force` admitted the documented metadata-only exception; the clean full Docker gate remained authoritative.
- One `apply-batch` transaction converted all five and passed `wariowareinc.gba: OK`, preserving ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. The sources contain no instruction-bearing or volatile inline asm; the DMA `volatile` uses are ordinary C memory accesses, not asm blocks. Receipts and near-miss candidates remain in `.decomp-runs/`, `.nearmiss/`, and `tools/attempts.tsv`.
- Fresh report: **1522 / 5956**, **25.554064%**, **6.9363704% matched code**, **1062 C / 5625 asm-only** units, and **1244** decomp files (`1043 standalone_tu` + `201 included_stub`).

## Round 55 — table-copy screen and byte-audited metadata force (2026-08-05)
- m2c supplied the table-copy and bounded-byte-copy skeletons; manual register-pinned pointer shaping transferred the proven `GraphicsTable` goto-loop pattern. The first screen compiled six candidates in one Docker invocation. Five had complete `.text` bodies but were reported as symbol-boundary near misses; `func_08007AD4` also had a genuine two-byte `R4`/`R5` prologue-order mismatch.
- A separate byte-audit extracted each candidate and target `.text` section with `arm-none-eabi-objcopy`. `func_08002FC0`, `func_08002FE8`, `func_08003028`, `func_08003040`, and `func_08003058` were equal at 40, 44, 24, 24, and 28 bytes respectively; the SHA-256 pairs are preserved in `.decomp-runs/20260805T-round-55-bytecheck.json`. `func_08007AD4` remained evidence-only because its two differing bytes were real instructions, not metadata.
- `func_08003058` initially failed compilation when its caller prototype narrowed incoming byte arguments. Widening the C form to `u32` and using an old-style declaration for the already-converted initializer preserved the target's untouched incoming R1–R3 registers. This is a useful ABI-preservation pattern for wrappers that forward inherited registers.
- The five-entry `apply-batch --force` transaction passed the clean Docker ROM/report gate with `wariowareinc.gba: OK`, ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`, and report progress **1522 → 1527** matched functions. The accepted sources contain no instruction-bearing or volatile inline asm; the round's `volatile`-free source-level register bindings and old-style call are ordinary C/compiler metadata.
- Fresh report: **1527 / 5951**, **25.659552%**, **6.9522724% matched code**, **1067 C / 5620 asm-only** units, and **1249** decomp files (`1048 standalone_tu` + `201 included_stub`).

## Round 56 — standalone global/scene screen (2026-08-05)
- The ld-backed inventory avoided a false queue: many low-address assembly files are already real C included stubs and do not increase linked coverage. Screening only objects still listed as `build/asm/*.s.o` selected four genuine standalone candidates.
- m2c/asmlift supplied useful semantic hypotheses, but the final exact spellings were project-aware manual C: `func_08024E34` needed a pointer-valued `D_083C8B64` declaration and `func_08030F9C` needed separate scene/table locals with staged `arg1 * 0xE + arg0` arithmetic. The screen classified **2 exact / 2 near miss** in one Docker invocation.
- `func_080020FC` demonstrates a new branch-layout seed: direct null-check C was semantically correct but emitted `BEQ` plus an extra zero-return block rather than the target's `BNE` fall-through. `func_08035FEC` demonstrates a register/zero-order gap: natural pointer-relative stores omitted the target's `MOVS #0` and `IP` preservation. Both were retained as evidence only.
- The apply preflight caught that `D_083C8B64` was present in `include/undefined_syms.inc` but absent from `undefined_syms.ld`; adding the one canonical linker assignment allowed the exact pair to pass the integrated gate. The full Docker ROM/report transaction advanced the report **1527 → 1529** and kept ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- No instruction-bearing or volatile inline asm was introduced. The cycle receipts and near-miss ledger record the four-way screen and exact-only apply; the post-apply Docker `make report` plus explicit `gen_objdiff.py` refresh produced **1069 C / 5618 asm-only** units.

## Round 58 — task-pool scan/cancel siblings (2026-08-06)
- m2c exposed the common `D_030006A0` task-slot semantics and the `0x1C` stride. asmlift declined the overlapping byte/halfword field reconstruction for `func_08005920` and failed to score the first `func_080059E4` normalization, which was useful evidence about its struct-model limits rather than a candidate failure.
- Isolated C permutations localized the codegen gaps: `func_08005920` needed goto-shaped result/entry blocks plus register-bound `R0`/`R1` probe values; `func_080059E4` needed a split raw `LSLS`/later `LSRS` sequence and an ordinary local constrained into the saved `R7` slot. The final linked `.text` hashes were exact even though normalized symbol coverage remained zero because the target's local literal-pool symbol ends early.
- `apply-batch --force` cannot reuse a non-exact isolation receipt by design, so the transaction reran one fresh two-entry isolation and then paid for one full Docker ROM gate. It accepted both functions, advanced the report **1530 → 1532** and matched code **69160 → 69292**, and kept ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Explicit post-apply Docker `make report` and host `python3 tools/gen_objdiff.py` refreshed **1072 C / 5615 asm-only**. The accepted C uses no instruction-bearing or volatile inline asm; register-bound declarations are compiler metadata only.

## Round 59 — task-pool mutation sibling sweep (2026-08-06)
- m2c supplied six useful semantic skeletons for the already-proven `D_030006A0`/`0x1C` task-slot family. asmlift was deliberately not added to this pass: its Round 58 overlapping-field reconstruction had already declined/failed, while the new candidates differed only by scan predicate, `task_stop` mode, or state-field offset. This kept discovery within one focused sibling batch without sacrificing evidence.
- The cycle's zero-filled `.align 2, 0` normalization mattered. The two candidates whose bodies end at a `2 mod 4` boundary were byte-different with the standalone helper's default assembler tail but became exact after reproducing `decomp_cycle.py`'s normalization. This is now recorded in the linked bytecheck receipt, not left as an informal compiler-tail assumption.
- All six normalized isolation results remained reported `near_miss`/`0%` because the legacy target symbol boundaries stop before the literal-pool bytes. The direct linked `.text` audit proved complete target/candidate equality for every function, so the forced transaction waived metadata classification only. The fresh isolation plus one full Docker gate accepted all six and advanced the report **1532 → 1538**, matched code **69292 → 69632**, and linked C units **1072 → 1078** while preserving ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Explicit post-apply Docker `make report` and host `python3 tools/gen_objdiff.py` refreshed **1078 C / 5609 asm-only**. The accepted sources use ordinary C and register-bound declarations only; no instruction-bearing or volatile inline asm was introduced. Durable evidence is in `.decomp-runs/20260805T-round-59-isolation.json`, `.decomp-runs/20260805T-round-59-linked-bytecheck.json`, `.decomp-runs/20260805T-round-59-apply.json`, `.nearmiss/`, and `tools/attempts.tsv`.

## Round 57 — scene-table setter sibling (2026-08-05)
- The exact `func_08030F9C` getter transferred directly to `func_08030F7C` setter; the same scene/table locals and staged index arithmetic matched. m2c/asmlift recognized the family, while project-aware C spelling remained the deciding artifact.
- One isolated candidate scored exact and one `apply-batch` full Docker ROM gate accepted it. The report advanced **1529 → 1530**, matched code **69128 → 69160**, and ROM SHA-1 stayed `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- No instruction-bearing or volatile inline asm was introduced. Explicit post-apply Docker `make report` and `python3 tools/gen_objdiff.py` refreshed **1070 C / 5617 asm-only**.

## Round 60 — heap/task helper screen (2026-08-06)
- m2c supplied usable skeletons for `func_08005B40`, `func_08005F64`, and `func_08005DE0`; asmlift provided a useful semantic struct view for the first two but declined the stack-frame/address-taken helper. Manual statement/register shaping made `func_08005F64` exact while `08005B40` and `08005DE0` remained genuine instruction-order/register near misses and were not applied.
- The screen showed that an empty nonvolatile `"+r"` output constraint can preserve the initial `ADDS R4,R0,#0` dependency in the heap-record allocator without adding instruction text. This is compiler metadata only; it is not a volatile or instruction-bearing asm workaround. The v3 receipt records the exact `08005F64` result and the rejected candidates remain in `.nearmiss/`.

## Round 61 — wrapper and allocator transaction (2026-08-06)
- m2c and asmlift both recognized the `0800200C` dispatcher and `080041B4` global-flag wrapper; project-aware declarations were still required for the converted C callees and `D_03000684`. The linker preflight caught that the symbol was present in `include/undefined_syms.inc` but absent from `undefined_syms.ld`; adding the canonical assignment fixed the integration gap before the ROM transaction.
- Normalized isolation classified `0800200C` and `080041B4` as metadata-only near misses, while `08005F64` was exact. The independent linked `.text` audit proved all three byte-equal (24/24/60 bytes), so `apply-batch --force` was used only with a fresh isolation run and the documented metadata exception. The clean Docker build/report gate accepted all three and advanced **1538 → 1541** matched functions without changing ROM SHA-1.
- The accepted sources contain no instruction-bearing or volatile inline asm. `08005F64` retains one empty nonvolatile `"+r"` compiler constraint; it emits no bytes. The lifecycle still produced isolation, bytecheck, apply, report, objdiff, near-miss, and attempt-ledger evidence, which is the useful division of labor for this family.
## Round 62 — soundplayer wrappers and linked-text boundary audit (2026-08-06)
- m2c supplied the useful semantic skeletons for the `08002038`/`0800207C`/`080020E0` soundplayer wrappers and the `08006148` bootstrap record. asmlift was useful as a second diagnostic path but declined the raw `.short`-bearing wrappers or failed to compile the project-specific prototypes; it did not replace the project-aware C shaping.
- The first corrected candidate screen exposed the same legacy symbol-boundary artifact seen in prior rounds: normalized linked objdiff reported the epilogues/pool coverage as inserted even though the full linked `.text` sections were equal. A persistent Docker artifact pass extracted complete target/candidate `.text` sections and proved equal SHA-256 pairs for **20**, **20**, **28**, and **44** bytes. The v2 candidates were selected because the alternate E0 spelling added an unnecessary `.rodata` section despite equal code bytes.
- `apply-batch --force` reran a fresh four-entry isolation and then ran one rollback-capable full Docker ROM/report gate. It accepted all four with `wariowareinc.gba: OK`, preserving SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. `make report`, `gen_objdiff.py`, and the policy scan then refreshed **1545 / 5937**, **1085 C / 5602 asm-only**, and **1267** decomp files.
- Accepted sources contain no instruction-bearing or volatile inline asm. `func_08006148` uses only register-bound declarations (`asm("r0")` etc.) as compiler allocation metadata; the accepted source emits ordinary compiler-generated instructions. Durable evidence is in `.decomp-runs/20260805T-round-62-{isolation,v2-isolation,linked-bytecheck,apply}.json`, `.nearmiss/`, and `tools/attempts.tsv`.

## Round 63 — standalone candidate inventory and bitmap wrapper cleanup (2026-08-06)
- m2c and the asmlift adapter supplied readable semantic skeletons for the short wrappers. The productive discovery step was a repo-local inventory of raw `glabel func_XXXXXXXX` files: Mizuchi's refreshed index scanned **5618** normalized asm files, but its ARM parser did not enumerate the repository's raw root `glabel`/`.thumb_func` standalone style. The inventory found **4196** standalone candidates, so future screening should use that parser until Mizuchi gains this syntax support.
- One focused screen classified six standalone candidates exact, four standalone candidates as near misses, and one as a compile error. Only the exact six entered the full-context transaction; the rejected hypotheses remain receipts/near-miss/attempt-ledger evidence. This was materially cheaper than paying for a full ROM build per permutation while preserving a single final ROM gate for the accepted batch.
- Included `func_0800C2E4` and `func_0800C5A0` exposed two durable C-shaping rules. Under `-mthumb-interwork`, a `void` return emitted `POP {R0}; BX R0`, while a widened `u32` return emitted the target `POP {R1}; BX R1`. Existing host-TU prototypes conflicted with the raw ABI needed by the target call; a unique local function-pointer typedef with widened `s32` parameters preserved the call registers without redeclaring the callee or embedding instruction asm.
- The bitmap candidates were near-exact in normalized isolation only because the linked comparison represented one relocated BL differently; the forced transactional full-ROM gate passed and the ROM remained byte-identical. This is acceptable only for a documented relocation/metadata artifact; it is not permission to waive a real instruction difference.
- The accepted batch added six matched standalone functions but only two included files. Fresh report/objdiff output is **1551 / 5937**, **1091 C / 5596 asm-only**, and **1275** decomp files (`1072 standalone_tu` + `203 included_stub`). Accepted sources contain no instruction-bearing or volatile inline asm.

## Round 64 — scene-slot and sprite-wrapper screen (2026-08-06)
- The raw-`glabel` inventory again found the useful candidates. m2c produced direct C skeletons for the key/scene leaves, sprite wrappers, and `0x20`-stride scene-slot family. asmlift successfully scored the `func_08022010` shape (9 instruction differences in its generated semantic candidate); the other short wrappers reached asmlift's project-header/prototype compile boundary and still yielded useful m2c output.
- One isolation container screened eleven candidates: **7 exact / 2 near miss / 2 exact-but-deferred**. The exact seven were applied as one transaction, while `func_08022070` and `func_0804F464` remain rejected near-miss evidence. The deferred exact candidates reference `D_083FC170` and `D_08124E38`, which are known in `include/undefined_syms.inc` but absent from `undefined_syms.ld`; adding those assignments must be part of a clean, rollback-aware transaction.
- The accepted C sources use no instruction-bearing or volatile inline asm. Full Docker verification advanced **1551 → 1558** matched functions and **1091 → 1098** linked C units while preserving ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. Receipts: `.decomp-runs/round-64-isolation-v1.json` and `.decomp-runs/round-64-apply-v1.json`.

## Round 64b — numeric ROM-address fallback (2026-08-06)
- The two exact Round 64 candidates that referenced `D_083FC170` and `D_08124E38` could not enter the normal transaction because the standalone linker map does not define those symbols. Re-spelling the same accesses as numeric absolute addresses produced exact linked isolation for both without adding a linker assignment.
- The full two-entry transaction passed with `wariowareinc.gba: OK`, advancing **1558 → 1560** matched functions and **1098 → 1100** linked C units. This is a proven fallback only when linked isolation and the full ROM gate both agree; it does not override the general relocation/symbol-address trap.
- Accepted sources contain no instruction-bearing or volatile inline asm. Receipts: `.decomp-runs/round-64b-isolation-v1.json` and `.decomp-runs/round-64b-apply-v1.json`.

## Round 65 — gameplay wrapper screen (2026-08-06)
- m2c supplied direct skeletons for the eight short gameplay/call-wrapper candidates. asmlift again reached its project-header/prototype compile boundary for the wrappers, so it was retained as a diagnostic signal rather than used to invent source; the raw-`glabel` inventory selected the batch efficiently.
- One isolation pass produced **5 exact / 2 near miss / 1 compile error**. The exact five entered one transaction; the rejected results remain in the near-miss ledger and receipt. Numeric ROM-address spellings continued to work for constants whose symbolic names are not in the standalone linker map.
- Full Docker verification advanced **1560 → 1565** matched functions and **1100 → 1105** linked C units, preserving ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. Accepted sources contain no instruction-bearing or volatile inline asm.

## Round 66 — scene-variable leaf screen (2026-08-06)
- m2c/manual sibling shaping supplied compact candidates for the scene-variable, sound/RNG, and indexed-store family. The prior numeric-address fallback was reused for ROM song constants and the hardware-register read; no new linker assignment was required. asmlift was not needed to decide this direct-wrapper batch, reinforcing that it is most valuable as a diagnostic/alternative skeleton when the C shape is ambiguous.
- One isolation container screened nine candidates and classified **8 exact / 1 near miss**. The exact-only apply transaction passed the clean Docker ROM gate; `func_080D3A60` was kept in `.nearmiss/` because its natural `0xDF << 2` post-call store still emitted a different instruction sequence.
- The accepted sources are ordinary C with no instruction-bearing or volatile inline asm. The lifecycle produced isolation/apply receipts, near-miss evidence, and attempt-ledger provenance; post-apply Docker `make report`, host `gen_objdiff.py`, and policy checks refreshed **1113 C / 5574 asm-only** units and **1573 / 5937** matched functions while preserving ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.

## Round 67 — beatscript and key-wrapper repair screen (2026-08-06)
- m2c supplied accurate skeletons for all seven compact candidates; asmlift was run as a comparison path and either failed at the project-header/prototype boundary or declined stack-address-taken wrappers. The useful repairs came from the isolated diff, not from relaxing the admission rule.
- The first isolation pass classified **3 exact / 3 near miss / 1 compile error**. A second pass fixed the missing `scenes.h` declaration and branch layout, and a third pass added the documented empty compiler constraint that prevents `3`/negation folding. The fourth pass classified **7 exact / 0 rejected**.
- `func_080166E4` initially appeared as a symbol-boundary near miss with identical instruction bytes but a one-byte literal-pool difference: the candidate callback pointer lacked the target's Thumb `+1` addend. Spelling `(void (*)(u32))(set_pause_beatscript_scene + 1)` produced the exact pool word and removed the need for any metadata-only waiver.
- The accepted C contains no instruction-bearing or volatile inline asm. `func_0800CDB0` has one empty nonvolatile `"+r"` constraint; it contributes no bytes and only preserves the compiler-visible mask dependency. Full Docker verification advanced **1573 → 1580** matched functions and **1113 → 1120** linked C units, preserving ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.

## Round 68 — scene wrapper and copy-loop fan-in (2026-08-06)
- The raw-root `glabel` inventory and m2c/asmlift comparison selected a compact nine-function family: three `func_080DF224`/`func_080DF28C` scene-data wrappers, three scene-byte predicates, a two-field clear, and byte/halfword copy loops. asmlift scored the predicates but declined the loop headers; m2c supplied the correct loop skeletons.
- One isolation container initially classified **6 exact / 1 near miss / 2 compile errors**. `func_080D74D0` was semantically right but used the opposite boolean branch layout; rewriting it as `if (p[0x43A] == 0) return 1; return 0;` reproduced the target `BEQ` fall-through and return constants. The loops initially failed because `types.h` depends on typedefs from `global.h`; adding that project header fixed both without changing the loop body.
- The repaired isolation receipt classified **9 exact / 0 rejected**, and `apply-batch` reused that exact receipt for one transactional full-ROM gate. The build emitted `wariowareinc.gba: OK`; report/objdiff/policy checks passed and ROM SHA-1 stayed `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Accepted sources contain ordinary C plus only register-bound compiler metadata in `func_080C69CC`; there is no instruction-bearing or volatile inline asm. The first predicate spelling is retained in `.nearmiss/func_080D74D0.*`, with the two isolation receipts recording the correction and the attempts ledger carrying the near-miss score.
- The batch advanced **1580 → 1589** matched functions, **1120 → 1129** linked C TUs, and **1304 → 1313** decomp files. This validates that copy loops are viable candidates when the project header context is supplied, while branch-direction screening remains essential for one-return boolean wrappers.

## Round 69 — scene-state leaf repair screen (2026-08-06)
- The raw-root inventory and m2c/asmlift comparison found ten compact pure-C candidates. asmlift scored the simple update/predicate shapes but did not solve the project-specific register roles; m2c supplied the semantic skeletons and the isolated diffs supplied the final source order.
- The first screen had three exact candidates and seven candidates needing repair. Reversing boolean source tests recovered the target `BEQ`/`BNE` fall-through layouts, pinning the result/factor registers fixed `0809E804`, spelling `r0 = r2 + r0` restored the target three-operand add in `080526D0`, and raw register sequencing recovered `0806F0A0` and the indexed byte fallback. The final screen classified **9 exact / 1 near miss**.
- `func_080DD8A4` is a useful new blocked seed: the target reloads the global pointer in R1, uses R1 as the indexed halfword offset, then reuses R1 for the accumulator. Multiple ordinary-C/register-scope and empty-constraint attempts preserved the body semantics but agbcc selected R0 for the offset or materialized an address add. It remains outside the transaction; its best near-miss source and all v1–v8 receipts are retained.
- The exact nine-entry apply used one fresh full-context Docker gate and passed `wariowareinc.gba: OK`; report/objdiff/policy checks passed and ROM SHA-1 stayed `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- Accepted sources contain ordinary C plus register-bound compiler metadata only, with no instruction-bearing or volatile inline asm. The batch advanced **1589 → 1598** matched functions, **1129 → 1138** linked C TUs, and **1313 → 1322** decomp files. Five more matched functions reach the active 27% goal.

## Round 70 — final five-function screen to 27% (2026-08-06)
- The raw-root inventory plus asmlift/m2c comparison selected five compact real-C candidates: a byte-copy loop, two scene predicates, a clamp/changed-flag helper, and a scene-time threshold predicate. asmlift scored `func_0805C5D8` and `func_08088B80` as exact starting skeletons; m2c supplied the project-aware pointer and branch semantics for the others.
- The first isolated pass classified **2 exact / 3 near miss**. The exact diff localized three compiler-shape repairs: equality-as-early-return for `0806EC7C`, less-than-or-equal as the taken threshold path for `0809C0C0`, and register-bound input/difference locals for `08089648`. The second pass reached **5 exact / 0 rejected**.
- This is a useful division of labor: asmlift accelerates semantic screening, while isolated objdiff remains the authority for branch polarity, literal-pool placement, and register order. The three rejected first-pass spellings are preserved in `.nearmiss/`, and `tools/attempts.tsv` plus both isolation receipts keep their provenance.
- The exact-only apply and an explicit clean Docker rebuild both emitted `wariowareinc.gba: OK`; the ROM and baserom SHA-1s are `3f556448d290fa5406d6ed367fee16cc02387ad3`. Host `make report`/`gen_objdiff.py` and policy checks refreshed **1603 / 5937** matched functions and **1143 C / 5544 asm-only** units.
- Accepted sources contain ordinary C and register-bound compiler metadata only. No volatile or instruction-bearing inline asm was introduced. The batch advanced **1598 → 1603** matched functions, **1138 → 1143** linked C TUs, and **1322 → 1327** decomp files, reaching the active 27% goal; 30% is now **179** matches away.

## Round 80 — strict ordinary-C main-menu screen (2026-08-07)
- The raw-root inventory selected four linked main-menu helpers: `func_08016D00`, `func_08016DB8`, `func_08016798`, and `func_08016850`. m2c recovered the call/branch/global skeletons; asmlift was run for comparison but its project compiler adapter stopped at the expected Splat/project-header boundary, so no generated lift was admitted.
- The first isolation pass exposed only integration/header issues. After adding the project header context, the screen reached **3 exact / 1 branch-layout near miss**; spelling the inactive path first as ordinary C (`if (inactive != 0)`) reproduced the target `BNE` fall-through and produced **4 exact / 0 rejected**. The near miss remains recorded rather than being force-applied.
- `tools/audit_decomp_source.py --strict` reports zero instruction asm, empty barriers, and compiler register pins across all four candidates. It reports three explicit packed-scene accesses as evidence: `gCurrentSceneData + 8`, `gCurrentSceneData + 0x3A`, and the current-scene byte store. This is the intended quality split: real C is mandatory, while incomplete struct recovery remains visible.
- Apply preflight found `D_083FBB44` in `include/undefined_syms.inc` but not in `undefined_syms.ld`; adding the canonical assignment in `fc3419a6` passed a separate full Docker SHA gate and avoided a magic numeric-address fallback. The exact batch then passed the transactional full Docker ROM/report gate, advancing **1662 → 1666** matched functions, **1202 → 1206** linked C TUs, and **1386 → 1390** decomp files with unchanged ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- The 27-test tool suite, strict source audit, policy check, `make report`, and `gen_objdiff.py` all passed after apply. Durable receipts are `round-80-isolation-v1` through `v5`, `round-80-linker-verify`, and `round-80-apply`.

## Round 81 — strict callback/sound wrapper screen (2026-08-07)
- The sibling pass selected `func_08016B4C`, `func_08016B88`, and `func_08016BC4`. m2c recognized the callback shape; asmlift was run as a diagnostic but again stopped at its project-header compile boundary and supplied no admitted source.
- The initial ordinary-C candidates had the correct bytes for the no-data wrapper and warning-only compile failures for the two callback-data wrappers because `sprite_set_callback` declares its fourth slot as `u32`. Adding the explicit `(u32)&D_083FF654/67C` address conversion fixed the project ABI without asm. The final screen classified **3 exact / 0 rejected**.
- The strict audit found zero instruction asm, barriers, or register pins. It reports only the two typed callback-address conversions as raw pointer evidence; no numeric ROM address or fragile function-body pointer arithmetic was used.
- Apply preflight required the two symbols to be added to `undefined_syms.ld`; commit `75c4148d` passed a separate full Docker SHA gate before the batch transaction. The exact-only transaction passed `wariowareinc.gba: OK`, advancing **1666 → 1669** matched functions, **1206 → 1209** linked C TUs, and **1390 → 1393** decomp files with ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3` unchanged.
- Post-apply `make report`, `gen_objdiff.py`, policy, source audit, and all **27** tooling tests passed. Receipts: `.decomp-runs/round-81-isolation-v1.json` through `round-81-isolation-v3.json`, `round-81-linker-verify.json`, and `round-81-apply.json`.

## Round 82 — scene initializer screen (2026-08-07)
- The candidate screen combined the state-machine `func_08016DE0`, scene initializer `func_08016F14`, and packed-store helper `func_08016C24`. m2c supplied useful skeletons; asmlift declined the stack-argument helper and hit its usual project-header compile boundary on the other two, so no generated lift was admitted.
- One exact and two genuine near misses resulted. `func_08016F14` matched with direct ordinary-C stores and the existing `func_0800A3FC`/`func_08005538` prototypes. `func_08016DE0`'s semantics were right but its C branch ladder differed from the target state machine; `func_08016C24` differed only in the global scene-base register roles. Both remain evidence-only, with no pins or force waiver.
- The strict audit found zero instruction asm, barriers, or register pins in the accepted initializer. It reports only the explicit current-scene stores at offsets `0` and `+4` as low-level evidence. `D_083AD81C` was added to the canonical linker map in `afd59602`, which passed a separate full Docker SHA gate before apply.
- The exact-only transaction passed `wariowareinc.gba: OK`, advancing **1669 → 1670** matched functions, **1209 → 1210** linked C TUs, and **1393 → 1394** decomp files with ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3` unchanged. Post-apply report/objdiff, policy, source audit, and all **27** tooling tests passed.

## Round 86 tooling follow-up — Conker provenance and no-cheap-shot admission (2026-08-07)
- The sister Conker repository supplied the useful workflow ideas: retain candidate hashes and near-miss source snapshots, record the exact isolation command/diff, and make the full-ROM SHA gate the final acceptance authority. It does not provide a strict ordinary-C source policy because its legacy MIPS source tree intentionally contains `GLOBAL_ASM`, volatile accesses, and compiler-shaping idioms.
- WarioWare's existing source gate already rejected original-asm wrappers, instruction-bearing asm, empty barriers, and register pins before isolation/apply. This pass closed two remaining admission gaps: scalar-pointer aliases are followed across subsequent lines for offset accounting, and non-mapped C `volatile` is rejected as a possible codegen-forcing shim. Direct fixed GBA mapped-memory accesses remain valid.
- Added regression coverage for the missed form (`u8 *p = ...; p[0x10] = ...`) and for volatile-vs-mapped-memory classification. The focused suite now passes **31 tests** (**33** across the tools suite). The Round 86 exact `func_080178C4` candidate still passes the stricter semantic audit; `func_08017930` remains a 0.041664 near miss caused only by the target's `SP,#0x54` frame versus the candidate's `SP,#8`, and `func_0801776C` remains a real register/table-order near miss.
- No source or ROM progress was admitted in this tooling pass. The verified baseline remains **1682 / 5934** matched functions, **75634 / 993802** matched code, **1222 C / 5465 asm-only** linked units, and ROM SHA-1 **`3f556448d290fa5406d6ed367fee16cc02387ad3`**.

## Round 86 acceptance — strict title-scene wrapper (2026-08-07)
- After the source-quality commit, the same three-candidate screen classified **1 exact / 2 near miss**. `func_080178C4` remained exact and uses only ordinary C plus a named offset-zero scene-root overlay; the two near misses were not changed or force-applied.
- The exact-only transaction passed the rollback-capable full Docker build/report gate with `wariowareinc.gba: OK`; `make report` and `gen_objdiff.py` refreshed **1683 / 5934** matched functions, **75710 / 993802** matched code, and **1223 C / 5464 asm-only** units. ROM SHA-1 remained `3f556448d290fa5406d6ed367fee16cc02387ad3`.
- The accepted source audit, 31 focused source-quality tests, and 33-test tools suite all passed. `func_08017930` is retained as a frame-size near miss, and `func_0801776C` as a table-base/register-order near miss; their full C seeds and scores remain in `.nearmiss/` and `tools/attempts.tsv`.

## New-machine Linux setup notes (2026-08-21)
- `compile_and_view_asm` (and any script calling `tools/decomp_cycle.py`'s objdiff step) fails with `objdiff error: spawnSync /home/kurt/wariowareinc/tools/objdiff-cli ENOENT` on a fresh Linux host: only the platform-suffixed binaries (`tools/objdiff-cli-linux-x86_64`, `tools/objdiff-cli-macos`) exist and both the generic name and suffixes are gitignored. Fix: `ln -s objdiff-cli-linux-x86_64 tools/objdiff-cli` (macOS hosts use `objdiff-cli-macos`). Applies to `compile_and_view_asm`, `tools/decomp_cycle.py` (line ~441), and `tools/gen_objdiff.py`.
- Fresh Fedora-class host with GCC 16 cannot build agbcc natively (`int (*)()` unprototyped calls are hard errors). Build agbcc inside the `devkitpro/devkitarm:latest` container instead (Debian 12, GCC 12): mount the checkout, run `./build.sh`, then copy `agbcc`/`old_agbcc`/`agbcc_arm` to `bin/`, ginclude+libc headers to `include/`, and `libgcc.a`/`libc.a` to `lib/`. Container-built binaries also link against the same glibc the ROM build uses.
- `tools/agbcc-swi.patch` was written against an older pret/agbcc commit; its zero-context hunks apply by line number and drift on current agbcc HEAD (da598c1): the tree.h enum entry landed after `_TREE_H`, the expr.c helper/case landed at file scope, and the c-decl.c builtin registration landed after `copy_lang_decl`. Fix by hand: enum entry after `BUILT_IN_TRAP,`; `expand_swi_div` before `expand_builtin` with the case before its `default:`; builtin registration inside `init_decl_processing` next to the other `builtin_function` calls. thumb.md's hunk still lands correctly. Consider rebasing the patch onto a pinned agbcc commit for future machines.
- Docker containers write root-owned files into the workspace (`objdiff.json`, `build/report.json`, install.sh output); chown them back via a throwaway container before host-side tooling runs.
- Mizuchi bootstrap on this machine: clone `macabeus/mizuchi` to `~/mizuchi`, init submodules over HTTPS (`git config --global url."https://github.com/".insteadOf "git@github.com:"`), `npm install && npm run build`, create `vendor/m2c/.venv` with graphviz installed, persist `MIZUCHI_ROOT=/home/kurt/mizuchi` in `~/.zshrc`.

## Round 98 tooling follow-up — layout-gate friction on overlay candidates (2026-08-21)
- `decomp_cycle.py apply` refused an otherwise-exact `sprite_handler_create` candidate twice with `opaque offset-heavy byte-pointer layout`. Root causes, both in `tools/check_decomp_policy.py::layout_quality`: (1) a `u8 pad[0x24]` gap array inside a named overlay still counts as a raw byte-pointer blob; (2) chained subscripts through a `u32 *` alias (`oam[0] = oam[1] = ...`) count one numeric offset per index, so a single line can blow the bounded threshold of two. Fixes were source-side (re-anchor overlay at `&handler->unk20`; model the 8-word fill as a named `OamClearChunk` struct with chained member assignment — identical codegen). The audit behavior itself is correct per policy; no tool change needed, but candidates touching partially-typed handler/record tails should be pre-screened with `python3 -c "from check_decomp_policy import source_audit; ..."` before spending an apply cycle. Applies to `tools/audit_decomp_source.py` / `tools/check_decomp_policy.py` consumers.

## Round 99 tooling follow-up — included-stub isolate near-miss false negative (2026-08-21)
- Symptom: `decomp_cycle.py isolate`/`apply` on an included_stub candidate whose manifest points at the prebuilt host object (`build/src/scenes/main_menu.c.o`) reported `near_miss` with an empty candidate side (symbol present, no instructions/sections), even though `compile_and_view_asm` showed 100%.
- Root cause: the isolation pipeline strips `.size` directives from candidate asm before assembling, and the single-symbol candidate ELF linked via `-Ttext=0 -e FUNC` leaves objdiff without an inferable size/instruction range for the candidate symbol, so the linked-ELF comparison degenerates.
- Workaround: pass `--force` to `apply` (as batch 247 already did); the full Docker ROM SHA gate remains the acceptance authority and rollback still protects failure. Applies to `tools/decomp_cycle.py` container/link script (~line 300) and any consumer of its `isolate` receipts for included_stub mode. A real fix would keep `.size` for the candidate side or synthesize an end symbol before objdiff.
- Second friction: `apply` refuses unrelated dirty source paths, which blocks coordinated sibling-prototype fixes in the same transaction. Escape hatch `WARIOWARE_ALLOW_DIRTY=1` exists and was sufficient; documented in the pattern library entry for batch 248.

## 2026-08-21 — compile_and_view_asm can show misleading "0 differ / MISMATCH 0.0%" listings
- Symptom: for included-stub functions whose declarations live in host-TU headers not visible to the tool's context (`src/lib_sprite.h`, `scenes.h`), `compile_and_view_asm` compiles the candidate with implicit-declaration errors and prints only the TARGET instruction list, all marked `✓`, ending in "0 instruction(s) differ" yet "MISMATCH 0.0%".
- Root cause: candidate disassembly is empty after failed compile, so the diff has zero differing pairs; the tool does not surface the compile errors.
- Fix/workaround: ignore the ✓-wall; always confirm with `tools/decomp_cycle.py isolate` or a manual Docker section audit with run-unique scratch paths before concluding anything.
- Applies to: Pi tool `compile_and_view_asm`.

## 2026-08-21 — agbcc IV-rotation trap for accumulation-style loops (func_08012BB8 family)
- Symptom: target ROM code uses naive register accumulation (offset var += 6 at outer-loop bottom, byte-index copy at outer top), but every real-C spelling tried compiles to gcc's ROTATED induction form (next-offset precomputed into a spare register at loop top, copy-back after inner loop).
- Tried and failed: `off += 6` in clause/body, `off = k` final copy (kills rotation but changes update shape to a copy), while/goto forms (RTL loop pass still rotates), u8/u16 types, cast barriers `(u32)((u8 *)p + c)`, declaration orders. Also stubborn: gcc emits `(data+k)+const` add order where target is `(data+const)+k`.
- Closest known spellings preserved under `.mizuchi-tmp/round-104/variants/` (y-family) and `.mizuchi-tmp/round-104b/` (func_08012AE8 v1-v6).
- Guidance: if a target function shows non-rotated accumulation codegen, expect significant effort; check later accepted siblings for the source idiom first. Do not pivot to register pins or asm wrappers.

## 2026-08-21 — apply --isolation-receipt requires an exact result even under --force
- Symptom: `decomp_cycle.py apply --force --isolation-receipt <near-miss receipt>` fails with "isolation receipt does not contain one exact result".
- Root cause: `reuse_isolation_receipt` validates receipt results before the `--force` branch is consulted; only a fresh in-apply isolation run flows into the force path.
- Workaround: invoke `apply --force` without `--isolation-receipt` (it records its own isolation receipt, then proceeds to the full ROM gate).
- Applies to: `tools/decomp_cycle.py apply`.

## 2026-08-21 — isolate scores BL-pair candidates as 0.34 near-miss despite byte-equal sections
- See pattern-library Round 105 entry: candidate-only links resolve undefined `BL` callees differently than target-TU links. Raw section audits and defsym-equivalence links are the reliable arbiters for such candidates.

## Round 107 tooling follow-up — host declarations and three BL-pair candidates (2026-08-23)
- The repo-local cycle was useful for the full lifecycle: it screened three candidates together, retained structured near-miss receipts, rolled back a declaration-conflict apply, and then admitted the corrected batch after one clean Docker gate. The first rollback was caused by redeclaring `D_083A4A1C` as a scalar when an earlier included file already declared it as an array; candidate declarations must match the host TU, even when the generated standalone object is unchanged.
- The three accepted main-menu helpers all showed the same linked-isolation limitation: candidate-only links resolve undefined `BL` callees at different addresses than the target host object. Raw object disassembly showed identical instructions and complete literal pools, and the host-TU gate confirmed ROM identity. This is the expected boundary-proof case, not a reason to waive a real instruction or pool mismatch.
- The attempted `func_080136F4` candidate was correctly stopped by the strict layout audit as an opaque offset-heavy scalar-pointer layout; `func_08014208` remained a genuine compiler-shaping near miss because agbcc folded the target's `MOVS #5; RSBS` mask into an immediate. Keeping both as evidence prevented a force-based source-quality regression.
- Fresh post-apply checks completed: Docker `make report`, `python3 tools/gen_objdiff.py`, strict source audits, and `git diff --check`. The accepted batch leaves ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3` unchanged.

## Round 108 tooling follow-up — source-cleanup screening after a fresh baseline rebuild (2026-08-23)
- The first local audit found `build/report.json` absent even though the branch and worktree were synchronized. A clean `devkitpro/devkitarm:latest` rebuild restored the report and confirmed the baseline before candidate selection; do not use missing build artifacts as evidence of a source regression.
- A seven-entry `decomp_cycle.py isolate` screen was productive for cleanup work: five one-pin sibling wrappers were policy-clean exact candidates, while the mask/copy experiments were correctly rejected for opaque raw-pointer layouts, constant-folded negations, or a real register-order near miss. The accepted three-entry follow-up receipt was exact for `func_0801911C`, `func_0801913C`, and `func_0801915C`.
- The full clean Docker gate, `make report`, and `python3 tools/gen_objdiff.py` all passed after source-only edits. Because these functions were already C-linked, report matching stayed **1704 / 5934** and linked units stayed **1244 / 5443**; source residue is the meaningful delta: **197 → 194** pin-bearing files and **772 → 769** compiler pins. The final verify receipt records equal ROM/base ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.

## Round 109 tooling follow-up — host-object screening for another no-pin batch (2026-08-23)
- Reusing the previous screen's exact ordinary-C spellings avoided relearning three stable compiler shapes. The included `func_08001B04` candidate was screened against `build/src/code_08001a70.c.o`; the two standalone candidates used their converted assembly targets. All three were exact and strict-policy clean.
- This is a useful cleanup lane even when the report does not advance: the integrated ROM remains the authority, while the source audit gives a direct measurable reduction in compiler-only shaping. Round 109 removes four pins across three files, leaving **191 pin-bearing files / 765 pins** and **13 empty barriers**.
- The clean Docker ROM gate, regenerated report/objdiff, and `decomp_cycle.py verify` all pass. ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.

## Round 110/117 tooling follow-up — cleanup screen and clean Docker handoff (2026-08-23)
- The barrier screen separated real scheduler dependencies from ordinary-C opportunities: `func_0801A688` and `func_080E1F48` still hoist their table literals past calls without an empty constraint, while `func_0801B174` became exact after a named overlay plus an explicit flags local. `func_0801F698` remained a genuine register-allocation near miss even with a policy-clean graphics overlay.
- For `func_080F282C`, the first no-barrier candidate differed only in the post-store reload home; using a separate typed `u16 reload` local was exact. A separate `u32 factor` reload changed earlier register allocation and was not equivalent. This is a useful candidate-screening reminder that allocator lifetime/coalescing can matter after the visible instruction sequence already matches.
- The one-pin cleanup screen found two exact named-record heap copiers; this batch selected `func_08004BD4` and kept the sibling `func_0800557C` as a ready exact candidate rather than expanding the requested three-function batch. Explicit pointer returns are safer than relying on the old implicit-return source while preserving the same bytes.
- The host-native `make` failed immediately with `tools/agbcc/bin/agbcc: cannot execute binary file`, confirming the documented macOS limitation. `make clean`, Docker `make -j4`, Docker `make report`, local `gen_objdiff.py`, and `decomp_cycle.py verify --no-report` were then run; all Docker gates reported `wariowareinc.gba: OK` with ROM/base ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`. The final report was restored after verify removed the generated build artifacts.

## Round 119/121 tooling follow-up — standalone pin cleanup (2026-08-23)
- Reusing the earlier exact heap-record spelling made `func_0800557C` a low-cost sibling win. The arithmetic screen found a new exact ordinary-C shape for `func_080F28F8`; changing the accumulator/copy local order closed the last register-home difference without any compiler metadata.
- The same screen correctly left `func_080A002C`, `func_080195E4`, and `func_0801D4B4` as near-miss evidence: their remaining differences are instruction scheduling or register homes, not source-policy artifacts. This is a useful filter for the next cleanup pass.
- The clean Docker build, Docker report, local `gen_objdiff.py`, and `decomp_cycle.py verify --no-report` all passed. The report remains **1704 / 5934** functions and **76392 / 993840** code; ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.

## Round 122/123 tooling follow-up — mask-family cleanup (2026-08-23)
- The first three raw-pointer mask candidates were rejected by the layout gate because one scalar alias carried multiple numeric offsets. Re-anchoring each field in a named scene overlay produced **3 exact / 0 rejected** candidates without changing the generated instruction stream.
- This is a productive cleanup pattern: short repeated `MOVS; RSBS; ANDS` helpers can often lose their pins when declaration order mirrors the target's `value`/`mask` lifetimes. The full Docker build, report, local `gen_objdiff.py`, and repo verifier all passed with unchanged ROM/base ROM SHA-1 `3f556448d290fa5406d6ed367fee16cc02387ad3`.

## Round 124 tooling follow-up — serializer and wrapper pin cleanup (2026-08-23)
- A thirteen-entry `decomp_cycle.py isolate` screen separated six exact ordinary-C spellings, one policy-clean near miss, and six strict-policy rejections. The final exact-only transaction selected `func_08003998`, `func_0801E44C`, and `func_080F1FB4`; the unused exact `func_080F0DE0` spelling remains ready for a later batch.
- The serializer's sequential `u8 *p` post-increment form, the sprite wrapper's declaration-order global locals, and the clamp's widened arithmetic temporary all reproduce their target register homes without pins. The rejected `func_080029D0` candidates reinforce that multiple numeric accesses through one scalar pointer alias are not admitted merely because the code is short.
- These were already C-linked units, so the report remains **1704 / 5934** functions, **76392 / 993840** matched code, and **1244 C / 5443 asm-only** units. The measurable maintenance result is **183 → 180** pin-bearing files and **749 → 743** pins, with the empty-barrier count unchanged.
- The clean Docker build, Docker report, local `gen_objdiff.py`, and `decomp_cycle.py verify --no-report` all passed; the report was restored afterward because the verifier removes generated report artifacts. ROM/base ROM SHA-1 remains `3f556448d290fa5406d6ed367fee16cc02387ad3`.
