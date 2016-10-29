/*
 * filter.c
 *
 *  Created on: Feb 4, 2016
 *      Author: spence13
 */

#include "filter.h"

static queue_t xQueue;//x queue
static queue_t yQueue;//x queue
static queue_t zQueueArray[FILTER_ZQUEUE_ARRAY_SIZE];//array filled with 10 queues
static queue_t outputQueueArray[FILTER_OUTPUTQUEUE_ARRAY_SIZE];//array filled with 10 queues
static double currentPowerValue[FILTER_CURRENT_POWER_SIZE];//size 10 array that holds current power value of each filter
static double oldestValue[FILTER_CURRENT_POWER_SIZE];//size 10 array that holds oldest value of each filter
static double normalizedArray[FILTER_CURRENT_POWER_SIZE];//size 10 array that holds normalized values

uint16_t indexOfMaxValue = 0;//create variable to store index of highest power filter


//queue that holds the FIR coefficients size 81
const static double fir_coeffs[FILTER_FIR_COEFF_COUNT] =
{
-3.7419571351545534e-04,
-1.4495989090580264e-04,
 1.3445839840069468e-04,
 4.4830642316213255e-04,
 7.6531807459718874e-04,
 1.0331373465135487e-03,
 1.1800731034839901e-03,
 1.1263682077278507e-03,
 8.0436317045205416e-04,
 1.8380494976796130e-04,
-7.0405362986212616e-04,
-1.7511880420372180e-03,
-2.7733700303716300e-03,
-3.5309149040156679e-03,
-3.7696318917137903e-03,
-3.2768418720744191e-03,
-1.9424748964815255e-03,
 1.8806259730477174e-04,
 2.8833348934840814e-03,
 5.7293897079931369e-03,
 8.1737286693190983e-03,
 9.6085101145458457e-03,
 9.4832385820237848e-03,
 7.4294791620151132e-03,
 3.3749454983488815e-03,
-2.3769781203971116e-03,
-9.1229022020837183e-03,
-1.5803150019703140e-02,
-2.1115275608450532e-02,
-2.3686491373717741e-02,
-2.2282246600580750e-02,
-1.6019415553930511e-02,
-4.5487451768411067e-03,
 1.1826555885688103e-02,
 3.2120940483322896e-02,
 5.4737049372318020e-02,
 7.7632371517867949e-02,
 9.8561720177527026e-02,
 1.1536190288748055e-01,
 1.2623683848863210e-01,
 1.3000000000000000e-01,
 1.2623683848863210e-01,
 1.1536190288748055e-01,
 9.8561720177527026e-02,
 7.7632371517867949e-02,
 5.4737049372318020e-02,
 3.2120940483322896e-02,
 1.1826555885688103e-02,
-4.5487451768411067e-03,
-1.6019415553930511e-02,
-2.2282246600580750e-02,
-2.3686491373717741e-02,
-2.1115275608450532e-02,
-1.5803150019703140e-02,
-9.1229022020837183e-03,
-2.3769781203971116e-03,
 3.3749454983488815e-03,
 7.4294791620151132e-03,
 9.4832385820237848e-03,
 9.6085101145458457e-03,
 8.1737286693190983e-03,
 5.7293897079931369e-03,
 2.8833348934840814e-03,
 1.8806259730477174e-04,
-1.9424748964815255e-03,
-3.2768418720744191e-03,
-3.7696318917137903e-03,
-3.5309149040156679e-03,
-2.7733700303716300e-03,
-1.7511880420372180e-03,
-7.0405362986212616e-04,
 1.8380494976796130e-04,
 8.0436317045205416e-04,
 1.1263682077278507e-03,
 1.1800731034839901e-03,
 1.0331373465135487e-03,
 7.6531807459718874e-04,
 4.4830642316213255e-04,
 1.3445839840069468e-04,
-1.4495989090580264e-04,
-3.7419571351545534e-04
};



