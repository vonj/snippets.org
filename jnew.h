/*--------------------------------------------------------------*/
/* Debugging extension by Jeff Dunlop                           */
/* Copyright 1992-1993, DB/Soft Publishing Co.                  */
/* License is hereby granted for use of JMalloc as a debugging  */
/* aid in any program.  JMalloc may not be sold or distributed  */
/* as or part of any for-profit debugging program or library    */
/* nor may it be included in any for-profit library that offers */
/* debugging features.  Any redistribution of JMalloc source    */
/* must include this copyright notice.                          */
/*--------------------------------------------------------------*/

#include <stdlib.h>

#ifndef jnew_h
#define jnew_h

#ifdef DBUG

#define NEW(a) (db_set(__FILE__, __LINE__), new a)
#define DELETE(a) (db_set(__FILE__, __LINE__), delete a)

void *operator new(size_t n);
void operator delete(void *p);
void db_set(char *file, int line);

#else

#define NEW(a) new a
#define DELETE(a) delete a

#endif /* DBUG */
#endif /* jnew_h */
