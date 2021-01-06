#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"

struct Performance *newPerformance(){

    struct Performance *new = (struct Performance*) malloc(sizeof(struct Performance));

    if (new == NULL){

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

struct Array *newArray( struct Performance *performance, unsigned int width, unsigned int capacity ){

    struct Array *new = (struct Array*) malloc(sizeof(struct Array));
    if (new == NULL){

        fprintf(stderr, "%s\n", "ERROR: FAILED TO ALLOCATE MEMORY");
        exit(0);
        
    } else {

        //allocates memory for the data pointer in the array struct
        //only works if memory for the structure was allocated successfully
        new -> data = malloc(width * capacity);

    }

    if (new -> data == NULL){

        fprintf(stderr, "%s\n", "ERROR: FAILED TO ALLOCATE MEMORY");
        exit(0);

    } else {

        //only initialize these variables if the malloc()  for *data does not fail
        new -> nel = 0;
        new -> width = width;
        new -> capacity = capacity;

        performance -> mallocs++; //imcrement the mallocs by 1

    }

    return(new);

}

void readItem( struct Performance *performance, struct Array *array, unsigned int index, void *dest ){

    int offset = 0;

    if (index >= array -> nel){

        fprintf(stderr, "%s\n", "ERROR: FAILED TO READ ITEM");
        exit(0);

    } else {

        //this code only runs if the index is valid
        offset = index * (array -> width);
        memcpy(dest, ((char *)(array -> data)) + offset, array -> width);

        performance -> reads++; //increment the reads by 1

    }

}

void writeItem( struct Performance *performance, struct Array *array, unsigned int index, void *src ){

    int offset = 0;

    if (index > array -> nel || index >= array -> capacity){

        fprintf(stderr, "%s\n", "ERROR: FAILED TO WRITE ITEM");
        exit(0);

    } else {

        //this code only runs if the index is valid
        if (index == array -> nel){

            //the nel is incremented whenever a brand new element is being added
            array -> nel++;

        }

        offset = index * (array -> width);
        memcpy(((char *)(array -> data)) + offset, src, array -> width);

        performance -> writes++; //increment the writes by 1

    }

}

void contract( struct Performance *performance, struct Array *array ){

    if (array -> nel == 0){

        fprintf(stderr, "%s\n", "ERROR: EMPTY ARRAY");
        exit(0);

    } else {

        //this code only runs if the array currently has 1 or more elements
        array -> nel--; //decrement number of elements

    }

}

void freeArray( struct Performance *performance, struct Array *array ){

    //free entire array structure
    free(array -> data);
    free(array);

    performance -> frees++; //increment the frees by 1

}

void appendItem( struct Performance *performance, struct Array *array, void *src ){

    writeItem(performance, array, array -> nel, src);

}

void insertItem( struct Performance *performance, struct Array *array, unsigned int index, void *src ){

    int i = 0;

    char *temp = (char *) malloc(array -> width); //allocates enough chars to fit one item of the array

    for (i = array -> nel - 1; i >= index && i >= 0; i--){
        
        //moves each item greater than the index one position greater in the array
        readItem(performance, array, i, temp);
        writeItem(performance, array, i + 1, temp); //the write will fail if the end of the array is reached

    }

    writeItem(performance, array, index, src); //actually inserts the new item here

    free(temp);

}

void prependItem( struct Performance *performance, struct Array *array, void *src ){
    
    //calls insertItem() function to insert at index 0 (start of the array)
    insertItem(performance, array, 0, src);

}

void deleteItem( struct Performance *performance, struct Array *array, unsigned int index ){

    int i = 0;

    char *temp = (char *) malloc(array -> width); //allocates enough chars to fit one item of the array

    for (i = index + 1; i < array -> nel; i++){

        //moves each item greater than the index one position less in the array
        readItem(performance, array, i, temp);
        writeItem(performance, array, i - 1, temp);  

    }

    contract(performance, array); //actually shrinks the array

    free(temp);

}

int findItem( struct Performance *performance, struct Array *array, int (*compar)(const void *, const void *), void *target ){

    int index = -1; //value to be returned if match not found
    int i = 0;

    char *temp = (char *) malloc(array -> width); //allocates enough chars to fit one item of the array

    for (i = 0; i < array -> nel; i++){

        readItem(performance, array, i, temp);

        if (compar(target, temp) == 0){
            
            //the loop is terminated once this if statement runs
            index = i;
            i = array -> nel;

        }

    }

    free(temp);

    return(index); //index of either the default -1 or of the actual element in the array is returned

}

int searchItem( struct Performance *performance, struct Array *array, int (*compar)(const void *, const void *), void *target ){

    int upperLimit = array -> nel - 1; //highest index in array
    int lowerLimit = 0;
    int middle = 0;

    char *tempMiddle = (char *) malloc(array -> width); //allocates enough chars to fit one item of the array

    while(upperLimit >= lowerLimit){

        middle = (upperLimit + lowerLimit) / 2;
        readItem(performance, array, middle, tempMiddle); //reads value at the middle index

        if (compar(tempMiddle, target) < 0){
            
            //case where target is greater than array[middle]
            lowerLimit = middle + 1;

        } else if (compar(tempMiddle, target) > 0){
            
            //case where target is less than array[middle]
            upperLimit = middle - 1;

        } else {
    
            //case where target is equal to array[middle]
            free(tempMiddle);
            return(middle); //return statement allows the program to end the loop as soon as the target is found

        }

    }

    free(tempMiddle);

    return(-1); //default value if the target is not found
    
}

