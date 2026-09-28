/*
 * timestamp.h
 *
 *  Created on: 27 Eyl 2026
 *      Author: furkan
 */

#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#include <stdint.h>

typedef enum
{
    TIMESTAMP_STATUS_OK = 0,
    TIMESTAMP_STATUS_ERROR

} TimestampStatus_t;


/*
 * Starts the 1 MHz free-running TIM2 counter.
 */
TimestampStatus_t Timestamp_Init(void);


/*
 * Returns current timestamp in microseconds.
 */
uint32_t Timestamp_GetUs(void);


/*
 * Returns elapsed time since start_us.
 *
 * uint32_t subtraction also handles one TIM2 counter wrap-around
 * correctly for intervals shorter than the full counter period.
 */
uint32_t Timestamp_ElapsedUs(uint32_t start_us);


#endif /* TIMESTAMP_H_ */
