//Disk Scheduler
//Team Members:
//Samira Sadat
//Brad Rasmussen
//Jacob Cox

//includes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//Define
#define REQUESTQUEUESIZE 1000
#define RANGE 200
#define HEADSTART 0 //0-(RANGE-1)

//Global Structures and Variables
//Disk Head
int head = HEADSTART;
//Randomized Queue - Array? Linked List? Global or passed into function?
int randomRequestQueue[REQUESTQUEUESIZE];
//Sorted Queue
//int sortedRequestQueue[REQUESTQUEUESIZE];


//Functions
void RandomRequestGenerator();
void fifo();
void lifo();
void nStepScan();
void fScan();
void printSummary();

//main
int main(int argc, char *argv[])
{
    srand(time(0));
    RandomRequestGenerator();
    fifo();
    lifo();
    return 0;
}
//initialize variables
//call policy functions

//Random Request Generator - Samira
// -Fill the main queue with randomly generated numbers
void RandomRequestGenerator()
{
    //fill in each request value
    for (int i = 0; i < REQUESTQUEUESIZE; i++)
    {
        randomRequestQueue[i] = (rand() % RANGE);
    }
}

//Manual Initialization of Queue - Brad

//FIFO - Samira
void fifo()
{
    //List of "Number of tracks traversed". Same size as above queues
    int numTracksTraversedQueue[REQUESTQUEUESIZE];
    int fifoRequestQueue[REQUESTQUEUESIZE];
    int averageSeekTime = 0;
    int cumulativeSum = 0;

    //Move head and calculate traverse time
    for (int i = 0;i<REQUESTQUEUESIZE;i++)
    {

        fifoRequestQueue[i] = randomRequestQueue[i];
        numTracksTraversedQueue[i] = abs(head - fifoRequestQueue[i]);
        cumulativeSum = cumulativeSum + numTracksTraversedQueue[i];

        head = fifoRequestQueue[i];
    }
    //calulate average
    averageSeekTime = cumulativeSum / REQUESTQUEUESIZE;

    //TODO: Call a print function to print this result column
    //TEMP PRINT
    /*printf("Request | Num Tracks Traversed\n");
    for(int j = 0; j<REQUESTQUEUESIZE; j++)
    {
        printf("%03d",fifoRequestQueue[j]);
        printf(" | ");
        printf("%d\n",numTracksTraversedQueue[j]);
    }*/
    
}

//LIFO - Samira
void lifo()
{
    //List of "Number of tracks traversed". Same size as above queues
    int numTracksTraversedQueue[REQUESTQUEUESIZE];
    int lifoRequestQueue[REQUESTQUEUESIZE];
    int averageSeekTime = 0;
    int cumulativeSum = 0;

    //Reverse queue order, 
    for (int i = REQUESTQUEUESIZE-1;i>=0;i--)
    {
        int requestIndex = (REQUESTQUEUESIZE - 1 )-i;
        lifoRequestQueue[i] = randomRequestQueue[requestIndex];
    }
    //Move head , and calculate traverse time
    for (int i = 0;i<REQUESTQUEUESIZE;i++)
    {
        numTracksTraversedQueue[i] = abs(head - lifoRequestQueue[i]);
        cumulativeSum = cumulativeSum + numTracksTraversedQueue[i];

        head = lifoRequestQueue[i];
    }
    //calulate average
    averageSeekTime = cumulativeSum / REQUESTQUEUESIZE;

    //TODO: Call a print function to print this result column
    //TEMP PRINT
    /*printf("RequestQueue | LifoQueue | Num Tracks Traversed\n");
    for(int j = 0; j<REQUESTQUEUESIZE; j++)
    {
        printf("%03d",randomRequestQueue[j]);
        printf(" | ");
        printf("%03d",lifoRequestQueue[j]);
        printf(" | ");
        printf("%d\n",numTracksTraversedQueue[j]);
    }*/
    
}

//Shortest Service Time First - Samira

//SCAN - Brad

//C-SCAN - Brad

//N-STEP-Scan - Jake
void nStepScan()
{
}

//FSCAN - Jake
void fScan()
{
}

//Print Summary Table -Jake
void printSummary()
{
}
