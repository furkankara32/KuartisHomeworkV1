/*
 * performance_monitor.h
 *
 *  Created on: 27 Eyl 2026
 *      Author: furkan
 */

#ifndef PERFORMANCE_MONITOR_H_
#define PERFORMANCE_MONITOR_H_

#include <stdint.h>

typedef struct
{
    uint32_t sample_count;
    uint32_t period_count;

    uint32_t min_period_us;
    uint32_t max_period_us;
    uint32_t average_period_us;

    uint32_t jitter_us;

    uint32_t min_sample_age_us;
    uint32_t max_sample_age_us;
    uint32_t average_sample_age_us;

} PerformanceStats_t;


void PerformanceMonitor_Init(void);

void PerformanceMonitor_Update(uint32_t timestamp_us);

void PerformanceMonitor_GetStats(PerformanceStats_t *stats);

void PerformanceMonitor_UpdateSampleAge(uint32_t sample_age_us);

#endif /* PERFORMANCE_MONITOR_H_ */
