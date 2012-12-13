/******** CALCULATE I/O PERFORMANCE TO NUL FILE ********/

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include "errors.h"           /* For cant()     */

#define CHK 100L              /* speed factor   */

long ticks(long tick)                                   /* GET BIOS TIME TICK */
{
        union REGS reg;
        reg.h.ah=0;
        int86(0x1A, &reg, &reg);
        return ((long)reg.x.cx<<16)+reg.x.dx-tick;
}

long time_it(void(*func)(void))
{
        long t = ticks(0L);
        (*func)();
        return ticks(t);
}

void show_it(long t)
{
        long lquot, lrem;
        t = (t*1000/182+5)/10;
        lquot = t/10;
        lrem = t%10;
        printf("%3ld.%02d sec", lquot, (int)lrem);
}

void t_printf(void)
{
        register FILE *fp;
        register unsigned u;

        fp = cant("NUL", "wt");
        for (u=0; u<50*CHK; ++u)
                fprintf(fp, "Now is %d time for %d little indians\n", 123, -456);
        fclose(fp);
}

void b_printf(void)
{
        register FILE *fp;
        register unsigned u;

        fp = cant("NUL", "wb");
        for (u=0; u<50*CHK; ++u)
                fprintf(fp, "Now is %d time for %d little indians\n", 123, -456);
        fclose(fp);
}

void tu_printf(void)
{
        register FILE *fp;
        register unsigned u;

        fp = cant("NUL", "wt");
        setbuf(fp, NULL);
        for (u=0; u<5*CHK; ++u)
                fprintf(fp, "Now is %d time for %d little indians\n", 123, -456);
        fclose(fp);
}

void bu_printf(void)
{
        register FILE *fp;
        register unsigned u;

        fp = cant("NUL", "wb");
        setbuf(fp, NULL);
        for (u=0; u<5*CHK; ++u)
                fprintf(fp, "Now is %d time for %d little indians\n", 123, -456);
        fclose(fp);
}

void t_write(void)
{
        register FILE *fp;
        register unsigned u;

        fp = cant("NUL", "wt");
        for (u=0; u<250*CHK; ++u)
                fwrite("Now is the time for all good men to come\n", 41, 1, fp);
        fclose(fp);
}

void b_write(void)
{
        register FILE *fp;
        register unsigned u;

        fp = cant("NUL", "wb");
        for (u=0; u<500*CHK; ++u)
                fwrite("Now is the time for all good men to come\n", 41, 1, fp);
        fclose(fp);
}

void tu_write(void)
{
        register FILE *fp;
        register unsigned u;

        fp = cant("NUL", "wt");
        setbuf(fp, NULL);
        for (u=0; u<100*CHK; ++u)
                fwrite("Now is the time for all good men to come\n", 41, 1, fp);
        fclose(fp);
}

void bu_write(void)
{
        register FILE *fp;
        register unsigned u;

        fp = cant("NUL", "wb");
        setbuf(fp, NULL);
        for (u=0; u<200*CHK; ++u)
                fwrite("Now is the time for all good men to come\n", 41, 1, fp);
        fclose(fp);
}

main(void)
{
        show_it(time_it(t_printf));
        printf(": time for text printf buffered\n");
        show_it(time_it(b_printf));
        printf(": time for binary printf buffered\n");
        show_it(time_it(tu_printf));
        printf(": time for text printf unbuffered\n");
        show_it(time_it(bu_printf));
        printf(": time for binary printf unbuffered\n");

        show_it(time_it(t_write));
        printf(": time for text write buffered\n");
        show_it(time_it(b_write));
        printf(": time for binary write buffered\n");
        show_it(time_it(tu_write));
        printf(": time for text write unbuffered\n");
        show_it(time_it(bu_write));
        printf(": time for binary write unbuffered\n");

        return 0;
}
