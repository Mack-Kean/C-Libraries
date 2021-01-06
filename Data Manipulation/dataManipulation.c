#include <stdlib.h>
#include <string.h>

void getbytes( unsigned char dest[], int bytes, void *src, int reverse ){

    int i;

    //non-reverse case
    if (reverse == 0){

        for (i = 0; i < bytes; i++){
            dest[i] = *((unsigned char *)(src) + i);
        }

    }

    //reverse case
    if (reverse != 0){

        for (i = bytes - 1; i >= 0; i--){
            dest[bytes - i - 1] = *((unsigned char *)(src) + i);
        }

    }

}

void getbits( char dest[], int bytes, void *src, int start, int end ){
    
    int numBits = start - end;
    int numTotalBits = bytes * 8;
    int startRemainder = start % 8; //gives bit position for the first byte
    int startShift = (startRemainder - 7) * (-1); //converts startRemainder into array index that can be used for arrayStartIndex
    int startQuotient = start / 8;
    int startByte = bytes - startQuotient - 1; // index of start byte in src
    int arrayStartIndex = 8 * startByte + startShift; //index of starting bit in binaryArray
    int i = 0;
    int j = 0;
    int currentIndex = 0;

    /* temp variable below is used to retrieve the lestmost bit in each byte */
    unsigned char temp = 128; // 10000000 in binary

    unsigned char *binaryArray = NULL;

    //copy src into separate array
    unsigned char *charPtr = malloc(sizeof(unsigned char) * bytes);
    getbytes(charPtr, bytes, src, 0); //done easily using getbytes() function

    //make array of binary
    binaryArray = malloc(sizeof(unsigned char) * numTotalBits);

    //populate binary array
    for (i = 0; i < bytes; i++){

        for (j = 0; j < 8; j++){

            //this loop goes through every bit in each byte
            if ((charPtr[i] & temp) == temp){

                binaryArray[currentIndex] = '1';

            } else {

                binaryArray[currentIndex] = '0';

            }

            charPtr[i] = charPtr[i] << 1;
            currentIndex++;

        }

    }

    //send specific range of bits to destination
    for (i = 0; i < numBits; i++){

        dest[i] = binaryArray[arrayStartIndex + i];

    }
    
    dest[numBits] = '\0'; //add null terminating character to the end of the string

    //free memory
    free(charPtr);
    free(binaryArray);
    
}

unsigned long long bits2ull( char *bits ){

    unsigned long long num = 0;
    unsigned long long powerOfTwo = 0; //used in loop that stores the power of 2 of a specific bit

    int numBits = strlen(bits);
    int i = 0;
    int j = 0;

    //do calculation using binary arithmetic
    for (i = 0; i < numBits; i++){
        
        powerOfTwo = 1;

        if (bits[i] == '1'){

            for (j = 0; j < numBits - i - 1; j++){

                powerOfTwo *= 2;

            }

            num += powerOfTwo;

        }

    }

    return(num);

}

long long bits2ll( char *bits ){

    long long num = 0;
    long long powerOfTwo = 0; //used in loop that stores the power of 2 of a specific bit

    int numBits = strlen(bits);
    int i = 0;
    int j = 0;

    //do calculation using binary arithmetic
    for (i = 0; i < numBits; i++){
        
        if (numBits == 1){

            powerOfTwo = -1;

        } else {

            powerOfTwo = 1;
            
        }

        if (bits[i] == '1'){

            for (j = 0; j < numBits - i - 1; j++){

                if (i == 0 && j == 0){

                    powerOfTwo *= -2;

                } else {

                    powerOfTwo *= 2;

                }

            }

            num += powerOfTwo;

        }

    }

    return(num);
}

void spff( char *sign, char *exponent, char *significand, float *src ){

    unsigned char *charPtr = malloc(sizeof(float));
    getbytes(charPtr, sizeof(float), src, 1); //reverses bytes so output will be in proper floating point format

    getbits(sign, sizeof(float), charPtr, 31, 30); //gets the sign bit using getbits()
    getbits(exponent, sizeof(float), charPtr, 30, 22); //gets the exponent portion using getbits()
    getbits(significand, sizeof(float), charPtr, 22, -1); //gets the significand using getbits()

    free(charPtr);

}

void dpff( char *sign, char *exponent, char *significand, double *src ){

    unsigned char *charPtr = malloc(sizeof(double));
    getbytes(charPtr, sizeof(double), src, 1); //reverses bytes so output will be in proper floating point format

    getbits(sign, sizeof(double), charPtr, 63, 62); //gets the sign bit using getbits()
    getbits(exponent, sizeof(double), charPtr, 62, 51); //gets the exponent portion using getbits()
    getbits(significand, sizeof(double), charPtr, 51, -1); //gets the significand using getbits()

    free(charPtr);

}