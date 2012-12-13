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

/*-------------------------[ jmalloc.c ]------------------------*/
/*                 drop-in for malloc with diags                */
/*--------------------------------------------------------------*/

/*--------------------------------------------------------------*/
/*---------------------------[ notes ]--------------------------*/
/*--------------------------------------------------------------*/

/*

The J... macros are intended for use on JMalloc'd memory blocks.  The
A... macros are intended for use on auto array blocks and will only
work if the base of the array is passed.  They will not work if the
address of an array element is passed.  All functions are designed to
be nearly drop-in replacements for standard library functions.  The one
case where debugging libraries traditionally fall short is when a block
operation occurs on an automatic array in a position other than at the
head.  This library could conceivably be extended to include a class of
functions that allow you to pass both a block and an offset into that
block so that the block and its size can be checked.  My style of
programming eschews this practice so I haven't had any motivation to so
extend this library.  Note that such an extension would depart slightly
from the standard syntax of the runtime library because of the
additional passed parameter.

JMemcheck(0) may be called at any time for an overwrite check of all
allocated blocks.  JMemcheck(1) additionally checks for orphaned blocks
and should be called just before program shutdown.

All functions are designed to log any detectable errors when the errors
occur, and to report either the line number of the overwrite, or the
line number that the block was allocated on, depending on which is more
useful.  Any blocks that are damaged by non-JMalloc functions will not
be noticed until they are JFree'd and will be tougher to debug.
Regardless of the damage that a function call produces, JMalloc never
departs from the runtime library's behavior.  Its job is to report,
nothing more.  All allocated blocks (except for JCalloc calls) are
dirtied and all blocks are dirtied before they are freed.  If you are
referencing freed memory or not initializing a new block, it will be
very obvious.  All blocks have an extra check byte that is checked when
the block is freed or during JMemcheck.  This doesn't catch random
memory writes, but it does catch overruns.

*/

/*--------------------------------------------------------------*/
/*-----------------------[ header files ]-----------------------*/
/*--------------------------------------------------------------*/

#define DBUG

#if defined __TURBOC__
 #include <alloc.h>
 #include <mem.h>
#else
 #if defined(__ZTC__)
  #include <dos.h>
 #else
  #include <malloc.h>
 #endif
#endif

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jmalloc.h"

/*--------------------------------------------------------------*/
/*--------------------[ local prototypes ]----------------------*/
/*--------------------------------------------------------------*/

static MLINK *JMemOwner(char *Addr);

/*--------------------------------------------------------------*/
/*---------------------[ global variables ]---------------------*/
/*--------------------------------------------------------------*/

MLINK *MalTop;                        /* top of allocation chain */

void db_prn(char *fmt, ...)
{
    va_list p;

    va_start(p, fmt);
    vprintf(fmt, p);
    putchar('\n');
    va_end(p);
}

void *j_deref(void *a, char *file, int line)
{
    if ( a == NULL )
    {
        DBUG_PRINT("alloc", ("Dereferenced NULL pointer - %s: line %d",
            file, line));
    }
    return a;
}

/*-------------------------[ j_malloc ]-------------------------*/
/*            Memory allocator with diagnostics                 */
/*--------------------------------------------------------------*/
/* input:                                                       */
/*      Size = number of bytes to allocate                      */
/* local:                                                       */
/*      CurMal = pointer current mem struct                     */
/*      PrevMal = pointer to previous mem struct                */
/*      AllocAddr = pointer to allocated ram                    */
/* return:                                                      */
/*      address of allocated ram, or NULL on error              */
/* note:                                                        */
/*       use the JMalloc macro                                  */
/*--------------------------------------------------------------*/

static int memc;

