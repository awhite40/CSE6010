/*
 * Queue.h
 *
 *  Created on: Sep 26, 2014
 *      Author: awhite40
 */

#ifndef FIFO_QUEUE_H_
#define FIFO_QUEUE_H_
// Initializing struct Event
struct node{
    void *p;
    struct node *Next;
};
//Function to add and event to the queue
//This function requires and input a pointer to the object to be added and the pointer the first location in the queue and a pointer to the last element of the queue
void addQ(void *, struct node *,struct node *);

//Function to remove an event from the priority queue
//This function requires an input of the pointer to the first element of the queue and returns the pointer to the object that was removed
struct node *removeQ(struct node *);
//A function to print all of the elements of the queue in timestamp order
//THis function requires and input of the pointer to the first element in the queue
void print_all(struct node *);


#endif /* FIFO_QUEUE_H_ */
