/**
 *  @file app.h
 *  @brief  APP header file
**/

#include <assert.h>
#include <string.h>

#include <openthread-system.h>
#include <openthread-core-config.h>
#include <openthread/cli.h>
#include <openthread/config.h>
#include <openthread/dataset_ftd.h>
#include <openthread/diag.h>
#include <openthread/instance.h>
#include <openthread/message.h>
#include <openthread/tasklet.h>
#include <openthread/thread.h>
#include <openthread/thread_ftd.h>
#include <openthread/udp.h>

#include <utils/code_utils.h>

#ifndef APP_H
#define APP_H

#define APP_ADC   /**< used for rapid enable or disable of ADC **/

void app_init(void);
void app_exit(void);
void app_process_action(void);

void setNetworkConfiguration(otInstance *aInstance);

#endif
