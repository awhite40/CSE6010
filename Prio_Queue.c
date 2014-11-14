/*
 * Queue.c

 *
 *  Created on: Sep 26, 2014
 *      Author: awhite40
 */
#include <stdio.h>
#include <math.h>
#include <stdlib.h> //10
#include "Prio_Queue.h"
//struct Event *PrioQ = NULL;
//struct Event *e;

void print_all(struct Event *PrioQ){
    if (PrioQ==NULL)
    {
        printf("empty queue");
    }else{
        struct Event *ptr= PrioQ;
        while (ptr != NULL && ptr->timestamp !=0) //print each element of the queue
        {
            printf ("%f\n",ptr->timestamp);
            ptr=ptr->Next;
        }
    }
}
struct Event *add_event(void *a, struct Event *PrioQ){
    
    //printf("Start add\n");
    struct Event *e;
    e = a;
    if (PrioQ == NULL){ //if there are no events in the Queue
        //printf("in the if\n");
        PrioQ = e; //change prioQ to the new first element
        e->Next = NULL;
    }else if (e->timestamp<= PrioQ->timestamp){ //if timestamp of new element is lowest
        e->Next=PrioQ;
        PrioQ=e;
        //printf("else1\n");
        /*}else if (PrioQ->Next ==NULL){
         PrioQ=e;
         e->Next=NULL;
         printf("else2\n");*/
    }else{
        //printf("else3\n");
        struct Event * temp=PrioQ;
        while (temp->Next != NULL && e->timestamp > temp->Next->timestamp){
            temp=temp->Next;
        }
        e->Next=temp->Next;
        temp->Next=e;
    }
    return PrioQ;
    
}
struct Event *remove_event(struct Event *PrioQ){
    struct Event *q;
    //If queue is empty print an error
    if (PrioQ == NULL) {
        printf("Empty Queue\n");
        exit(1);
    }
    //Create a dummy pointer to the event to be removed
    q = PrioQ;
    //Move highest priority pointer to next entry
    PrioQ = PrioQ->Next;
    return PrioQ;
}


