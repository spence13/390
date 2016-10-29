/*
 * transmitter.c
 *
 *  Created on: Feb 18, 2016
 *      Author: spence13
 */


#include "transmitter.h"
#include "filter.h"
#include "supportFiles/interrupts.h"
#include "src/Lab2/switches.h"
#include "src/Lab2/buttons.h"
#include "supportFiles/utils.h"


uint32_t frequency = 0; //create frequency variable
uint32_t used_frequency = 0;//create used frequency variable to control when it changes
uint32_t tick_counter = 0;//create counter variable
uint32_t overall_counter = 0;//create overall counter variable
bool continuous = false;//create bool to control when to use continous
bool runTransmitter = false;//create a bool to control the enabling of the state machine

// The transmitter state machine generates a square wave output at the chosen frequency
// as set by transmitter_setFrequencyNumber(). The step counts for the frequencies
// are provided in filter.h

// Standard init function.
void transmitter_init()
{
    mio_init(false);  // false disables any debug printing if there is a system failure during init.
    mio_setPinAsOutput(TRANSMITTER_OUTPUT_PIN);  // Configure the signal direction of the pin to be an output.
}

// Starts the transmitter.
void transmitter_run()
{
    runTransmitter = true;//enables the transmitter to run
}

// Returns true if the transmitter is still running.
bool transmitter_running()
{
    return runTransmitter;//returns if the transmitter is running
}

// Sets the frequency number. If this function is called while the
// transmitter is running, the frequency will not be updated until the
// transmitter stops and transmitter_run() is called again.
void transmitter_setFrequencyNumber(uint16_t frequencyNumber)
{
    if(frequencyNumber >= TRANSMITTER_ARRAY_NUMBER_PLAYERS)//if you try to access a number higher than what the array holds
    {
        frequency = filter_frequencyTickTable[TRANSMITTER_ARRAY_NUMBER_PLAYERS];//just set it to the highest possible player frequency
    }
    else
    {
        frequency = filter_frequencyTickTable[frequencyNumber];//else just do whatever player frequency from the array they choose
    }
}


enum Verify_States
{
    inactive_st,//this state just idles
    init_st,// Start here, stay in this state for just one tick.
    high_st,//enable button handler
    low_st,//check to see if you've finished
}
currentState = inactive_st;//initialize the state to be inactive

