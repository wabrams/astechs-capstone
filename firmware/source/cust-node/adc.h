/**
 *  @file adc.h
 *  @brief  ADC header file
**/

#include "em_adc.h"
#include "em_cmu.h"
#include <stdio.h>

#ifndef ADC_H
#define ADC_H

void initADC(void);
void startADC(void);
uint32_t getRMSADC(void);
void ADC0_IRQHandler(void);

#endif /* ADC_H */
