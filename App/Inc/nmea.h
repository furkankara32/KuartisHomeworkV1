/*
 * nmea.h
 *
 *  Created on: 27 Eyl 2026
 *      Author: furkan
 */
#ifndef NMEA_H_
#define NMEA_H_

#include <stdint.h>

typedef enum
{
    NMEA_STATUS_OK = 0,
    NMEA_STATUS_INVALID_ARGUMENT,
    NMEA_STATUS_BUFFER_TOO_SMALL

} NMEA_Status_t;


/*
 * Generates:
 *
 * $HCHDM,123.4,M*CS\r\n
 *
 * heading_deg range:
 * 0.0 <= heading_deg < 360.0
 */
NMEA_Status_t NMEA_FormatHCHDM(
        float heading_deg,
        uint8_t *buffer,
        uint16_t buffer_size,
        uint16_t *message_length);

#endif /* NMEA_H_ */
