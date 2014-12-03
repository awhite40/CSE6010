#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "Prio_Queue.h"
#include "Simulation_Engine.h"

#define COUnits 250 //number of units to process before doing a changeover to the other type
#define RIGHT 1
#define LEFT 0

int COCounter = 1; //counter to keep track of the number of units processed before making a changeover
int ass;
double Time = 0.0; //running tally of the total time to complete the entire simulation
double now = 0.0;
int ctr = 0;
int start = 0;
int stamp_up_ctr = 1;
int weld1_up_ctr = 1, weld2_up_ctr =1;
double stamp_per = 0.15,weld1_per = 0.2,weld2_per = 0.2;
double stamp_time = 20,weld1_time = 38, weld2_time = 45;
double Stamp_change = 15*60;
int stamp_ctr =0;
//Variables to keep track of the number of shipments that are output
int shipmentLeft = 0;
int leftctr = 0;
int shipmentRight = 0;
int rightctr = 0;
int OutgoingShipments = 0;

//Variables to keep track of the endtime of a given process to accurately schedule the new process
double sw1_endtime = 0.0;
double sw2_endtime = 0.0;
double assleft_endtime = 0.0;
double assright_endtime = 0.0;

//list of variables that will give the max length in each queue throughout the simulation
int sweld1_q = 0;
int sweld1_max = 0;
int sweld2_q = 0;
int sweld2_max = 0;
int assleft_q = 0;
int assleft_max = 0;
int assright_q = 0;
int assright_max = 0;

//Data structure definition
typedef enum {Stamp, SWeld1, SWeld2, LeftAssem, RightAssem, Ship} KindsOfEvents;
typedef enum {Left, Right} AssemblyType;
struct Data{
    double timestamp;
    int unitnum; //number of the unit
    KindsOfEvents Process;
    AssemblyType Assembly;
};

//Function declarations
void Stamping(struct Data *e);
void SpotWeld1(struct Data *e);
void SpotWeld2(struct Data *e);
void LeftAssembly(struct Data *e);
void RightAssembly(struct Data *e);
void Shipping(struct Data *e);

int main(int argc, char *argv[]){
    /*if (argc != 2) {
		fprintf (stderr, "usage: program order_number\n");
		exit (1);
    }
    if ((sscanf(argv[1],"%d", &num_units)) == NULL) {
		fprintf (stderr, "need to enter a total order number\n", argv[1]);
		exit (1);
    }*/

    srand(time(NULL)); //seed the random function once
    //Randomly assign the first batch of assemblies as either left or right
    struct Data *e;
	e = malloc(sizeof(struct Data));
	if (e==NULL) {
		printf("Memory not available\n");
		exit(1);
	}

	//use random numbers to determine initially whether to process right or left assembly
    double rorl = (double)rand()/(double)RAND_MAX;
    if (rorl > 0.5){
        ass = RIGHT;
        e->Assembly = Right;
    }
    else if (rorl <= 0.5){
        ass = LEFT;
        e->Assembly = Left;
    }
    e->unitnum = COCounter;
    start = 1;
    e->Process = Stamp;
    schedule(now, e, (void *)Stamping);
    // this loop will call the simulation for each unit that the customer has ordered.

    int order_num = 4000; //argv[2];
    clock_t begin, finish;
    begin = clock();
    while (ctr < order_num){
        RunSim();
        //printf("%d\n", ctr); ok produces 10000
    }
    finish = clock();

    double TotTime = ((double)(finish - begin))/CLOCKS_PER_SEC;
    printf("Maximum queue lengths:\n");
    printf("Spot Weld 1\t %d\n", sweld1_max);
    printf("Spot Weld 2\t %d\n", sweld2_max);
    printf("Left Assembly\t %d\n", assleft_max);
    printf("Right Assembly\t %d\n\n", assright_max);
    printf("Total run time: %.2f days\n", now/(double)54000);
    printf("Total shipments: %d\n", OutgoingShipments);
    printf("Total Time of simulation: %e\n", TotTime);

    return 0;
}