void *j_malloc(unsigned Size, char *file, int line)
{
    /* 1. Allocate the memory */
    /* 2. Add to allocation chain */
    /* 3. Fill with non-null */

    MLINK *CurMal = MalTop,
          *PrevMal;

    void *AllocAddr;

    /* Allocate the memory + 1 (for check byte) */

    if ( (AllocAddr = malloc(Size + 1)) == NULL )
    {
        DBUG_PRINT("alloc", ("Allocation failed: %s Line %d", file, line));
        return((void*)0);
    }

    /* Add to the allocation chain */

    PrevMal = CurMal;
    while ( CurMal != NULL )
    {
        PrevMal = CurMal;
        CurMal = CurMal->NextLink;
    }
    if ( (CurMal = malloc(sizeof *CurMal)) == NULL )
    {
        DBUG_PRINT("alloc", ("Control allocation failed: %s Line %d", file,
            line));
        return(AllocAddr);
    }

    /* Deal with bootstrap */
    if ( PrevMal == NULL )
        MalTop = CurMal;
    else
        PrevMal->NextLink = CurMal;
    CurMal->NextLink = NULL;
    CurMal->MAddr = AllocAddr;
    CurMal->MSize = Size;
    CurMal->MLine = line;
    AStrCpy(CurMal->MFile, file);

    memset(AllocAddr, CKBYT, Size + 1);
    memc++;
    return(AllocAddr);
}

/*-------------------------[ j_calloc ]-------------------------*/
/*            Memory allocator with diagnostics                 */
/*--------------------------------------------------------------*/
/* method:                                                      */
/*      - Allocate the memory via JMalloc                       */
/*      - Fill with null                                        */
/*      - Set check-byte                                        */
/* input:                                                       */
/*      Size = number of bytes to allocate                      */
/* local:                                                       */
/*      CurCal = pointer current mem struct                     */
/*      PrevCal = pointer to previous mem struct                */
/*      AllocAddr = pointer to allocated ram                    */
/* return:                                                      */
/*      address of allocated ram, or NULL on error              */
/*  note:                                                       */
/*      Use the JCalloc macro                                   */
/*--------------------------------------------------------------*/

void *j_calloc(unsigned Size, unsigned SizEach, char *file, int line)
{
    char *AllocAddr;

    /* Allocate the memory + 1 (for check byte) */

    /* Do not piss over NULL return - JMalloc handled that */
    if ( (AllocAddr = j_malloc(Size * SizEach, file, line)) != NULL )
    {
        /* Prep allocated ram */
        memset(AllocAddr, 0, Size * SizEach);
        AllocAddr[Size * SizEach] = CKBYT;
    }

    return(AllocAddr);
}

/*-------------------------[ j_realloc ]------------------------*/
/*                     Reallocate memory                        */
/*--------------------------------------------------------------*/
/* input:                                                       */
/*  Addr = block's current address                              */
/*  Size = block's new size                                     */
/* return:                                                      */
/*  Block's new address, or NULL if memory not available.       */
/* note:                                                        */
/*  Use the JRealloc macro.                                     */
/*  If NULL is returned, Addr was also freed                    */
/*--------------------------------------------------------------*/

void *j_realloc(void *Addr, unsigned Size, char *file, int line)
{
    MLINK *CurMal = MalTop;

    void *tmp;

    if ( Addr == NULL )
        return(j_malloc(Size, file, line));
    /* Find the old block in the alloc list */
    while ( CurMal->MAddr != Addr && CurMal != NULL )
        CurMal = CurMal->NextLink;
    if ( CurMal == NULL )
    {
        /* Just call the standard realloc since we don't know anything
         * about this block
         */
        DBUG_PRINT("alloc", ("Realloc attempted on unrecorded block: %s "
            "Line %d", file, line));
        return(realloc(Addr, Size));
    }
    else
    {
        tmp = j_malloc(Size, file, line);
        if ( tmp != NULL )
        {
            memcpy(tmp, Addr, min(CurMal->MSize, Size));
            j_free(Addr, file, line);
        }
        else
        {
            j_free(Addr, file, line);
            return NULL;
        }
    }
    return(tmp);
}

