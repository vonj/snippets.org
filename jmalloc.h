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

/*------------------------[ jmalloc.h ]----------------------------*/
/*                   main header file for jmalloc                  */
/*-----------------------------------------------------------------*/

/*--------------------------------------------------------------*/
/*---------------------------[ defines ]------------------------*/
/*--------------------------------------------------------------*/
#ifndef _jmalloc
#   define _jmalloc

#define FALSE 0
#define TRUE 1

#define CKBYT 1
#define DIRTY 2

/*--------------------------------------------------------------*/
/*------------------------[ structures ]------------------------*/
/*--------------------------------------------------------------*/

typedef struct mlist
{
    struct mlist *NextLink;         /* link to next struct */
    char *MAddr;                    /* address assigned    */
    unsigned MSize;                 /* size allocated      */
    char MFile[20];                 /* Allocation file name */
    int MLine;                      /* Allocation line number */
} MLINK;

/*--------------------------------------------------------------*/
/*--------------------[ public prototypes ]---------------------*/
/*--------------------------------------------------------------*/

#ifdef __cplusplus
   extern   "C"   {
#endif

void db_prn(char *fmt, ...);
#define DBUG_ENTER(a)
#define DBUG_RETURN(a) return(a)
#define DBUG_PRINT(a, b) db_prn b
#define DBUG_PUSH(a)
#define DBUG_VOID_RETURN return

#define JCheckStr(a) j_checkstr((a))

int j_checkstr(MLINK *str);

#ifdef DBUG

#define dr(a) j_deref(a, __FILE__, __LINE__)

#define JStrnSet(a, b, c) j_strnset((a), (b), (c), __FILE__, __LINE__)
#define JStrDup(a) j_strdup((a), __FILE__, __LINE__)
#define JStrCat(a, b) j_strcat((a), (b), __FILE__, __LINE__)
#define JMalloc(a) j_malloc((a), __FILE__, __LINE__)
#define JCalloc(a, b) j_calloc((a), (b), __FILE__, __LINE__)
#define AStrCpy(a, b) a_strcpy((a), (b), sizeof (a), __FILE__, __LINE__)
#define AStrCat(a, b) a_strcat((a), (b), sizeof (a), __FILE__, __LINE__)
#define AStrnCpy(a, b, c) a_strncpy((a), (b), (c), sizeof (a), __FILE__, __LINE__)
#define AStrnSet(a, b, c) a_strnset((a), (b), (c), sizeof (a), __FILE__, __LINE__)
#define JFree(a) j_free(a, __FILE__, __LINE__)
#define JStrCpy(a, b) j_strcpy((a), (b), __FILE__, __LINE__)
#define JStrnCpy(a, b, c) j_strncpy((a), (b), (c), __FILE__, __LINE__)
#define JMemCpy(a, b, c) j_memcpy((a), (b), (c), __FILE__, __LINE__)
#define JMemSet(a, b, c) j_memset((a), (b), (c), __FILE__, __LINE__)
#define JMemcheck(a) j_memcheck((a))
#define AMemCpy(a, b, c) a_memcpy((a), (b), (c), sizeof (a), __FILE__, __LINE__)
#define AMemSet(a, b, c) a_memset((a), (b), (c), sizeof (a), __FILE__, __LINE__)
#define JRealloc(a, b) j_realloc((a), (b), __FILE__, __LINE__)

void *j_deref(void *a, char *file, int line);
char *j_strnset(char *str, int ch, size_t n, char *file, int line);
char *j_strdup(char *str, char *file, int line);
char *j_strcat(char *__dest, char *__src, char *file, int line);
void *j_malloc(unsigned size, char *file, int line);
void *j_calloc(unsigned size, unsigned sizeach, char *file, int line);
int j_memcheck(int CheckFree);
void j_free(void *AllocAddr, char *file, int line);
char *j_strcpy(char *__dest, const char *__src, char *file, int line);
char *j_strncpy(char *__dest, const char *__src, size_t maxlen, char *file,
                int line);
void *j_realloc(void *addr, unsigned Size, char *file, int line);
void *j_memset(void *dest, int ch, size_t n, char *file, int line);
void *j_memcpy(void *dest, const void *src, size_t n, char *file, int line);
char *a_strcpy(char *__dest, const char *__src, int size, char *file,
               int line);
char*a_strcat(char *dest, const char *src, int size, char *file, int line);
void *a_memcpy(void *dest, const void *src, size_t n, int size, char *file,
             int line);
void *a_memset(void *dest, int ch, size_t n, int size, char *file, int line);
char *a_strncpy(char *__dest, const char *__src, size_t maxlen, int size,
    char *file, int line);
char *a_strnset(char *str, int ch, size_t n, int size, char *file, int line);


#else

#define dr(a) (a)

#define JStrnSet(a, b, c) strnset((a), (b), (c))
#define JStrDup(a) strdup((a))
#define JStrCat(a, b) strcat((a), (b))
#define JMalloc(a) malloc((a))
#define JCalloc(a, b) calloc((a), (b))
#define JFree(a) free(a)
#define JStrCpy(a, b) strcpy((a), (b))
#define JStrnCpy(a, b, c) strcpy((a), (b), (c))
#define JMemCpy(a, b, c) memcpy((a), (b), (c))
#define JMemSet(a, b, c) memset((a), (b), (c))
#define JMemcheck(a)
#define JRealloc(a, b) realloc((a), (b))

#define AStrCpy(a, b) strcpy((a), (b))
#define AStrCat(a, b) strcat((a), (b))
#define AStrnCpy(a, b, c)  strncpy((a), (b), (c))
#define AStrnSet(a, b, c), strnset((a), (b), (c))
#define AMemCpy(a, b, c) memcpy((a), (b), (c))
#define AMemSet(a, b, c) memset((a), (b), (c))

#endif

#ifdef __cplusplus
   }
#endif

/*--------------------------------------------------------------*/
/*---------------------[ public variables ]---------------------*/
/*--------------------------------------------------------------*/

extern MLINK *MalTop;
#endif
