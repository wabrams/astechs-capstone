/*
 * adc.h
 *
 *  Created on: Apr 7, 2021
 *      Author: Will
 */
#include "em_adc.h"
#include "em_cmu.h"
#include <stdio.h>

#ifndef ADC_H
#define ADC_H

void initADC(void);
void ADC0_IRQHandler(void);

#endif /* ADC_H */
