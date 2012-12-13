/*
**  Port Borland random() and randomize() functions to other compilers
*/

#ifndef BC_RAND__H
#define BC_RAND__H

#include <stdlib.h>           /* For RAND_MAX, NULL, rand() and srand() */
#include <time.h>             /* For time()                             */

#ifndef __TURBOC__
 #define random(num) (int)(((rand())*(long)(num))/(((long)RAND_MAX)+1))
 #define randomize() srand((unsigned)time(NULL)|1)
#endif

#endif /* BC_RAND__H */
