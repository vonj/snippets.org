/*
**  KBFLIP.C
**
**  a public domain demo by: Bob Stout
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <process.h>

#ifdef __TURBOC__
 #define FAR far
#else
 #define FAR _far
#endif

#define SHOW(str) fputs(str"\n", stderr)

#define BitSet(arg,posn) ((arg) | (1L << (posn)))
#define BitClr(arg,posn) ((arg) & ~(1L << (posn)))

#define LOCKS_POSN 4
#define BYTE unsigned char

BYTE FAR *kb_status = (BYTE FAR *) 0x00400017L;

/*
**  Tell the folks how this works
*/

void usage(void)
{
      SHOW("Usage: KBFLIP {+|-}[switches] [...{+|-}[switches]]");
      SHOW("Where \"switches\" are one or more of:");
      SHOW(" +/-C - Turn Caps Lock on/off");
      SHOW(" +/-N - Turn Num Lock on/off");
      SHOW(" +/-S - Turn Scroll Lock on/off");
      SHOW("Note switches may be upper or lower case\n");
      SHOW("Example: \"KBFLIP +Cn -S\" turns Caps Lock and Num Lock on "
            "and Scroll lock off");
      exit(-1);
}

/*
**  The real works starts here
**
**  This works by checking the user input against a string containing the
**  allowable switch characters in the same relative positions they
**  occupy in the BIOS data area, offset by 4 (LOCKS_POSN).
**
**  Note that all changes are made to a copy of the BIOS data so any
**  input errors will not cause incomplete changes to be applied.
*/

int main(int argc, char *argv[])
{
      int i, j;
      char *args = "SNC";
      BYTE template = *kb_status;              /* Make changes to copy */

      if (2 > argc)                             /* Help 'em             */
            usage();
      for (i = 1; i < argc; ++i)
      {
            if (NULL == strchr("+-", *argv[i]))
                  usage();

            for (j = 1; argv[i][j]; ++j)
            {
                  char *found;

                  if (NULL != (found = strchr(args, toupper(argv[i][j]))))
                  {
                        int posn = LOCKS_POSN + (found - args);

                        if ('+' == *argv[i])
                              template = (BYTE)BitSet(template, posn);
                        else  template = (BYTE)BitClr(template, posn);
                  }
                  else  usage();
            }
      }
      *kb_status = template;                    /* Apply all changes    */
      return(0);
}
