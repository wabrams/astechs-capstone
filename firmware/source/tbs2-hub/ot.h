/**
 * @file ot.h
 * @brief   OpenThread header file
**/

#include <openthread/cli.h>

#ifndef OT_H
#define OT_H

extern bool serialInput;

void hubCommand(void * context, uint8_t argc, char * argv[]);
void ot_custom_cli_init(void);

#endif /* OT_H */