/*-------------------------[ j_free ]---------------------------*/
/*           Memory deallocator with diagnostics                */
/*--------------------------------------------------------------*/
/* input:                                                       */
/*      AllocAddr = pointer to ram to deallocate                */
/* local:                                                       */
/*      CurMal = pointer to current mem struct                  */
/*      PrevMal = pointer to prev mem struct                    */
/* note:                                                        */
/*      This function is designed to be implemented with the    */
/*      macro JFree                                             */
/*--------------------------------------------------------------*/

void j_free(void *AllocAddr, char *file, int line)
{
    /* 1. Find the block in the alloc list */
    /* 2. Check for check-byte overwrite   */
    /* 3. Remove allocation record         */
    /* 3. Free the ram                     */

    MLINK *CurMal = MalTop,
          *PrevMal = MalTop;

    /* Find the block in the alloc list */
    while ( CurMal != NULL && CurMal->MAddr != AllocAddr )
    {
        PrevMal = CurMal;
        CurMal = CurMal->NextLink;
    }
    if ( CurMal == NULL )
    {
        DBUG_PRINT("alloc", ("Freeing an unrecorded block: %s %d", file,
            line));
    }
    else
    {
        /* Check for check-byte overwrite */
        if ( CurMal->MAddr[CurMal->MSize] != CKBYT )
            DBUG_PRINT("alloc", ("Memory overrun detected on block allocated"
            " at %s Line %d", CurMal->MFile, CurMal->MLine));
        memset(AllocAddr, DIRTY, CurMal->MSize + 1);

        if ( CurMal == MalTop )
            MalTop = CurMal->NextLink;
        else
            PrevMal->NextLink = CurMal->NextLink;
        free(CurMal);
    }

    /* Free the ram regardless of validity */
    free(AllocAddr);
    memc--;
    return;
}

/*-------------------------[ JMemcheck ]------------------------*/
/*                Verify memory chain integrity                 */
/*--------------------------------------------------------------*/
/* input:                                                       */
/*  CheckFree != if currently allocated blocks are to be        */
/*  reported                                                    */
/* local:                                                       */
/*      i = link number                                         */
/*      CurMal = link pointer                                   */
/*      status = current error condition                        */
/* return:                                                      */
/*      -1 = error,                                             */
/*      0 = no error detected                                   */
/*--------------------------------------------------------------*/

int j_memcheck(int CheckFree)
{
    int status = 0;

    MLINK *CurMal = MalTop;

    /* Walk the alloc list */
    if ( CheckFree >= 2 )
    {
        DBUG_PRINT("alloc", ("There should be %d unfreed blocks", memc));
    }

    while ( CurMal != NULL )
    {
        if ( CurMal->MAddr[CurMal->MSize] != CKBYT )
        {
            DBUG_PRINT("alloc", ("Memory overrun detected in link "
            "allocated at %s Line %d", CurMal->MFile, CurMal->MLine));
            status = -1;
        }
        if ( CheckFree >= 1 )
        {
            DBUG_PRINT("alloc", ("Unfreed block of size %d allocated at "
            "%s Line %d", CurMal->MSize, CurMal->MFile, CurMal->MLine));
            status = -1;
        }
        CurMal = CurMal->NextLink;
    }
    return(status);
}

/*-------------------------[ JMemValid ]------------------------*/
/*          Verify an address is in a JMalloc space             */
/*--------------------------------------------------------------*/

int JMemValid(char *Addr)
{
    MLINK *CurMal = MalTop;

    while ( CurMal != NULL )
    {
        if ( Addr >= CurMal->MAddr && Addr <= CurMal->MAddr + CurMal->MSize )
            return(TRUE);
        CurMal = CurMal->NextLink;
    }

    return(FALSE);
}

/*-------------------------[ JMemOwner ]------------------------*/
/*            Return the owner of a block of ram                */
/*--------------------------------------------------------------*/

