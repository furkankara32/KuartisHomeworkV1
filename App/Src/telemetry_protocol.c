/*
 * telemetry_protocol.c
 *
 *  Created on: 27 Eyl 2026
 *      Author: furkan
 */


#include "telemetry_protocol.h"

#include <stdio.h>
#include <math.h>


static uint8_t Telemetry_CalculateChecksum(const char *data)
{
    uint8_t checksum = 0U;

    while ((*data != '\0') && (*data != '*'))
    {
        checksum ^= (uint8_t)(*data);
        data++;
    }

    return checksum;
}


typedef struct
{
    char sign;
    uint32_t integer;
    uint32_t fraction;

} Fixed6_t;


static Fixed6_t Telemetry_FloatToFixed6(float value)
{
    Fixed6_t result;
    uint32_t scaled;

    if (value < 0.0f)
    {
        result.sign = '-';
        value = -value;
    }
    else
    {
        result.sign = '+';
    }

    scaled = (uint32_t)lroundf(value * 1000000.0f);

    result.integer = scaled / 1000000U;
    result.fraction = scaled % 1000000U;

    return result;
}


TelemetryStatus_t Telemetry_FormatQuaternion(
        const OrientationSample_t *sample,
        uint8_t *buffer,
        uint16_t buffer_size,
        uint16_t *message_length)
{
    char body[88];

    Fixed6_t qx;
    Fixed6_t qy;
    Fixed6_t qz;
    Fixed6_t qw;

    uint8_t checksum;

    int body_length;
    int total_length;


    if ((sample == NULL) ||
        (buffer == NULL) ||
        (message_length == NULL))
    {
        return TELEMETRY_STATUS_INVALID_ARGUMENT;
    }


    qx = Telemetry_FloatToFixed6(sample->qx);
    qy = Telemetry_FloatToFixed6(sample->qy);
    qz = Telemetry_FloatToFixed6(sample->qz);
    qw = Telemetry_FloatToFixed6(sample->qw);

    body_length = snprintf(
        body,
        sizeof(body),
        "PKRT,QTN,%lu,%lu,"
        "%c%lu.%06lu,"
        "%c%lu.%06lu,"
        "%c%lu.%06lu,"
        "%c%lu.%06lu,"
        "%u",

        (unsigned long)sample->sequence,
        (unsigned long)sample->timestamp_us,

        qx.sign,
        (unsigned long)qx.integer,
        (unsigned long)qx.fraction,

        qy.sign,
        (unsigned long)qy.integer,
        (unsigned long)qy.fraction,

        qz.sign,
        (unsigned long)qz.integer,
        (unsigned long)qz.fraction,

        qw.sign,
        (unsigned long)qw.integer,
        (unsigned long)qw.fraction,

        (unsigned int)sample->accuracy
    );


    if ((body_length < 0) ||
        ((uint16_t)body_length >= sizeof(body)))
    {
        return TELEMETRY_STATUS_BUFFER_TOO_SMALL;
    }


    checksum = Telemetry_CalculateChecksum(body);


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
        return TELEMETRY_STATUS_BUFFER_TOO_SMALL;
    }


    *message_length = (uint16_t)total_length;

    return TELEMETRY_STATUS_OK;
}
