/**
 *  @file udp.c
 *  @brief  UDP source file
**/

#include "udp.h"

static char UDP_WRITE_PAYLOAD[512];     /**< UDP outgoing buffer          **/
static char OT_READ_PAYLOAD[512];       /**< UDP incoming buffer          **/
static size_t datagram_len;             /**< UDP outgoing length          **/
static otUdpSocket sUdpSocket;          /**< UDP socket for communication **/

/**
 * @name initUDP
 * @brief   initializes OpenThread instance for UDP
 * @param aInstance
 *          the OpenThread instance to be configured
**/
void initUdp(otInstance *aInstance)
{
  otSockAddr  listenSockAddr;

  memset(&sUdpSocket, 0, sizeof(sUdpSocket));
  memset(&listenSockAddr, 0, sizeof(listenSockAddr));

  listenSockAddr.mPort    = UDP_PORT; //register udp port (1212)
  //open socket and register handleUdpReceive as the UDP receive callback
  otUdpOpen(aInstance, &sUdpSocket, handleUdpReceive, aInstance);
  otUdpBind(aInstance, &sUdpSocket, &listenSockAddr); //Bind UDP/IPv6 Socket
}

/**
 * @name sendUDP
 * @brief   sends message over OpenThread instance
 * @param aInstance
 *          the OpenThread instance to use
**/
void sendUdp(otInstance * aInstance)
{
  // variable inputs
  otError       error = OT_ERROR_NONE;
  otMessage *   message;
  otMessageInfo messageInfo;
  otIp6Address  destinationAddr;

  memset(&messageInfo, 0, sizeof(messageInfo));
  // UDP settings
  otIp6AddressFromString(UDP_DEST_ADDR, &destinationAddr);
  messageInfo.mPeerAddr    = destinationAddr;
  messageInfo.mPeerPort    = UDP_PORT;
  // UDP message setup
  message = otUdpNewMessage(aInstance, NULL);
  otEXPECT_ACTION(message != NULL, error = OT_ERROR_NO_BUFS);
  error = otMessageAppend(message, UDP_WRITE_PAYLOAD, datagram_len);
  otEXPECT(error == OT_ERROR_NONE);
  // UDP message send
  error = otUdpSend(aInstance, &sUdpSocket, message, &messageInfo);

  // error checking - ignore
  exit:
  if (error != OT_ERROR_NONE && message != NULL)
  {
      otMessageFree(message);
      // might want to print console msg here
  }
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
  OT_UNUSED_VARIABLE(aContext);
  OT_UNUSED_VARIABLE(aMessageInfo);
  // parse
  uint16_t payloadLength = otMessageGetLength(aMessage) - otMessageGetOffset(aMessage);
  otMessageRead(aMessage,otMessageGetOffset(aMessage), OT_READ_PAYLOAD,payloadLength);
  OT_READ_PAYLOAD[payloadLength] = '\0'; // important, add string terminator!


  if (!strcmp(OT_READ_PAYLOAD, UDP_EXPECTED_ON ))
  {
    // power on command
    gpio_power_on();
  }
  else if (!strcmp(OT_READ_PAYLOAD, UDP_EXPECTED_OFF))
  {
    // power off command
    gpio_power_off();
  }
  else if (!strcmp(OT_READ_PAYLOAD, UDP_EXPECTED_TOG))
  {
    // power tog command
    gpio_power_tog();
  }
}

/**
 * @name setUdpPayload
 * @brief   string copies string into the UDP buffer
 * @param msg
 *          the string to be sent
 */
void setUdpPayload(char * msg)
{
  strcpy(UDP_WRITE_PAYLOAD, msg);
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
  memcpy(UDP_WRITE_PAYLOAD, msg, s);
  datagram_len = s;
}
