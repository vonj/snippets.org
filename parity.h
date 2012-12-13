/*
**  PARITY.H
*/

typedef enum {Even_, Odd_} Parity_T;

unsigned parity32(unsigned long x, Parity_T even);
unsigned parity16(unsigned short x, Parity_T even);
unsigned parity8(unsigned char x, Parity_T even);

union longlong {
      unsigned long lo;
      unsigned long hi;
};

unsigned parity64(void *x, Parity_T even);
