/*
 * Simulation_Engine.c
 *
 *  Created on: Sep 26, 2014
 *      Author: awhite40
 */


#include <stdio.h>
#include <stdlib.h>
#include "Simulation_Engine.h"
#include "Prio_Queue.h"
//typedef enum{Arrival, Departure} KindsofEvents;

struct Event *PrioQ = NULL;
//Initialize functions

double Now = 1;

void schedule(double ts, void *data, void (*cb)(void *d)) {
	//printf("got here1");
	struct Event *e;
	e = malloc(sizeof(struct Event));
	if (e==NULL) {
		printf("Memory not available\n");
		exit(1);
	}
	e->timestamp = ts;
	e->AppData = data;
	e->callback = cb;
	PrioQ = add_event(e,PrioQ);
    //printf("Print Priority Queue\n");
    //print_all(PrioQ);
    
	//printf("got here 2");
	//free(e);
}
double CurrentTime(void){
    //printf("Now is %f\n", Now);
	return Now;
}
void RunSim(double Finish){
	while (Now <=Finish){
		struct Event *E;
		E = malloc(sizeof(struct Event));
        E = PrioQ;
		PrioQ = remove_event(PrioQ);
        //printf("Print Priority Queue\n");
        //print_all(PrioQ);
        double *nptr = &Now;
        //printf("Now %f \n", Now);
        //printf("New timestamp %f \n", E->timestamp);
        if (E->timestamp > Now){
           *nptr = E->timestamp;
        }
		
		E->callback(E->AppData);
		free (E);
	}
}
