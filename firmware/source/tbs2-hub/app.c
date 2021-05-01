/**
 * @file app.c
 * @brief   APP source file
**/

#include "app.h"

#ifdef SL_CATALOG_POWER_MANAGER_PRESENT
  #include "sl_power_manager.h"
#endif

static void setNetworkConfiguration(otInstance *aInstance);

// TODO: do not delete
#if OPENTHREAD_CONFIG_LOG_OUTPUT == OPENTHREAD_CONFIG_LOG_OUTPUT_APP
void otPlatLog(otLogLevel aLogLevel, otLogRegion aLogRegion, const char *aFormat, ...)
{
    OT_UNUSED_VARIABLE(aLogLevel);
    OT_UNUSED_VARIABLE(aLogRegion);
    OT_UNUSED_VARIABLE(aFormat);

    va_list ap;
    va_start(ap, aFormat);
    otCliPlatLogv(aLogLevel, aLogRegion, aFormat, ap);
    va_end(ap);
}
#endif

#ifndef OPENTHREAD_ENABLE_COVERAGE
  #define OPENTHREAD_ENABLE_COVERAGE 0
#endif

static otInstance *    sInstance       = NULL;
static bool            sButtonPressed  = false;
static bool            sStayAwake      = true;

otInstance * otGetInstance(void)
{
  return sInstance;
}

void sl_ot_rtos_application_tick(void)
{
  if (sButtonPressed)
  {
    sButtonPressed = false;
    sStayAwake = !sStayAwake;
    if (sStayAwake)
    {
      #ifdef SL_CATALOG_POWER_MANAGER_PRESENT
        sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
      #endif
    }
    else
    {
      #ifdef SL_CATALOG_POWER_MANAGER_PRESENT
        sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
      #endif
    }
  }
}

void sl_ot_create_instance(void)
{
  sInstance = otInstanceInitSingle();
}

void sl_ot_cli_init(void)
{
  otCliUartInit(sInstance);
}

void app_init(void)
{
  ot_custom_cli_init();
  initUdp(sInstance);
  setNetworkConfiguration(sInstance);
  otIp6SetEnabled(sInstance, true);
  otThreadSetEnabled(sInstance, true);
}

void app_process_action(void)
{
  otTaskletsProcess(sInstance);
  otSysProcessDrivers(sInstance);

  if (serialInput)
  {
    sendUdp(sInstance);
    serialInput = false;
  }
}

void app_exit(void)
{
  otInstanceFinalize(sInstance);
}

void setNetworkConfiguration(otInstance *aInstance)
{
    static char          aNetworkName[] = "AsTechNet";
    otOperationalDataset aDataset;

    memset(&aDataset, 0, sizeof(otOperationalDataset));

    aDataset.mActiveTimestamp                      = 1;
    aDataset.mComponents.mIsActiveTimestampPresent = true;
    aDataset.mChannel                              = 15;
    aDataset.mComponents.mIsChannelPresent         = true;
    aDataset.mPanId                                = (otPanId)0x2222;
    aDataset.mComponents.mIsPanIdPresent           = true;

    /* Set Extended Pan ID to C0DE1AB5C0DE1AB5 */
    uint8_t extPanId[OT_EXT_PAN_ID_SIZE] = {0xC0, 0xDE, 0x1A, 0xB5, 0xC0, 0xDE, 0x1A, 0xB5};
    memcpy(aDataset.mExtendedPanId.m8, extPanId, sizeof(aDataset.mExtendedPanId));
    aDataset.mComponents.mIsExtendedPanIdPresent = true;

    /* Set master key to 1234C0DE1AB51234C0DE1AB51234C0DE */
    uint8_t key[OT_MASTER_KEY_SIZE] = {0x12, 0x34, 0xC0, 0xDE, 0x1A, 0xB5, 0x12, 0x34, 0xC0, 0xDE, 0x1A, 0xB5, 0x12, 0x34, 0xC0, 0xDE};
    memcpy(aDataset.mMasterKey.m8, key, sizeof(aDataset.mMasterKey));
    aDataset.mComponents.mIsMasterKeyPresent = true;

    /* Set Network Name to AsTechNet */
    size_t length = strlen(aNetworkName);
    memcpy(aDataset.mNetworkName.m8, aNetworkName, length);
    aDataset.mComponents.mIsNetworkNamePresent = true;

#if OPENTHREAD_FTD
    otDatasetSetActive(aInstance, &aDataset);
    // Warning: For demo purposes only - not to be used in a real product
    uint8_t jitterValue = 20;
    otThreadSetRouterSelectionJitter(aInstance, jitterValue);
#else
    OT_UNUSED_VARIABLE(aInstance);
#endif
}
