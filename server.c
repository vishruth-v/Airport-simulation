#include "airport.h"

//Function to predict the random number of requests made in a unit of time by using poisson process on rate of requests (expected number of planes per unit time)
int poissonrandom(double expected)
{
    int n = 0;
    double limit, temp;
    limit = exp(-expected); //Limit of the poisson process
    temp = rand() / (double) RAND_MAX;
    while (temp > limit) //Applying poisson counting process
    {
        n++;
        temp *= rand() / (double) RAND_MAX;
    }
    return n;
}

//Function to add a plane to a queue at the end (As FIFO)
void enqueue(Que* que, Plane* plane)
{
    if (que->rear == NULL) //If no elements present in the queue
    {
        que->front = plane;
        que->rear = plane;
    }
    else
    {
        Plane* temp = que->rear; 
        temp->next = plane; //Connect/Add to the queue
        que->rear = plane; //Update 
    }
    que->count += 1;
}   

//Function to delete a plane from queue at the front (AS FIFO)
Plane* dequeue(Que* que)
{
    if (que->front == NULL)
        return NULL; //NULL if unsuccessful in dequeue

    Plane* first = que->front;
    que->front = first->next; //Disconnect the plane from the queue

    if (que->front == NULL)
        que->rear = NULL;
    
    que->count -= 1; 
    return first; //Return details of the plane removed from queue
}

//Function to reject service to a plane
void reject(Plane plane)
{
    if (plane.action == LANDING)
        printf("\n\tPlane %d wanted to land; Refused request and Plane redirected as airport capacity reached", plane.planeno);
    else if (plane.action == TAKEOFF)
        printf("\n\tPlane %d wanted to takeoff; Refused request as airport capacity reached", plane.planeno);
}

//Function to create a New dynamic plane node
Plane* newplane(Action action, int planeno, int reqtime)
{
    Plane* new = (Plane*)malloc(sizeof(Plane));
    if (new == NULL)
        return NULL; //NULL returned if malloc unsuccessful
    
    new->action = action;
    new->planeno = planeno;
    new->reqtime = reqtime;
    new->next = NULL;
    return new; 
}

//Function to initialize queues at the start of simulation
void initqueue(Que* queue)
{
    queue->count = 0;
    queue->front = NULL;
    queue->rear = NULL;
}

//Fucntion to print message while dequeueing plane to any queue
int printmsgdeque(int time, Plane plane)
{
    int waittime = time - plane.reqtime - 1;
    if(plane.action == LANDING)
        printf("\n%d : Plane %d landed in Runway 1; in queue for %d units", time, plane.planeno, waittime);
    else if(plane.action == TAKEOFF)
        printf("\n%d : Plane %d took off from Runway 2; in queue for %d units", time, plane.planeno, waittime);
    return waittime;
}

//Fucntion to print message while enqueueing plane to any queue
void printmsgenqueue(Plane plane)
{
    if(plane.action == LANDING)
        printf("\n\tPlane %d ready to land", plane.planeno);
    else if(plane.action == TAKEOFF)
         printf("\n\tPlane %d ready to takeoff", plane.planeno);
}

//Function to print the final statistics of simulation
void simstatistics(int simtime, int planes, int landed, int takenoff, int idle, int landwait, int takeoffwait, int left2land, int left2takeoff, int rejected)
{
    printf("\n\nSimulation has ended after %d units of time", simtime);
    printf("\nNumber of planes processed\t          :\t%5d", planes);
    printf("\n\tNumber of planes landed           :\t%5d", landed);
    printf("\n\tNumber of planes taken off        :\t%5d", takenoff);
    printf("\n\tNumber of planes refused use      :\t%5d", rejected);
    printf("\n\tNumber of planes left to land     :\t%5d", left2land);
    printf("\n\tNumber of planes left to take off :\t%5d", left2takeoff);
    float peridle = ((float)idle/ simtime) * 100.0;
    printf("\n\tPercentage of time runway idle    :\t%5.2f", peridle);
    float avglandwait = (float)landwait / landed;
    float avgtakeoffwait = (float)takeoffwait / takenoff;
    printf("\n\tAverage wait time to land         :\t%5.2f", avglandwait);
    printf("\n\tAverage wait time to take off     :\t%5.2f", avgtakeoffwait);
}

//Function to simulate the whole functioning of the airport
int simulate(int totaltime, double exparr, double expdep, Que* takeoff, Que* landing)
{
    initqueue(takeoff);
    initqueue(landing);
    int curtime = 0; //Current time
    int planeno = 1;
    int notakenoff = 0, nolanded = 0; //Number of flights takenoff and landed
    int idletime = 0;
    int totlandwait = 0, tottakeoffwait = 0;
    int rejected = 0;
    int noarr, nodep; //number of arrival and departure requests received in a unit of time
    Plane* new = NULL;
    Plane* landedplane;
    Plane* tookoffplane;

    for (curtime = 0; curtime <= totaltime; ++curtime) //Loop for time 
    {
        if (curtime > 0) //No dequeues done before 1st unit 
        {
            if (landing->count > 0) //If any landing request present, given priority over takeoff
            {
                landedplane = dequeue(landing);
                if (tookoffplane == NULL)
                {
                    printf("ERROR: UNABLE TO DEQUEUE PLANE");
                    return 2;
                }
                int wait = printmsgdeque(curtime, *landedplane);
                free(landedplane);
                nolanded++;
                totlandwait += wait;
            }
            else if (takeoff->count > 0)
            {
                tookoffplane = dequeue(takeoff);
                if (tookoffplane == NULL)
                {
                    printf("ERROR: UNABLE TO DEQUEUE PLANE");
                    return 2;
                }
                int wait = printmsgdeque(curtime, *tookoffplane);
                free(tookoffplane);
                notakenoff++;
                tottakeoffwait += wait;
            }
            else //If both takeoff and landing queues empty then runway idle
            {
                printf("\n%d: Runways are idle", curtime);
                idletime++;
            }
        }

        if (curtime < totaltime) //Requests not taken in the final unit of simulation
        {
            noarr = poissonrandom(exparr); //Getting random number of landing requests ny poisson process
            for (int i = 0; i < noarr; ++i)
            {
                new = newplane(LANDING, planeno, curtime); 
                if (new != NULL && landing->count < MAXQUEUE) 
                {
                    enqueue(landing, new);
                    printmsgenqueue(*new);
                    planeno++;
                }   
                else if (landing->count >= MAXQUEUE) //If queue capacity exceeded then service refused
                {
                    reject(*new);
                    planeno++;
                    rejected++;
                }
                else
                {
                    printf("ERROR: UNABLE TO ENQUEUE PLANE");
                    return 1;
                }
            }

            nodep = poissonrandom(expdep); //Getting random number of takeoff requests ny poisson process
            for (int i = 0; i < nodep; ++i)
            {
                new = newplane(TAKEOFF, planeno, curtime); 
                if (new != NULL && takeoff->count < MAXQUEUE)
                {
                    enqueue(takeoff, new);
                    printmsgenqueue(*new);
                    planeno++;
                }
                else if (takeoff->count >= MAXQUEUE) //If queue capacity exceeded then service refused
                {
                    reject(*new);
                    planeno++;
                    rejected++;
                }
                else
                {
                    printf("ERROR: UNABLE TO ENQUEUE PLANE");
                    return 1;
                }
                
            }
        }
    }
    planeno -= 1;

    //Call function to print statistics of simulation
    simstatistics(totaltime, planeno, nolanded, notakenoff, idletime, totlandwait, tottakeoffwait, landing->count, takeoff->count, rejected);
    
    return 0;
}

