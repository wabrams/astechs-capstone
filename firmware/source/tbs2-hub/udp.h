/**
 *  @file udp.c
 *  @brief  UDP source file
**/

#include <string.h>

#include <openthread/message.h>
#include <openthread/udp.h>
#include <openthread/cli.h>

#include <utils/code_utils.h>

#ifndef UDP_H
#define UDP_H

// port definition
#define UDP_PORT 1212

// destination address
static const char UDP_DEST_ADDR[] = "ff03::1";

// UDP function definitions
void initUdp(otInstance *aInstance);
void sendUdp(otInstance *aInstance);
void handleUdpReceive(void *aContext, otMessage *aMessage, const otMessageInfo *aMessageInfo);
void setUdpPayload(char * msg);
void setUdpPayloadN(uint8_t * msg, size_t s);

#endif /* UDP_H */