void Stamping(struct Data *e){

    if (start == 1){
        COCounter++;
        start = 0;
    }
    if (e->Process != Stamp){
        printf("UNEXPECTED EVENT TYPE\n\n");
        exit(1);
    }
    //printf("%d\n",e->unitnum);
    now = e->timestamp;
    double downtime = 0.0;
    double mod_now = now - (double)stamp_up_ctr*27000.0+22950.0;
    if (mod_now > 0.0){
        downtime = stamp_per*27000;
        stamp_up_ctr++;
    }

    double CODone = 0.0; //variable to define when the changeover is done, if it occurs
    double extratime = 0.0; //defines any additional time to be added due to a unit in process while another unit is placed into queue
    Time = 0.0;
    double stamptime = 0.0;
    struct Data *f;
	f = malloc(sizeof(struct Data));
	if (f==NULL) {
		printf("Memory not available\n");
		exit(1);
	}

    if (COCounter > COUnits){
        if (ass == RIGHT){
            ass = LEFT;
            f->Assembly = Left;
        }
        else{
            ass = RIGHT;
            f->Assembly = Right;
        }
        CODone = 15.0*60.0; //Update the time to include the changeover that must occur (takes fifteen minutes)
        //printf("%d\n", COCounter);
        COCounter = 1;
        //e->unitnum = COCounter;
    }
    else{
        if (ass == RIGHT){
            f->Assembly = Right;
            //printf("RightAssembly\n");
        }
        else if (ass == LEFT){
            f->Assembly = Left;
        }
    }

    f->unitnum = COCounter;
    f->Process = Stamp;

    stamptime = now + (double)stamp_time + CODone + downtime;

    //printf("Stamptime: %.2f\n", stamptime);
    f->timestamp = stamptime;
    if (stamp_ctr < 4000){
        schedule(stamptime, f, (void *)Stamping);
    }
    stamp_ctr++;
    int eff_q = sweld1_q;
    //This is the time for the next sweld process
    if (sw1_endtime > now+stamp_time){
        extratime = sw1_endtime - (now+stamp_time);
    }
    else if (sw1_endtime < now+stamp_time && (double)sweld1_q > stamp_time/weld1_time){
        extratime = weld1_time - (now+stamp_time - sw1_endtime);
        eff_q = sweld1_q - ceil(stamp_time/weld1_time);
    }
    
    double downtime_w1 = 0.0;
    double mod_now_w1 = now - (double)weld1_up_ctr*27000.0+22950.0;
    if (mod_now_w1 > 0.0){
        downtime_w1 = 27000*weld1_per;
        weld1_up_ctr++;
    }

    Time = now + stamp_time + (double)eff_q*(double)weld1_time + extratime + downtime; //the cycle time for stamping is 20 seconds, so add 20 sec to the total running time
    e->Process = SWeld1;
    e->timestamp = Time;
    sweld1_q++;
    //printf("COCtr: %d\n",COCounter);
    COCounter++;
    //printf("Time: %.2f\n", Time);
    schedule(Time, e, (void *)SpotWeld1);
}

void SpotWeld1(struct Data *e){
    now = e->timestamp;
    double downtime = 0.0;
    double mod_now = now - (double)weld2_up_ctr*27000.0-22950.0;
    if (mod_now > 0.0){
        downtime = 27000*weld2_per;
        weld2_up_ctr++;
    }


    Time = 0.0;
    if (sweld1_q > sweld1_max){
        sweld1_max = sweld1_q;
    }
    sweld1_q--;
    //if (now < 5000.0){
    //printf("Sweld 1 Q: %d\t %d\n", sweld1_q, e->unitnum);
    //}
    double extratime = 0.0;
    if (e->Process != SWeld1){
        printf("UNEXPECTED EVENT TYPE\n\n");
        exit(1);
    }

    int eff_q = sweld2_q;
    if (sw2_endtime > now+weld1_time){
        extratime = sw2_endtime - (now+weld1_time);
    }
    else if (sw2_endtime < now+weld1_time && (double)sweld2_q > weld1_time/weld2_time){
        extratime = weld2_time - (now+weld1_time - sw2_endtime);
        eff_q = sweld2_q - ceil(weld1_time/weld2_time);
    }

    sw1_endtime = now + (double)weld1_time; //process takes 38 seconds
    Time = sw1_endtime + (double)eff_q*(double)weld2_time + extratime + downtime; //calculate the time at which the unit will enter the next process
    e->Process = SWeld2;
    e->timestamp = Time;
    sweld2_q++;
    schedule(Time,e,(void *)SpotWeld2);
}

