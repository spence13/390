/*
 * trigger.h
 *
 *  Created on: Feb 18, 2016
 *      Author: spence13
 */

#ifndef TRIGGER_H_
#define TRIGGER_H_



#include <stdint.h>
#include "supportFiles/mio.h"
#include "src/Lab2/buttons.h"
#include "src/Lab2/switches.h"
#include "transmitter.h"

#define TRIGGER_INPUT_PIN 10//input pin number ten
#define TRIGGER_DEBOUNCE_EXPIRED_VALUE 5000//debounce for 50ms
#define TRIGGER_BTN0_ON 0x1//btn 0 is pressed
#define TRIGGER_BTN0_OFF 0x0//btn 0 is not pressed
#define TRIGGER_PRESSED 1//trigger is being pulled
#define TRIGGER_SWITCH0_ON 0x1//switch 0 is high
#define TRIGGER_SWITCH0_OFF 0x0//switch 0 is low
#define TRIGGER_BTN2_ON 0x4//btn 2 is pressed

// The trigger state machine debounces both the press and release of gun trigger.
// Ultimately, it will activate the transmitter when a debounced press is detected.

// Init trigger data-structures.
// Determines whether the trigger switch of the gun is connected (see discussion in lab web pages).
// Initializes the mio subsystem.
void trigger_init();

// Enable the trigger state machine. The trigger state-machine is inactive until this function is called.
// This allows you to ignore the trigger when helpful (mostly useful for testing).
// I don't have an associated trigger_disable() function because I don't need to disable the trigger.
void trigger_enable();

// Standard tick function.
void trigger_tick();

bool triggerPressed();

void trigger_runTest();

#endif /* TRIGGER_H_ */
