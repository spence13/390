/*
 * main.c
 *
 *  Created on: Feb 4, 2016
 *      Author: spence13
 */

#include "runningModes.h"
#include <assert.h>
#include <stdio.h>
#include "supportFiles/display.h"
#include "supportFiles/interrupts.h"
#include "src/Lab2/switches.h"
#include "src/Lab2/buttons.h"
#include "filter.h"
#include "filterTest.h"
#include "histogram.h"
#include "isr.h"
#include "transmitter.h"
#include "hitLedTimer.h"
#include "lockoutTimer.h"
#include "trigger.h"
#include "supportFiles/utils.h"
#include "supportFiles/leds.h"
#include "supportFiles/mio.h"
#include "detector.h"


#define BUTTONS_BTN2_MASK 0x4   // Bit mask for BTN2

// The program comes up in continuous mode.
// Hold BTN2 while the program starts to come up in shooter mode.
int main()
{
  buttons_init();  // Init the buttons.
    if (buttons_read() & BUTTONS_BTN2_MASK) // Read the buttons to see if BTN2 is drepressed.
      runningModes_continuous();            // Otherwise, go to continuous mode.
    else
      runningModes_shooter();               // Run shooter mode if BTN2 is depressed.
}