//b coefficients of IIr filter
const static double iir_b_coeffs[FILTER_PLAYER_COUNT][FILTER_IIR_B_COEFF_COUNT] =
{
         {2.7688712213941621e-08,  -0.0000000000000000e+00,  -1.3844356106970810e-07,  -0.0000000000000000e+00,   2.7688712213941620e-07,  -0.0000000000000000e+00,  -2.7688712213941620e-07,  -0.0000000000000000e+00,   1.3844356106970810e-07,  -0.0000000000000000e+00,  -2.7688712213941621e-08},
         {2.7688712800132612e-08,   0.0000000000000000e+00,  -1.3844356400066304e-07,   0.0000000000000000e+00,   2.7688712800132608e-07,   0.0000000000000000e+00,  -2.7688712800132608e-07,   0.0000000000000000e+00,   1.3844356400066304e-07,   0.0000000000000000e+00,  -2.7688712800132612e-08},
         {2.7688714102719350e-08,   0.0000000000000000e+00,  -1.3844357051359675e-07,   0.0000000000000000e+00,   2.7688714102719351e-07,   0.0000000000000000e+00,  -2.7688714102719351e-07,   0.0000000000000000e+00,   1.3844357051359675e-07,   0.0000000000000000e+00,  -2.7688714102719350e-08},
         {2.7688714030813837e-08,   0.0000000000000000e+00,  -1.3844357015406917e-07,   0.0000000000000000e+00,   2.7688714030813835e-07,   0.0000000000000000e+00,  -2.7688714030813835e-07,   0.0000000000000000e+00,   1.3844357015406917e-07,   0.0000000000000000e+00,  -2.7688714030813837e-08},
         {2.7688714056502999e-08,   0.0000000000000000e+00,  -1.3844357028251500e-07,   0.0000000000000000e+00,   2.7688714056502999e-07,   0.0000000000000000e+00,  -2.7688714056502999e-07,   0.0000000000000000e+00,   1.3844357028251500e-07,   0.0000000000000000e+00,  -2.7688714056502999e-08},
         {2.7688713697691508e-08,  -0.0000000000000000e+00,  -1.3844356848845752e-07,  -0.0000000000000000e+00,   2.7688713697691505e-07,  -0.0000000000000000e+00,  -2.7688713697691505e-07,  -0.0000000000000000e+00,   1.3844356848845752e-07,  -0.0000000000000000e+00,  -2.7688713697691508e-08},
         {2.7688717202195231e-08,  -0.0000000000000000e+00,  -1.3844358601097617e-07,  -0.0000000000000000e+00,   2.7688717202195234e-07,  -0.0000000000000000e+00,  -2.7688717202195234e-07,  -0.0000000000000000e+00,   1.3844358601097617e-07,  -0.0000000000000000e+00,  -2.7688717202195231e-08},
         {2.7688707740536391e-08,   0.0000000000000000e+00,  -1.3844353870268195e-07,   0.0000000000000000e+00,   2.7688707740536389e-07,   0.0000000000000000e+00,  -2.7688707740536389e-07,   0.0000000000000000e+00,   1.3844353870268195e-07,   0.0000000000000000e+00,  -2.7688707740536391e-08},
         {2.7688696872724591e-08,   0.0000000000000000e+00,  -1.3844348436362296e-07,   0.0000000000000000e+00,   2.7688696872724592e-07,   0.0000000000000000e+00,  -2.7688696872724592e-07,   0.0000000000000000e+00,   1.3844348436362296e-07,   0.0000000000000000e+00,  -2.7688696872724591e-08},
         {2.7688725334925538e-08,   0.0000000000000000e+00,  -1.3844362667462771e-07,   0.0000000000000000e+00,   2.7688725334925543e-07,   0.0000000000000000e+00,  -2.7688725334925543e-07,   0.0000000000000000e+00,   1.3844362667462771e-07,   0.0000000000000000e+00,  -2.7688725334925538e-08}
};

