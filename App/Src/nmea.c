/*
 * nmea.c
 *
 *  Created on: 27 Eyl 2026
 *      Author: furkan
 */


#include "nmea.h"

#include <stdio.h>


static uint8_t NMEA_CalculateChecksum(const char *data)
{
    uint8_t checksum = 0U;

    while ((*data != '\0') && (*data != '*'))
    {
        checksum ^= (uint8_t)(*data);
        data++;
    }

    return checksum;
}


NMEA_Status_t NMEA_FormatHCHDM(
        float heading_deg,
        uint8_t *buffer,
        uint16_t buffer_size,
        uint16_t *message_length)
{
    char body[32];

    uint16_t heading_tenths;
    uint16_t heading_integer;
    uint16_t heading_decimal;

    uint8_t checksum;

    int body_length;
    int total_length;


    if ((buffer == NULL) || (message_length == NULL))
    {
        return NMEA_STATUS_INVALID_ARGUMENT;
    }


    if ((heading_deg < 0.0f) || (heading_deg >= 360.0f))
    {
        return NMEA_STATUS_INVALID_ARGUMENT;
    }


    /*
     * Example:
     *
     * heading = 123.44
     *
     * heading_tenths = 1234
     *
     * integer = 123
     * decimal = 4
     *
     * This avoids floating-point printf.
     */
    heading_tenths =
        (uint16_t)((heading_deg * 10.0f) + 0.5f);


    /*
     * Rounding 359.96 could produce 3600.
     * Wrap it back to 0.0 degrees.
     */
    if (heading_tenths >= 3600U)
    {
        heading_tenths = 0U;
    }


    heading_integer = heading_tenths / 10U;
    heading_decimal = heading_tenths % 10U;


    /*
     * Do NOT include '$' or '*' in checksum body.
     *
     * Example:
     *
     * HCHDM,123.4,M
     */
    body_length = snprintf(
        body,
        sizeof(body),
        "HCHDM,%u.%u,M",
        (unsigned int)heading_integer,
        (unsigned int)heading_decimal
    );


    if ((body_length < 0) ||
        ((uint16_t)body_length >= sizeof(body)))
    {
        return NMEA_STATUS_BUFFER_TOO_SMALL;
    }


    checksum = NMEA_CalculateChecksum(body);


    /*
     * Final:
     *
     * $HCHDM,123.4,M*CS\r\n
     */
    total_length = snprintf(
        (char *)buffer,
        buffer_size,
        "$%s*%02X\r\n",
        body,
        checksum
    );


    if ((total_length < 0) ||
        ((uint16_t)total_length >= buffer_size))
    {
        return NMEA_STATUS_BUFFER_TOO_SMALL;
    }


    *message_length = (uint16_t)total_length;

    return NMEA_STATUS_OK;
}
