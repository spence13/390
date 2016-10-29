/*
 * trigger.c
 *
 *  Created on: Feb 18, 2016
 *      Author: spence13
 */


#include "trigger.h"


bool runTrigger = false;//bool that enables the SM
uint32_t trigger_tick_counter = 0;//create a counter variable


// The trigger state machine debounces both the press and release of gun trigger.
// Ultimately, it will activate the transmitter when a debounced press is detected.

// Init trigger data-structures.
// Determines whether the trigger switch of the gun is connected (see discussion in lab web pages).
// Initializes the mio subsystem.
void trigger_init()
{
    mio_init(false);  // false disables any debug printing if there is a system failure during init.
    mio_setPinAsInput(TRIGGER_INPUT_PIN);  // Configure the signal direction of the pin to be an output.
}

// Enable the trigger state machine. The trigger state-machine is inactive until this function is called.
// This allows you to ignore the trigger when helpful (mostly useful for testing).
// I don't have an associated trigger_disable() function because I don't need to disable the trigger.
void trigger_enable()
{
    runTrigger = true;//enable the SM
}

//create the states
enum Trigger_States
{
    inactive_st,//do nothing here
    wait_for_touch_st,// Start here, stay in this state for just one tick.
    debounce_st,//enable button handler
    fire_wait_for_release_st,//check to see if you've finished
    debounce_release,//debounces the release
}
triggerState = inactive_st;//begin in the inactive state

// Standard tick function.
void trigger_tick()
{
    switch(triggerState)// actions----------------------------------------------
    {
    case inactive_st:
        break;

    case wait_for_touch_st:
        trigger_tick_counter = 0;//reset the counter
        break;

    case debounce_st:
        trigger_tick_counter++;//increment the counter
        break;

    case fire_wait_for_release_st:
        trigger_tick_counter = 0;//reset the counter
        break;

    case debounce_release:
        trigger_tick_counter++;//increment the counter
        break;
    }

    switch(triggerState)// transitions----------------------------------------------
    {
    case inactive_st:
        if(runTrigger == true)//if the SM is enabled
        {
            triggerState = wait_for_touch_st;//go to wait for touch state
        }
        break;

    case wait_for_touch_st:

        if(triggerPressed() == true)//if trigger has been pressed
        {
            triggerState = debounce_st;//do to debounce state
        }
        break;

    case debounce_st:

        if(triggerPressed() == false)//if the value has changed go back to waiting for touch
        {
            triggerState = wait_for_touch_st;//go back to wait for touch state
        }
        else if(trigger_tick_counter >= TRIGGER_DEBOUNCE_EXPIRED_VALUE)//if the debounce value has been reached
        {
            transmitter_run();//send the transmit signal to activate transmitter SM
            //printf("D\n\r");//print a "d"
            triggerState = fire_wait_for_release_st;//go to firing state
        }
        else
        {
            triggerState = debounce_st;//else stay in debounce state until debounce time has been reached
        }
        break;

    case fire_wait_for_release_st:

        if(triggerPressed() == false)//if trigger was released
        {
            triggerState = debounce_release;//debounce the release
        }
        else
        {
            triggerState = fire_wait_for_release_st;//else stay in the firing state
        }
        break;

    case debounce_release:

        if(triggerPressed() == true)//if the value has changed go back to waiting for touch
        {
            triggerState = fire_wait_for_release_st;//go back to firing state
        }
        else if(trigger_tick_counter >= TRIGGER_DEBOUNCE_EXPIRED_VALUE)//if debounce timer has been reached
        {
            //runTrigger = false;//disable the SM
            //printf("U\n\r");//print a "d"
            triggerState = inactive_st;//go to inactive state
        }
        else
        {
            triggerState = debounce_release;//else stay in the debounce release state
        }
        break;

    default:
        triggerState = inactive_st;//default is to stay in the inactive state
        break;
    }
}


//check to see if trigger was pressed
bool triggerPressed()
{
    if(((mio_readPin(TRIGGER_INPUT_PIN) == TRIGGER_PRESSED)) || (buttons_read() == TRIGGER_BTN0_ON))// if either button or trigger was pressed
    {
        return true;//trigger was pulled
    }
    else
    {
        return false;//trigger has not been pulled
    }
}

//tests the functionality of the SM
void trigger_runTest()
{
    trigger_init();//initialize the SM
    trigger_enable();//enalbe the SM
    while(buttons_read() != TRIGGER_BTN2_ON)// stay here while btn 2 is not pressed
    {
        trigger_enable();//keep re enabling the trigger SM
    }
    runTrigger = false;//disable the SM as you leave the run test
}



