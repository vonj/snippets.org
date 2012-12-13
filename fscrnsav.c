/*
**  Portable PC screen functions
**  Public domain by Bob Stout
**  Uses SCRNMACS.H and HUGEREAD.C, also from SNIPPETS
*/
 
#include <stdio.h>
#include "sniptype.h"
#include "scrnmacs.h"

/*
**  Prototypes from HUGEREAD.C, also in SNIPPETS
*/

long hugefread(FILE *fp, char FAR *buf, long size);
long hugefwrite(FILE *fp, char FAR *buf, long size);

/*
**  Save the text screen to a file
*/

Boolean_T fSaveScrn(const char *fname)
{
      FILE *file;

      if (NULL == (file = fopen(fname, "wb")))
            return Error_;
      if ((long)SCRNBYTES != hugefwrite(file, (char FAR *)SCRBUFF,
            (long)SCRNBYTES))
      {
            return Error_;
      }
      fclose(file);
      return Success_;
}

/*
**  Restore the text screen from a file
*/

Boolean_T fRestoreScrn(const char *fname)
{
      FILE *file;

      if (NULL == (file = fopen(fname, "rb")))
            return Error_;
      if ((long)SCRNBYTES != hugefread(file, (char FAR *)SCRBUFF,
            (long)SCRNBYTES))
      {
            return Error_;
      }
      fclose(file);
      return Success_;
}

#ifdef TEST

#include <conio.h>

/*
**  Run this test with a screenful of misc. stuff
**
**  Note that this test requires that VIDPORT.C and SCROLL.C, also from
**  SNIPPETS, be linked.
*/

main()
{
      if (Error_ == fSaveScrn("fscrnsav.tst"))
      {
            puts("Unable to save the screen");
            return 1;
      }
      ClrScrn(7);
      GotoXY(0, 0);
      fputs("fClrScrn() tested", stderr);
      fputs("\nHit any key to continue...\n", stderr);
      getch();
      fRestoreScrn("fscrnsav.tst");
      return 0;
}

#endif /* TEST */
