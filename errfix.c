/*
** ERRFIX.C - redirect stderr to some other file under MS-DOS
**
** by Bob Jarvis
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include "extkword.h"

#ifdef __SC__
 #define SVP_CAST(x) (const char * const *)(x)
#else
 #define SVP_CAST(x) (x)
#endif

int usage(void)
{
      puts("ERRFIX [filename] [prog] { {parm1} {parm2} ... {parmN} }");
      puts("   Redirects stderr to another file, then invokes a program");
      puts("   which will inherit the new definition of stderr.\n");
      puts("Parameters:");
      puts("   filename (required) - the name of the file stderr should");
      puts("      be redirected to.  Output written to stderr will");
      puts("      be routed to this file instead of the console.");
      puts("   prog (required) - name of the program to be run.");
      puts("   parm1...parmN (optional) - command-line parameters needed");
      puts("      to run the program specified by the 'prog' argument.");
      return 1;
}

int main(int argc, char *argv[])
{
      char **args = argv;

      if (3 > argc)
            return usage();

      if (NULL != argv[argc]) /* may be a problem under some compilers */
      {
            args = malloc((argc+1) * sizeof(char *));
            if (NULL == args)
            {
                  printf("Unable to allocate storage");
                  return 2;
            }

            memcpy(args, argv, argc * sizeof(char *));

            args[argc] = NULL;
      }

      freopen(args[1], "w", stderr);

      spawnvp(0, args[2], SVP_CAST(&args[2]));

      return 0;
}
