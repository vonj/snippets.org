                        STORAGE TYPES

        A C language crib sheet from Jeff Galbraith


int x;
 - x is an int.

int *x;
 - x is a pointer to an int.

int **x;
 - x is a pointer to a pointer to an int.

const int x;
 - x is a const int (constant integer).

const int *x;
 - x is a pointer to a const int. The value of x may change, but
   the integer that it points to not be changed. In other words,
   x cannot be used to alter the value to which it points.

int *const x;
 - x is a constant pointer to an int. The value may not change,
   but the integer that it points to may change. In other words,
   x will always point at the same location, but the contents may
   vary.

const int *const x;
 - x is a constant pointer to a constant integer. The value of x
   may not change, and the integer that it points to may not
   change. In other words, x will always point at the same
   location, which cannot be modified via x.

int x[];
 - x is an array of int.

int x[99];
 - x is an array of 99 int's.

int *x[];
 - x is an array of pointers to int.

int (*x)[];
 - x is a pointer to an array of int.

int *(*x)[];
 - x is a pointer to an array of pointers to int.

int F();
 - F is a function returning int.

int *F();
 - F is a function returning a pointer to int.

int (*x)();
 - x is a pointer to a function returning int.

int (*x[99])();
 - x is an array of 99 pointers to functions returning int.

int (*F())();
 - F is a function returning a pointer to a function returning int.

int *(*F())();
 - F is a function returning a pointer to a function returning a
   pointer to an int.

int (*F())[];
 - F is a function returning a pointer to an array of int.

int (*(*F())[])();
 - F is a function returning a pointer to an array of pointers to
   functions returning int.

int *(*(*F())[])();
 - F is a function returning a pointer to an array of pointers to
   functions returning a pointer to int.