//a coefficients ofIIR filter
const static double iir_a_coeffs[FILTER_PLAYER_COUNT][FILTER_IIR_A_COEFF_COUNT] =
{
         {-5.9047104265069450e+00,   1.8744369762834090e+01,  -3.9138014487159964e+01,   5.9096571757477790e+01,  -6.6562077721040964e+01,   5.6741148047249325e+01,  -3.6080288397640032e+01,   1.6591168846245445e+01,  -5.0180940564712042e+00,   8.1597668002427848e-01},
         {-4.5918642672579653e+00,   1.3231643103100250e+01,  -2.5372334871154358e+01,   3.7052229059172362e+01,  -4.0906824214757449e+01,   3.5575496843443304e+01,  -2.3390138680410708e+01,   1.1711742396745455e+01,  -3.9023771062861456e+00,   8.1597668002428358e-01},
         {-3.0288356503796345e+00,   8.4666002500905364e+00,  -1.3848723384192677e+01,   2.0447433195696206e+01,  -2.1089951766333314e+01,   1.9632551715352164e+01,  -1.2766843818676282e+01,   7.4941060862414588e+00,  -2.5740436155798814e+00,   8.1597668002428014e-01},
         {-1.3932389482246779e+00,   5.5732080156112014e+00,  -5.5638930353094285e+00,   1.1473194149423316e+01,  -8.1171293533470994e+00,   1.1016006867180515e+01,  -5.1292490791236984e+00,   4.9331057919917676e+00,  -1.1840384337808019e+00,   8.1597668002427959e-01},
         {8.1198710318235146e-01,   5.0604381679831327e+00,   3.1593769430257215e+00,   9.9701351546828612e+00,   4.5699337253828300e+00,   9.5728557330326929e+00,   2.9125722928492843e+00,   4.4792427743860248e+00,   6.9006392559390606e-01,   8.1597668002428414e-01},
         {2.6812674200407498e+00,   7.6726577676710175e+00,   1.1833697589443144e+01,   1.7901539287865774e+01,   1.7824359318204472e+01,   1.7188132609908369e+01,   1.0909241915432322e+01,   6.7913714528710152e+00,   2.2786641735919773e+00,   8.1597668002427992e-01},
         {4.8989811224568758e+00,   1.4397683432058713e+01,   2.8212025454676507e+01,   4.1461477784067199e+01,   4.6042887114628897e+01,   3.9808994126716314e+01,   2.6007963605435904e+01,   1.2743828399460158e+01,   4.1633791122096433e+00,   8.1597668002427992e-01},
         {6.1090828056506732e+00,   1.9726586599709695e+01,   4.1692985671745106e+01,   6.3343372159185662e+01,   7.1517241261433270e+01,   6.0818667683556647e+01,   3.8435633273391758e+01,   1.7460548938678102e+01,   5.1917790887607298e+00,   8.1597668002428314e-01},
         {7.3359132993024634e+00,   2.6325163213323780e+01,   5.9749112198106246e+01,   9.4375712094960846e+01,   1.0800265485372331e+02,   9.0614016064511588e+01,   5.5080971679768609e+01,   2.3301083259506363e+01,   6.2343959766024950e+00,   8.1597668002427737e-01},
         {8.4893898596691528e+00,   3.3627566817021432e+01,   8.1545022824818972e+01,   1.3379567242752088e+02,   1.5504223931021377e+02,   1.2846260855445604e+02,   7.5173865235407291e+01,   2.9764589159239677e+01,   7.2146733236288334e+00,   8.1597668002427870e-01}
};


//initializes the x queues
void initXQueue()
{
    queue_init(&xQueue, FILTER_XQUEUE_SIZE);//initialize xQ to be of size 81
    for(uint32_t i=0; i< FILTER_XQUEUE_SIZE; i++)
    {
        queue_overwritePush(&xQueue, 0.0);//initialize the queue with zeros
    }
}

//initializes the Y queues
void initYQueue()
{
    queue_init(&yQueue, FILTER_YQUEUE_SIZE);//initialize yQ to be of size 11
    for(uint32_t i=0; i< FILTER_YQUEUE_SIZE; i++)
    {
        queue_overwritePush(&yQueue, 0.0);//intialize the queue with zeros
    }
}

//initialize the Z queues
void initZQueues()
{
    for(uint32_t i=0; i<FILTER_ZQUEUE_ARRAY_SIZE; i++)//got through the each of the ten queues in Z
    {
        queue_init(&(zQueueArray[i]), FILTER_ZQUEUE_SIZE);//initialize zQ to be of size 10

        for(uint32_t j=0; j< FILTER_ZQUEUE_SIZE; j++)//go through each of the indices in each queue
        {
            queue_overwritePush(&(zQueueArray[i]), 0.0);//intialize the queue with zeros
        }
    }
}

