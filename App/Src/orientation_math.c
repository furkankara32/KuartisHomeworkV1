/*
 * orientation_math.c
 *
 *  Created on: 27 Eyl 2026
 *      Author: furkan
 */


#include "orientation_math.h"

#include <math.h>

#define RAD_TO_DEG    57.2957795131f


OrientationStatus_t Orientation_QuaternionToHeadingDeg(
        float qx,
        float qy,
        float qz,
        float qw,
        float *heading_deg)
{
    float norm;
    float siny_cosp;
    float cosy_cosp;
    float yaw_deg;

    if (heading_deg == NULL)
    {
        return ORIENTATION_STATUS_ERROR;
    }

    /*
     * Normalize quaternion.
     */
    norm = sqrtf(
        (qx * qx) +
        (qy * qy) +
        (qz * qz) +
        (qw * qw)
    );

    if (norm <= 0.0f)
    {
        return ORIENTATION_STATUS_ERROR;
    }

    qx /= norm;
    qy /= norm;
    qz /= norm;
    qw /= norm;


    /*
     * Quaternion -> Yaw
     */
    siny_cosp = 2.0f * ((qw * qz) + (qx * qy));

    cosy_cosp = 1.0f -
                (2.0f * ((qy * qy) + (qz * qz)));

    yaw_deg = atan2f(siny_cosp, cosy_cosp) * RAD_TO_DEG;


    /*
     * Convert:
     * -180 ... +180
     *
     * to:
     * 0 ... 360
     */
    if (yaw_deg < 0.0f)
    {
        yaw_deg += 360.0f;
    }

    *heading_deg = yaw_deg;

    return ORIENTATION_STATUS_OK;
}
