/**
 * @file app.c
 * @brief   APP source file
**/

#include "app.h"
#include "astechs.h"
#include "adc.h"
#include "gpio.h"
#include "udp.h"

#include "sl_component_catalog.h"
#include "sl_sleeptimer.h"

#ifdef APP_ADC
  #define DELAY_MS 4000
  sl_sleeptimer_timer_handle_t timer;
  bool delay_timeout = false;
  uint32_t rms;
  char rms_str[32];
#endif

#ifdef SL_CATALOG_POWER_MANAGER_PRESENT
  #include "sl_power_manager.h"
#endif

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

#ifdef APP_ADC
  /**
   * @name  on_timeout
   * @brief   the callback function for the ADC timer, prompts app.c to transmit power readings periodically
   * @param handle  unused
   * @param data    unused
  **/
  static void on_timeout(sl_sleeptimer_timer_handle_t *handle, void * data)
  {
    UNUSED(handle); UNUSED(data);
    delay_timeout = true;
  }
#endif

/**
 * @name  otGetInstance
 * @returns returns the pointer to the OpenThread instance
**/
otInstance * otGetInstance(void)
{
  return sInstance;
}

/**
 * @name sl_ot_create_instance
 * @brief   creates the single OpenThread instance
**/
void sl_ot_create_instance(void)
{
  sInstance = otInstanceInitSingle();
}

/**
 *  @name sl_ot_cli_init
 *  @brief  initializes the OpenThread CLI
**/
void sl_ot_cli_init(void)
{
  otCliUartInit(sInstance);
}

/**
 * @name app_init
 * @brief   initializes peripherals for the application
**/
void app_init(void)
{
  // peripheral inits
  gpio_init();
  initUdp(sInstance);

  // configurable inits
  #ifdef APP_ADC
    initADC();
    sl_sleeptimer_start_periodic_timer_ms(&timer, DELAY_MS, on_timeout, NULL, 0, SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG);
  #endif

  // openthread inits
  setNetworkConfiguration(sInstance);
  otIp6SetEnabled(sInstance, true);
  otThreadSetEnabled(sInstance, true);
}

/**
 * @name app_process_action
 * @brief the application event handler loop
**/
void app_process_action(void)
{
  otTaskletsProcess(sInstance);
  otSysProcessDrivers(sInstance);

  // update the system if the node power status has changed
  if (gpio_flag())
  {
      if (gpio_status())
          setUdpPayload(UDP_RESPONSE_ON);
      else
          setUdpPayload(UDP_RESPONSE_OFF);
      sendUdp(sInstance);
      gpio_flag_clear();
  }

  #ifdef APP_ADC
    // send power readings from ADC
    if (delay_timeout)
    {
      delay_timeout = false;
      rms = getRMSADC();
      sprintf(rms_str, "NODE1 ADC %d\n", (gpio_status() ? (int) rms : 0));
      setUdpPayload(rms_str);
      sendUdp(sInstance);

      // when we send the ADC data, send node status too!
      if (gpio_status())
        setUdpPayload(UDP_RESPONSE_ON);
      else
        setUdpPayload(UDP_RESPONSE_OFF);
      sendUdp(sInstance);

      // start another set of measurements
      startADC();
    }
  #endif
}

/**
 * @name app_exit
 * @brief   closes the application
**/
void app_exit(void)
{
  otInstanceFinalize(sInstance);
}

/**
 * @name  setNetworkConfiguration
 * @brief   configures the OpenThread instance for our AsTechs network
 * @param aInstance the OpenThread instance
**/
void setNetworkConfiguration(otInstance *aInstance)
{
  // network name variable
  static char aNetworkName[] = "AsTechNet";

  // configure the dataset
  otOperationalDataset aDataset;
  memset(&aDataset, 0, sizeof(otOperationalDataset));
  aDataset.mActiveTimestamp                      = 1;
  aDataset.mComponents.mIsActiveTimestampPresent = true;
  aDataset.mChannel                              = 15;
  aDataset.mComponents.mIsChannelPresent         = true;
  aDataset.mPanId                                = (otPanId)0x2222;
  aDataset.mComponents.mIsPanIdPresent           = true;

  // configure the PAN ID
  uint8_t extPanId[OT_EXT_PAN_ID_SIZE] = {0xC0, 0xDE, 0x1A, 0xB5, 0xC0, 0xDE, 0x1A, 0xB5};
  memcpy(aDataset.mExtendedPanId.m8, extPanId, sizeof(aDataset.mExtendedPanId));
  aDataset.mComponents.mIsExtendedPanIdPresent = true;

  // Set master key to 1234C0DE1AB51234C0DE1AB51234C0DE
  uint8_t key[OT_MASTER_KEY_SIZE] = {0x12, 0x34, 0xC0, 0xDE, 0x1A, 0xB5, 0x12, 0x34, 0xC0, 0xDE, 0x1A, 0xB5, 0x12, 0x34, 0xC0, 0xDE};
  memcpy(aDataset.mMasterKey.m8, key, sizeof(aDataset.mMasterKey));
  aDataset.mComponents.mIsMasterKeyPresent = true;

  // Set Network Name to AsTechNet
  size_t length = strlen(aNetworkName);
  memcpy(aDataset.mNetworkName.m8, aNetworkName, length);
  aDataset.mComponents.mIsNetworkNamePresent = true;

  #if OPENTHREAD_FTD
    otDatasetSetActive(aInstance, &aDataset);
    // override the 2 minute default, for faster pairing during demos
    uint8_t jitterValue = 20;
    otThreadSetRouterSelectionJitter(aInstance, jitterValue);
  #else
    OT_UNUSED_VARIABLE(aInstance);
  #endif
}