//initialize the output queues
void initOutputQueues()
{
    for(int i=0; i<FILTER_OUTPUTQUEUE_ARRAY_SIZE; i++)//got through the each of the ten queues in output
    {
        queue_init(&(outputQueueArray[i]), FILTER_OUTPUTQUEUE_SIZE);//initialize outputQueueArray to be of size 2000

        for(int j=0; j< FILTER_OUTPUTQUEUE_SIZE; j++)//go through each sample point in the queue
        {
            queue_overwritePush(&(outputQueueArray[i]), 0.0);//initialize the queue with zeros
        }
    }
    for(uint16_t i=0; i<10; i++)
    {
        currentPowerValue[i] = 0.0;
        oldestValue[i] = 0.0;
    }
}


// Must call this prior to using any filter functions.
void filter_init()
{
    initXQueue();//intialize X
    initYQueue();//initialize Y
    initZQueues();//intialize Z
    initOutputQueues();//initialie outputqueue
}

// Use this to copy an input into the input queue of the FIR-filter (xQueue).
void filter_addNewInput(double x)
{
    queue_overwritePush(&xQueue, x);//adds a new input to the z queue
}

// Complete fills a queue with the given fillValue, overwriting anything currently in the queue.
void filter_fillQueue(queue_t* q, double fillValue)
{
    for(uint32_t i=0; i< queue_size(q); i++)//fill the queue
    {
        queue_overwritePush(q, fillValue);//intialize the queue with zeros
    }
}

// Invokes the FIR-filter. Input is contents of xQueue.
// Output is returned and is also pushed on to yQueue.
double filter_firFilter()
{
    double FIRoutput = 0.0;//initialize fir output to zero
    for(uint32_t i=0; i<FILTER_XQUEUE_SIZE; i++)//go through each of hte 81 queueu elements
    {
        FIRoutput = FIRoutput + (queue_readElementAt(&xQueue, FILTER_XQUEUE_SIZE-1-i)*fir_coeffs[i]);//summation of the multiplication
    }
    queue_overwritePush(&yQueue, FIRoutput);
    return FIRoutput;//reuturn the value of the FIR output
}

// Use this to invoke a single iir filter. Input comes from yQueue.
// Output is returned and is also pushed onto zQueue[filterNumber].
double filter_iirFilter(uint16_t filterNumber)
{
    double bSummation = 0.0;//variable for tracking sum of b coeffs
    double aSummation = 0.0;//variable for tracking sum of a coeffs
    double IIRoutput = 0.0;//variable for tracking difference of b and a

    for(uint32_t i=0; i<FILTER_YQUEUE_SIZE; i++)//iterate through size of Y
    {
        bSummation = bSummation + (queue_readElementAt(&yQueue, FILTER_YQUEUE_SIZE-1-i)*iir_b_coeffs[filterNumber][i]);//summation of the multiplication y*b
    }
    for(uint32_t i=0; i<FILTER_ZQUEUE_SIZE; i++)//iterate through size of Z
    {
        aSummation = aSummation + (queue_readElementAt(&(zQueueArray[filterNumber]), FILTER_ZQUEUE_SIZE-1-i)*iir_a_coeffs[filterNumber][i]);//summation of the multiplication z*a
    }
    IIRoutput = bSummation - aSummation;//store the difference of b and a coeffs
    queue_overwritePush(&(zQueueArray[filterNumber]), IIRoutput);//add the output to one of the z queues

    queue_overwritePush(&(outputQueueArray[filterNumber]), IIRoutput);//add the output to one of the z queues
    //filter_computePower(filterNumber, false, false);
    return IIRoutput;//return the IIr output
}



// Use this to compute the power for values contained in a queue.
// If force == true, then recompute everything from scratch.
double filter_computePower(uint16_t filterNumber, bool forceComputeFromScratch, bool debugPrint)
{
    double power = 0.0;

    if(forceComputeFromScratch==false)//easy computation method
    {
        double newest_value = queue_readElementAt(&(outputQueueArray[filterNumber]), (FILTER_OUTPUTQUEUE_SIZE -1));//last element to have been added to the queue

        power = currentPowerValue[filterNumber] - (oldestValue[filterNumber]*oldestValue[filterNumber]) + (newest_value*newest_value);//calculate the current power

        //oldestValue[filterNumber] = queue_readElementAt(&(outputQueueArray[filterNumber]), 0);//save the oldest value before you overwrite push next time
    }

    else if(forceComputeFromScratch == true)//compute the power using each element
    {
        for(uint32_t i = 0; i < (FILTER_OUTPUTQUEUE_SIZE ); i++)
        {
            power = power + queue_readElementAt(&(outputQueueArray[filterNumber]), i)*queue_readElementAt(&(outputQueueArray[filterNumber]), i);
        }
    }
    oldestValue[filterNumber] = queue_readElementAt(&(outputQueueArray[filterNumber]), 0);//save the oldest value before you overwrite push next time
    if(power < 0.0)
    {
        power = 0.0;
    }
    currentPowerValue[filterNumber] = power;//put the power value into the array that holds current power
    return power;//return the variable in this case cuz you computed from scratch
}



