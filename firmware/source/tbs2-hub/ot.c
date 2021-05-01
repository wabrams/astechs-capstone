/**
 * @file ot.c
 * @brief   OpenThread source file
**/

#include "ot.h"
#include "udp.h"

bool serialInput = false;

static char OT_HUB_BUF[64];
// command extension
static otCliCommand custCommands[] =
{
    {"hub", &hubCommand}
};

/**
 * @name hubCommand()
 * @brief   broadcasts commands received from serial over Thread
 * @param context - unused
 * @param argc argument count
 * @param argv argument values
 */
void hubCommand(void * context, uint8_t argc, char * argv[])
{
  memset(OT_HUB_BUF, 0, 64);

  int s = 0;
  for (unsigned int i = 0; i < argc; i++)
  {
    for (unsigned int j = 0; j < strlen(argv[i]); j++)
    {
      OT_HUB_BUF[s] = argv[i][j];
      s++;
    }
    OT_HUB_BUF[s] = ' ';
    s++;
  }

  if (s > 0)
    OT_HUB_BUF[s-1] = '\r';
  if (s < 63)
  {
    OT_HUB_BUF[s]   = '\n';
    s++;
  }
  else
    OT_HUB_BUF[s]   = '\0';

  setUdpPayload(OT_HUB_BUF);
  serialInput = true;

  OT_UNUSED_VARIABLE(context);
}

/**
 * @name ot.c
 * @brief   registers custom commands to the OpenThread CLI
 * @note    this is how we were able to get two-way serial communication working, as the OpenThread SDK hijacks the UART communication to listen for CLI inputs like 'thread start'
**/
void ot_custom_cli_init(void)
{
  otCliSetUserCommands(custCommands, (sizeof(custCommands) / sizeof(otCliCommand)), NULL);
}
