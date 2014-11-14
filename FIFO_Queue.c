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

void print_all(struct node *PrioQ){
    if (PrioQ==NULL)
    {
        printf("empty queue");
    }else{
        struct node *ptr= PrioQ;
        while (ptr != NULL && ptr->d !=0) //print each element of the queue
        {
            printf ("%d\n",ptr->d);
            ptr=ptr->Next;
        }
    }
}
struct node *addQ(void *a, struct node *PrioQ){
    
    //printf("Start add\n");
    struct node *e;
    e = a;
    if (PrioQ == NULL){ //if there are no events in the Queue
        //printf("in the if\n");
        PrioQ = e; //change prioQ to the new first element
        e->Next = NULL;
    }else if (e->d<= PrioQ->d){ //if d of new element is lowest
        e->Next=PrioQ;
        PrioQ=e;
        //printf("else1\n");
    /*}else if (PrioQ->Next ==NULL){
        PrioQ=e;
        e->Next=NULL;
        printf("else2\n");*/
    }else{
        //printf("else3\n");
        struct node * temp=PrioQ;
        while (temp->Next != NULL && e->d > temp->Next->d){
            temp=temp->Next;
        }
        e->Next=temp->Next;
        temp->Next=e;
    }
    return PrioQ;
    
}
struct node *addQ_max(void *a, struct node *PrioQ){
    
    //printf("Start add\n");
    struct node *e;
    e = a;
    if (PrioQ == NULL){ //if there are no events in the Queue
        //printf("in the if\n");
        PrioQ = e; //change prioQ to the new first element
        e->Next = NULL;
    }else if (e->d>= PrioQ->d){ //if d of new element is highest
        e->Next=PrioQ;
        PrioQ=e;
        //printf("else1\n");
        /*}else if (PrioQ->Next ==NULL){
         PrioQ=e;
         e->Next=NULL;
         printf("else2\n");*/
    }else{
        //printf("else3\n");
        struct node * temp=PrioQ;
        while (temp->Next != NULL && e->d > temp->Next->d){
            temp=temp->Next;
        }
        e->Next=temp->Next;
        temp->Next=e;
    }
    return PrioQ;
    
}
struct node *removeQ(struct node *PrioQ){
	struct node *q;
    //If queue is empty print an error
	if (PrioQ == NULL) {
		printf("Empty Queue\n");
		exit(1);
	}
    //Create a dummy pointer to the event to be removed
    q = PrioQ;
    //Move highest priority pointer to next entry
	PrioQ = PrioQ->Next;
	return q;
}

struct node *removeQ_ran(struct node *PrioQ, int nums){
    //struct node *q;
    //If queue is empty print an error
    if (PrioQ == NULL) {
        printf("Empty Queue\n");
        exit(1);
    }else{
        struct node *ptr= PrioQ;
        struct node *ptr2 = NULL;
        if (PrioQ->num ==nums){
            PrioQ = PrioQ->Next;
            return ptr;
        }
        while (ptr != NULL && ptr->d >=0) //scan through each element of the queue
        {
            if (ptr->num == nums){
                ptr2->Next = ptr->Next;
                return ptr;
            }
            ptr2 = ptr;
            ptr=ptr->Next;
        }
    }

    //Create a dummy pointer to the event to be removed
    //Move highest priority pointer to next entry
    return NULL;
}

struct node *searchQ(struct node *PrioQ, int nums){
    if (PrioQ==NULL)
    {
        //printf("empty queue search \n");
    }else{
        struct node *ptr= PrioQ;
        while (ptr != NULL){// && ptr->d >=0) //scan through each element of the queue
            //printf("ptr->num = %d \n",ptr->num);
            
            if (ptr->num == nums){
                //printf("found it\n");
                return ptr;
            }
            
            ptr=ptr->Next;
        }
    }
    return 0;
}


