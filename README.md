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
