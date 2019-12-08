//Disk Scheduler
//Team Members:
//Samira Sadat
//Brad Rasmussen
//Jacob Cox

//includes
#include <stdio.h>
#include <stdlib.h>
//Define
#define REQUESTQUEUESIZE 1000

//Structures and Variables
//Disk Head
int head = 0; 
int averageSeekTime = 0;
//Randomized Queue - Array? Linked List? Global or passed into function?
int randomRequestQueue[1000];
//Sorted Queue
int sortedRequestQueue[1000]; 
//List of "Number of tracks traversed". Same size as above queues
int numTracksTraversedQueue[1000];

//Functions
void RandomRequestGenerator();

//main
int main(){
    RandomRequestGenerator();
    return 0;
}
//initialize variables
//call policy functions

//Random Request Generator - Samira
// -Fill the main queue with randomly generated numbers
void RandomRequestGenerator()
{
    //fill in each request value
    for(int i = 0; i < REQUESTQUEUESIZE; i++)
    {
        randomRequestQueue[i] = (rand() % 200) -1 ;
    }
}

//Manual Initialization of Queue - Brad

//FIFO - Samira

//LIFO - Samira

//Shortest Service Time First - Samira

//SCAN - Brad

//C-SCAN - Brad

//N-STEP-Scan - Jake

//FSCAN - Jake

//Print Summary Table -Jake
