/*
 * queue.c
 *
 *  Created on: Jan 6, 2016
 *      Author: spence13
 */



#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

// Standard queue implementation that leaves one spot empty so easier to check for full/empty.
void queue_init(queue_t* q, queue_size_t size)
{
  q->indexIn = 0;//initialize index in to 0
  q->indexOut = 0;//initialize index out to 0
  q->elementCount = 0;//initialize element count to 0
  q->size = size + 1; // Add one additional location for the empty location.
  q->data = (queue_data_t *) malloc(q->size * sizeof(queue_data_t));
}

// Returns the size of the queue..
queue_size_t queue_size(queue_t* q)
{
    return q->size - 1;//return size of queue
}

// Returns true if the queue is full.
bool queue_full(queue_t* q)
{
    if(q->indexIn == q->size - 1)//check to see if index is at top of queue
    {
        if(q->indexOut == 0)//if index out is at zero position
        {
            return true;//it is full
        }
        else
        {
            return false;//the queue is not full
        }
    }
    else//if index in is at any other position other than top
    {
        if(q->indexIn + 1 == q->indexOut)//if index one is on position behind index out
        {
            return true;//queue is full
        }
        else
        {
            return false;//queue is not full
        }
    }
}

// Returns true if the queue is empty.
bool queue_empty(queue_t* q)
{
    if(q->indexOut == q->indexIn)//if they are pointed to the same spot
    {
        return true;//queue is empty
    }
    else
    {
        return false;//queue is not empty
    }
}

// Pushes a new element into the queue. Reports an error if the queue is full.
void queue_push(queue_t* q, queue_data_t value)
{
    if(queue_full(q))//check if the queue is full
    {
      printf("q is full!!!!\n\r");//print that the queue is full
    }
    else //if its not full you can push
    {
        q->data[q->indexIn] = value;//make data at position in equal to value
        q->indexIn = q->indexIn + 1;
        if(q->indexIn > q->size - 1)//check to see if index in is at top
        {
            q->indexIn = 0;//loop around to bottom
        }
        q->elementCount = q->elementCount + 1;//increment element count by one
    }
}

// Removes the oldest element in the queue.
queue_data_t queue_pop(queue_t* q)
{
    if(queue_empty(q))//only pop if the queue is not empty
    {
        printf("q is empty!!!!\n\r");//print that the queue is empty
        return 0;//it is null
    }
    else
    {
        queue_data_t popped_value = q->data[q->indexOut];//assign value at index to variable
        q->indexOut = q->indexOut + 1;//increment index out by one
        if(q->indexOut > q->size - 1)//check to see if index out is at top
        {
            q->indexOut = 0;//loop back around
        }
        q->elementCount = q->elementCount - 1;//decrement element count by one
        return popped_value;//return the value that you popped
    }
}

// If the queue is full, remove the oldest element and then push the new element.
void queue_overwritePush(queue_t* q, queue_data_t value)
{
    if(queue_full(q))//check if the queue is full
    {
        queue_pop(q);//pop the oldest
    }
    queue_push(q, value);//push the new value
}

// Provides random-access read capability to the queue.
// Low-valued indexes access older queue elements while higher-value indexes access newer elements
// (according to the order that they were added).
queue_data_t queue_readElementAt(queue_t* q, queue_index_t index)
{
    if(index >= q->size - 1)//if the index is beyond the array size
    {
        return 0;//it is null
    }
    index = index + q->indexOut;//make the zeroth index where index out is pointing to
    if(index > q->size - 1)//check to see if index is larger than array size
    {
        index = index - q->size;//keep index within bounds
    }
    return q->data[index];//return value of data at whatever index you give it
}

// Returns a count of the elements currently contained in the queue.
queue_size_t queue_elementCount(queue_t* q)
{
    return q->elementCount;//return the number of elements
}

// Just free the malloc'd storage.
void queue_garbageCollect(queue_t* q)
{
  free(q->data);//free the memory you allocated
}

// Prints the current contents of the queue. Handy for debugging.
// This must print out the contents of the queue in the order of oldest element first to newest element last.
void queue_print(queue_t* q)
{
    for(uint32_t i = 0; i<queue_elementCount(q); i++)//read through however many elements you have
    {
        printf("data[%d]: %f\n\r", i, queue_readElementAt(q, i));//print the value at the out index
    }
}


