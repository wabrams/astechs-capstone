/**
 *  @file gpio.h
 *  @brief  GPIO header file
**/

#include <stdbool.h>

#include "em_gpio.h"

#ifndef GPIO_H
#define GPIO_H

// status LED, should be the same as the power control, always
#define GPIO_LED_STAT_PORT  gpioPortD
#define GPIO_LED_STAT_PIN   13
// this is the header pin that controls the TRIAC circuitry
#define GPIO_PWR_CTRL_PORT  gpioPortF
#define GPIO_PWR_CTRL_PIN   7
// differential input +
#define GPIO_PWR_RPOS_PORT  gpioPortC
#define GPIO_PWR_RPOS_PIN   10
// differential input -
#define GPIO_PWR_RNEG_PORT  gpioPortC
#define GPIO_PWR_RNEG_PIN   11

void gpio_init();

bool gpio_status();

bool gpio_flag();
void gpio_flag_clear();

void gpio_power_on();
void gpio_power_off();
void gpio_power_tog();

#endif /* GPIO_H */
