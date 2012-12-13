#include <stdio.h>
#if !defined(__unix__)
 #include <conio.h>
#endif
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <float.h>
#include "sniptype.h"
#include "getopts.h"

Boolean_T     test1     = True_,
              test2     = False_;
int           test3     = -37;
long          test4     = 100000L;
double        test5;                      /* Initialized in main()      */
unsigned char test6     = 0x5a;
char          test7[80] = "Initial string 1";
char          test8[80] = "Initial string 2";
char          test9[99] = "";
char          test10[9] = "";

char         test3min[]  = "-100";
char         test3max[]  = "32000";
char         test4min[]  = "-100000";
char         test4max[]  = "300000";
char         test5min[]  = "-12345.6789";
char         test5max[]  = "98765.4321";
char         test6max[]  = "ee";

char         test3dflt[] = "123";
char         test4dflt[] = "1234567";
char         test5dflt[] = "PI";
char         test6dflt[] = "a5";
char         test7dflt[] = "Default string";
char         test9dflt[] = "2";

struct Option_Tag options[] = {                 /* Valid options        */
      {
            'A',                                /* Option letter        */
            False_,                             /* Case sensitiivity    */
            Boolean_Tag,                        /* Data type            */
            False_,                             /* Cumulative option    */
            (void *)&test1,                     /* Storage address      */
            NULL,                               /* Range - min          */
            NULL,                               /* Range - max          */
            NULL                                /* Default value        */
      },
      {
            'B',                                /* Option letter        */
            False_,                             /* Case sensitiivity    */
            Boolean_Tag,                        /* Data type            */
            False_,                             /* Cumulative option    */
            (void *)&test2,                     /* Storage address      */
            NULL,                               /* Range - min          */
            NULL,                               /* Range - max          */
            NULL                                /* Default value        */
      },
      {
            'C',                                /* Option letter        */
            False_,                             /* Case sensitiivity    */
            Int_Tag,                            /* Data type            */
            False_,                             /* Cumulative option    */
            (void *)&test3,                     /* Storage address      */
            test3min,                           /* Range - min          */
            test3max,                           /* Range - max          */
            test3dflt                           /* Default value        */
      },
      {
            'D',                                /* Option letter        */
            False_,                             /* Case sensitiivity    */
            Long_Tag,                           /* Data type            */
            False_,                             /* Cumulative option    */
            (void *)&test4,                     /* Storage address      */
            test4min,                           /* Range - min          */
            test4max,                           /* Range - max          */
            test4dflt                           /* Default value        */
      },
      {
            'E',                                /* Option letter        */
            False_,                             /* Case sensitiivity    */
            DFloat_Tag,                         /* Data type            */
            False_,                             /* Cumulative option    */
            (void *)&test5,                     /* Storage address      */
            test5min,                           /* Range - min          */
            test5max,                           /* Range - max          */
            test5dflt                           /* Default value        */
      },
      {
            'F',                                /* Option letter        */
            False_,                             /* Case sensitiivity    */
            Byte_Tag,                           /* Data type            */
            False_,                             /* Cumulative option    */
            (void *)&test6,                     /* Storage address      */
            NULL,                               /* Range - min          */
            test6max,                           /* Range - max          */
            test6dflt                           /* Default value        */
      },
      {
            'G',                                /* Option letter        */
            False_,                             /* Case sensitiivity    */
            String_Tag,                         /* Data type            */
            False_,                             /* Cumulative option    */
            (void *)test7,                      /* Storage address      */
            NULL,                               /* Range - min          */
            NULL,                               /* Range - max          */
            (void *)test7dflt                   /* Default value        */
      },
      {
            'H',                                /* Option letter        */
            False_,                             /* Case sensitiivity    */
            String_Tag,                         /* Data type            */
            False_,                             /* Cumulative option    */
            (void *)test8,                      /* Storage address      */
            NULL,                               /* Range - min          */
            NULL,                               /* Range - max          */
            NULL                                /* Default value        */
      },
      {
            'O',                                /* Option letter        */
            False_,                             /* Case sensitiivity    */
            String_Tag,                         /* Data type            */
            True_,                              /* Cumulative option    */
            test9,                              /* Storage address      */
            NULL,                               /* Range - min          */
            NULL,                               /* Range - max          */
            (void *)test9dflt                   /* Default value        */
      },
      {
            NUL,                                /* Terminating record   */
            False_,
            Error_Tag,
            False_,
            NULL,
            NULL,
            NULL,
            NULL
      }
};

struct Voption_tag voptions[] = {               /* Valid options        */
      {
            "Test10",
            False_,
            String_Tag,
            False_,
            test10,
            "",
            "",
            "Bye!",
      },
      {
                  NULL,                               /* Terminating record   */
                  False_,
                  Error_Tag,
                  False_,
                  NULL,
                  NULL,
                  NULL,
                  NULL
      }
};

#define TFprint(v) ((v) ? "TRUE" : "FALSE")

#define NULLstr(s) (s) ? (s) : "<>NULL<>"

int main(int argc, char *argv[])
{
      int i;

      test5 = getopts_eval("exp(1)");

      printf("Defaults:\ntest1 = %s\ntest2 = %s\ntest3 = %d\ntest4 = %ld\n"
             "test5 = %g\ntest6 = 0%2Xh\ntest7 = \"%s\"\ntest8 = \"%s\"\n"
             "test9 = \"%s\"\ntest10 = \"%s\"\n\n",
             TFprint(test1), TFprint(test2), test3, test4, test5, test6, test7,
             test8, test9, test10);

      printf("getopts() returned %d\n", getopts(argc, argv));

      printf("Options are now:\ntest1 = %s\ntest2 = %s\ntest3 = %d\ntest4 = %ld\n"
             "test5 = %g\ntest6 = 0%2Xh\ntest7 = \"%s\"\ntest8 = \"%s\"\n"
             "test9 = \"%s\"\ntest10 = \"%s\"\n\n",
             TFprint(test1), TFprint(test2), test3, test4, test5, test6,
             test7, test8, test9, test10);

#if !defined(__unix__)
      puts("Hit any key to continue");
      getch();
#endif
      for (i = 0; i < xargc; ++i)
            printf("xargv[%d] = \"%s\"\n", i, NULLstr(xargv[i]));
      printf("\nxargc = %d\n", xargc);
      return 0;
}
