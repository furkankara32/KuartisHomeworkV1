/*
 * app_types.h
 *
 *  Created on: 27 Eyl 2026
 *      Author: furkan
 */

#ifndef APP_TYPES_H_
#define APP_TYPES_H_

#include <stdint.h>

/*
 * Maximum UART message length.
 *
 * Large enough for:
 *  - NMEA HCHDM sentence
 *  - Quaternion telemetry sentence
 */
#define UART_MESSAGE_MAX_LENGTH    96U


/*
 * Represents one complete orientation sample acquired from BNO085.
 *
 * Quaternion is the primary orientation representation.
 * heading_deg is derived from the same quaternion sample.
 *
 * All data in this structure belongs to the same sensor sample.
 */
typedef struct
{
    float qx;
    float qy;
    float qz;
    float qw;

    float heading_deg;

    uint32_t timestamp_us;
    uint32_t sequence;

    uint8_t accuracy;

} OrientationSample_t;


/*
 * Message passed to UartTxTask.
 *
 * The producer prepares the complete serial message.
 * UartTxTask is responsible for writing it to the TX ring buffer.
 */
typedef struct
{
    uint8_t data[UART_MESSAGE_MAX_LENGTH];
    uint16_t length;

} UartMessage_t;


#endif /* APP_TYPES_H_ */
