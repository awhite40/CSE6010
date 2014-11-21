#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "FIFo_Queue.h"

#define COUnits 500 //number of units to process before doing a changeover to the other type
#define RIGHT 1
#define LEFT 0
int COCounter = 1; //counter to keep track of the number of units processed before making a changeover
int ass;
int Time = 0; //running tally of the total time to complete the entire simulation
int shipmentLeft = 0;
int shipmentRight = 0;

typedef enum {Stamping, SWeld1, SWeld2, LeftAssem, RightAssem, Shipping} KindsOfEvents;
typedef enum {Left, Right} AssemblyType;
struct Data{
    int timestamp;
    int unitnum; //number of the unit
    KindsOfEvents Process;
    AssemblyType Assembly;


    struct Data *next;
};

int main(void){
    srand(time(NULL)); //seed the random function once
    //Randomly assign the first batch of assemblies as either left or right
    rorl = (double)rand()/(double)RAND_MAX;
    if (rorl > 0.5){
        ass = RIGHT;
    }
    else if (rorl <= 0.5){
        ass = LEFT;
    }

}


void Stamping(struct Data *e){
    if (e->Process != Stamping){
        printf("UNEXPECTED EVENT TYPE\n\n");
        exit(1);
    }

    if (COCounter > COUnits){
        if (ass == RIGHT){
            ass = LEFT;
            e->Assembly = Left;
        }
        else if(ass == LEFT){
            ass == RIGHT;
            e->Assembly = Right;
        }
        Time = Time + 15*60; //Update the time to include the changeover that must occur (takes fifteen minutes)
        COCounter = 1;
    }

    //Now that the assembly condition is correct need to schedule the s.Weld 1 process, putting it into the FIFO queue for that process
    Time = Time + 20; //the cycle time for stamping is 20 seconds, so add 20 sec to the total running time
    e->Process = SWeld1;
    COCounter++;
    addQ(e);
}

void SpotWeld1(struct Data *e){
        if (e->Process != SWeld1){
        printf("UNEXPECTED EVENT TYPE\n\n");
        exit(1);
    }

    Time = Time + 38; //process takes 38 seconds
    e->Process = SWeld2;

    InsertQueue(e);
}

void SpotWeld2(struct Data *e){
        if (e->Process != SWeld2){
        printf("UNEXPECTED EVENT TYPE\n\n");
        exit(1);
    }

    Time = Time + 45; //process takes 38 seconds
    if (e->Assembly == Left){
        e->Process = LeftAssem;
    }
    else if (e->Assembly == Right){
        e->Process = RightAssem;
    }

    InsertQueue(e);
}

void LeftAssembly(struct Data *e){
        if (e->Process != LeftAssem){
        printf("UNEXPECTED EVENT TYPE\n\n");
        exit(1);
    }

    Time = Time + 61; //process takes 38 seconds
    e->Process = Shipping;

    InsertQueue(e);
}

void RightAssembly(struct Data *e){
        if (e->Process != RightAssem){
        printf("UNEXPECTED EVENT TYPE\n\n");
        exit(1);
    }

    Time = Time + 39; //process takes 38 seconds
    e->Process = Shipping;

    InsertQueue(e);
}

void Shipping(struct Data *e){
    if (e->Process != Shipping){
        printf("UNEXPECTED EVENT TYPE\n\n");
        exit(1);
    }

    Time = Time + 61; //process takes 38 seconds

    if (e->Assembly == Left && e->unitnum == 500){
        shipmentLeft++;
    }
    else if (e->Assembly == Right && e->unitnum == 500){
        shipmentRight++;
    }
    if (shipmentLeft >= shipmentRight && shipmentRight != 0){
        OutgoingShipments++;
        shipmentLeft--;
        shipmentRight--;
        printf("Shipment #%d time = %d\n", OutgoingShipments, Time);
    }
    if (shipmentRight >= shipmentLeft && shipmentLeft != 0){
        OutgoingShipments++;
        shipmentLeft--;
        shipmentRight--;
        printf("Shipment #%d time = %d\n", OutgoingShipments, Time);
    }

}
