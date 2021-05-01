/**
 * @file gpio.c
 * @brief   GPIO source file
**/

#include "gpio.h"

static bool status = false;   /**< status keeps track of whether the outlet is on or off **/
static bool flag = false;     /**< flag keeps track of whether the outlet status has changed **/

/**
 * @name gpio_init
 * @brief initializes GPIO ports and pins
**/
void gpio_init()
{
  GPIO_PinModeSet(GPIO_LED_STAT_PORT, GPIO_LED_STAT_PIN, gpioModePushPull, 0);
  GPIO_PinModeSet(GPIO_PWR_CTRL_PORT, GPIO_PWR_CTRL_PIN, gpioModePushPull, 0);
}

/**
 * @name gpio_status
 * @brief getter for the status variable
**/
bool gpio_status()
{
  return status;
}

/**
 * @name gpio_flag
 * @brief getter for the flag variable
**/
bool gpio_flag()
{
  return flag;
}

/**
 * @name gpio_flag_clear
 * @brief clears the flag variable
 * @note  this is usually called after gpio_flag() returns true
**/
void gpio_flag_clear()
{
  flag = false;
}

/**
 * @name gpio_power_on
 * @brief turns on the outlet
**/
void gpio_power_on()
{
  GPIO_PinOutSet(GPIO_LED_STAT_PORT, GPIO_LED_STAT_PIN);
  GPIO_PinOutSet(GPIO_PWR_CTRL_PORT, GPIO_PWR_CTRL_PIN);

  if (!status)
  {
    status = true;
    flag = true;
  }
}

/**
 * @name gpio_power_off
 * @brief turns off the outlet
**/
void gpio_power_off()
{
  GPIO_PinOutClear(GPIO_LED_STAT_PORT, GPIO_LED_STAT_PIN);
  GPIO_PinOutClear(GPIO_PWR_CTRL_PORT, GPIO_PWR_CTRL_PIN);

  if (status)
  {
    status = false;
    flag = true;
  }
}

/**
 * @name gpio_power_tog
 * @brief toggles the outlet
**/
void gpio_power_tog()
{
  GPIO_PinOutToggle(GPIO_LED_STAT_PORT, GPIO_LED_STAT_PIN);
  GPIO_PinOutToggle(GPIO_PWR_CTRL_PORT, GPIO_PWR_CTRL_PIN);

  status = !status;
  flag = true;
}
