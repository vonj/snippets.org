/*_ timer.c   Mon Feb 29 1988   Modified by: Jerry Coffin   */
/* Written by Walter Bright                                 */
/* To compile (with Zortech C):                             */
/*    ZTC -mti timer                                        */

#include    <stdio.h>
#include    <time.h>
#include    <process.h>
#include    <stdlib.h>
#include    <string.h>
#include    "extkword.h"

#if defined(_QC) || defined(_MSC_VER)
 #include    <malloc.h>
#endif

#ifdef __SC__
 #define SVP_CAST(x) (const char * const *)(x)
#else
 #define SVP_CAST(x) (x)
#endif

#ifdef __ZTC__
int _okbigbuf = 0;      /* Use as little memory as possible */
#endif

main(int argc, char *argv[])
{
      clock_t clock(),starttime;
      int status, i;

      if (argc < 2)
      {     printf("Time execution of a command.\nUse:\n\ttimer command\n");
            exit(1);
      }
      argv[argc] = 0;         /* terminate with a 0 (unportable method) */
      starttime = clock();

/*
** This block added to keep arguments with embedded whitespace from getting
** broken up when timing is done.  A *nix version should be a bit different.
** Also, the assignment to argv[i] technically isn't portable, but I don't
** know of an implementation where it causes a problem. <JC>
*/
      for (i=1;i<argc;i++)
      {
            if (NULL != strchr(argv[i], ' '))
            {
                  size_t len = strlen(argv[i]+3);
                  char *temp = malloc(len);

                  strcpy(temp+1, argv[i]);
                  temp[0] = '"';
                  temp[len-2] = '"';
                  temp[len-1] = '\0';
                  argv[i] = temp;
            }
      }

/* This reduces memory usage with MS compilers by releasing unused heap.
 * Watcom may support it as well, but I'm not sure.
 */
#if defined(_QC) || defined(_MSC_VER)
      _heapmin();
#endif
      status = spawnvp(0,argv[1],SVP_CAST(argv + 1));
      starttime = clock() - starttime;
      if (status == -1)
      {     printf("'%s' failed to execute\n",argv[1]);
            exit(1);
      }
#if !defined(__TURBOC__)
      printf("Elapsed time = %d.%02d seconds\n",(int) (starttime/CLK_TCK),
            (int) (starttime%CLK_TCK));
#else
      printf("Elapsed time = %.2f seconds\n", starttime/CLK_TCK);
#endif
      if (status != 0)
            printf("--- errorlevel %d\n",status);
      /* exit(0); changed to `return 0;' to reduce program size (minutely)
       * and possibility or warnings that main doesn't return a value.
       */
      return 0;
}

#ifdef __ZTC__

/* Prevent exit() and fclose() from being linked in from library  */
/* (to conserve the size of the output file).                     */

void exit(int exitstatus)
{
      _exit(exitstatus);
}

#endif
