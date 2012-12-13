/*
** fork.h -- get characters from a pipe and display to all files
**           needed (assumes stdout), Public Domain.
**
** PROGRAMMER : Dustin Puryear <dpuryear@delphi.com>
** COMPILER   : Mix PowerC v2.2.0a (MSDOS)
** HISTORY    : May 4, 1994  - Original
**              May 11, 1994 - Output in binary, buffer read of stdin.
**              June 8, 1994 - Cleaned up header, and removed some
**                             unneeded code.
*/

#include <stdio.h>    /* standard include file        */
#include <stdlib.h>   /* malloc(), free(), exit()     */
#include <string.h>   /* strcpy()                     */
#include <ctype.h>    /* tolower()                    */
#include <direct.h>   /* MAXPATH definition (MSDOS)   */

#ifndef __DATE__
#define __DATE__ "June 8 1994"    /* Date last updated              */
#endif
#ifndef MAXPATH
#define MAXPATH 80                /* Make sure there is a MAXPATH   */
#endif
#ifndef __STDC__
#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0            /* Define exit values             */
#endif
#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif
#endif
#define VER     "1.02"            /* Version number                 */
#define MAXBUFF 4096              /* Maximum buffer size            */

typedef struct filenode
{
      FILE            *ptr;
      struct filenode *next;
} FILENODE;                 /* Node in ll containing a file pointer */
typedef struct
{
      FILENODE     *head;
      FILENODE     *tail;
} LL;                       /* ll of FILE pointers                  */

void llopen(LL *files, FILE *fptr);
void llfree(LL *files);
void output(LL *files, char *buffer, int size);
void help(void);