static MLINK *JMemOwner(char *Addr)
{
    MLINK *CurMal = MalTop;

    while ( CurMal != NULL )
    {
        unsigned long Cur = (unsigned long)CurMal->MAddr,
                      Adr = (unsigned long)Addr;
        if ( Adr >= Cur && Adr <= Cur + CurMal->MSize )
            return(CurMal);
        CurMal = CurMal->NextLink;
    }

    return((void*)NULL);
}

/*-------------------------[ j_strcpy ]-------------------------*/
/*          String copy with checks and protections             */
/*--------------------------------------------------------------*/

char *j_strcpy(char *__dest, const char *__src, char *file, int line)
{
    MLINK *MemOwner = JMemOwner(__dest);

    if ( MemOwner == NULL )
    {
        DBUG_PRINT("alloc", ("Strcpy destination invalid - %s: line %d",
            file, line));
    }
    else if ( strlen(__src) > MemOwner->MAddr + MemOwner->MSize - __dest )
    {
        DBUG_PRINT("alloc", ("Strcpy destination overrun - %s: line %d",
            file, line));
    }
    return(strcpy(__dest, __src));
}

/*-------------------------[ j_strncpy ]------------------------*/
/*                 String n copy with checks                    */
/*--------------------------------------------------------------*/

char *j_strncpy(char *__dest, const char *__src, size_t maxlen, char *file,
                int line)
{
    MLINK *MemOwner = JMemOwner(__dest);

    if ( MemOwner == NULL )
    {
        DBUG_PRINT("alloc", ("strncpy destination invalid - %s: line %d",
            file, line));
    }
    else if ( maxlen > MemOwner->MAddr + MemOwner->MSize - __dest )
    {
        DBUG_PRINT("alloc", ("strncpy destination overrun - %s: line %d",
            file, line));
    }

    return(strncpy(__dest, __src, maxlen));
}

char *j_strcat(char *__dest, char *__src, char *file, int line)
{
    MLINK *MemOwner = JMemOwner(__dest);

    if ( MemOwner == NULL )
    {
        DBUG_PRINT("alloc", ("strcat destination invalid - %s: line %d",
            file, line));
    }
    else if ( strlen(MemOwner->MAddr) + strlen(__src) + 1 > MemOwner->MSize )
    {
        DBUG_PRINT("alloc", ("strcat destination overrun - %s: line %d",
            file, line));
    }

    return(strcat(__dest, __src));
}

char *j_strdup(char *__str, char *file, int line)
{
    char *p = j_malloc(strlen(__str) + 1, file, line);

    if ( p != NULL )
    {
        j_strcpy(p, __str, file, line);
    }
    return(p);
}

char *j_strnset(char *str, int ch, size_t n, char *file, int line)
{
    MLINK *MemOwner = JMemOwner(str);

    if ( MemOwner == NULL )
    {
        DBUG_PRINT("alloc", ("strnset destination invalid - %s: line %d",
            file, line));
    }
    return(strnset(str, ch, n));
}

char *j_strset(char *str, int ch, char *file, int line)
{
    MLINK *MemOwner = JMemOwner(str);

    if ( MemOwner == NULL )
    {
        DBUG_PRINT("alloc", ("strset destination invalid - %s: line %d",
            file, line));
    }
    return(strset(str, ch));
}

/*------------------------[ j_checkstr ]------------------------*/
/*             Check a MLINK string for problems                */
/* j_sprintf      sprintf replacement                           */
/*--------------------------------------------------------------*/

int j_checkstr(MLINK *str)
{
    if ( str->MAddr[str->MSize] != CKBYT || strlen(str->MAddr) > str->MSize - 1 )
    {
        DBUG_PRINT("alloc",
            ("Bad string, allocated at %s Line %d", str->MFile, str->MLine));
        return 1;
    }
    return 0;
}

