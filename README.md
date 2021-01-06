# C-Libraries
---

### Data Manipulation
**Library Description:** used to manipulate bits and bytes of data, move them around, and convert them to other data types.
* > `getBytes( unsigned char dest[], int bytes, void *src, int reverse )` this function copies 'bytes' number of bytes from the memory location pointed to by 'src' into an unsigned char array (array of bytes) 'dest'.  The reverse parameter can be set to 0 to read bytes in order and 1 to read bytes in reverse order.  This is helpful for dealing with the endianess of the data as little endian systems may store bytes in the opposite order as desired.

* > `getbits( char dest[], int bytes, void *src, int start, int end )` this function creates a bitstring for 'bytes' number of bytes starting at the memory location 'src' and returns a substring of this bitstring using the 'start' and 'end' binary indices.  Substring will be returned to 'dest'.

* > `bits2ull( char *bits )` this function takes the bitstring stored in 'bits' and converts it to an unsigned long long.

* > `bits2ll( char *bits )` this function takes the bitstring stored in 'bits' and converts it to a long long

* > `spff( char *sign, char *exponent, char *significand, float *src )` this function uses `getbits` to spit apart the single precision floating point number stored in 'src' into its sign bit, exponent bits, and significand bits.  These are return through parameters 'sign', 'exponent', and 'significand' respectively.

* > `dpff( char *sign, char *exponent, char *significand, double *src )` this function is identical to 'spff' but splits apart a double precision floating point number instead of a single precision.

### Array
**Library Description:** used to simulate what the C language is doing when an array is declared along with some added features such as a struct member to keep track of array size.  The library uses a structure of type Array which is defined in `array.h` 

* > `newPerformance()` this function creates a struct of type Performance which is used to track the reads, writes, mallocs, and frees used in the other library functions

* > `newArray( struct Performance *performance, unsigned int width, unsigned int capacity )` this function creates and allocates memory for a new struct of type Array. 'width' represents the size of each array element in bytes and 'capacity' represents the number of 'width' sized elements that will be allocated and pointed to by 'Array -> data'.

* > `readItem( struct Performance *performance, struct Array *array, unsigned int index, void *dest )` this function reads an element from struct parameter 'array' member 'array -> data'  at index 'index' and copies it to the memory location pointed to by 'dest'.

* > `writeItem( struct Performance *performance, struct Array *array, unsigned int index, void *src )` this function reads data from the memory location pointed to by 'src' and copies this data to the Array sturct 'array' member 'array -> data' at index 'index'.  This function also increments the number of elements, 'array -> nel' if a new element is added.

* > `contract( struct Performance *performance, struct Array *array )` this function decrements the number of elements in the 'array' structure ('array -> nel).  Used when elements are removed from a structure of type Array.

* > `freeArray( struct Performance *performance, struct Array *array )` this function frees the Array struct pointed to by parameter 'array'.

* > `appendItem( struct Performance *performance, struct Array *array, void *src )` this function calls `writeItem` to write the data pointed to by 'src' to the last free index in 'array -> data'. Will cause an error if the array is at capacity.

* > `insertItem( struct Performance *performance, struct Array *array, unsigned int index, void *src )` this function inserts an item into 'array -> data' by using the `readItem` and `writeItem` functions to shift existing elements at an index greater than or equal to 'index' one index greater.  this creates a space at index 'index' that is overwritten with the data pointed to by 'src'.

* > `prependItem( struct Performance *performance, struct Array *array, void *src )` this function inserts an item to 'array -> data' at index 0.

* > `deleteItem( struct Performance *performance, struct Array *array, unsigned int index )` this function deletes an item in 'array -> data' at index 'index' and shifts all array elements at with an index greater than 'index' to a position one less than their current position.  This overwrites the deleted element.

* > `findItem( struct Performance *performance, struct Array *array, int (*compar)(const void *, const void *), void *target )` this function uses any 'compar' function pointer to allow it to be used with any data type.  This function pointer points to a function that indicates whether the two pieces of data passed to it are equivalent.  The main function uses `compar` to compare the data pointed to by 'target' and each element in 'array -> data'.  If the `compar` function returns zero the array element is equivalent to the target and the index is returned.  if no match is found -1 will be returned.

* > `searchItem( struct Performance *performance, struct Array *array, int (*compar)(const void *, const void *), void *target )` this function implents a binary search algorithm using any 'compar' function pointer to allow it to be used with any data type.  This function will only work correctly if 'array -> data' has previously been sorted.  The function assumes that `compar(arrayElement, target)` will return a value less than 0 if 'target' > 'arrayElement', greater than 0 if 'target' < 'arrayElement', and equal to 0 if 'target == arrayElement'.  Once a match to 'target' is found the function returns the index of the match.  If no match is found the function returns -1.
