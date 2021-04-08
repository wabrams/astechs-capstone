/*
 * adc.c
 *
 *  Created on: Apr 7, 2021
 *      Author: Will
 */
#include "adc.h"

// Init to max ADC clock for Series 1
#define adcFreq  16000000 // 16 MHz was the default

volatile int32_t sample;
volatile int16_t millivolts;
volatile uint32_t count;

/**************************************************************************//**
 * @brief  Initialize ADC function
 *****************************************************************************/
void initADC(void)
{
  // Enable required clocks
  CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(cmuClock_ADC0, true);

  // Declare init structs
  ADC_Init_TypeDef init = ADC_INIT_DEFAULT;
  ADC_InitSingle_TypeDef initSingle = ADC_INITSINGLE_DEFAULT;

  // Modify init structs and initialize
  init.prescale = ADC_PrescaleCalc(adcFreq, 0);
  init.timebase = ADC_TimebaseCalc(0);
  init.ovsRateSel = adcOvsRateSel4096;

  initSingle.diff       = true;        // differential
  initSingle.reference  = adcRef2V5;   // internal 2.5V reference
  initSingle.resolution = adcResOVS;
  initSingle.acqTime    = adcAcqTime4; // set acquisition time to meet minimum requirements


  // Select ADC input. See README for corresponding EXP header pin.
  initSingle.posSel = adcPosSelAPORT2XCH11;
  initSingle.negSel = adcNegSelAPORT2YCH10;

  // Initialize ADC and Single conversions
  ADC_Init(ADC0, &init);
  ADC_InitSingle(ADC0, &initSingle);

  // Enable ADC Single Conversion Complete interrupt
  ADC_IntEnable(ADC0, ADC_IEN_SINGLE);

  // Enable ADC interrupts
  NVIC_ClearPendingIRQ(ADC0_IRQn);
  NVIC_EnableIRQ(ADC0_IRQn);

  count = 0;
  ADC_Start(ADC0, adcStartSingle);
}

/**************************************************************************//**
 * @brief  ADC Handler
 *****************************************************************************/
void ADC0_IRQHandler(void)
{
  // Get ADC result
  sample = ADC_DataSingleGet(ADC0);

  // Calculate input voltage in mV
  millivolts = (sample * 2500) / 4096;
  count += 1;
  // Start next ADC conversion
  ADC_Start(ADC0, adcStartSingle);
}