// Performs a comprehensive test of all queue functions. Returns -1 if the test fails, 0 otherwise.
int16_t queue_MyrunTest()
{
    queue_t MyQ;//create a variable of type queue_t

    queue_init(&MyQ, 4);//initialize MyQ to be of size 4

    printf("queue size : %d\n\r", queue_size(&MyQ));//print the queue size

    if(queue_empty(&MyQ))//check to see if queue is empty
    {
        printf("queue is empty\n\r");//print if it is empty
    }
    else
    {
        printf("queue is not empty\n\r");//print if not empty
    }

    queue_push(&MyQ, 45);//push a value of 45
    queue_push(&MyQ, 35);//push a value of 35
    queue_push(&MyQ, 67);//push a value of 67

    if(queue_empty(&MyQ))//check to see if queue is empty
    {
        printf("queue is empty\n\r");//print if it is empty
    }
    else
    {
        printf("queue is not empty\n\r");//print if not empty
    }

    if(queue_full(&MyQ))//check to see if queue is full
    {
        printf("queue is full\n\r");//print if the queue is full
    }
    else
    {
        printf("queue is not full\n\r");//print if queue is not full
    }

    queue_print(&MyQ);//print the contents of queue

    queue_pop(&MyQ);//pop the top value
    queue_pop(&MyQ);//pop the top value
    queue_pop(&MyQ);//pop the top value

    printf("element count : %d\n\r", queue_elementCount(&MyQ));//print element count
    queue_pop(&MyQ);//pop the top value
    printf("element count : %d\n\r", queue_elementCount(&MyQ));//print element count

    queue_overwritePush(&MyQ, 2);//push a value of 2
    queue_overwritePush(&MyQ, 5);//push a value of 5
    queue_overwritePush(&MyQ, 7);//push a value of 7
    queue_overwritePush(&MyQ, 9);//push a value of 9
    queue_overwritePush(&MyQ, 13);//push a value of 13

    queue_pop(&MyQ);//pop the top
    queue_pop(&MyQ);//pop the top
    queue_pop(&MyQ);//pop the top

    queue_overwritePush(&MyQ, 21);//push a value of 21
    queue_overwritePush(&MyQ, 18);//push a value of 18
    queue_overwritePush(&MyQ, 17);//push a value of 17
    queue_overwritePush(&MyQ, 74);//push a value of 74
    queue_overwritePush(&MyQ, 13);//push a value of 13


    for(int i = 0; i<5; i++)
    {
        queue_push(&MyQ, i);//push a value of 45
    }

    queue_print(&MyQ);//print the contents of the queue

    queue_overwritePush(&MyQ, 99);//push a value of 21
    queue_overwritePush(&MyQ, 100);//push a value of 21

    queue_print(&MyQ);//print the contents of the queue

    for(int i = 0; i<100; i++)
    {
      printf("element at index %d : %f\n\r", i, queue_readElementAt(&MyQ, i));//print value of queue at index 0
    }
}



#define SMALL_QUEUE_SIZE 10
#define SMALL_QUEUE_COUNT 10
static queue_t smallQueue[SMALL_QUEUE_COUNT];
static queue_t largeQueue;

// smallQueue[SMALL_QUEUE_COUNT-1] contains newest value, smallQueue[0] contains oldest value.
// Thus smallQueue[0](0) contains oldest value. smallQueue[SMALL_QUEUE_COUNT-1](SMALL_QUEUE_SIZE-1) contains newest value.
// Presumes all queue come initialized full of something (probably zeros).
static double popAndPushFromChainOfSmallQueues(double input) {
  // Grab the oldest value from the oldest small queue before it is "pushed" off.
  double willBePoppedValue = queue_readElementAt(&(smallQueue[0]), 0);
  // Sequentially pop from the next newest queue and push into next oldest queue.
  for (int i=0; i<SMALL_QUEUE_COUNT-1; i++) {
    queue_overwritePush(&(smallQueue[i]), queue_pop(&(smallQueue[i+1])));
  }
  queue_overwritePush(&(smallQueue[SMALL_QUEUE_COUNT-1]), input);
  return willBePoppedValue;
}

