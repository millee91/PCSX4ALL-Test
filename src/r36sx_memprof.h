/*
 * R36SX / PCSX4ALL memory diagnostics.
 * Compiled only for profiling builds; no runtime cost in normal releases.
 * /proc values are in kB on Linux. Page-fault counters are per-process deltas.
 */
#ifndef R36SX_MEMPROF_H
#define R36SX_MEMPROF_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>

static inline void r36sx_memprof_dump(void)
{
    unsigned long rss = 0, highwater = 0, swap = 0;
    unsigned long mem_total = 0, mem_available = 0;
    unsigned long swap_total = 0, swap_free = 0;
    char line[256];
    FILE *f = fopen("/proc/self/status", "r");
    if (f) {
        while (fgets(line, sizeof(line), f)) {
            if      (!strncmp(line, "VmRSS:", 6))  rss = strtoul(line + 6, NULL, 10);
            else if (!strncmp(line, "VmHWM:", 6))  highwater = strtoul(line + 6, NULL, 10);
            else if (!strncmp(line, "VmSwap:", 7)) swap = strtoul(line + 7, NULL, 10);
        }
        fclose(f);
    }

    f = fopen("/proc/meminfo", "r");
    if (f) {
        while (fgets(line, sizeof(line), f)) {
            if      (!strncmp(line, "MemTotal:", 9))      mem_total = strtoul(line + 9, NULL, 10);
            else if (!strncmp(line, "MemAvailable:", 13)) mem_available = strtoul(line + 13, NULL, 10);
            else if (!strncmp(line, "SwapTotal:", 10))    swap_total = strtoul(line + 10, NULL, 10);
            else if (!strncmp(line, "SwapFree:", 9))      swap_free = strtoul(line + 9, NULL, 10);
        }
        fclose(f);
    }

    static long prev_minflt = -1, prev_majflt = -1;
    long minflt_delta = -1, majflt_delta = -1;
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        minflt_delta = (prev_minflt < 0) ? usage.ru_minflt :
                       usage.ru_minflt - prev_minflt;
        majflt_delta = (prev_majflt < 0) ? usage.ru_majflt :
                       usage.ru_majflt - prev_majflt;
        prev_minflt = usage.ru_minflt;
        prev_majflt = usage.ru_majflt;
    }

    fprintf(stderr,
            "PCSX4ALL_MEM: rss_kb=%lu peak_rss_kb=%lu process_swap_kb=%lu "
            "system_total_kb=%lu system_available_kb=%lu "
            "swap_total_kb=%lu swap_free_kb=%lu "
            "minor_faults=%ld major_faults=%ld\n",
            rss, highwater, swap, mem_total, mem_available,
            swap_total, swap_free, minflt_delta, majflt_delta);
}

#endif /* R36SX_MEMPROF_H */
