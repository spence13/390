/*
 * detector.c
 *
 *  Created on: Mar 5, 2016
 *      Author: spence13
 */

#include "detector.h"
#include "lockoutTimer.h"
#include "filter.h"
#include "isr.h"
#include "hitLedTimer.h"
#include "supportFiles/interrupts.h"

bool hitDetected = false;//create a boolean that regulates hits
detector_hitCount_t detector_hitArray[DETECTOR_HITARRAY_SIZE];//array to keep track of hits per player
uint32_t addFilterCounter = 0;//create variable to keep track of filter counter
double powerArray[DETECTOR_HITARRAY_SIZE];//array to track the power of each player frequency



// Always have to init things.
void detector_init()
{
    filter_init();//intialize the filter before running the detectore
}

// Runs the entire detector: decimating fir-filter, iir-filters, power-computation, hit-detection.
// if interruptsNotEnabled = true, interrupts are not running.
// if ignoreSelf == true, ignore hits that are detected on your frequency.
// Use this to determine whether you should disable and re-enable interrrupts when accessing the adc queue.
void detector(bool interruptsEnabled, bool ignoreSelf)
{
    uint32_t elementCount = isr_adcBufferElementCount();//create variable to keep track of how many elements are in buffer
    double rawAdcValue = 0;//create variable to hold value pulled from buffer
    for(uint32_t i = 0; i < elementCount; i++)//loop through the detector for every element in buffer
    {
        if(interruptsEnabled == true)//you need to disable the interrupts when interacting with buffer
        {
            interrupts_disableArmInts();//disable the interrupts
            rawAdcValue = isr_removeDataFromAdcBuffer();//pull the value from the buffer
            interrupts_enableArmInts();//re enable the interrupts
        }
        else
        {
            rawAdcValue = isr_removeDataFromAdcBuffer();//pull value from the buffer
            interrupts_enableArmInts();//make sure the interrupts are enabled
        }

        double scaledAdcValue = (rawAdcValue/DETECTOR_ADC_MAPPING) - 1.0;//map the adc to value between -1 and 1
        addFilterCounter++;//increment the counter cuz you are about to add to the filters
        filter_addNewInput(scaledAdcValue);//add value to the filters

        if(addFilterCounter == DETECTOR_NUMBEROFPLAYERS)//perform decimation for every ten elements you add to the buffers
        {
            addFilterCounter = 0;//reset the counter back to zero

            filter_firFilter();//call fir filter once
            for (uint16_t i=0; i<DETECTOR_NUMBEROFPLAYERS; i++)//call these ten times
            {
                filter_iirFilter(i);//call iir filter
                filter_computePower(i, false, false);//compute the power
            }

            if(lockoutTimer_running() == false)//only if you havent been shot within the last 1/2 second
            {
                detector_detectionAlgorithm();//call the hit detection algorithm to see if theres been a hit
            }
        }
    }
}

//this will detect if there has been a hit by determing the relative
//powers of all then player frequencies
void detector_detectionAlgorithm()
{
    for(uint16_t i =0; i<DETECTOR_NUMBEROFPLAYERS; i++)//loop thru all ten player frequencies
    {
        powerArray[i] = filter_getCurrentPowerValue(i);//save the power values into a new array
    }

    for (uint16_t j = 0; j < DETECTOR_NUMBEROFPLAYERS-1; j++)//loop through all but last index
    {
        uint16_t iMin = j;//save the min value

        for (uint16_t i = j+1; i < DETECTOR_NUMBEROFPLAYERS; i++)//loop through all but first(min) index
        {
            if (powerArray[i] < powerArray[iMin])//find the min value
            {
                iMin = i;//save the min value
            }
        }
        if(iMin != j)//see if the min value is less than the previous
        {
            double dummy = powerArray[j];//save it to a dummy variable
            powerArray[j] = powerArray[iMin];//swap the new min with previous min
            powerArray[iMin] = dummy;//finish the swap
        }
    }

    uint16_t index = 0;//create variable to save index
    for(uint16_t w=0; w<DETECTOR_NUMBEROFPLAYERS; w++)//loop thru all ten indices of players
    {
        if(filter_getCurrentPowerValue(w) == powerArray[DETECTOR_MAX_INDEX])//this will find which player had the highest power
        {
            index = w;//save the index of the highest power player
        }
    }

    double median = powerArray[DETECTOR_MEDIAN_INDEX];//save the median power value from sorted values
    double threshold = median*DETECTOR_FUDGEFACTOR;//multiply by the fudge factor to determin threshold

    if(powerArray[DETECTOR_MAX_INDEX] > threshold && powerArray[DETECTOR_MAX_INDEX] > 0.0)//you have officially been shot
    {
        lockoutTimer_start();//start the lockout timer
        hitLedTimer_start();//start the led SM
        detector_hitArray[index] = detector_hitArray[index] + 1;//increment hit count by one at approriate player index
        hitDetected = true;//set hit detected to true
    }
}

// Returns true if a hit was detected.
bool detector_hitDetected()
{
    return hitDetected;//returns if a hit was detected or not
}

// Clear the detected hit once you have accounted for it.
void detector_clearHit()
{
    hitDetected = false;//sets the hit bool to false
}

// Get the current hit counts.
void detector_getHitCounts(detector_hitCount_t hitArray[])
{
    for(uint16_t i = 0; i <DETECTOR_NUMBEROFPLAYERS; i++)//loop thru all ten players
    {
        hitArray[i] = detector_hitArray[i];//copy the hit detected array
    }
}

//i implemented this to test the functionality
void detector_runTest()
{
    //TEST 1 will detect====
    filter_PowerValues(DETECTOR_RIGGED_VALUE14, DETECTOR_RIGGED_VALUE3, DETECTOR_RIGGED_VALUE7, DETECTOR_RIGGED_VALUE1, DETECTOR_RIGGED_VALUE2,
            DETECTOR_RIGGED_VALUE5, DETECTOR_RIGGED_VALUE6, DETECTOR_RIGGED_VALUE2, DETECTOR_RIGGED_VALUE4, DETECTOR_RIGGED_VALUE13);//fill power value array with rigged values known to yield a hit
    detector_detectionAlgorithm();//call the detection algorithm
    if(detector_hitDetected() == true)
    {
        printf("hit was detected\n\r");//print that there was a hit
    }
    else
    {
        printf("no hit was detected\n\r");//print that there was not a hit
    }

    detector_clearHit();//clear the bool for the second test

    //TEST 2 wont detect====
    filter_PowerValues(DETECTOR_RIGGED_VALUE1, DETECTOR_RIGGED_VALUE4, DETECTOR_RIGGED_VALUE5, DETECTOR_RIGGED_VALUE5, DETECTOR_RIGGED_VALUE8,
            DETECTOR_RIGGED_VALUE9, DETECTOR_RIGGED_VALUE10, DETECTOR_RIGGED_VALUE11, DETECTOR_RIGGED_VALUE12, DETECTOR_RIGGED_VALUE14);//these will not yield a hit
    detector_detectionAlgorithm();//call the detection algorithm
    if(detector_hitDetected() == true)//if there was a hit
    {
        printf("hit was detected\n\r");//print that a hit was registered
    }
    else
    {
        printf("no hit was detected\n\r");//print that there was no hit registered
    }
}
