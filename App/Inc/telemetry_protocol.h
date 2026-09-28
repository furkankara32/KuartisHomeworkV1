/*
 * telemetry_protocol.h
 *
 *  Created on: 27 Eyl 2026
 *      Author: furkan
 */

#ifndef TELEMETRY_PROTOCOL_H_
#define TELEMETRY_PROTOCOL_H_

#include <stdint.h>
#include "app_types.h"

typedef enum
{
    TELEMETRY_STATUS_OK = 0,
    TELEMETRY_STATUS_INVALID_ARGUMENT,
    TELEMETRY_STATUS_BUFFER_TOO_SMALL

} TelemetryStatus_t;


TelemetryStatus_t Telemetry_FormatQuaternion(
        const OrientationSample_t *sample,
        uint8_t *buffer,
        uint16_t buffer_size,
        uint16_t *message_length);

#endif /* TELEMETRY_PROTOCOL_H_ */
