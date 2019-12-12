/* 
 * Disk Scheduler
 * 
 * Samira Sadat
 * Brad Rasmussen
 * Jacob Cox
 */

/*
 * Includes
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/*
 * Define
 */

//sets the total amount of requests to be made in the simulation
#define REQUESTQUEUESIZE 10
//guarantees printing every detail when made value set to 1
#define PRINTVERBOSE 0
#define RANGE 200
#define HEADSTART 0 //0-(RANGE-1)

/*
 * Structures and Variables
 */

//Disk Head
int head = HEADSTART;
int averageSeekTime = 0;
int randomRequestQueue[REQUESTQUEUESIZE];

//Functions
void RandomRequestGenerator();
void fifo();
void lifo();
void shortestServiceTimeFirst();
void bubbleSort(int arr[], int n);
void nStepScan(int N);
void fScan();
void printSummary();

//main
int main(int argc, char *argv[])
{
    srand(time(0));
    RandomRequestGenerator();
    //fifo();
    //lifo();
    shortestServiceTimeFirst();
    //nStepScan(7);
    //fScan(7);
    return 0;
}

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
    for (int i = 0; i < REQUESTQUEUESIZE; i++)
    {

        fifoRequestQueue[i] = randomRequestQueue[i];
        numTracksTraversedQueue[i] = abs(head - fifoRequestQueue[i]);
        cumulativeSum = cumulativeSum + numTracksTraversedQueue[i];

        head = fifoRequestQueue[i];
    }
    //calulate average
    averageSeekTime = cumulativeSum / REQUESTQUEUESIZE;

    printSummary("FIFO", numTracksTraversedQueue, fifoRequestQueue, REQUESTQUEUESIZE, REQUESTQUEUESIZE, averageSeekTime);
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
    for (int i = REQUESTQUEUESIZE - 1; i >= 0; i--)
    {
        int requestIndex = (REQUESTQUEUESIZE - 1) - i;
        lifoRequestQueue[i] = randomRequestQueue[requestIndex];
    }
    //Move head , and calculate traverse time
    for (int i = 0; i < REQUESTQUEUESIZE; i++)
    {
        numTracksTraversedQueue[i] = abs(head - lifoRequestQueue[i]);
        cumulativeSum = cumulativeSum + numTracksTraversedQueue[i];

        head = lifoRequestQueue[i];
    }
    //calulate average
    averageSeekTime = cumulativeSum / REQUESTQUEUESIZE;

    printSummary("LIFO", numTracksTraversedQueue, lifoRequestQueue, REQUESTQUEUESIZE, REQUESTQUEUESIZE, averageSeekTime);
}

//Shortest Service Time First - Samira
void shortestServiceTimeFirst()
{
    int accessCount = 0;
    int accessedIndex;
    int requestValue;
    int lastSeekTime = RANGE;
    int averageSeektime = 0;
    int cumulativeSum = 0;

    //create an "accessed" array to keep track of what request has been processed
    int requestAccessed[REQUESTQUEUESIZE] = {0};
    int numTracksTraversedQueue[REQUESTQUEUESIZE];
    int shortestServiceQueue[REQUESTQUEUESIZE];

    //while accessed count is less than request array size
    while(accessCount < (REQUESTQUEUESIZE))
    {
        lastSeekTime = RANGE;
        //For loop - Scan the request array
        for (int i = 0; i < REQUESTQUEUESIZE; i++)
        {
            int traverseTime = abs(head - randomRequestQueue[i]);
            
            //Compare the track head to the request value //if | track head - request value | < last difference
            if ((traverseTime <=  lastSeekTime) && (requestAccessed[i] == 0))
            {
                //Store the request value, seek time, and array index 
                requestValue = randomRequestQueue[i];
                lastSeekTime = traverseTime;
                accessedIndex = i;
            }
        }
        //Mark the arrayIndex of accessed array as TRUE
        requestAccessed[accessedIndex] = 1;
        //Put the seek time into the numTracks traversed array
        numTracksTraversedQueue[accessCount] = lastSeekTime;
        //Put the request value into the shortest service array
        shortestServiceQueue[accessCount] = requestValue;
        //increment access count
        accessCount++;
        //reset head
        head = randomRequestQueue[accessedIndex];
    }

    //calculate average seek time
    for (int i = 0; i < REQUESTQUEUESIZE; i++)
    {
        cumulativeSum = cumulativeSum + numTracksTraversedQueue[i];
    }
    averageSeekTime = cumulativeSum / REQUESTQUEUESIZE;

    printSummary("Shortest Serivce Time First", numTracksTraversedQueue,shortestServiceQueue,REQUESTQUEUESIZE,REQUESTQUEUESIZE, averageSeekTime);

}

