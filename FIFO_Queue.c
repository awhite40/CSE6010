/*
 * Queue.c

 *
 *  Created on: Sep 26, 2014
 *      Author: awhite40
 */
#include <stdio.h>
#include <math.h>
#include <stdlib.h> //10
#include "FIFO_Queue.h"
//struct Event *PrioQ = NULL;
//struct Event *e;

void print_all(struct node *First){
    if (First==NULL)
    {
        printf("empty queue");
    }else{
        struct node *ptr= First;
        while (ptr != NULL) //print each element of the queue
        {
            printf ("%d\n",ptr->d);
            ptr=ptr->Next;
        }
    }
}
void addQ(void *a, struct node *First, struct node *Last){
    
    //printf("Start add\n");
    struct node *e;
    e = a;
    if (First == NULL){ //if there are no events in the Queue
        //printf("in the if\n");
        First = e; //First and last point to the same element
        Last = e;
        e->Next = NULL;
    }else{
        //printf("else3\n");
        Last->Next = e;
        Last = e;
        e->Next = NULL;
    }
    
}

struct node *removeQ(struct node *First){
	struct node *q;
    //If queue is empty print an error
	if (First == NULL) {
		printf("Empty Queue\n");
		exit(1);
	}
    //Create a dummy pointer to the event to be removed
    q = First;
    //Move highest priority pointer to next entry
	First= First->Next;
	return q;
}


