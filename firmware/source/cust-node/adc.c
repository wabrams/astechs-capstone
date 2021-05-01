/**
 * @file adc.c
 * @brief   ADC source file
**/
#include "adc.h"
#include "math.h"

#define adcFreq  2000000
#define ADC_BUF_LEN 64

volatile int32_t sample;
volatile int32_t millivolts;
volatile int32_t adc_buf[ADC_BUF_LEN];
volatile uint8_t i;

/**
 * @name  initADC
 * @brief   initializes the ADC for singleshot differential measurement
**/
void initADC()
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
  init.ovsRateSel = adcOvsRateSel64;

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
}

/**
 * @name  startADC
 * @brief   starts a sequence of ADC measures
**/
void startADC(void)
{
  i = 0;
  ADC_Start(ADC0, adcStartSingle);
}

/**
 * @name  getRMSADC
 * @brief   calculates and returns the RMS value for the WebUI
 * @returns average power consumption
**/
uint32_t getRMSADC(void)
{
  int32_t max = 0;
  int32_t new;

  for (int i = 0; i < ADC_BUF_LEN; i++)
  {
    new = adc_buf[i];

    if (new < 0)
      new = new * -1;

    if (new > max)
      max = new;
  }

//  uint32_t avg = sum / ADC_BUF_LEN;
//  uint32_t rms = sqrt(avg);

    int32_t rms = (double)max * 7 / 20;
    return rms;
}

/**
 * @name  ADC0_IRQHandler
 * @brief   the interrupt handler for the ADC0
**/
void ADC0_IRQHandler(void)
{
  // Get ADC result
  sample = ADC_DataSingleGet(ADC0);

  // Calculate input voltage in mV
  millivolts = (sample  * 2500) >> 16;
  adc_buf[i] = millivolts;
  i += 1;
  // Start next ADC conversion
  if (i != ADC_BUF_LEN)
    ADC_Start(ADC0, adcStartSingle);
}
