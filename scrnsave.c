/*
**  Portable PC screen functions
**  Public domain by Bob Stout
**  Uses VIDPORT.C, also from SNIPPETS
**  Uses _fmemcpy(), available in most PC compiler libraries
**  A portable _fmemcpy() is available in FMEMOPS.C, also from SNIPPETS
*/
 
#include <stdlib.h>
 #if defined(__POWERC) || (defined(__ZTC__) && !defined(__SC__)) || \
       (defined(__TURBOC__) && !defined(__BORLANDC__))
  #include "fmemops.h"
 #else
  #include <string.h>
 #endif
#include "scrnmacs.h"         /* Also in SNIPPETS     */

/*
**  Save the text screen
*/

struct SCREEN *SaveScrn(void)
{
      struct SCREEN *screen;

      if (NULL == (screen = malloc(sizeof(struct SCREEN))))
            return NULL;
      if (NULL == (screen->vbuf = malloc(SCRNBYTES)))
      {
            free(screen);
            return NULL;
      }
      _fmemcpy((unsigned short FAR *)(screen->vbuf), SCRBUFF, SCRNBYTES);
      GetCurPos(&screen->curX, &screen->curY);
      return screen;
}

/*
**  Restore the text screen
*/

void RestoreScrn(struct SCREEN *screen)
{
      _fmemcpy(SCRBUFF, (unsigned short FAR *)(screen->vbuf), SCRNBYTES);
      GotoXY(screen->curX, screen->curY);
}

/*
**  Free a saved screen buffer
*/

void FreeScrnBuf(struct SCREEN *screen)
{
      free(screen->vbuf);
      free(screen);
}


#ifdef TEST

#include <stdio.h>
#include <conio.h>

/*
**  Run this test with a screenful of misc. stuff
*/

main()
{
      struct SCREEN *screen;
      int vatr = GetCurAtr();

      if (NULL == (screen = SaveScrn()))
      {
            puts("Unable to save the screen");
            return 1;
      }
      ClrScrn(vatr);
      GotoXY(0, 0);
      fputs("ClrScrn() tested", stderr);
      fputs("\nHit any key to continue...\n", stderr);
      getch();
      RestoreScrn(screen);
      FreeScrnBuf(screen);
      return 0;
}

#endif /* TEST */
