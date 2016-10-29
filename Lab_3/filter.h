/*
 * filter.h
 *
 *  Created on: Feb 4, 2016
 *      Author: spence13
 */


#ifndef FILTER_H_
#define FILTER_H_

#include <stdint.h>
#include "src/390_1/queue.h"
#include "src/Lab2/buttons.h"
#include "supportFiles/utils.h"

#define FILTER_SAMPLE_FREQUENCY_IN_KHZ 100//sampling frequency
#define FILTER_FREQUENCY_COUNT 10//10 players in the game
#define FILTER_FIR_DECIMATION_FACTOR 10// FIR-filter needs this many new inputs to compute a new output.
#define FILTER_INPUT_PULSE_WIDTH 2000// This is the width of the pulse you are looking for, in terms of decimated sample count.

#define FILTER_PLAYER_COUNT 10//number of player frequencies

#define FILTER_XQUEUE_SIZE 81//stores 81 values(multiply fir)
#define FILTER_YQUEUE_SIZE 11//stores 11 values(multiply b)
#define FILTER_ZQUEUE_SIZE 10//stores 10 values(multiply a)
#define FILTER_ZQUEUE_ARRAY_SIZE 10//has ten z queues
#define FILTER_OUTPUTQUEUE_SIZE 2000//2000 data sample points
#define FILTER_OUTPUTQUEUE_ARRAY_SIZE 10//stores 10 queues
#define FILTER_CURRENT_POWER_SIZE 10//stores 10 queues

#define FILTER_FIR_COEFF_COUNT 81//81 coeffs for fir
#define FILTER_IIR_B_COEFF_COUNT 11//11 b coefficients in iir
#define FILTER_IIR_A_COEFF_COUNT 10//eliminate the leading ones so one less than B
#define FILTER_BTN3_ON 0x8//button 3 is on
#define FILTER_BTN1_ON 0x2//button 2 is on
#define FILTER_DELAY 50//delay of debounce
#define FILTER_QUEUE_NEW_VALUE_TWO 2//second filter in the queue
#define FILTER_QUEUE_REPLACEMENT_INDEX 2//fill the queues with twos

// These are the tick counts that are used to generate the user frequencies. Not used in filter.h but are used to TEST the filter code.
// The transmitter will also use these.
const uint16_t filter_frequencyTickTable[FILTER_FREQUENCY_COUNT] = {68, 58, 50, 44, 38, 34, 30, 28, 26, 24};

// Filtering routines for the laser-tag project.
// Filtering is performed by a two-stage filter, as described below.

// 1. First filter is a decimating FIR filter with a configurable number of taps and decimation factor.
// 2. The output from the decimating FIR filter is passed through a bank of 10 IIR filters. The
// characteristics of the IIR filter are fixed.

/*********************************************************************************************************
****************************************** Main Filter Functions *****************************************
**********************************************************************************************************/

// Must call this prior to using any filter functions.
void filter_init();

void initXQueue();
void initYQueue();
void initZQueues();
void initOutputQueues();

// Use this to copy an input into the input queue of the FIR-filter (xQueue).
void filter_addNewInput(double x);

// Complete fills a queue with the given fillValue, overwriting anything currently in the queue.
void filter_fillQueue(queue_t* q, double fillValue);

// Invokes the FIR-filter. Input is contents of xQueue.
// Output is returned and is also pushed on to yQueue.
double filter_firFilter();

// Use this to invoke a single iir filter. Input comes from yQueue.
// Output is returned and is also pushed onto zQueue[filterNumber].
double filter_iirFilter(uint16_t filterNumber);

// Use this to compute the power for values contained in a queue.
// If force == true, then recompute everything from scratch.
double filter_computePower(uint16_t filterNumber, bool forceComputeFromScratch, bool debugPrint);

// Returns the last-computed output power value for the IIR filter [filterNumber].
double filter_getCurrentPowerValue(uint16_t filterNumber);

// Get a copy of the current power values.
void filter_getCurrentPowerValues(double* powerValues);

// Uses the last computed-power values, scales them to the provided lowerBound and upperBound args, returns the index of element containing the max value.
// The caller provides the normalizedArray that will contain the normalized values. indexOfMaxValue indicates the channel with max. power.
void filter_getNormalizedPowerValues(double normalizedArray[], uint16_t* indexOfMaxValue);

/*********************************************************************************************************
********************************** Verification-assisting functions. *************************************
********* Test functions access the internal data structures of the filter.c via these functions. ********
*********************** These functions are not used by the main filter functions. ***********************
**********************************************************************************************************/

// Returns the array of FIR coefficients.
const double* filter_getFirCoefficientArray();

// Returns the number of FIR coefficients.
uint32_t filter_getFirCoefficientCount();

// Returns the array of a-coefficients for a particular filter number.
const double* filter_getIirACoefficientArray(uint16_t filterNumber);

// Returns the number of a-coefficients.
uint32_t filter_getIirACoefficientCount();

// Returns the array of b-coefficients for a particular filter number.
const double* filter_getIirBCoefficientArray(uint16_t filterNumber);

// Returns the number of b-coefficients.
uint32_t filter_getIirBCoefficientCount();

// Returns the size of the yQueue.
uint32_t filter_getYQueueSize();

// Returns the decimation value.
uint16_t filter_getDecimationValue();

// Returns the address of xQueue.
queue_t* filter_getXQueue();

// Returns the address of yQueue.
queue_t* filter_getYQueue();

// Returns the address of zQueue for a specific filter number.
queue_t* filter_getZQueue(uint16_t filterNumber);

// Returns the address of the IIR output-queue for a specific filter-number.
queue_t* filter_getIirOutputQueue(uint16_t filterNumber);

void filter_runTest();

void filter_PowerValues(double a,double b,double c,double d,double e,double f,double g,double h,double i,double j);



#endif /* FILTER_H_ */
