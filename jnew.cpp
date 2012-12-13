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

/*--------------------------------------------------------------*/
/* Usage:                                                       */
/*  If you want line number information available on your       */
/*  allocated memory blocks, your new and delete calls have to  */
/*  be replaced with NEW() and DELETE() macro calls.  This is   */
/*  not absolutely required but finding where an orphaned block */
/*  was allocated is much easier if the line information can be */
/*  saved.  Otherwise, JNew is a drop-in replacement for        */
/*  operators new and delete.                                   */
/*                                                              */
/*  If you compile jnew.cpp and jmalloc.c into .obj files, your */
/*  linker will find jnew's operator new before that in the     */
/*  standard library.  If you store jnew.obj and jmalloc.obj in */
/*  a library file, that library must appear in your list       */
/*  before the standard library or the linker will link in the  */
/*  standard library's operator new.  Define DBUG to turn on    */
/*  line numbers.  Undefine DBUG and remove the jnew binaries   */
/*  from your linker list to disable.                           */
/*                                                              */
/*  There is a bit of memory overhead involved in having        */
/*  diagnostic information stored for every allocated block.    */
/*  Depending on your compiler, this can run from 30 bytes per  */
/*  block with Borland C++ to 50+ bytes per block for C Set/2.  */
/*  If your compiler supports duplicate string merging and your */
/*  program won't break using it, file information stored in    */
/*  the static data area will be compressed.                    */
/*  If memory is tight and you want to use jnew on a            */
/*  file-by-file basis you will need to redefine the NEW and    */
/*  DELETE macros to use a different operator and rename        */
/*  operators new and delete in jnew.cpp.  Performance doesn't  */
/*  take much of a hit unless you have a LOT of small allocated */
/*  blocks floating around most of the time.  Then, searching   */
/*  the diagnostic list can become significant.                 */
/*--------------------------------------------------------------*/

#define DBUG

#include <stdlib.h>
#include <string.h>

#include <new.h>
#include "extkword.h"
#include "jmalloc.h"
#include "jnew.h"

static char *db_file;
static int db_line;

#ifdef _MSC_VER
 typedef int (CDECL *NEW_HANDLER)(size_t);
 #define set_new_handler _set_new_handler
#else
 typedef void (*NEW_HANDLER)(void);
#endif

void db_set(char *file, int line)
{
    if ( db_file )
        free(db_file);
    db_file = strdup(file);
    db_line = line;
}

void *operator new(size_t n)
{
    void *q;

    NEW_HANDLER new_handler = set_new_handler(NULL);
    set_new_handler(new_handler);

    while ( (q = j_malloc(n, db_file, db_line)) == NULL )
    {
        if ( new_handler )
        {
#ifdef _MSC_VER
            new_handler(n);
#else
            new_handler();
#endif
        }
        else
            return NULL;
    }
    return q;
}

void operator delete(void *p)
{
    j_free(p, db_file, db_line);
}

#ifdef TEST

int main(void)
{
    char *n,
         *p,
         *q,
         r[30],
         *s[256] = {NULL};
    int i = 1;

    n = NEW(char[64 * 56]);
    JStrCpy(n + 10, "Test string");
    DBUG_PRINT("alloc", ("r is invalid"));
    JStrCpy(r, "Test");
    q = NEW(char[16]);
    q[16] = 3;

    DBUG_PRINT("alloc", ("p was never allocated"));
    DELETE(p);

    i = 0;

    DBUG_PRINT("alloc", ("Deplete memory"));
    do
    {
        s[i] = NEW(char[32000]);
        i++;
    } while ( s[i - 1] != NULL );

    i = 0;
    while ( s[i] != NULL )
    {
        DELETE(s[i++]);
    }

    DBUG_PRINT("alloc", ("New and q are orphaned blocks"));
    DBUG_PRINT("alloc", ("q has an overrun"));
    JMemcheck(1);
    return 0;
}

#endif /* TEST */
