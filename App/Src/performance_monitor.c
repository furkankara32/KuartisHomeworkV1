/*
 * performance_monitor.c
 *
 *  Created on: 27 Eyl 2026
 *      Author: furkan
 */


#include "performance_monitor.h"

#include <stddef.h>
#include <stdint.h>

static uint32_t last_timestamp_us = 0U;
static uint32_t sample_count = 0U;
static uint32_t period_count = 0U;

static uint32_t min_period_us = 0U;
static uint32_t max_period_us = 0U;

static uint64_t total_period_us = 0ULL;

static uint32_t sample_age_count = 0U;

static uint32_t min_sample_age_us = 0U;
static uint32_t max_sample_age_us = 0U;

static uint64_t total_sample_age_us = 0ULL;

void PerformanceMonitor_Init(void)
{
    last_timestamp_us = 0U;

    sample_count = 0U;
    period_count = 0U;

    min_period_us = 0U;
    max_period_us = 0U;

    total_period_us = 0ULL;

    sample_age_count = 0U;

    min_sample_age_us = 0U;
    max_sample_age_us = 0U;

    total_sample_age_us = 0ULL;
}


void PerformanceMonitor_Update(uint32_t timestamp_us)
{
    uint32_t period_us;

    sample_count++;

    /*
     * First sample has no previous timestamp,
     * therefore no period can be calculated.
     */
    if (sample_count == 1U)
    {
        last_timestamp_us = timestamp_us;
        return;
    }


    /*
     * Unsigned subtraction also handles TIM2 wrap-around.
     */
    period_us = timestamp_us - last_timestamp_us;

    last_timestamp_us = timestamp_us;

    period_count++;


    if (period_count == 1U)
    {
        min_period_us = period_us;
        max_period_us = period_us;
    }
    else
    {
        if (period_us < min_period_us)
        {
            min_period_us = period_us;
        }

        if (period_us > max_period_us)
        {
            max_period_us = period_us;
        }
    }


    total_period_us += period_us;
}

void PerformanceMonitor_UpdateSampleAge(uint32_t sample_age_us)
{
    sample_age_count++;

    if (sample_age_count == 1U)
    {
        min_sample_age_us = sample_age_us;
        max_sample_age_us = sample_age_us;
    }
    else
    {
        if (sample_age_us < min_sample_age_us)
        {
            min_sample_age_us = sample_age_us;
        }

        if (sample_age_us > max_sample_age_us)
        {
            max_sample_age_us = sample_age_us;
        }
    }

    total_sample_age_us += sample_age_us;
}

void PerformanceMonitor_GetStats(PerformanceStats_t *stats)
{
    if (stats == NULL)
    {
        return;
    }


    stats->sample_count = sample_count;
    stats->period_count = period_count;

    stats->min_period_us = min_period_us;
    stats->max_period_us = max_period_us;

    stats->min_sample_age_us = min_sample_age_us;
    stats->max_sample_age_us = max_sample_age_us;

    if (sample_age_count > 0U)
    {
        stats->average_sample_age_us =
            (uint32_t)(total_sample_age_us / sample_age_count);
    }
    else
    {
        stats->average_sample_age_us = 0U;
    }

    if (period_count > 0U)
    {
        stats->average_period_us =
            (uint32_t)(total_period_us / period_count);

        /*
         * Peak-to-peak jitter definition:
         *
         * jitter = max period - min period
         */
        stats->jitter_us =
            max_period_us - min_period_us;
    }
    else
    {
        stats->average_period_us = 0U;
        stats->jitter_us = 0U;
    }
}
