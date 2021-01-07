#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

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

void push( struct Performance *performance, struct Node **list_ptr, void *src, unsigned int width ){

    struct Node *new = (struct Node*) malloc(sizeof(struct Node));

    if (new == NULL) {

        fprintf(stderr, "%s\n", "ERROR: FAILED TO ALLOCATE MEMORY");
        exit(0);

    } else {

        new -> data = malloc(width); //malloc for data can only be done after successful struct malloc

        if (new -> data == NULL) {

            fprintf(stderr, "%s\n", "ERROR: FAILED TO ALLOCATE MEMORY");
            exit(0);

        } else {

            //this code is run if both mallocs are successful
            memcpy(new -> data, src, width);
            new -> next = *list_ptr; //sets the next pointer of the new node to the current head pointer

            *list_ptr = new; //new node is now pointed to by the head pointer

            performance -> mallocs++; //increment mallocs and writes by 1
            performance -> writes++; 

        }

    }

}

void readHead( struct Performance *performance, struct Node **list_ptr, void *dest, unsigned int width ){

    if (*list_ptr == NULL) {

        fprintf(stderr, "%s\n", "ERROR: EMPTY LIST");
        exit(0);

    } else {

        //this code is run if the list is not empty (i.e. it has a head node)
        memcpy(dest, (*list_ptr) -> data, width);

        performance -> reads++; //increment reads by 1

    }

}

void pop( struct Performance *performance, struct Node **list_ptr, void *dest, unsigned int width ){

    struct Node *temp = NULL;

    if (*list_ptr == NULL) {

        fprintf(stderr, "%s\n", "ERROR: EMPTY LIST");
        exit(0);

    } else {

        //this code is run if the list is not empty (i.e. it has a head node)
        memcpy(dest, (*list_ptr) -> data, width);

        temp = *list_ptr; //stores current head pointer
        *list_ptr = (*list_ptr) -> next; //updates the head pointer to the next node

        free(temp -> data); //frees data in the former head node
        free(temp); //frees actual head pointer

        performance -> frees++; //increment frees and reads by 1
        performance -> reads++;

    }

}

struct Node **next( struct Performance *performance, struct Node **list_ptr ){

    if (*list_ptr == NULL) {

        fprintf(stderr, "%s\n", "ERROR: EMPTY LIST");
        exit(0);

    } else {

        //this code is run if the list is not empty (i.e head pointer is not NULL)
        performance -> reads++; //increment reads by 1

        return(&((*list_ptr) -> next));

    }

}

int isEmpty( struct Performance *performance, struct Node **list_ptr ){

    if (*list_ptr == NULL) {

        return(1); //case when list is empty

    } else {

        return(0); //case when list is not empty

    }

}

void freeList( struct Performance *performance, struct Node **list_ptr){

    char temp = 0; //variable to store values retrieved from list

    while (isEmpty(performance, list_ptr) != 1) {

        /*
         * The variable temp does absolutely nothing except for taking the value that pop() retreives from each node.
         * I have assumed that the size of the data is a char because it makes pop() read fewer bytes.
         * The width value does not matter in this situation because we are doing nothing with the data,
         * our only goal is to free it
        */
        pop(performance, list_ptr, &temp, sizeof(char));

    }

}

void readItem( struct Performance *performance, struct Node **list_ptr, unsigned int index, void *dest, unsigned int width ){

    struct Node **tempPtr = list_ptr; //done so list_ptr value doesn't change

    int i = 0;

    while (i != index) {

        //when this loop ends, tempPtr will be pointing at the appropriate node for data to be read
        i++;
        tempPtr = next(performance, tempPtr); //moves tempPtr to the next node

    }

    readHead(performance, tempPtr, dest, width);

}

void appendItem( struct Performance *performance, struct Node**list_ptr, void *src, unsigned int width ){

    struct Node **tempPtr = list_ptr; //done so list_ptr value doesn't change

    while (isEmpty(performance, tempPtr) != 1) {

        //by the end of this loop, tempPtr will point at the tail of the list
        tempPtr = next(performance, tempPtr);

    }

    push(performance, tempPtr, src, width);

}

void insertItem( struct Performance *performance, struct Node **list_ptr, unsigned int index, void *src, unsigned int width ){

    struct Node **tempPtr = list_ptr; //done so list_ptr value doesn't change

    int i = 0;

    while (i != index) {

        //when this loop ends, tempPtr will be pointing at the appropriate node for insertion
        i++;
        tempPtr = next(performance, tempPtr); //moves tempPtr to the next node

    }

    push(performance, tempPtr, src, width);

}

void prependItem( struct Performance *performance, struct Node **list_ptr, void *src, unsigned int width ){

    insertItem(performance, list_ptr, 0, src, width);

}

void deleteItem( struct Performance *performance, struct Node **list_ptr, unsigned int index ){

    struct Node **tempPtr = list_ptr; //done so list_ptr value doesn't change

    char temp = 0; //variable to store values retrieved from list

    int i = 0;

    while (i != index) {

        //when this loop ends, tempPtr will be pointing at the appropriate node for insertion
        i++;
        tempPtr = next(performance, tempPtr); //moves tempPtr to the next node

    }

    pop(performance, tempPtr, &temp, sizeof(char)); //see comment in freeList() for why I used chars here

}

int findItem( struct Performance *performance, struct Node **list_ptr, int (*compar)(const void *, const void *), void *target, unsigned int width ){

    struct Node **tempPtr = list_ptr; //done so list_ptr value doesn't change

    unsigned char *data = malloc(width); //makes a char array large enough to store data in node

    int i = 0;
    int defaultIndex = -1;

    while (isEmpty(performance, tempPtr) != 1) {

        readHead(performance, tempPtr, data, width); //puts the value of the head node into the data char array 

        if (compar(target, data) == 0){

            //index gets returned if a match is found
            free(data);
            return(i);

        }

        tempPtr = next(performance, tempPtr); //moves tempPtr to the next node
        i++;

    }

    free(data);
    return(defaultIndex);

}