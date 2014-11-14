/*
 * Queue.h
 *
 *  Created on: Sep 26, 2014
 *      Author: awhite40
 */

#ifndef QUEUE_2_H_
#define QUEUE_2_H_
// Initializing struct Event
struct Event{
    double timestamp;
    void *AppData;
    void(*callback)(void *);
    struct Event *Next;
};


//Function to add and event to the queue
//This function requires and input a pointer to the event to be added and the pointer the first location in the queue and outputs the pointer to the new first event
struct Event *add_event(void *, struct Event *);

//Function to remove an event from the priority queue
//This function requires and input of the pointer to the front of the priority queue and returns the pointer to the new first location
struct Event *remove_event(struct Event *);


//A function to print all of the elements of the queue in timestamp order
//THis function requires and input of the pointer to the first element in the queue
void print_all(struct Event *);


#endif /* QUEUE_2_H_ */