static bool compareChainOfSmallQueuesWithLargeQueue(uint16_t iterationCount) {
  bool success = true;
  static uint16_t oldIterationCount;
  static bool firstPass = true;
  // Start comparing the oldest element in the chain of small queues, and the large queue
  // and move towards the newest values.
  for (uint16_t smallQIdx=0; smallQIdx<SMALL_QUEUE_COUNT; smallQIdx++) {
    for (uint16_t smallQEltIdx=0; smallQEltIdx<SMALL_QUEUE_SIZE; smallQEltIdx++) {
      double smallQElt = queue_readElementAt(&(smallQueue[smallQIdx]), smallQEltIdx);
      double largeQElt = queue_readElementAt(&largeQueue, (smallQIdx*SMALL_QUEUE_SIZE) + smallQEltIdx);
      if (smallQElt != largeQElt) {
    if (firstPass || (iterationCount != oldIterationCount)) {
      printf("Iteration:%d\n\r", iterationCount);
      oldIterationCount = iterationCount;
      firstPass = false;
    }
    printf("largeQ(%d):%lf", (smallQIdx*SMALL_QUEUE_SIZE) + smallQEltIdx, largeQElt);
    printf(" != ");
    printf("smallQ[%d](%d): %lf\n\r", smallQIdx, smallQEltIdx, smallQElt);
        success = false;
      }
    }
  }
  return success;
}

#define TEST_ITERATION_COUNT 105
#define FILLER 5
int16_t queue_runTest() {
  int success = 0;  // Be optimistic.
  // Let's make this a real torture test by testing queues against themselves.
  // Test the queue against an array to make sure there is agreement between the two.
  double testData[SMALL_QUEUE_SIZE + FILLER];
  queue_t q;
  queue_init(&q, SMALL_QUEUE_SIZE);
  // Generate test values and place the values in both the array and the queue.
  for (int i=0; i<SMALL_QUEUE_SIZE + FILLER; i++) {
    double value = (double)rand()/(double)RAND_MAX;
    queue_overwritePush(&q, value);
    testData[i] = value;
  }
  // Everything is initialized, compare the contents of the queue against the array.
  for (int i=0; i<SMALL_QUEUE_SIZE; i++) {
    double qValue = queue_readElementAt(&q, i);
    if (qValue != testData[i+FILLER]) {
      printf("testData[%d]:%lf != queue_readElementAt(&q, %d):%lf\n\r", i, testData[i+FILLER], i+FILLER, qValue);
      success = -1;
    }
  }
  if (success) {
    printf("Test 1 failed. Array contents not equal to queue contents.\n\r");
  } else {
    printf("Test 1 passed. Array contents match queue contents.\n\r");
  }
  success = 0;  // Remain optimistic.
  // Test 2: test a chain of 5 queues against a single large queue that is the same size as the cumulative 5 queues.
  for (int i=0; i<SMALL_QUEUE_COUNT; i++)
    queue_init(&(smallQueue[i]), SMALL_QUEUE_SIZE);
  for (int i=0; i<SMALL_QUEUE_COUNT; i++) {
    for (int j=0; j<SMALL_QUEUE_SIZE; j++)
      queue_overwritePush(&(smallQueue[i]), 0.0);
  }
  queue_init(&largeQueue, SMALL_QUEUE_SIZE * SMALL_QUEUE_COUNT);
  for (int i=0; i<SMALL_QUEUE_SIZE*SMALL_QUEUE_COUNT; i++)
    queue_overwritePush(&largeQueue, 0.0);
  for (int i=0; i<TEST_ITERATION_COUNT; i++) {
    double newInput = (double)rand()/(double)RAND_MAX;
    popAndPushFromChainOfSmallQueues(newInput);
    queue_overwritePush(&largeQueue, newInput);
    if (!compareChainOfSmallQueuesWithLargeQueue(i)) {  // i is passed to print useful debugging messages.
      success = -1;
    }
  }

  if (!success)
    printf("Test 2 passed. Small chain of queues behaves identical to single large queue.\n\r");
  else
    printf("Test 2 failed. The content of the chained small queues does not match the contents of the large queue.\n\r");
  return success;
}



