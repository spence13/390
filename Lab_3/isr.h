/*
 * isr.h
 *
 *  Created on: Feb 18, 2016
 *      Author: spence13
 */

#ifndef ISR_H_
#define ISR_H_

#include <stdint.h>

#define ISR_ADC_INTEGER_MAX_VALUE 4095.0
#define ISR_ADC_INTEGER_MID_VALUE 2048.0
#define ISR_ADC_INTEGER_MIN_VALUE 0.0



// isr provides the isr_function() where you will place functions that require accurate timing.
// A buffer for storing values from the Analog to Digital Converter (ADC) is implemented in isr.c

// Performs inits for anything in isr.c
void isr_init();

void isr_adcBufferInit();

// This function is invoked by the timer interrupt at 100 kHz.
void isr_function();

/*******************************************************************
* The functions below will be implemented in the next milestone.   *
* You can ignore them for now.                                     *
*******************************************************************/

// This is only used for testing purposes.
// Makes it possible to replace ADC data with synthetic test data.
// This is used to test the frequency responses for the filters.
void isr_addDataToAdcBuffer(uint32_t adcData);

// This removes a value from the ADC buffer.
uint32_t isr_removeDataFromAdcBuffer();

// This returns the number of values in the ADC buffer.
uint32_t isr_adcBufferElementCount();


#endif /* ISR_H_ */