//SCAN - Brad

//C-SCAN - Brad

/* 
 * N-Step-Scan
 * 
 * @param 
 * @param 
 * 
 * @return 
 */
void nStepScan(int N)
{
    // Declare Variables
    int s = sizeof(randomRequestQueue) / sizeof(int);
    int masterLocation = 0;
    int localLocation = 0;
    int queue[N];
    int fillQueue = 1;
    int localHead = HEADSTART;

    // Output variables
    int outputTracksAccessed[REQUESTQUEUESIZE];
    int outputLocationAccessed = 0;
    int outputTracksTraversed[REQUESTQUEUESIZE];
    int outputLocationTraversed = 0;

    // Keep simulating until the entire master queue has been operated on
    while (masterLocation < s)
    {
        // If the queue has been emptied fillit again
        if (fillQueue == 1)
        {
            for (int i = 0; i < N; i++)
                queue[i] = -1;

            localLocation = 0;
            for (int i = 0; i < N; i++)
            {
                if ((masterLocation) < s)
                {
                    queue[i] = randomRequestQueue[masterLocation];
                    masterLocation++;
                }
            }
            fillQueue = 0;
        }

        // After the queue has been filled, sort it from low to high
        bubbleSort(queue, N);

        // After sort put the elements in order that they will be processed.
        int shiftAmount = 0;
        for (int i = 0; i < N; i++)
        {
            if (queue[i] < localHead)
            {
                shiftAmount++;
            }
        }

        if (shiftAmount > 0)
        {
            for (int i = 0; i < shiftAmount; i++)
            {
                int temp = queue[0];
                for (int i = 0; i < N; i++)
                {
                    queue[i] = queue[i + 1];
                }
                queue[N - 1] = temp;
            }
        }

        //printf("%d \n", localHead);

        for (int i = 0; i < N; i++)
        {
            int trackAccessed = queue[i];
            if (trackAccessed != -1)
            {
                int tracksTraversed = abs(trackAccessed - localHead);
                localHead = trackAccessed;

                outputTracksAccessed[outputLocationAccessed] = trackAccessed;
                outputTracksTraversed[outputLocationTraversed] = tracksTraversed;

                outputLocationAccessed++;
                outputLocationTraversed++;
            }
        }

        fillQueue = 1;
    }

    int total = 0;

    for (int i = 0; i < REQUESTQUEUESIZE; i++)
    {
        total = total + outputTracksTraversed[i];
    }

    total = total / REQUESTQUEUESIZE;

    printSummary("N-Step-Scan", outputTracksAccessed, outputTracksTraversed, REQUESTQUEUESIZE, REQUESTQUEUESIZE, total);
}

/* 
 * F Scan
 * 
 * @param int N : The number of elements in the queues
 * 
 * @return void
 */
