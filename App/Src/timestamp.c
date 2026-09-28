/*
 * timestamp.c
 *
 *  Created on: 27 Eyl 2026
 *      Author: furkan
 */

#include "timestamp.h"
#include "tim.h"


TimestampStatus_t Timestamp_Init(void)
{
    __HAL_TIM_SET_COUNTER(&htim2, 0U);

    if (HAL_TIM_Base_Start(&htim2) != HAL_OK)
    {
        return TIMESTAMP_STATUS_ERROR;
    }

    return TIMESTAMP_STATUS_OK;
}


uint32_t Timestamp_GetUs(void)
{
    return __HAL_TIM_GET_COUNTER(&htim2);
}


uint32_t Timestamp_ElapsedUs(uint32_t start_us)
{
    return (uint32_t)(Timestamp_GetUs() - start_us);
}
