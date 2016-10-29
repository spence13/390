/*
 * lockoutTimer.c
 *
 *  Created on: Feb 18, 2016
 *      Author: spence13
 */


#include "lockoutTimer.h"
#include "src/Lab3/intervalTimer.h"

volatile static bool run_lockoutTimer = false;//bool to enable the SM
uint32_t lockout_counter = 0;//counter to time the lockout

// Standard init function. Implement even if you don't find it necessary at present.
// Might be handy later.
void lockoutTimer_init()
{

}

// Calling this starts the timer.
void lockoutTimer_start()
{
    run_lockoutTimer = true;//enables the running of the SM
}

// Returns true if the timer is running.
bool lockoutTimer_running()
{
    return run_lockoutTimer;//tells if the SM is running
}

//create the states
enum Lockout_States
{
    idle_st,//this state does nothing
    init_st,// Start here, stay in this state for just one tick.
    timer_st,//enable button handler
}
lockoutState = idle_st;// begin in idle state

// Standard tick function.
void lockoutTimer_tick()
{
    switch(lockoutState)// actions----------------------------------------------
        {
        case idle_st:
            break;

        case init_st:
            lockoutTimer_init();//init the SM
            lockout_counter = 0;//reset the counter
            break;

        case timer_st:
            lockout_counter++;//increment the counter
            break;
        }

        switch(lockoutState)// transitions----------------------------------------------
        {
        case idle_st:
            if(run_lockoutTimer == true)//if SM is enabled
            {
                lockoutState = init_st;//go to the init state
            }
            else
            {
                lockoutState = idle_st;//remain in the idle state
            }
            break;

        case init_st:
            lockoutState = timer_st;//go to timer state
            break;

        case timer_st:
            if(lockout_counter >= LOCKOUT_TIMER_EXPIRE_VALUE)//if its gone for a half second
            {
                run_lockoutTimer = false;//disable the SM
                lockoutState = idle_st;//go to the idle state
            }
            else
            {
                lockoutState = timer_st;//else remain in the timer state
            }
            break;

        default:
            lockoutState = idle_st;//default is to stay in the idle state
            break;
        }
}


void lockoutTimer_runTest()
{
    double duration1 = 0;//holds value of interval timer
    lockoutTimer_init();//initialze the SM
    intervalTimer_init(LOCKOUT_TIMER_TIMER_NO_ONE);//init the interval timer
    intervalTimer_reset(LOCKOUT_TIMER_TIMER_NO_ONE);//reset the interval timer
    intervalTimer_start(LOCKOUT_TIMER_TIMER_NO_ONE);//start interval timer
    lockoutTimer_start();//enables the SM
    while( lockoutTimer_running() == true)//delay while the SM is running
    {

    }
    intervalTimer_stop(LOCKOUT_TIMER_TIMER_NO_ONE);//stop interval timer
    intervalTimer_getTotalDurationInSeconds(LOCKOUT_TIMER_TIMER_NO_ONE, &duration1);//get the value of the timer
    printf("Time Duration 1: %6.2e seconds.\n\r", duration1);//print the time duration of the interval timer
}



