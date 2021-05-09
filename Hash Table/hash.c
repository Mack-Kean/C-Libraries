#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

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

struct HashTable *createTable( struct Performance *performance, unsigned int capacity, int (*hash)(void *, int ), int (*compar)(const void *, const void*) ){

    struct HashTable *new = (struct HashTable*) malloc(sizeof(struct HashTable));
    
    int i;

    if (new == NULL) {

        fprintf(stderr, "%s\n", "ERROR: FAILED TO ALLOCATE MEMORY");
        exit(0);

    } else {

        new -> data = malloc(sizeof(void *) * capacity); //allocate capacity many pointers

        if (new -> data == NULL) {

            fprintf(stderr, "%s\n", "ERROR: FAILED TO ALLOCATE MEMORY");
            exit(0);

        } else {

            //this code will only run if all memory is allocated successfully
            new -> capacity = capacity;
            new -> hash = hash;
            new -> compar = compar;
            new -> nel = 0; //number of elements is zero

            for (i = 0; i < capacity; i++) {

                //loop to set all data pointers in array to null
                new -> data[i] = NULL;
                performance -> reads++; //will end up incrementing reads by capacity (one for each pointer)

            }

            performance -> mallocs++; //increment mallocs by 1

        }

    }

    return(new);

}

void addElement( struct Performance *performance, struct HashTable *table, void *src ){

    int index; //for linear probing

    if (table -> nel == table -> capacity) {

        fprintf(stderr, "%s\n", "ERROR: TABLE IS FULL");
        exit(0);

    } else {

        index = table -> hash(src, table -> capacity); //calculates initial index

        if (table -> data[index] == NULL) {

            //checks to see if the initial index is correct
            table -> data[index] = src;

        } else {

            //this else statement is where the actual linear probing happens and doesn't run if there are no collisions

            while (table -> data[index] != NULL){

                if (index == table -> capacity - 1){

                    index = 0; //loops back around to other end of the table

                } else {

                    index++; //increments the index value

                }

                performance -> reads++; // increment reads each time index is incremented/changed

            }

            //once the previous loop ends we should be at the correct index
            table -> data[index] = src;

        }

        table -> nel++; //increment number of elements by 1
        performance -> writes++; //increment writes by 1

    }

}

int getIdx( struct Performance *performance, struct HashTable *table, void *src ){

    int index = table -> hash(src, table -> capacity);
    int originalIndex = index;
    int result = -1; //stores the result of the compar functions
    
    do {

        if (table -> data[index] != NULL) {

            //checks to see if value is null before comparison
            result = table -> compar(src, table -> data[index]);
            performance -> reads++; //run every time a compar is run

        }

        if (result == 0) {

            return(index); //case when value is found

        } else if (index == table -> capacity - 1){

            index = 0; //when index reaches the end of the array it will loop around to the beginning

        }

        index++;

    } while (index != originalIndex);

    return(-1);
    
}

void freeTable( struct Performance *performance, struct HashTable *table ){

    free(table -> data); //free data in structure
    free(table); //free actual structure

    performance -> frees++; //increment frees by 1

}

void *getElement( struct Performance *performance, struct HashTable *table, void *src ){

    int index = getIdx(performance, table, src);

    if (index == -1){
        
        return(NULL); //for when src is not in the HashTable

    } else {
        
        return(table -> data[index]); //returns pointer 

    }

}

void removeElement( struct Performance *performance, struct HashTable *table, void *target) {

    int index = getIdx(performance, table, target);

    if (index != -1) {

        table -> data[index] = NULL; //set pointer to NULL
        table -> nel--; //decrement number of elements

    }

    performance -> writes++; //increment writes by 1

}

int hashAccuracy( struct HashTable *table ) {

    int individualDifference = 0;
    int differenceSum = 0;
    int i, hashValue;

    for (i = 0; i < table -> capacity; i++) {

        if (table -> data[i] != NULL) {

            //if statement ensures that NULL pointers are not put into the has function
            hashValue = table -> hash(table -> data[i], table -> capacity);
            individualDifference = i - hashValue; //difference between index and hashValue

            if (i < hashValue) {
                
                //if the index is less that the hash value (looped back to 0 while probing)
                individualDifference = (table -> capacity - hashValue) + i;

            }

            differenceSum += individualDifference; //adds the difference calculated in this loop iteration to the sum

        }  

    }

    return(differenceSum); //returns the sum of all the differences

}

void rehash( struct HashTable *table ) {

    int i, j, hashValue, swapped;

    for (i = 0; i < table -> capacity; i++) {

        if (table -> data[i] != NULL) {

            //this code will only run if the pointer in data is not null at index i
            hashValue = table -> hash(table -> data[i], table -> capacity);

            if (hashValue < i) {

                for (j = hashValue; j < i; j++) {

                    //loop checks every index between the hash value and the current index
                    if (table -> data[j] == NULL) {

                        //case where current item in the list and null value closer to hash value are swapped
                        table -> data[j] = table -> data[i];
                        table -> data[i] = NULL;
                        j = i; //ends loop once the closest option is found

                    }

                }

            } else if (i < hashValue) { 

                swapped = 0; //used as boolean to see whether or not swap has occured

                for (j = hashValue; j < table -> capacity; j++) {

                    //loop checks every index between the hash value and capacity of the table
                    if (table -> data[j] == NULL) {

                        //case where current item in the list and null value closer to hash value are swapped
                        table -> data[j] = table -> data[i];
                        table -> data[i] = NULL;
                        j = table -> capacity; //ends loop once the closest option is found
                        swapped = 1; //indicates that value has already swapped and second loop is not needed

                    }

                }

                if (swapped == 0) {

                    //only run the second loop if a swap has not already been made
                    for (j = 0; j < i; j++) {
                        
                        //loop checks every index between 0 and the current index
                        if (table -> data[j] == NULL) {

                            //case where current item in the list and null value closer to hash value are swapped
                            table -> data[j] = table -> data[i];
                            table -> data[i] = NULL;
                            j = i; //ends loop once the closest option is found

                        }

                    }

                }

            }

        }

    }

}