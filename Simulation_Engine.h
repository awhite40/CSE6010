/*
 * Simulation_Engine.h
 *
 *  Created on: Sep 23, 2014
 *      Author: awhite40
 */

#ifndef SIMULATION_ENGINE_H_
#define SIMULATION_ENGINE_H_

//Function to schedule a new event into the priority queue
//This function requires inputs of a double timestamp (or other indicator of priority) a void pointer to any relevant data and a void pointer to the callback funtion which takes in a void pointer
void schedule(double ts, void *data, void (*cb)(void *));


//This function runs the simulation engine while loop
//This function requires an input of type double to indicate how long the simulation should run
void RunSim(void);


//Function to find the current time
//No inputs returns a double time
double CurrentTime(void);


#endif /* SIMULATION_ENGINE_H_ */
