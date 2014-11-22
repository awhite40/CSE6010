//
//  main.c
//  manufacture
//
//  Created by Alicia White on 11/21/14.
//  Copyright (c) 2014 Alicia White. All rights reserved.
//

#include <stdio.h>
#include "FIFO_Queue.h"
#include <stdlib.h>
struct obj{
    int num;
};


struct obj *Create_event(int);

struct obj *Create_event(int num){
    struct obj *T;
    T = malloc(sizeof(struct obj));
    T->num = num;
    return T;

}

int main(int argc, const char * argv[]) {
    
    //Create queue
    struct node *ptrs;
    ptrs = CreateQ();
    
    
    // Create objects to be inserted to the queue
    int i=1;
    while (i<10){
        struct obj *T;
        T= Create_event(i);
        i=i+1;
        // Add them to the queue
        addQ(T,ptrs);
        
    }
    // Count the number of elements in the queue
    int nums = count_all(ptrs);
    printf("%d\n",nums);
    
    
    //Remove from queue
    int j =1;
    while (j<=5){
        struct obj *new = removeQ(ptrs);
        printf("Queue number %d\n",new->num);
        j=j+1;
    }
    
    
    
    return 0;
}