// Returns the last-computed output power value for the IIR filter [filterNumber].
double filter_getCurrentPowerValue(uint16_t filterNumber)
{
    return currentPowerValue[filterNumber];//returns the current power value of all ten filters in the size ten array
}

// Get a copy of the current power values.
void filter_getCurrentPowerValues(double* powerValues)
{
    for(uint16_t i =0; i<FILTER_CURRENT_POWER_SIZE;i++)
    {
        powerValues[i] = currentPowerValue[i];
    }
}

// Uses the last computed-power values, scales them to the provided lowerBound and upperBound args, returns the index of element containing the max value.
// The caller provides the normalizedArray that will contain the normalized values. indexOfMaxValue indicates the channel with max. power.
void filter_getNormalizedPowerValues(double normalizedArray[], uint16_t* indexOfMaxValue)
{
    double maxpower = 0;//create maxpower variable
    for(uint16_t i = 0; i < FILTER_CURRENT_POWER_SIZE; i++)//go through each of the 10 elements in the array
    {
        if(currentPowerValue[i] > maxpower)
        {
            maxpower = currentPowerValue[i];//remember the highest power value between the 10 filters
            *indexOfMaxValue = i;//remember which filter the highest power value came from
        }
        else
        {
            maxpower = maxpower;//otherwise just keep the same value
        }
    }

    for(uint16_t i = 0; i < FILTER_CURRENT_POWER_SIZE; i++)//go through each ofhte ten elements in the power array
    {
        normalizedArray[i] = currentPowerValue[i] / maxpower;//normalize each of the elements and save these values in the normalized array
    }
}

/*********************************************************************************************************
********************************** Verification-assisting functions. *************************************
********* Test functions access the internal data structures of the filter.c via these functions. ********
*********************** These functions are not used by the main filter functions. ***********************
**********************************************************************************************************/

// Returns the array of FIR coefficients.
const double* filter_getFirCoefficientArray()
{
    return fir_coeffs;//returns the entire array of fir coeffs
}

// Returns the number of FIR coefficients.
uint32_t filter_getFirCoefficientCount()
{
    return FILTER_FIR_COEFF_COUNT;//returns the size of fir
}

// Returns the array of a-coefficients for a particular filter number.
const double* filter_getIirACoefficientArray(uint16_t filterNumber)
{
    return iir_a_coeffs[filterNumber];//returns the entire array or ten elements in filternumber array
}

// Returns the number of a-coefficients.
uint32_t filter_getIirACoefficientCount()
{
  return FILTER_IIR_A_COEFF_COUNT;//returns the size of each array in the A matrix
}

// Returns the array of b-coefficients for a particular filter number.
const double* filter_getIirBCoefficientArray(uint16_t filterNumber)
{
       return iir_b_coeffs[filterNumber];//returns the entire array or eleven elements in filternumber array
}

// Returns the number of b-coefficients.
uint32_t filter_getIirBCoefficientCount()
{
    return FILTER_IIR_B_COEFF_COUNT;//returns the size of each array in the B matrix
}

// Returns the size of the yQueue.
uint32_t filter_getYQueueSize()
{
    return FILTER_YQUEUE_SIZE;//returns size of Y
}

// Returns the decimation value.
uint16_t filter_getDecimationValue()
{
    return FILTER_FIR_DECIMATION_FACTOR;//returns decimation factor
}

// Returns the address of xQueue.
queue_t* filter_getXQueue()
{
    return &xQueue; //address of X
}

// Returns the address of yQueue.
queue_t* filter_getYQueue()
{
    return &yQueue;//address of Y
}

// Returns the address of zQueue for a specific filter number.
queue_t* filter_getZQueue(uint16_t filterNumber)
{
    return &zQueueArray[filterNumber]; //address of Z
}

