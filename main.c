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

/*
 * Define
 */

//sets the total amount of requests to be made in the simulation
#define REQUESTQUEUESIZE 1000
//guarantees printing every detail when made value set to 1
#define PRINTVERBOSE 0

/*
 * Structures and Variables
 */

//Disk Head
int head = 100;
int averageSeekTime = 0;
//Randomized Queue - Array? Linked List? Global or passed into function?
int randomRequestQueue[1000];
//Sorted Queue
int sortedRequestQueue[1000];
//List of "Number of tracks traversed". Same size as above queues
int numTracksTraversedQueue[1000];

//Functions
void RandomRequestGenerator();
void bubbleSort(int arr[], int n);
void nStepScan(int N);
void fScan();
void printSummary();

//main
int main(int argc, char *argv[])
{
    RandomRequestGenerator();
    nStepScan(7);
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
        randomRequestQueue[i] = (rand() % 200);
    }
}

//Manual Initialization of Queue - Brad

//FIFO - Samira

//LIFO - Samira

//Shortest Service Time First - Samira

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
    int localHead = head;

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
 * @param 
 * @param 
 * 
 * @return 
 */
void fScan()
{
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
        printf("\nTest: %-18s \nStarting at HEAD: %-18d\n", title, head);
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