void SpotWeld2(struct Data *e){
    now = e->timestamp;
    Time = 0.0;
    //printf("%d\n",e->unitnum);
    double extratime = 0.0;
    if (sweld2_q > sweld2_max){
        sweld2_max = sweld2_q;
    }
    sweld2_q--;
    if (e->Process != SWeld2){
        printf("UNEXPECTED EVENT TYPE\n\n");
        exit(1);
    }
    int eff_q = assleft_q;
    sw2_endtime = now + (double)weld2_time; //process takes 38 seconds
    if (e->Assembly == Left){
        e->Process = LeftAssem;
        if (assleft_endtime > sw2_endtime){
            extratime = assleft_endtime - sw2_endtime;
        }
        else if (assleft_endtime < sw2_endtime && (double)assleft_q > weld2_time/61.0){
            extratime = 61.0 - (now+weld2_time - assleft_endtime);
            eff_q = assleft_q - ceil(weld2_time/61.0);
        }
        Time = sw2_endtime + (double)eff_q*61.0 + extratime;
        e->timestamp = Time;
        schedule(Time,e,(void *)LeftAssembly);
        assleft_q++;
    }
    else if (e->Assembly == Right){
        e->Process = RightAssem;
        if (assright_endtime > now){
            extratime = assright_endtime - sw2_endtime;
        }
        else if (assright_endtime < sw2_endtime && (double)assright_q > weld2_time/39.0){
            extratime = 39.0 - (now+weld2_time - assright_endtime);
            eff_q = assright_q - ceil(weld2_time/39.0);
        }
        Time = sw2_endtime + (double)eff_q*39.0 + extratime;
        e->timestamp = Time;
        schedule(Time,e,(void *)RightAssembly);
        assright_q++;
    }
}

void LeftAssembly(struct Data *e){
    now = e->timestamp;

    Time = 0.0;
    if (assleft_q > assleft_max){
        assleft_max = assleft_q;
    }
    assleft_q--;
    if (e->Process != LeftAssem){
        printf("UNEXPECTED EVENT TYPE\n\n");
        exit(1);
    }

    Time = now + (double)61; //process takes 61 seconds
    assleft_endtime = Time;
    e->Process = Ship;
    e->timestamp = Time;
    schedule(Time,e,(void *)Shipping);
}

void RightAssembly(struct Data *e){
    now = e->timestamp;

    Time = 0.0;
    if (assright_q > assright_max){
        assright_max = assright_q;
    }
    assright_q--;
    if (e->Process != RightAssem){
        printf("UNEXPECTED EVENT TYPE\n\n");
        exit(1);
    }

    Time = now + (double)39; //process takes 38 seconds
    assright_endtime = Time;
    e->Process = Ship;
    schedule(Time,e,(void *)Shipping);
}

void Shipping(struct Data *e){
    now = e->timestamp;
    //printf("unitnum: %d\n", e->unitnum);
    //printf("time: %.2f\n\n", e->timestamp);

    if (e->Process != Ship){
        printf("UNEXPECTED EVENT TYPE\n\n");
        exit(1);
    }
    if (e->Assembly == Left){
        leftctr++;
    }
    else if (e->Assembly == Right){
        rightctr++;
    }

    if (e->Assembly == Left && leftctr == 250){
        shipmentLeft++;
        leftctr = 1;
        //printf("ShipLeft: %d\n",shipmentLeft);
    }
    else if (e->Assembly == Right && rightctr == 250){
        shipmentRight++;
        rightctr = 1;
        //printf("ShipRight: %d\n",shipmentRight);
    }
    if (shipmentLeft >= shipmentRight && shipmentRight != 0){
        OutgoingShipments++;
        shipmentLeft--;
        shipmentRight--;
        printf("Shipment #%d time = %.2f days\n", OutgoingShipments, now/(double)54000); //use 54000 to indicate the total amount of working time for 2 8 hour shifts per day


        //printf("AssLeft Q: %d\n", assleft_q);
        //printf("AssRight Q: %d\n", assright_q);
        //printf("Sweld 1 Q: %d\n", sweld1_q);
        //printf("sweld 2 Q: %d\n", sweld2_q);
    }
    if (shipmentRight >= shipmentLeft && shipmentLeft != 0){
        OutgoingShipments++;
        shipmentLeft--;
        shipmentRight--;
        printf("Shipment #%d time = %.2f days\n", OutgoingShipments, now/(double)54000);
    }

    ctr++;
    //printf("%d\n", ctr);
    free(e);
}
