/**
 *  @file udp.c
 *  @brief  UDP source file
**/

#include "udp.h"

static char UDP_INPT_BUF[512];        /**< UDP incoming buffer          **/
static char UDP_LAST_BUF[512];        /**< UDP incoming buffer (last)   **/
static char UDP_OUTP_BUF[512];        /**< UDP outgoing buffer          **/
static size_t datagram_len;           /**< UDP outgoing length          **/

static otUdpSocket sUdpSocket;        /**< UDP socket for communication **/

/**
 * @name initUDP
 * @brief   initializes OpenThread instance for UDP
 * @param aInstance
 *          the OpenThread instance to be configured
**/
void initUdp(otInstance *aInstance)
{
    otSockAddr listenSockAddr;

    memset(&sUdpSocket, 0, sizeof(sUdpSocket));
    memset(&listenSockAddr, 0, sizeof(listenSockAddr));

    listenSockAddr.mPort = UDP_PORT;

    otUdpOpen(aInstance, &sUdpSocket, handleUdpReceive, aInstance);
    otUdpBind(aInstance, &sUdpSocket, &listenSockAddr);
}

/**
 * @name sendUDP
 * @brief   sends message over OpenThread instance
 * @param aInstance
 *          the OpenThread instance to use
**/
void sendUdp(otInstance * aInstance)
{
    otError       error = OT_ERROR_NONE;
    otMessage *   message;
    otMessageInfo messageInfo;
    otIp6Address  destinationAddr;

    memset(&messageInfo, 0, sizeof(messageInfo));

    otIp6AddressFromString(UDP_DEST_ADDR, &destinationAddr);
    messageInfo.mPeerAddr = destinationAddr;
    messageInfo.mPeerPort = UDP_PORT;

    message = otUdpNewMessage(aInstance, NULL);
    otEXPECT_ACTION(message != NULL, error = OT_ERROR_NO_BUFS);

    error = otMessageAppend(message, UDP_OUTP_BUF, datagram_len);
    otEXPECT(error == OT_ERROR_NONE);

    error = otUdpSend(aInstance, &sUdpSocket, message, &messageInfo);

 exit:
    if (error != OT_ERROR_NONE && message != NULL)
        otMessageFree(message);
}

/**
 * @name handleUDPReceive
 * @brief   message receive handler / parser
 * @param aContext - unused
 * @param aMessage
 *          the message received
 * @param aMessageInfo - unused
 */
void handleUdpReceive(void * aContext, otMessage * aMessage, const otMessageInfo * aMessageInfo)
{
   uint16_t payloadLength = otMessageGetLength(aMessage) - otMessageGetOffset(aMessage);
   otMessageRead(aMessage, otMessageGetOffset(aMessage), UDP_INPT_BUF, payloadLength);

   if (payloadLength < 64 || memcmp(UDP_INPT_BUF, UDP_LAST_BUF, payloadLength))
   {
     otCliOutput(UDP_INPT_BUF, payloadLength);
     memcpy(UDP_LAST_BUF, UDP_INPT_BUF, payloadLength);
   }

   OT_UNUSED_VARIABLE(aContext);
   OT_UNUSED_VARIABLE(aMessageInfo);
}

/**
 * @name setUdpPayload
 * @brief   string copies string into the UDP buffer
 * @param msg
 *          the string to be sent
 */
void setUdpPayload(char * msg)
{
  strcpy(UDP_OUTP_BUF, msg);
  datagram_len = (size_t) strlen(msg);
}

/**
 * @name setUdpPayloadN
 * @brief   memory copies buffer (as bytes) into the UDP buffer
 * @param msg
 *          the buffer to be sent
 * @param s
 *          the size of the buffer
 */
void setUdpPayloadN(uint8_t * msg, size_t s)
{
  memcpy(UDP_OUTP_BUF, msg, s);
  datagram_len = s;
}
