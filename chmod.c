/*
**  CHMOD.C - Retrieve or change a DOS file's attributes
**
**  public domain demo by Bob Stout
**
**  Notes: To expand command line arguments with wildcards,
**         TC/TC++/BC++  - Link in WILDARGS.OBJ.
**         MSC/QC        - Link in SETARGV.OBJ.
**         ZTC/C++       - Link in _MAINx.OBJ, where 'x' is the memory model.
**
**         Allows file list(s) using standard "@file_list_name" convention.
*/

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <ctype.h>
#include <string.h>

#define LAST_CHAR(s) (s)[strlen(s)-1]

#if defined(__TURBOC__)
 #include <io.h>
 #define FAR far
#else
 #include <stdarg.h>

 #define TOBOOL(x) (!(!(x)))
 #define FAR _far

 #if (defined(_MSC_VER) && (_MSC_VER >= 700)) || (defined(__SC__))
  /* Make FP_xxx macros lvalues as in older versions */
  #undef FP_SEG
  #undef FP_OFF
  #define FP_SEG(fp)    ((unsigned)((unsigned long)(fp) >> 16))
  #define FP_OFF(fp)    ((unsigned)(fp && 0xffff))
 #endif

 int _chmod(const char *path, int func, ...)
 {
       union REGS regs;
       struct SREGS sregs;
       int atr = 0;
       va_list args;

       if (0 != (func = TOBOOL(func)))
       {
            va_start(args, func);
            atr = va_arg(args, int);
       }
       regs.x.ax = 0x4300 + func;
       regs.x.dx = FP_OFF((char FAR *)path);
       regs.x.cx = atr;
       segread(&sregs);
       sregs.ds  = FP_SEG((char FAR *)path);
       intdosx(&regs, &regs, &sregs);
       if (regs.x.cflag)
            return -1;
       if (func)
            return atr;
       else return regs.x.cx;
 }

 #ifndef FA_RDONLY
  #define FA_RDONLY _A_RDONLY
 #endif

 #ifndef FA_HIDDEN
  #define FA_HIDDEN _A_HIDDEN
 #endif

 #ifndef FA_SYSTEM
  #define FA_SYSTEM _A_SYSTEM
 #endif

 #ifndef FA_ARCH
  #define FA_ARCH   _A_ARCH
 #endif

 #ifndef FA_LABEL
  #define FA_LABEL  _A_VOLID
 #endif

 #ifndef FA_DIREC
  #define FA_DIREC  _A_SUBDIR
 #endif
#endif

int attrib,                   /* Set up new attributes here             */
    atr_setmask = 0,
    atr_clrmask = -1,
    flag = 0;                 /* Passed as func to _chmod()             */

void usage(void)              /* Tell 'em they messed up                */
{
      puts("Usage: CHMOD file [file [...file] [+switches] [-switches]");
      puts("Where switches are one or more of:");
      puts("    A: Archive");
      puts("    R: Read only");
      puts("    H: Hidden");
      puts("    S: System");
      puts("File lists may be specified with \"@file_list_name\"");
      puts("With no switches, diplays current attributes.");
      puts("Displayed attributes are as above plus:");
      puts("    D: Subdirectory");
      puts("    V: Volume label");
      exit(1);
}

void setattr(char atr)        /* Set bits in attribute                  */
{
      switch (toupper(atr))
      {
      case 'A':
            atr_setmask |= FA_ARCH;
            break;
      case 'R':
            atr_setmask |= FA_RDONLY;
            break;
      case 'S':
            atr_setmask |= FA_SYSTEM;
            break;
      case 'H':
            atr_setmask |= FA_HIDDEN;
            break;
      default:
            usage();
      }
}

void clrattr(char atr)        /* Clear bits in attribute                */
{
      switch (toupper(atr))
      {
      case 'A':
            atr_clrmask &= ~FA_ARCH;
            break;
      case 'R':
            atr_clrmask &= ~FA_RDONLY;
            break;
      case 'S':
            atr_clrmask &= ~FA_SYSTEM;
            break;
      case 'H':
            atr_clrmask &= ~FA_HIDDEN;
            break;
      default:
            usage();
      }
}

void show_atr(char *path)
{
      char astr[7], *ptr;

      if (-1 == (attrib = _chmod(strupr(path), 0)))
      {
ATR_ERR:    printf("\aCHMOD: Error! (file: %s)", path);
            exit(-1);
      }
      attrib |= atr_setmask;
      attrib &= atr_clrmask;
      if (-1 == (attrib = _chmod(path, flag, attrib)))
            goto ATR_ERR;
      ptr = astr;
      *ptr++ = (char)((attrib & FA_ARCH)   ? 'A' : '.');
      *ptr++ = (char)((attrib & FA_RDONLY) ? 'R' : '.');
      *ptr++ = (char)((attrib & FA_SYSTEM) ? 'S' : '.');
      *ptr++ = (char)((attrib & FA_HIDDEN) ? 'H' : '.');
      *ptr++ = (char)((attrib & FA_DIREC)  ? 'D' : '.');
      *ptr++ = (char)((attrib & FA_LABEL)  ? 'V' : '.');
      *ptr = '\0';
      printf("%-15s %s\n", path, astr);
}

int main (int argc, char *argv[])
{
      int i, j;

      if (2 > argc)
            usage();
      for (i = 1; i < argc; ++i)    /* Build attribute masks            */
      {
            switch (*argv[i])
            {
            case '+':
                  for (j = 1; argv[i][j]; ++j)
                        setattr(argv[i][j]);
                  flag = 1;
                  break;
            case '-':
                  for (j = 1; argv[i][j]; ++j)
                        clrattr(argv[i][j]);
                  flag = 1;
                  break;
            default:
                  break;            /* Assume it's a file name          */
            }
      }
      for (i = 1; i < argc; ++i)    /* Scan filenames                   */
      {
            if (strchr("+-", *argv[i]))
                  continue;
            if ('@' == *argv[i])
            {
                  FILE *fp;
                  char buf[256], *ptr = &argv[i][1];

                  if (NULL == (fp = fopen(ptr, "r")))
                  {
                        printf("\aCHMOD: Error opening %s\n", ptr);
                        return -1;
                  }
                  while (NULL != fgets(buf, 255, fp))
                  {
                        while ('\n' == LAST_CHAR(buf))     /* Strip '\n'*/
                              LAST_CHAR(buf) = '\0';
                        show_atr(buf);
                  }
                  fclose(fp);
            }
            else  show_atr(argv[i]);
      }
      return 0;
}
