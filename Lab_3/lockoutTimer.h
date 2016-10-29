/*
 * lockoutTimer.h
 *
 *  Created on: Feb 18, 2016
 *      Author: spence13
 */

#ifndef LOCKOUTTIMER_H_
#define LOCKOUTTIMER_H_

#include <stdint.h>

// The lockoutTimer is active for 1/2 second once it is started.
// It is used to lock-out the detector once a hit has been detected.
// This ensure that only one hit is detected per 1/2-second interval.

#define LOCKOUT_TIMER_EXPIRE_VALUE 50000// Defined in terms of 100 kHz ticks.
#define LOCKOUT_TIMER_TIMER_NO_ONE 1

// Standard init function. Implement even if you don't find it necessary at present.
// Might be handy later.
void lockoutTimer_init();

// Calling this starts the timer.
void lockoutTimer_start();

// Returns true if the timer is running.
bool lockoutTimer_running();

// Standard tick function.
void lockoutTimer_tick();

void lockoutTimer_runTest();

#endif /* LOCKOUTTIMER_H_ */