void fScan(int N)
{
    int s = sizeof(randomRequestQueue) / sizeof(int);
    int workingQueueOne[N];
    int workingQueueTwo[N];
    int masterLocation = 0;
    int localHead = HEADSTART;

    int outputTracksAccessed[REQUESTQUEUESIZE];
    int outputLocationAccessed = 0;
    int outputTracksTraversed[REQUESTQUEUESIZE];
    int outputLocationTraversed = 0;

    // Start the first queue off with the first set of requests.
    for (int i = 0; i < N; i++)
    {
        if (i < s)
        {
            workingQueueTwo[i] = randomRequestQueue[i];
            masterLocation++;
        }
    }

    while (masterLocation < s)
    {
        //Simulate loading the other queue with requests
        for (int i = 0; i < N; i++)
        {
            if (masterLocation + N < s)
            {
                workingQueueOne[i] = randomRequestQueue[masterLocation + N];
            }
        }

        masterLocation = masterLocation + N;

        //Operate with the current queues requests
        bubbleSort(workingQueueTwo, N);

        // After sort put the elements in order that they will be processed.
        int shiftAmount = 0;
        for (int i = 0; i < N; i++)
        {
            if (workingQueueTwo[i] < localHead)
            {
                shiftAmount++;
            }
        }

        if (shiftAmount > 0)
        {
            for (int i = 0; i < shiftAmount; i++)
            {
                int temp = workingQueueTwo[0];
                for (int i = 0; i < N; i++)
                {
                    workingQueueTwo[i] = workingQueueTwo[i + 1];
                }
                workingQueueTwo[N - 1] = temp;
            }
        }

        for (int i = 0; i < N; i++)
        {
            int trackAccessed = workingQueueTwo[i];
            if (trackAccessed != -1)
            {
                int tracksTraversed = abs(trackAccessed - localHead);
                localHead = trackAccessed;

                outputTracksAccessed[outputLocationAccessed] = trackAccessed;
                outputTracksTraversed[outputLocationTraversed] = tracksTraversed;

                outputLocationAccessed++;
                outputLocationTraversed++;
            }
        }

        //switch queues (technically)
        for (int i = 0; i < N; i++)
        {
                workingQueueTwo[i] = workingQueueOne[i];
        }
    }

    int total = 0;

    for (int i = 0; i < REQUESTQUEUESIZE; i++)
    {
        total = total + outputTracksTraversed[i];
    }

    total = total / REQUESTQUEUESIZE;

    printSummary("F Scan", outputTracksAccessed, outputTracksTraversed, REQUESTQUEUESIZE, REQUESTQUEUESIZE, total);
}

/* 
 * Print Summary, used to print the data and information after each simulation test
 * 
 * @param char title[] : A string of text that holds the title of the simulation
 * @param int nextTrack[] : The array of integers that holds all tracks in order of traversal
 * @param int TraversedTrack[] : The array of integers that holds the number of tracks traversed in same order as nextTrack
 * @param int lengthNextTrack : The length of nextTrack
 * @param int lengthTraversedTrack : The length of TraversedTrack
 * @param int avgSeekLength : The average seek length calculated for output
 * 
 * @return void
 */
void printSummary(char title[15], int nextTrack[], int traversedTrack[], int lengthNextTrack, int lengthTraversedTrack, int avgSeekLength)
{
    //If the lengths of both arrays are not the same, do not do anything and print an error
    if (lengthNextTrack == lengthTraversedTrack)
    {
        // Print the heading of the table
        printf("\nTest: %-18s \nStarting at HEAD: %-18d\n", title, HEADSTART);
        printf("%-18s%-18s\n", "Next Track", "Traversed Tracks");
        printf("__________        ________________\n");

        // If the length is too long, adjust what is printed accordingly.
        if (!PRINTVERBOSE && lengthNextTrack > 50)
        {
            // Print the first portion
            for (int i = 0; i < 7; i++)
            {
                printf("%-18d%-18d\n", nextTrack[i], traversedTrack[i]);
            }

            // Print a divider to show there is data in between
            for (int i = 0; i < 3; i++)
            {
                printf("%-18s%-18s\n", ".", ".");
            }

            // Print some fo the end data
            for (int i = (lengthNextTrack - 7); i < lengthNextTrack; i++)
            {
                printf("%-18d%-18d\n", nextTrack[i], traversedTrack[i]);
            }
        }
        else
        {
            // Print all of the values
            for (int i = 0; i < lengthNextTrack; i++)
            {
                printf("%-18d%-18d\n", nextTrack[i], traversedTrack[i]);
            }
        }

        printf("__________________________________\n");
        // Print the average seek length
        printf("%-18s: %-18d\n\n", "Average Seek Length", avgSeekLength);
    }
    else
    {
        printf("\nSomething went wrong, both input and output arrays are not the same size?\n");
    }
}

/* 
 * Bubble Sort Method
 * 
 * @param int arr[] : The array of integers to be sorted least to greatest
 * @param int n : The number of elements inside the array to be sorted
 * 
 * @return void
 */
void bubbleSort(int arr[], int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)

        // Last i elements are already in place
        for (j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
}
