/*
 * orientation_math.h
 *
 *  Created on: 27 Eyl 2026
 *      Author: furkan
 */

#ifndef ORIENTATION_MATH_H_
#define ORIENTATION_MATH_H_

#include <stdint.h>

typedef enum
{
    ORIENTATION_STATUS_OK = 0,
    ORIENTATION_STATUS_ERROR

} OrientationStatus_t;


/*
 * Converts a quaternion to heading in degrees.
 *
 * Output range:
 * 0.0 <= heading_deg < 360.0
 */
OrientationStatus_t Orientation_QuaternionToHeadingDeg(
        float qx,
        float qy,
        float qz,
        float qw,
        float *heading_deg);

#endif /* ORIENTATION_MATH_H_ */
