/*
 * detector.h
 *
 *  Created on: Mar 5, 2016
 *      Author: spence13
 */

#ifndef DETECTOR_H_
#define DETECTOR_H_

#include <stdint.h>
#include <stdbool.h>


typedef uint16_t detector_hitCount_t;

#define DETECTOR_FUDGEFACTOR 150// fudgefactor to determine threshold
#define DETECTOR_HITARRAY_SIZE 10// size of the hit array
#define DETECTOR_MEDIAN_INDEX 4//middle of power values
#define DETECTOR_MAX_INDEX 9// max value of power indices
#define DETECTOR_ADC_MAPPING 2048.0//used to map the adc values
#define DETECTOR_NUMBEROFPLAYERS 10

//these deal with the run test functionality
#define DETECTOR_RIGGED_VALUE1 10
#define DETECTOR_RIGGED_VALUE2 15
#define DETECTOR_RIGGED_VALUE3 20
#define DETECTOR_RIGGED_VALUE4 25
#define DETECTOR_RIGGED_VALUE5 30
#define DETECTOR_RIGGED_VALUE6 35
#define DETECTOR_RIGGED_VALUE7 40
#define DETECTOR_RIGGED_VALUE8 45
#define DETECTOR_RIGGED_VALUE9 50
#define DETECTOR_RIGGED_VALUE10 55
#define DETECTOR_RIGGED_VALUE11 65
#define DETECTOR_RIGGED_VALUE12 70
#define DETECTOR_RIGGED_VALUE13 80
#define DETECTOR_RIGGED_VALUE14 150


// Always have to init things.
void detector_init();

// Runs the entire detector: decimating fir-filter, iir-filters, power-computation, hit-detection.
// if interruptsNotEnabled = true, interrupts are not running.
// if ignoreSelf == true, ignore hits that are detected on your frequency.
// Use this to determine whether you should disable and re-enable interrrupts when accessing the adc queue.
void detector(bool interruptsEnabled, bool ignoreSelf);

// Returns true if a hit was detected.
bool detector_hitDetected();

//detects if a hit was registered
void detector_detectionAlgorithm();

// Clear the detected hit once you have accounted for it.
void detector_clearHit();

// Get the current hit counts.
void detector_getHitCounts(detector_hitCount_t hitArray[]);

//i implemented this to test the functionality
void detector_runTest();

#endif /* DETECTOR_H_ */
