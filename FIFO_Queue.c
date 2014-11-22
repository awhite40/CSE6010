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


struct node *CreateQ(void){
    struct node *ptrs=(struct node *) malloc(sizeof(struct node));
    if (ptrs == NULL) {printf("Could not allocate memory for Queue"); exit(1);}
    ptrs->Next = NULL;
    ptrs->Last = NULL;
    return ptrs;
}

int count_all(struct node *ptrs){
    int count = 0;
    if (ptrs->Next==NULL)
    {
        printf("empty queue");
    }else{
        struct node *ptr= ptrs->Next;
        while (ptr != NULL) //print each element of the queue
        {
            count = count+1;
            ptr=ptr->Next;
        }
    }
    return count;
}
void addQ(void *a, struct node *ptrs){
    
    //printf("Start add\n");
    struct node *e;
    e = a;
    if (ptrs->Next == NULL){ //if there are no events in the Queue
        //printf("in the if\n");
        ptrs->Next = e; //First and last point to the same element
        ptrs->Last = e;
        e->Next = NULL;
    }else{
        //printf("else3\n");
        ptrs->Last->Next = e;
        ptrs->Last = e;
        e->Next = NULL;
    }
    
}

void *removeQ(struct node *ptrs){
	struct node *q;
    //If queue is empty print an error
	if (ptrs->Next == NULL) {
		printf("Empty Queue\n");
		exit(1);
	}
    //Create a dummy pointer to the event to be removed
    q = ptrs->Next;
    //Move highest priority pointer to next entry
	ptrs->Next= ptrs->Next->Next;
	return q;
}


