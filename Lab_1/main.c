/*
 * main.c
 *
 *  Created on: Jan 6, 2016
 *      Author: spence13
 */



#include <stdio.h>
#include "supportFiles/display.h"
#include "queue.h"

int main()
{
    queue_MyrunTest();
    queue_runTest();
}

// This function must be defined but can be left empty for now.
// You will use this function in a later lab.
// It is called in the timer interrupt service routine (see interrupts.c in supportFiles).
void isr_function()
{
    // Empty for now.
}