// Returns the address of the IIR output-queue for a specific filter-number.
queue_t* filter_getIirOutputQueue(uint16_t filterNumber)
{
    return &outputQueueArray[filterNumber];//address of output
}

//runs a test to see if correct funcitonality of compute power
void filter_runTest()
{
    while(buttons_read() != FILTER_BTN3_ON)//stay in the test until you press button 3
    {
        if(buttons_read() == FILTER_BTN1_ON)//compute the power if you press button 1
        {
            for(uint32_t i=0; i<FILTER_OUTPUTQUEUE_ARRAY_SIZE; i++)//go through the each of the ten queues in output
            {
                queue_init(&(outputQueueArray[i]), FILTER_OUTPUTQUEUE_SIZE);//initialize outputQueueArray to be of size 4

                for(uint32_t j=0; j< FILTER_OUTPUTQUEUE_SIZE; j++)//go through each sample point in the queue
                {
                    queue_overwritePush(&(outputQueueArray[i]), i);//initialize the queue with 0 - 10
                }
            }

            utils_msDelay(FILTER_DELAY);

                for(uint16_t k=0; k<FILTER_OUTPUTQUEUE_ARRAY_SIZE; k++)//go through the each of the ten queues in output
                {
                    filter_computePower(k, true, false);//compute power from scratch
                }
                for(uint16_t w=0; w<FILTER_OUTPUTQUEUE_ARRAY_SIZE; w++)//go through the each of the ten queues in output
                {
                    printf("power[%d] : %lf\n\r", w, filter_getCurrentPowerValue(w));//print the power value
                }


                printf("output with largest power : %d\n\r", indexOfMaxValue);//print which filter had the max value


                for(uint16_t j=0; j< FILTER_OUTPUTQUEUE_SIZE+7; j++)//go through each sample point in the queue
                {
                    queue_overwritePush(&(outputQueueArray[1]), FILTER_QUEUE_NEW_VALUE_TWO);//initialize the queue with 0 - 10
                    filter_computePower(1, false, true);//compute power not from scratch
                }
                printf("power : %lf\n\r", filter_getCurrentPowerValue(1));//print the new power value of the filter



            for(uint32_t i=0; i<FILTER_OUTPUTQUEUE_ARRAY_SIZE; i++)//go through the each of the ten queues in output
            {
                queue_init(&(outputQueueArray[i]), FILTER_OUTPUTQUEUE_SIZE);//initialize outputQueueArray to be of size 4

                for(uint32_t j=0; j< FILTER_OUTPUTQUEUE_SIZE; j++)//go through each sample point in the queue
                {
                    queue_overwritePush(&(outputQueueArray[i]), i);//initialize the queue with 0 - 10
                }
            }

            utils_msDelay(FILTER_DELAY);

                for(uint16_t k=0; k<FILTER_OUTPUTQUEUE_ARRAY_SIZE; k++)//go through the each of the ten queues in output
                {
                    filter_computePower(k, true, false);//compute power from scratch
                }
                for(uint16_t w=0; w<FILTER_OUTPUTQUEUE_ARRAY_SIZE; w++)//go through the each of the ten queues in output
                {
                    printf("power[%d] : %lf\n\r", w, filter_getCurrentPowerValue(w));//print the power value
                }


                printf("output with largest power : %d\n\r", indexOfMaxValue);//print which filter had the max value


                for(uint16_t j=0; j< FILTER_OUTPUTQUEUE_SIZE; j++)//go through each sample point in the queue
                {
                    queue_overwritePush(&(outputQueueArray[1]), FILTER_QUEUE_NEW_VALUE_TWO);//initialize the queue with 0 - 10
                    filter_computePower(1, false, true);//compute power not from scratch
                }
                printf("power : %lf\n\r", filter_getCurrentPowerValue(1));//print the new power value of the filter

                utils_msDelay(2000);

        }
    }
}



void filter_PowerValues(double a,double b,double c,double d,double e,double f,double g,double h,double i,double j)
{
    currentPowerValue[0] = a;
    currentPowerValue[1] = b;
    currentPowerValue[2] = c;
    currentPowerValue[3] = d;
    currentPowerValue[4] = e;
    currentPowerValue[5] = f;
    currentPowerValue[6] = g;
    currentPowerValue[7] = h;
    currentPowerValue[8] = i;
    currentPowerValue[9] = j;

}







