/*
**  Compiler I/O benchmarks
**  public domain by Dave Knapp & Bob Stout
*/

#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include "errors.h"           /* For cant()                             */
#include "sniptype.h"         /* For DWORD                              */
#include "extkword.h"         /* For FAR                                */
#include "scrnmacs.h"         /* For portable screen I/O                */
#include "dvidport.h"         /* For portable direct video I/O          */

#ifndef ITERATIONS
 #define ITERATIONS 5000
#endif

#ifdef M_I86      /* Identifier for MSC, QC, Watcom, or ZTC */
 #ifndef __ZTC__
  #include <graph.h>
  #ifndef __WATCOMC__
   #ifdef _MSC_VER
    #define LOGFILE "dspdtst.msc"
   #else
    #define LOGFILE "dspdtst.qc"
   #endif
  #else
   #define LOGFILE "dspdtst.wc"
  #endif    /* not Watcom   */
 #else      /* if ZTC       */
  #include <disp.h>
  #ifdef __SC__
   #define LOGFILE "dspdtst.sc"
  #else
   #define LOGFILE "dspdtst.ztc"
  #endif
 #endif     /* if ZTC       */
#else       /* must be Borland or Mix     */
 #ifdef __TURBOC__
  #ifdef __BORLANDC__
   #define LOGFILE "dspdtst.bc"
  #else
   #define LOGFILE "dspdtst.tc"
  #endif
 #endif
 #ifdef __POWERC
  #define LOGFILE "dspdtst.pc"
 #endif
#endif

#ifndef LOGFILE
 #error Unrecognized compiler
#endif

DWORD FAR *bios_time = (DWORD FAR *)(0x0040006cL);
DWORD time1, time2, time3, time4, time5, time6;

main()
{
        int i;
        FILE *log = stdout, *nulfile;

        ClrScrn(BG_(BLACK_)+GRAY_);
        nulfile = cant("NUL", "w");
        time1 = *bios_time;
        for(i = 1; i < ITERATIONS; i++)
        {
                gotoxy(10,5);
                puts("puts      test.");
                puts("this is the second line.\n");
        }
        time1 = *bios_time - time1;
        time2 = *bios_time;
        for(i = 1; i < ITERATIONS; i++)
        {
                gotoxy(10,8);
                printf("printf    test.\n");
                printf("this is the second line.\n");
        }
        time2 = *bios_time - time2;
        dvid_open();
        time3 = *bios_time;
        for(i = 1; i < ITERATIONS; i++)
        {
                gotoxy(10,11);
                cputs("cputs     test.\r\n");
                cputs("this is the second line.\r\n");
        }
        time3 = *bios_time - time3;
        time4 = *bios_time;
        for(i = 1; i < ITERATIONS; i++)
        {
                gotoxy(10,14);
                cprintf("cprintf   test.\r\n");
                cprintf("this is the second line.\r\n");
        }
        time4 = *bios_time - time4;
        dvid_close();
        time5 = *bios_time;
        for(i = 1; i < ITERATIONS; i++)
        {
                fputs("fputs     test.\n", nulfile);
                fputs("this is the second line.\n", nulfile);
        }
        time5 = *bios_time - time5;
        time6 = *bios_time;
        for(i = 1; i < ITERATIONS; i++)
        {
                fprintf(nulfile, "fprintf   test.\n");
                fprintf(nulfile, "this is the second line.\n");
        }
        time6 = *bios_time - time6;

        log = cant(LOGFILE, "w");
        fprintf(log, "Times for %d iterations:\n\n", ITERATIONS);
        fprintf(log, "puts     %10.3f seconds\n", (double)time1 * .054945);
        fprintf(log, "printf   %10.3f seconds\n", (double)time2 * .054945);
        fprintf(log, "cputs    %10.3f seconds\n", (double)time3 * .054945);
        fprintf(log, "cprintf  %10.3f seconds\n", (double)time4 * .054945);
        fprintf(log, "fputs    %10.3f seconds\n", (double)time5 * .054945);
        fprintf(log, "fprintf  %10.3f seconds\n", (double)time6 * .054945);
        fclose(log);
        printf("\nDone - results are in %s\n", LOGFILE);
        return 0;
}
