#include "airport.h"

int main()
{
    srand(time(0)); //Seeding the randomizer 
    int totaltime;
    double exparr, expdep; //exparr and expdep are the expected number of arrivals/depatures per unit time or rate of arrival/dep
    Que takeoff, landing;
    int validflag = 1; //Validity of rate of arrival and departures
    char choice;
    printf("\n\n\t\t\tWELCOME TO AIRPORT SIMULATION");
    printf("\n\nHow many units of time will the simulation run through?  ");
    while(!scanf("%d", &totaltime) )  //Validating integer input for simultion time
    {
        fflush(stdin);
        printf("\nEnter again; How many units of time will the simulation run through?  ");
    } 
    
    if (totaltime < 0) //Validating positive input
    {
        printf("INVALID INPUT FOR TOTAL TIME; RUN SIMULATION AGAIN");
        return 1;
    }

    do 
    {
        printf("Expected number of arrivals per unit time / rate of arrival (betwen 0 and 1)? ");
        while(!scanf("%lf", &exparr)) //Validating float type input for rate of arrivals
        {
            fflush(stdin);
            printf("Enter again; Expected number of arrivals per unit time / rate of arrival (betwen 0 and 1)? ");
        } 

        printf("Expectd number of departures per unit time / rate of departures (betwen 0 and 1)? ");
        while(!scanf("%lf", &expdep)) //Validating float type input for rate of departures
        {
            fflush(stdin);
            printf("Enter again; Expected number of departures per unit time / rate of departures (betwen 0 and 1)? ");
        } 

        if (exparr < 0.0 || expdep < 0.0) //Checking if negative
        {
            printf("\nRate of arrival/departure is invalid (-ve); Please try again");
            validflag = 0;
        }
        if (exparr + expdep > 1.0) //If total rate of flights the airport handles per unit > 1, it will get overloaded
        {
            printf("\nWarning : Airport might get saturated to peak capacity; Continue(y) or Enter again(n)?");
            scanf(" %c", &choice);
            switch(tolower(choice))
            {
                case 'y': validflag = 1; break;
                case 'n': validflag = 0; break;
                default: validflag = 0;
            }
        }
    } while (!validflag);

    int simreturn = simulate(totaltime, exparr, expdep, &takeoff, &landing);

    if (simreturn)  //Checking if simulation ran properly
        printf("\nSIMULATION FAILED");

    return 0;
}