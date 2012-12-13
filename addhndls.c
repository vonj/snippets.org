/*
**  ADDHNDLS.C
**
**  A compilation of public domain sources originally written by
**  Doug Burger and Bob Jarvis
**
**  Collected and modified for Zortech, Microsoft, and Borland by Bob Stout
**
**  Demonstrates relocating the file handle table under DOS 3.x
**  for having more than the usual 20 files open in a single
**  program
*/

#include <stdlib.h>
#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include "dosfiles.h"

#ifdef TEST
 #include "mk_fp.h"
 #if defined(_MSC_VER)
  #include <stdlib.h>

  /* MSC's open() is funny - this code only works with _dos_open()      */

  int open(const char *name, int mode, ...)
  {
          int hdl;

          if (0 == _dos_open(name, mode, &hdl))
                  return hdl;
          else    return -1;
  }
 #endif /* MSC */
#endif /* TEST */

unsigned char handle_table[TABLE_SIZE];     /* table of file DOS handles    */
unsigned char far * far * handle_ptr;       /* ptr to DOS's ptr to hand.    */
unsigned int far *handle_count;             /* ptr to handle count          */

int relocate(void)
{
    switch (_osmajor)
    {
    case 2:
        return -1;
    case 3:
        if (3 > _osminor)
        {                                       /* by Doug Burger           */
            unsigned int i;

            handle_count = MK_FP(_psp, 0x32);   /* handle count at PSP:32h  */
            handle_ptr = MK_FP(_psp, 0x34);     /* table ptr at PSP:34h     */
            for (i = 0; i < *handle_count; i++) /* relocate exiting table   */
                handle_table[i] = (*handle_ptr)[i];
            for (i = *handle_count; i < TABLE_SIZE; i++)    /* init. rest   */
                handle_table[i] = 255;
            *handle_ptr = handle_table;         /* set pointer to new table */
            *handle_count = TABLE_SIZE;         /* set new table size       */
            return 0;
        }
        else
    default:                                    /* DOS 4+                   */
        {                                       /* by Bob Jarvis            */
            union REGS regs;

            regs.h.ah = 0x67;
            regs.x.bx = TABLE_SIZE | 1;         /* has to be an odd number  */

            intdos(&regs, &regs);

            if(regs.x.cflag)                    /* error                    */
                return -1;
            else
                return 0;
        }
    }
}   /*  relocate()  */

/*
**  Test code
*/

#ifdef TEST

main()
{
    int c, h;

    relocate();

    c = 0;
    while ((h = open("CON", O_RDONLY)) >= 0)        /* DOS closes files */
    {
        c++;                                        /* on exit, so I    */
        printf("handle = %d\n", h);                 /* don't bother     */
    }                                               /* saving handles   */
    printf("total opened files = %d\n", c);
    return 0;
}   /*  ADDHNDLS.C  */

#endif /* TEST */