void *j_memcpy(void *dest, const void *src, size_t n, char *file, int line)
{
    MLINK *MemOwner = JMemOwner(dest);

    if ( MemOwner == NULL )
    {
        DBUG_PRINT("alloc", ("memcpy destination invalid - %s: line %d",
            file, line));
    }
    else if ( n > MemOwner->MAddr + MemOwner->MSize - (char *)dest )
    {
        DBUG_PRINT("alloc", ("memcpy destination overrun - %s: line %d",
            file, line));
    }

    return(memcpy(dest, src, n));
}

void *j_memset(void *dest, int ch, size_t n, char *file, int line)
{
    MLINK *MemOwner = JMemOwner(dest);

    if ( MemOwner == NULL )
    {
        DBUG_PRINT("alloc", ("memset destination invalid - %s: line %d",
            file, line));
    }
    else if ( n > MemOwner->MAddr + MemOwner->MSize - (char *)dest )
    {
        DBUG_PRINT("alloc", ("memset destination overrun - %s: line %d",
            file, line));
    }

    return(memset(dest, ch, n));
}

char *a_strcpy(char *__dest, const char *__src, int size, char *file,
    int line)
{
    if ( strlen(__src) >= size)
        DBUG_PRINT("alloc", ("strcpy destination overrun - %s: line %d",
            file, line));
    return(strcpy(__dest, __src));
}

char *a_strncpy(char *dest, const char *src, size_t n, int size, char *file,
    int line)
{
    if ( n >= size )
        DBUG_PRINT("alloc", ("strncpy destination overrun - %s: line %d",
            file, line));
    return(strncpy(dest, src, n));
}

char *a_strcat(char *dest, const char *src, int size, char *file, int line)
{
    if ( strlen(dest) + strlen(src) + 1 > size )
        DBUG_PRINT("alloc", ("strcat destination overrun = %s: line %d",
            file, line));
    return(strcat(dest, src));
}

char *a_strnset(char *str, int ch, size_t n, int size, char *file, int line)
{
    if ( n + 1 > size )
        DBUG_PRINT("alloc", ("strnset destination overrun - %s: line %d",
            file, line));
    return(strnset(str, ch, n));
}

void *a_memcpy(void *dest, const void *src, size_t n, int size, char *file,
             int line)
{
    if ( n > size )
        DBUG_PRINT("alloc", ("memcpy destination overrun - %s: line %d",
            file, line));
    return(memcpy(dest, src, n));
}

void *a_memset(void *dest, int ch, size_t n, int size, char *file, int line)
{
    if ( n > size )
        DBUG_PRINT("alloc", ("memset destination overrun - %s: line %d",
            file, line));
    return memset(dest, ch, n);
}

#ifdef TEST

#ifdef __WATCOMC__
 #pragma off (unreferenced);
#endif
#ifdef __TURBOC__
 #pragma argsused
#endif

int main(int argc, char *argv[])
{
    char *new,
         *p,
         *q,
         r[30],
         *s[256] = {NULL};
    int i = 1;

    while ( (p = argv[i++]) != NULL )
    {
        switch ( *p++ )
            case '-':
                switch ( *p++ )
                    case '#':
                        DBUG_PUSH(argv[i - 1]);
    }
    new = JCalloc(64, 56);
    new = JRealloc(new, 64);
    JStrCpy(new + 10, "Test string");
    DBUG_PRINT("alloc", ("r is invalid"));
    JStrCpy(r, "Test");
    q = JCalloc(4, 4);
    q[4 * 4] = 3;

    DBUG_PRINT("alloc", ("p was never allocated"));
    JFree(p);

    i = 0;

    DBUG_PRINT("alloc", ("Deplete memory"));
    do
    {
        s[i] = JMalloc(32000);
        i++;
    } while ( s[i - 1] != NULL );

    i = 0;
    while ( s[i] != NULL )
    {
        JFree(s[i++]);
    }

    DBUG_PRINT("alloc", ("New and q are orphaned blocks"));
    DBUG_PRINT("alloc", ("q has an overrun"));
    JMemcheck(1);
    return(0);
}
#endif
