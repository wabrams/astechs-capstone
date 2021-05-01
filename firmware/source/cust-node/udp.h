/**
 *  @file udp.h
 *  @brief  UDP header file
**/

// UDP code originated from: https://openthread.io/codelabs/openthread-apis#6

#include <string.h>

#include <openthread/message.h>
#include <openthread/udp.h>

#include "utils/code_utils.h"

#include "gpio.h"

#ifndef UDP_H
#define UDP_H

// port definition
#define UDP_PORT 1212

// expected commands that this device can handle
static const char UDP_DEST_ADDR[]    = "ff03::1";
static const char UDP_EXPECTED_ON[]  = "NODE1 ON\r\n";
static const char UDP_EXPECTED_OFF[] = "NODE1 OFF\r\n";
static const char UDP_EXPECTED_TOG[] = "NODE1 TOG\r\n";

static const char UDP_RESPONSE_ON[]  = "NODE1 IS ON\r\n";
static const char UDP_RESPONSE_OFF[] = "NODE1 IS OFF\r\n";

// UDP function definitions
bool isAck();
void initUdp(otInstance *aInstance);
void sendUdp(otInstance *aInstance);
void handleUdpReceive(void *aContext, otMessage *aMessage, const otMessageInfo *aMessageInfo);
void setUdpPayload(char * msg);
void setUdpPayloadN(uint8_t * msg, size_t s);

#endif /* UDP_H */
