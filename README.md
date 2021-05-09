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
**Library Description:** used to simulate what the C language is doing when an array is declared along with some added features such as a struct member to keep track of array size.  The library uses a structure of type Array to implement the customized array data structure and a structure of type Performance to keep track of operations performed on memory.  Both struct types are defined in `array.h`  

* > `newPerformance()` this function creates a struct of type Performance which is used to track the reads, writes, mallocs, and frees used in the other library functions

* > `newArray( struct Performance *performance, unsigned int width, unsigned int capacity )` this function creates and allocates memory for a new struct of type Array. 'width' represents the size of each array element in bytes and 'capacity' represents the number of 'width' sized elements that will be allocated and pointed to by 'Array -> data'.

* > `readItem( struct Performance *performance, struct Array *array, unsigned int index, void *dest )` this function reads an element from struct parameter 'array' member 'array -> data'  at index 'index' and copies it to the memory location pointed to by 'dest'.

* > `writeItem( struct Performance *performance, struct Array *array, unsigned int index, void *src )` this function reads data from the memory location pointed to by 'src' and copies this data to the Array struct 'array' member 'array -> data' at index 'index'.  This function also increments the number of elements, 'array -> nel' if a new element is added.

* > `contract( struct Performance *performance, struct Array *array )` this function decrements the number of elements in the 'array' structure ('array -> nel).  Used when elements are removed from a structure of type Array.

* > `freeArray( struct Performance *performance, struct Array *array )` this function frees the Array struct pointed to by parameter 'array'.

* > `appendItem( struct Performance *performance, struct Array *array, void *src )` this function calls `writeItem` to write the data pointed to by 'src' to the last free index in 'array -> data'. Will cause an error if the array is at capacity.

* > `insertItem( struct Performance *performance, struct Array *array, unsigned int index, void *src )` this function inserts an item into 'array -> data' by using the `readItem` and `writeItem` functions to shift existing elements at an index greater than or equal to 'index' one index greater.  this creates a space at index 'index' that is overwritten with the data pointed to by 'src'.

* > `prependItem( struct Performance *performance, struct Array *array, void *src )` this function inserts an item to 'array -> data' at index 0.

* > `deleteItem( struct Performance *performance, struct Array *array, unsigned int index )` this function deletes an item in 'array -> data' at index 'index' and shifts all array elements at with an index greater than 'index' to a position one less than their current position.  This overwrites the deleted element.

* > `findItem( struct Performance *performance, struct Array *array, int (*compar)(const void *, const void *), void *target )` this function uses any 'compar' function pointer to allow it to be used with any data type.  This function pointer points to a function that indicates whether the two pieces of data passed to it are equivalent.  The main function uses `compar` to compare the data pointed to by 'target' and each element in 'array -> data'.  If the `compar` function returns zero the array element is equivalent to the target and the index is returned.  if no match is found -1 will be returned.

* > `searchItem( struct Performance *performance, struct Array *array, int (*compar)(const void *, const void *), void *target )` this function implements a binary search algorithm using any 'compar' function pointer to allow it to be used with any data type.  This function will only work correctly if 'array -> data' has previously been sorted.  The function assumes that `compar(arrayElement, target)` will return a value less than 0 if 'target' > 'arrayElement', greater than 0 if 'target' < 'arrayElement', and equal to 0 if 'target == arrayElement'.  Once a match to 'target' is found the function returns the index of the match.  If no match is found the function returns -1.

### List
**Library Description:** Used to implement a linked list data structure.  The library uses a structure of type Node to act as nodes in the list and a structure of type Performance to keep track of operations performed on memory.  Both struct types are defined in `list.h`

* > `newPerformance()` this function creates a struct of type Performance which is used to track the reads, writes, mallocs, and frees used in the other library functions

* > `push( struct Performance *performance, struct Node **list_ptr, void *src, unsigned int width )` this function creates a new Node and adds it to the front of the list pointed to by 'list_ptr' to become the new list head.  The data pointed to by 'src' is copied to 'newNode -> data'.  'width' indicates the size of the node data in bytes (should be the size of the data at 'src').

* > `readHead( struct Performance *performance, struct Node **list_ptr, void *dest, unsigned int width )` this function reads 'width' bytes of data from the head Node of the list pointed to by 'list_ptr' and copies them to the memory location pointed to by 'dest'.

* > `pop( struct Performance *performance, struct Node **list_ptr, void *dest, unsigned int width )` this function removes the head node from the list pointed to by 'list_ptr' and copies 'width' bytes of it's data to the memory location pointed to by 'dest'.  The memory of the removed node is freed.

* > `next( struct Performance *performance, struct Node **list_ptr )` this function returns a pointer to a pointer of the next node in the list pointed to by 'list_ptr'.  For example, if 'list_ptr' is pointing at the head of the list this function will return a pointer to 'head -> next' (see header struct).

* > `isEmpty( struct Performance *performance, struct Node **list_ptr )` this function returns a 1 if the list pointed to by 'list_ptr' is empty (ie null) and returns a 0 if the list is not empty.

* > `freeList( struct Performance *performance, struct Node **list_ptr)` this function continuously calls the `pop` function and does nothing with the data returned through the `pop` parameters.  This frees the entire list pointed to by 'list_ptr'.

* > `readItem( struct Performance *performance, struct Node **list_ptr, unsigned int index, void *dest, unsigned int width )` this function copies 'width' bytes of data from the node in the list pointed to by 'list_ptr' at index 'index'.

* > `appendItem( struct Performance *performance, struct Node**list_ptr, void *src, unsigned int width )` this function adds a Node to the tail of the list pointed to by 'list_ptr'.  The 'width' bytes of data used to create the new tail node are pointed to by 'src'.

* > `insertItem( struct Performance *performance, struct Node **list_ptr, unsigned int index, void *src, unsigned int width )` this function inserts a Node into the list pointed to by 'list_ptr' at index 'index'. The 'width' bytes of data used to create the new node are pointed to by 'src'.

* > `prependItem( struct Performance *performance, struct Node **list_ptr, void *src, unsigned int width )` this function calls `insertItem` with an 'index' of 0.  This adds a node the head of the list and is equivalent to using `push`.

* > `deleteItem( struct Performance *performance, struct Node **list_ptr, unsigned int index )` this function removes a node from the list pointed to by 'list_ptr' at index 'index'.

* > `findItem( struct Performance *performance, struct Node **list_ptr, int (*compar)(const void *, const void *), void *target, unsigned int width )` this function uses any 'compar' function pointer to allow it to be used with any data type.  This function pointer points to a function that indicates whether the two pieces of data passed to it are equivalent. The main function uses `compar` to compare the data pointed to by 'target' with the 'width' bytes of data stored in 'node -> data' where 'node' is any node in the list pointed to by 'list_ptr'.  This function returns the appropriate index in the list where a match is found or -1 if no match is found.

### Binary Tree
**Library Description:** Used to implement a binary tree data structure. The library uses a structure of type Node to act as nodes in the tree and a structure of type Performance to keep track of operations performed on memory.  Each Node contains 2 pointers, one for data of lesser value (node -> lt) and one for data of greater than or equal value (node -> gte).  Both struct types are defined in `binaryTree.h`

* > `newPerformance()` this function creates a struct of type Performance which is used to track the reads, writes, mallocs, and frees used in the other library functions

* > `attachNode( struct Performance *performance, struct Node **node_ptr, void *src, unsigned int width )` this function attaches a node to one of the leaves of the tree. 'node_ptr' points to a pointer to a leaf node (at the bottom of the tree).  The data pointer in the new node is allocated 'width' bytes of memory which is populated with the data pointed to by 'src'.

* > `comparNode( struct Performance *performance, struct Node **node_ptr, int (*compar)(const void *, const void *), void *target )` this function compares the data stored in the node pointed to by '*node_ptr' with the data pointed to by 'target' using the function pointed to by 'compar'.  `compar(target, (*node_ptr) -> data)` should return an integer that is greater than 0 when the data pointed to by 'target' is greater than the node data, an integer less than 0 when the data pointed to by 'target' is less than the node data, and 0 when both are equal.  The result of this compar function is what is returned.

* > `next( struct Performance *performance, struct Node **node_ptr, int direction )` this function returns a pointer to the next desired node in the tree. 'direction specifies whether to return a pointer to a pointer to the 'lt' (less than) node or the 'gte' (greater than or equal) node.  A direction value of 0 or greater indicates that the gte branch will be followed, otherwise the lt branch will be followed.  'node_ptr' is a pointer to a pointer to the desired starting node in the tree.

* > `readNode( struct Performance *performance, struct Node **node_ptr, void *dest, unsigned int width )` this function copies 'width' bytes of data from the node pointed to by '*node_ptr' to the memory location pointed to by 'dest'.

* > `detachNode(struct Performance *performance, struct Node **node_ptr )` this function detaches the node pointed to by '*node_ptr' from the tree.  This function should only be called on leaves of the tree that have no children.  Otherwise major memory leaks may result from improper use.

* > `isEmpty( struct Performance *performance, struct Node **node_ptr )` this function treats '*node_ptr' as the pointer to a root node of a tree/subtree and returns 1 if this tree/subtree is empty (no children) and 0 otherwise.

* > `addItem( struct Performance *performance, struct Node **node_ptr, int (*compar)(const void *, const void *), void *src, unsigned int width )` this function uses the same 'compar' function as outlined in the `comparNode` description.  It treats '*node_ptr' as the pointer to a root node of a tree/subtree and adds a node containing 'width' bytes of the data pointed to by 'src' to the appropriate parent.  This parent node is determined using the `comparNode` function to determine which branches to take until a node with a null gte/lt pointer is found.

* > `freeTree( struct Performance *performance, struct Node **node_ptr )`  this function treats '*node_ptr' as the pointer to a root node of a tree/subtree and recursively frees the entire tree.

* > `searchItem( struct Performance *performance, struct Node **node_ptr, int (*compar)(const void *, const void *), void *target, unsigned int width )` this function uses the same 'compar' function as outlined in the `comparNode` description. It treats '*node_ptr' as the pointer to a root node of a tree/subtree and searches this tree to try and find a match between the 'width' bytes of data stored in each node and the 'width' bytes of data pointed to by 'target'.  If a match is found the function will return 1 and if no match is found the function will return 0.

### Hash Table
**Library Description:**