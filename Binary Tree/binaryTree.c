#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "binaryTree.h"

struct Performance *newPerformance(){

    struct Performance *new = (struct Performance*) malloc(sizeof(struct Performance));

    if (new == NULL) {

        fprintf(stderr, "%s\n", "ERROR: FAILED TO ALLOCATE MEMORY");
        exit(0);

    } else {

        //only initialize these variables if the malloc() does not fail
        new -> reads = 0;
        new -> writes = 0;
        new -> mallocs = 0;
        new -> frees = 0;

    }

    return(new);

}

void attachNode( struct Performance *performance, struct Node **node_ptr, void *src, unsigned int width ){

    struct Node *new = (struct Node*) malloc(sizeof(struct Node));

    if (new == NULL) {

        fprintf(stderr, "%s\n", "ERROR: FAILED TO ALLOCATE MEMORY");
        exit(0);
        
    } else {

        //allocates memory for the data pointer in the Node struct
        //only works if the memory for the Node structure was allocated successfully
        new -> data = malloc(width);

        if (new -> data == NULL) {

            fprintf(stderr, "%s\n", "ERROR: FAILED TO ALLOCATE MEMORY");
            exit(0);

        } else {

            //this code is only run if the structure and data pointer have been allocated memory
            memcpy(new -> data, src, width);

            new -> lt = NULL; //sets both node pointers in the struct to null
            new -> gte = NULL;

            *node_ptr = new; //the new node is now pointed to by node_ptr

            performance -> mallocs++; //increment mallocs and writes by 1
            performance -> writes++;

        }

    }

}

int comparNode( struct Performance *performance, struct Node **node_ptr, int (*compar)(const void *, const void *), void *target ){

    performance -> reads++; //increment reads by 1

    return( compar(target, (*node_ptr) -> data) ); //returns whatever compar() returns

}

struct Node **next( struct Performance *performance, struct Node **node_ptr, int direction ){

    if (*node_ptr == NULL) {

        fprintf(stderr, "%s\n", "ERROR: EMPTY TREE");
        exit(0);

    } else {

        performance -> reads++; //increment reads by 1

        //this is where direction will be checked
        if (direction < 0){

            return(&((*node_ptr) -> lt));

        } else {

            //this is the case for direction being greater than or equal to 0
            return(&((*node_ptr) -> gte));

        }

    }

}

void readNode( struct Performance *performance, struct Node **node_ptr, void *dest, unsigned int width ){

    if (*node_ptr == NULL) {

        fprintf(stderr, "%s\n", "ERROR: EMPTY TREE");
        exit(0);

    } else {

        memcpy(dest, (*node_ptr) -> data, width); //copy data from node to destination

        performance -> reads++; //increment reads by 1

    }

}

void detachNode(struct Performance *performance, struct Node **node_ptr ){

    if (*node_ptr == NULL) {

        fprintf(stderr, "%s\n", "ERROR: EMPTY TREE");
        exit(0);

    } else {

        free((*node_ptr) -> data); //frees data allocated within the node
        free(*node_ptr); //frees actual node

        *node_ptr = NULL; //update pointer to NULL

        performance -> frees++; //increment frees by 1

    }

}

int isEmpty( struct Performance *performance, struct Node **node_ptr ){

    if (*node_ptr == NULL) {

        return(1);

    } else {

        return(0);

    }

}

void addItem( struct Performance *performance, struct Node **node_ptr, int (*compar)(const void *, const void *), void *src, unsigned int width ){

    struct Node **tempNodePtr = node_ptr; //so value of node_ptr does not get changed
    int direction; //used for next()

    while (*tempNodePtr != NULL) {

        direction = comparNode(performance, tempNodePtr, compar, src); //this will compare the current node's data with src
        tempNodePtr = next(performance, tempNodePtr, direction);

    } 

    //tempNodePtr is an empty pointer after the loop
    attachNode(performance, tempNodePtr, src, width);

}

void freeTree( struct Performance *performance, struct Node **node_ptr ) {

    if (*node_ptr == NULL) {
        //returning nothing ends the function once the base case is reached
        return;
    } 

    //follows gte branch
    freeTree(performance, next(performance, node_ptr, 0)); //node_ptr will now be the gte Node

    //follows lt branch
    freeTree(performance, next(performance, node_ptr, -1)); //node_ptr will now be the lt Node

    detachNode(performance, node_ptr); //this is what frees the memory

}

int searchItem( struct Performance *performance, struct Node **node_ptr, int (*compar)(const void *, const void *), void *target, unsigned int width ) {

    struct Node **tempNodePtr = node_ptr; //so value of node_ptr does not get changed
    int direction;

    while (*tempNodePtr != NULL) {

        direction = comparNode(performance, tempNodePtr, compar, target);
        
        if (direction == 0) {

            //target node found
            readNode(performance, tempNodePtr, target, width); //the macthing Node will be stored in target
            return(1);

        }

        tempNodePtr = next(performance, tempNodePtr, direction);

    }

    return(0); //return statement for when target node is not found
    
}