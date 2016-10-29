/*
 * hitLedTimer.h
 *
 *  Created on: Feb 18, 2016
 *      Author: spence13
 */

#ifndef HITLEDTIMER_H_
#define HITLEDTIMER_H_

// The hitLedTimer illuminates the LEDs (LD0 and the LED attached to pin JF-3) for 1/2 second when activated.

#define HIT_LED_TIMER_EXPIRE_VALUE 100000   // Defined in terms of 100 kHz ticks.
#define HIT_LED_TIMER_OUTPUT_PIN 11 // JF-3
#define HIT_LED_TIMER_HIGH 1//pulse a hit
#define HIT_LED_TIMER_LOW 0//you are not shooting
#define HIT_LED_TIMER_BTN3_ON 0x8//button number 3 is on
#define HIT_LED_TIMER_HALF_COUNTER 2//counts up to .5 second interval
#define HIT_LED_TIMER_LED_OFF_PULSE 300//dealy pulse between each led flash

#include <stdint.h>



// Standard init function. Implement it even if it is not necessary. You may need it later.
void hitLetTimer_init();

// Calling this starts the timer.
void hitLedTimer_start();

// Returns true if the timer is currently running.
bool hitLedTimer_running();

// Standard tick function.
void hitLedTimer_tick();

// Turns the gun's hit-LED on.
void hitLedTimer_turnLedOn();

// Turns the gun's hit-LED off.
void hitLedTimer_turnLedOff();

void hitLedTimer_runTest();


#endif /* HITLEDTIMER_H_ */