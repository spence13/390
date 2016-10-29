/*
 * hitLedTimer.c
 *
 *  Created on: Feb 18, 2016
 *      Author: spence13
 */

#include "hitLedTimer.h"
#include "supportFiles/leds.h"
#include "supportFiles/mio.h"
#include "src/Lab2/buttons.h"
#include "supportFiles/utils.h"


volatile static bool run_LedTimer = false;//bool to enable the SM to run
uint32_t led_counter = 0;//create a counter variable


// Standard init function. Implement it even if it is not necessary. You may need it later.
void hitLetTimer_init()
{
    leds_init(false);//initializes the leds
    mio_init(false);  // false disables any debug printing if there is a system failure during init.
    mio_setPinAsOutput(HIT_LED_TIMER_OUTPUT_PIN);  // Configure the signal direction of the pin to be an output.
}

// Calling this starts the timer.
void hitLedTimer_start()
{
    run_LedTimer = true;//enable the SM to run
}

// Returns true if the timer is currently running.
bool hitLedTimer_running()
{
    return run_LedTimer;//tells you if the SM is currently running
}

//create the states
enum Led_States
{
    idle_low_st,//this does nothing just waits
    init_st,// Start here, stay in this state for just one tick.
    high_st,//enable button handler
}
ledState = idle_low_st;//initialize the first state to be idle

// Standard tick function.
void hitLedTimer_tick()
{
    switch(ledState)// actions----------------------------------------------
    {
    case idle_low_st:
        hitLedTimer_turnLedOff();//turns the led off when in low state
        break;

    case init_st:
        hitLetTimer_init();//initialze the SM
        led_counter = 0;//reset the counter
        break;

    case high_st:
        led_counter++;//increment the counter
        break;
    }

    switch(ledState)// transitions----------------------------------------------
    {
    case idle_low_st:
        if(run_LedTimer == true)//start the SM if enabled
        {
            ledState = init_st;//go to init state
        }
        else
        {
            ledState = idle_low_st;//else stay in idle state until enabled
        }
        break;

    case init_st:
        hitLedTimer_turnLedOn();//turn the led on
        ledState = high_st;//go immediateley ot high state no matter what
        break;

    case high_st:
        if(led_counter >= HIT_LED_TIMER_EXPIRE_VALUE/HIT_LED_TIMER_HALF_COUNTER)//if you reached .5 seconds
        {
            hitLedTimer_turnLedOff();//turn the led off
            run_LedTimer = false;//disable the state machine
            ledState = idle_low_st;//return to idle state
        }
        else
        {
            ledState = high_st;//else stay in high state until .5 seconds are over
        }
        break;

    default:
        ledState = idle_low_st;//default is to stay in idle state
        break;
    }
}

// Turns the gun's hit-LED on.
void hitLedTimer_turnLedOn()
{
    leds_write(HIT_LED_TIMER_HIGH);//turn on the leds
    mio_writePin(HIT_LED_TIMER_OUTPUT_PIN, HIT_LED_TIMER_HIGH); // Write a '1' to JF-3.
}

// Turns the gun's hit-LED off.
void hitLedTimer_turnLedOff()
{
    leds_write(HIT_LED_TIMER_LOW);//turn off the leds
    mio_writePin(HIT_LED_TIMER_OUTPUT_PIN, HIT_LED_TIMER_LOW); // Write a '0' to JF-3.
}

void hitLedTimer_runTest()
{
    hitLetTimer_init();//intiliaze the SM
    hitLedTimer_start();//enable the SM to run

    while(buttons_read() != HIT_LED_TIMER_BTN3_ON)//do this until btn 3 is pressed
    {
        while(hitLedTimer_running() == true)//delay while the SM runs through
        {

        }
        utils_msDelay(HIT_LED_TIMER_LED_OFF_PULSE);//delay between each .5 second led flash
        hitLedTimer_start();//re enable the SM
    }
}
