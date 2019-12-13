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
#define REQUESTQUEUESIZE 100000
//guarantees printing every detail when made value set to 1
#define PRINTVERBOSE 0
#define RANGE 200
#define HEADSTART 100 //0-(RANGE-1)

/*
 * Structures and Variables
 */

//Disk Head
int head = HEADSTART;
int averageSeekTime = 0;
int randomRequestQueue[REQUESTQUEUESIZE];

//Functions
void RandomRequestGenerator();
void WeightedRandomRequestGenerator();
void fifo();
void lifo();
void shortestServiceTimeFirst();
void bubbleSort(int arr[], int n);
void nStepScan(int N);
void fScan();
void printSummary();
void countTheCrap(int arg[]);

//main
int main(int argc, char *argv[])
{
    srand(time(0));
    RandomRequestGenerator();
    WeightedRandomRequestGenerator();
    fifo();
    lifo();
    shortestServiceTimeFirst();
    nStepScan(7);
    fScan(7);
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

void countTheCrap(int arg[])
{
    int bob[REQUESTQUEUESIZE];

    for(int i = 0; i < RANGE; i++)
    {
        bob[i] = 0;
    }

    for(int i = 0; i < REQUESTQUEUESIZE; i++)
    {
        bob[arg[i]]++;
    }

    for(int i = 0; i < RANGE; i++)
    {
        printf("%d \n", bob[i]);
    }

    
}

//Manual Initialization of Queue - All
void WeightedRandomRequestGenerator()
{
    float probabilityArray[RANGE];
    int localHead = HEADSTART;
    int diskOptions[RANGE];
    //fill in each request value
    for (int i = 0; i < RANGE; i++)
    {
        diskOptions[i] = i;
    }

    for(int k = 0; k < REQUESTQUEUESIZE; k++)
    {
         //fill in probabilities
        for (int i = 0; i < RANGE; i++)
        {   
            int furthest = 0;
            int closest = localHead;
            if (localHead <= (RANGE/2))
            {
                furthest = RANGE - 1;
            }
            else
            {
                furthest = 0;
            }
            
            float increment = 0.0;
            if(localHead == diskOptions[i])
            {
                probabilityArray[i] = 10.0;
            }
            else
            {
                increment = 9.0f / (fabsf((float)closest - (float)furthest));
                float distance = abs(localHead - i);
                probabilityArray[i] = 10 - (increment*distance);
            }

            float sum = 0.0;

            for(int j = 0; j < RANGE; j++)
            {
                sum += probabilityArray[j];
            }

            float randValue = (rand() % (int)sum);
            int randTrack = 0;

            for(int j = 0; j < RANGE; j++)
            {
                if((randValue < probabilityArray[j] ) && (j < localHead))
                {
                    if((localHead + j) < RANGE)
                    {
                        int r = rand() % 1;
                        if(r == 0)
                        {
                            randTrack = j;
                        }
                        else
                        {
                            randTrack = localHead + j;
                        }
                    }
                    else 
                    {
                        randTrack = j;
                    }
                    break;
                }
                else if(randValue < probabilityArray[j])
                {
                    randTrack = j;
                    break;
                }
                else
                {
                    randValue -= probabilityArray[j];
                }

                
                
            }
            randomRequestQueue[k] = randTrack;
            localHead = randTrack;
        }
    }
}

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
void scan()
{
    int i,j,sum=0,n;
    int d[20];
    int disk;   //loc of head
    int temp,max;     
    int dloc;   //loc of disk in array

    clrscr();
    n = RANGE;
    disk = HEADSTART;

    printf("enter elements of disk queue\n");

    for(i=0;i<n;i++)
    {
        scanf("%d",&d[i]);
    }
    d[n]=disk;
    n=n+1;
    for(i=0;i<n;i++)    // sorting disk locations
    {
        for(j=i;j<n;j++)
        {
            if(d[i]>d[j])
            {
                temp=d[i];
                d[i]=d[j];
                d[j]=temp;
            }
        }
    }
    
    max=d[n];
    for(i=0;i<n;i++)   // to find loc of disc in array
    {
        if(disk==d[i]) 
        { 
            dloc=i; 
            break;  
        }
    }

    sum=disk+max;
    getch();

}

//C-SCAN - Brad
void cscan(int N)
{
    int queue[20],n,head,i,j,k,seek=0,max,diff,temp,queue1[20],queue2[20],
    temp1=0,temp2=0;
    float avg;

    max = RANGE;
    head = HEADSTART;
    n = N;

    printf("Enter the queue of disk positions to be read\n");
    for(i=1;i<=n;i++)
    {
        scanf("%d",&temp);

        if(temp>=head)
        {
            queue1[temp1]=temp;

            temp1++;
        }
        else
        {
            queue2[temp2]=temp;

            temp2++;
        }
    }

    for(i=0;i<temp1-1;i++)
    {
        for(j=i+1;j<temp1;j++)
        {
            if(queue1[i]>queue1[j])
            {
                temp=queue1[i];
                queue1[i]=queue1[j];
                queue1[j]=temp;
            }
        }
    }

    for(i=0;i<temp2-1;i++)
    {
        for(j=i+1;j<temp2;j++)
        {
            if(queue2[i]>queue2[j])
            {
                temp=queue2[i];
                queue2[i]=queue2[j];
                queue2[j]=temp;
            }
        }
    }

    for(i=1,j=0;j<temp1;i++,j++)
    {
        queue[i]=queue1[j];
        queue[i]=max;
        queue[i+1]=0;
    }

    for(i=temp1+3,j=0;j<temp2;i++,j++)
    {
        queue[i]=queue2[j];
        queue[0]=head;
    }

    for(j=0;j<=n+1;j++)
    {
        diff=abs(queue[j+1]-queue[j]);
        seek+=diff;
        printf("Disk head moves from %d to %d with seek %d\n",queue[j],queue[j+1],diff);
    }

    printf("Total seek time is %d\n",seek);
    avg=seek/(float)n;
    printf("Average seek time is %f\n",avg);
}

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
