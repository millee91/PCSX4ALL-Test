# R36SX / Tekken 3 — PCSX4ALL benchmark

This repository is an **experimental** TreeFrogUI PCSX4ALL fork. The Actions workflow
uses `gameblabla/pcsx4all_rs97_rg350` as its complete source tree and overlays
this repository's R36SX-specific sources and build configuration.

## Build artifacts

Run [Build PCSX4ALL R36SX](../../actions/workflows/build-r36sx.yml) or push
an applicable source change to `main`. The workflow produces:

- `pcsx4all-r36sx-baseline`: no profiling instrumentation.
- `pcsx4all-r36sx-memory-profile`: the same emulator with GPU/GTE,
  presentation, audio, process-memory and page-fault diagnostics enabled.

The two binaries have **not** been guaranteed working on real hardware until
tested. Never overwrite your only known-good copy.

## Install on the R36SX

1. Power off, remove the SD card and back up `cubegm/pcsx4all`.
2. Download and unzip the chosen GitHub Actions artifact.
3. Replace only `cubegm/pcsx4all` on the SD card, then eject safely.
4. Start Tekken 3 from `PS`, `ps1` or `psx` in TreeFrogUI.
5. Open PCSX4ALL settings with **SELECT + L1**.
6. Use a real PS1 BIOS, the MIPS recompiler, frame limiter ON,
   frame skip OFF, interlace OFF, dithering OFF and Hi-Res Fix OFF.
7. Test a reproducible match/scene. Record your game region
   (NTSC 60 Hz or PAL 50 Hz), settings and apparent performance.
8. Exit using **SELECT + START** and power off normally.
9. Read `cubegm/logs/last_standalone.log` from the SD card.

The profiler writes `DSPR2_PROF:` roughly every 120 displayed frames.
For those same reporting intervals, `PCSX4ALL_MEM:` reports:

- `rss_kb`: current resident process memory;
- `peak_rss_kb`: resident high-water mark;
- `process_swap_kb`: process swap;
- `system_total_kb` / `system_available_kb`: RAM visible/available to Linux;
- `swap_total_kb` / `swap_free_kb`: system swap;
- `minor_faults` / `major_faults`: process page faults since the previous
  report (first report covers startup).

The graphics and memory readouts are **not** a hardware cache-miss counter or
a direct DRAM bandwidth measurement. They help identify the next experiment;
they do not by themselves prove a bandwidth bottleneck.

## A/B rules

Run the **baseline** first, then the **memory-profile** build; keep the same
ROM/region, BIOS, settings and game scene. Profiling consumes some CPU time,
so compare its measurements with a non-instrumented performance test.
Do not compare PAL and NTSC as if they shared the same full-speed target.
After each test, restore your known-good binary if any crashes, audio glitches
or graphical regressions appear.

Only after gathering metrics should we experiment with changing the dynarec
code cache, GPU texture access or any other behavior.
