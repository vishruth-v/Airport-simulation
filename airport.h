#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <ctype.h>

//Defining the Maximum elements in the queue 
#define MAXQUEUE 5

//Defining TAKEOFF and LANDING actions to numbers
typedef enum action {TAKEOFF = 1 , LANDING = 2} Action;

//Plane details struture with provision for sibgly linked list
typedef struct plane
{
    int planeno;
    int reqtime;
    Action action; //TAKEOFF OR LANDING
    struct plane* next;
}Plane;

//Queue data structure (FIFO linked list) to store details of the queue
typedef struct queue
{
    int count; //Number of Planes in the queue
    Plane* front; // first plane of the queue
    Plane* rear; // last plane of the queue 
}Que;


//Function to predict the random number of requests made in a unit of time by using poisson process on rate of requests (expected number of planes per unit time)
int poissonrandom(double); 

//Function to add a plane to a queue at the end (As FIFO)
void enqueue(Que*, Plane*);

//Function to delete a plane from queue at the front (AS FIFO)
Plane* dequeue(Que*);

//Function to reject service to a plane
void reject(Plane);

//Function to create a New dynamic plane node
Plane* newplane(Action, int no, int reqtime);

//Function to initialize queues at the start of simulation
void initqueue(Que* queue);

//Fucntion to print message while dequeueing plane to any queue
int printmsgdeque(int , Plane);

//Fucntion to print message while enqueueing plane to any queue
void printmsgenqueue(Plane);

//Function to simulate the whole functioning of the airport
int simulate(int, double, double, Que*, Que*);

//Function to print the final statistics of simulation
void simstatistics(int, int, int, int, int, int, int, int, int, int);