// Standard tick function
void transmitter_tick()
{
    switch(currentState)// actions----------------------------------------------
    {
    case inactive_st:
        used_frequency = frequency; //only change the player frequency when in inactive state
        break;

    case init_st:
        tick_counter = 0;//reset the counter
        overall_counter = 0;//reset the overall counter
        break;

    case high_st:
        tick_counter++;//increment the counter
        overall_counter++;//increment the overall counter
        break;

    case low_st:
        tick_counter++;//increment the counter
        overall_counter++;//increment the overall counter
        break;
    }

    switch(currentState)// transitions----------------------------------------------
    {
    case inactive_st:
        if(runTransmitter == true)//leave inactive state if SM is enabled
        {
            currentState = init_st;//go to init state
        }
        break;

    case init_st:
        currentState = high_st;// go to high state
        mio_writePin(TRANSMITTER_OUTPUT_PIN, TRANSMITTER_HIGH_VALUE); // Write a '1' to JF-1.
        break;

    case high_st:
        if(continuous == false)//this is the non continuous mode
        {
            if(overall_counter >= TRANSMITTER_PULSE_WIDTH)//pulse for 200ms
            {
                mio_writePin(TRANSMITTER_OUTPUT_PIN, TRANSMITTER_LOW_VALUE); // Write a '0' to JF-1.
                runTransmitter = false;//reset to false after reading it is true so that SM doesnt always run
                currentState = inactive_st;//go to inactive state
            }
            else
            {
                if(tick_counter >= (used_frequency)/TRANSMITTER_HALF_FREQUENCY)//this makes it pulse for the right frequency
                {
                    mio_writePin(TRANSMITTER_OUTPUT_PIN, TRANSMITTER_LOW_VALUE); // Write a '0' to JF-1.
                    tick_counter = 0;//reset the counter
                    currentState = low_st;//go to low state
                }
                else
                {
                    currentState = high_st;//keep pulsing high
                }
            }
        }
        else//continuous is true
        {
            if(tick_counter >= (frequency)/TRANSMITTER_HALF_FREQUENCY)//changes the frequency immediately whenever it is changed
            {
                mio_writePin(TRANSMITTER_OUTPUT_PIN, TRANSMITTER_LOW_VALUE); // Write a '0' to JF-1.
                tick_counter = 0;//reset the counter
                currentState = low_st;//go to low state
            }
            else
            {
                currentState = high_st;//keep pulsing high
            }
        }
        break;

    case low_st:
        if(continuous == false)//this is the non continuous mode
        {
            if(overall_counter >= TRANSMITTER_PULSE_WIDTH)//pulse for 200ms
            {
                mio_writePin(TRANSMITTER_OUTPUT_PIN, TRANSMITTER_LOW_VALUE); // Write a '0' to JF-1.
                runTransmitter = false;//reset to false after reading it is true so that SM doesnt always run
                currentState = inactive_st;//go to inactive
            }
            else
            {
                if(tick_counter >= (used_frequency)/TRANSMITTER_HALF_FREQUENCY)//this makes it pulse for the right frequency
                {
                    mio_writePin(TRANSMITTER_OUTPUT_PIN, TRANSMITTER_HIGH_VALUE); // Write a '1' to JF-1.
                    tick_counter = 0;//reset the counter
                    currentState = high_st;//go to low state
                }
                else
                {
                    currentState = low_st;//keep pulsing low until you reach right amount of ticks
                }
            }
        }
        else//continuous is true
        {
            if(tick_counter >= (frequency)/TRANSMITTER_HALF_FREQUENCY)//changes the frequency immediately whenever it is changed
            {
                mio_writePin(TRANSMITTER_OUTPUT_PIN, TRANSMITTER_HIGH_VALUE); // Write a '1' to JF-1.
                tick_counter = 0;//reset the counter
                currentState = high_st;//go to low state
            }
            else
            {
                currentState = low_st;//keep pulsing low until you reach right amount of ticks
            }
        }
        break;

    default:
        currentState = inactive_st;//the default state is inactive state
        break;
    }
}

// Runs the transmitter continuously.
// if continuousModeFlag == true, transmitter runs continuously, otherwise, transmits one pulse-width and stops.
// To set continuous mode, you must invoke this function prior to calling transmitter_run().
// If the transmitter is currently in continuous mode, it will stop running if this function is
// invoked with continuousModeFlag == false.
void transmitter_setContinuousMode(bool continuousModeFlag)
{
    continuous = continuousModeFlag;//set the continous variablee to whatever you pass in true or false
}

// Tests the transmitter.
void transmitter_runTest()
{
    transmitter_init();//initialize the transmitter before anything

    //continuous
    transmitter_setContinuousMode(true);//set the mode ot continuous
    transmitter_run();//enable the transmitter to run
    while (buttons_read() != TRANSMITTER_BTN3_ON)//do thus until you hit btn 3
    {
        transmitter_setFrequencyNumber(switches_read());//set the frequency to whatever the swithces are
    }

    //non-continuous
    transmitter_setContinuousMode(false);//set it to non continous mode
    transmitter_setFrequencyNumber(switches_read());//intialize the frequency to switches
    transmitter_run();//enable the transmitter to run
    while (buttons_read() != TRANSMITTER_BTN2_ON)//do this until you hit btn 2
    {
        utils_msDelay(TRANSMITTER_PULSE_DELAY);//delay before you do the next 200 ms state machine pulse
        transmitter_setFrequencyNumber(switches_read());//read the frequency you want for the next pulse
        transmitter_run();//re enable the transmitter to run for another pulse
    }
}


