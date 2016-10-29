/*
 * transmitter.h
 *
 *  Created on: Feb 18, 2016
 *      Author: spence13
 */

#ifndef TRANSMITTER_H_
#define TRANSMITTER_H_

#define TRANSMITTER_OUTPUT_PIN 13 // JF1 (pg. 25 of ZYBO reference manual).
#define TRANSMITTER_HIGH_VALUE 1//a shot is transmitted
#define TRANSMITTER_LOW_VALUE 0//no signal
#define TRANSMITTER_PULSE_WIDTH 20000   // Based on a system tick-rate of 100 kHz.
#define TRANSMITTER_BTN3_ON 0x8//button number 3
#define TRANSMITTER_BTN2_ON 0x4//botton number 2
#define TRANSMITTER_ARRAY_NUMBER_PLAYERS 9//10 players in the game
#define TRANSMITTER_PULSE_DELAY 500  //delay between pulse
#define TRANSMITTER_HALF_FREQUENCY 2//used to divide the frequency to control the pulse

#include <stdint.h>
#include "supportFiles/mio.h"


// The transmitter state machine generates a square wave output at the chosen frequency
// as set by transmitter_setFrequencyNumber(). The step counts for the frequencies
// are provided in filter.h

// Standard init function.
void transmitter_init();

// Starts the transmitter.
void transmitter_run();

// Returns true if the transmitter is still running.
bool transmitter_running();

// Sets the frequency number. If this function is called while the
// transmitter is running, the frequency will not be updated until the
// transmitter stops and transmitter_run() is called again.
void transmitter_setFrequencyNumber(uint16_t frequencyNumber);

// Standard tick function.
void transmitter_tick();

// Tests the transmitter.
void transmitter_runTest();

// Runs the transmitter continuously.
// if continuousModeFlag == true, transmitter runs continuously, otherwise, transmits one pulse-width and stops.
// To set continuous mode, you must invoke this function prior to calling transmitter_run().
// If the transmitter is in currently in continuous mode, it will stop running if this function is
// invoked with continuousModeFlag == false.
void transmitter_setContinuousMode(bool continuousModeFlag);

#endif /* TRANSMITTER_H_ */
