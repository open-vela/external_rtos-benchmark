// SPDX-License-Identifier: Apache-2.0

/**
 * @file Measuring malloc and free times
 *
 * This test module measures the system usage of
 * malloc and free usage time.
 */

#include "bench_api.h"
#include "bench_utils.h"

#define KB(x) ((x) * 1024)

static struct bench_stats time_to_malloc;  /* time to malloc*/
static struct bench_stats time_to_free;    /* time to free */

static size_t table[] = {
	8, 16, 32, 64, 128, 256, 512, KB(1), KB(2), KB(4), KB(8),
	KB(16), KB(32), KB(64),
};

/**
 * @brief Reset time statistics
 */
static void reset_time_stats(void)
{
	bench_stats_reset(&time_to_malloc);
	bench_stats_reset(&time_to_free);
}

/**
 * @brief Measure time to malloc and free.
 */
static void gather_set1_stats(size_t size)
{
	bench_time_t start;
	bench_time_t mid;
	bench_time_t end;
	uint32_t i;
	void *p;

	for (i = 0; i < ITERATIONS; i++) {
		start = bench_timing_counter_get();
		p = bench_malloc(size);
		mid = bench_timing_counter_get();
		bench_free(p);
		end = bench_timing_counter_get();
		bench_stats_update(&time_to_malloc,
					bench_timing_cycles_get(&start, &mid),
					ITERATIONS);
		bench_stats_update(&time_to_free,
					bench_timing_cycles_get(&mid,&end),
					ITERATIONS);
	}
}

/**
 * @brief Test setup function
 */
void bench_malloc_free(void *arg)
{
	uint32_t i;

	for (i = 0; i < sizeof(table) / sizeof(table[0]); i++) {
		bench_timing_init();
		reset_time_stats();
		bench_timing_start();
		PRINTF("Alloc/Free onece size %zu [avg, min, max] in nanoseconds **\n\r", table[i]);
		gather_set1_stats(table[i]);
		bench_stats_report_line("Malloc", &time_to_malloc);
		bench_stats_report_line("Free", &time_to_free);
		bench_timing_stop();
		}

}

#ifdef RUN_MALLOC_FREE
int main(void)
{
	PRINTF("\n\r *** Starting! ***\n\n\r");

	bench_test_init(bench_malloc_free);

	PRINTF("\n\r *** Done! ***\n\r");

	return 0;
}
#endif
