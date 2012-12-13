// Ansi/AVT0+ file viewer
// public domain
// by David Nugent <davidn@unique.blaze.net.au>
// 3:632/348@fidonet
// V1.1 David Nugent  Added cmdline wildcard expansion using
//                    SNIPPETS dirport.h
//                    Added avatar/0+ file support.

#define PROGRAM   "showansi"
#define VERSION   "V1.10-PD"

#if defined(__WATCOMC__)
  // Turn off warnings for resolution of temp object
#pragma warning 604 9
#pragma warning 595 9
#endif

#include <fstream.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "ansisys.hpp"
#include "avatar.hpp"
extern "C"
{ // SNIPPETS portable directory routines
  #include "dirport.h"
}

int
main(int argc, char * argv[])
{
  if (argc == 1)
    cout << PROGRAM "; ANSI & AVT/0+ file interpreter; " VERSION "\n"
    "This program is in the public domain. Compiled " __DATE__ "\n"
    "by David Nugent <davidn@unique.blaze.net.au> 3:632/348@fidonet\n\n"
    "  usage: " PROGRAM " [-p] [-a] [-f] file1 [file2 ...]\n"
    "    Displays file1[,file2..] to the screen handling ANSI/Avatar\n"
    "    -p     Toggles screen pausing (default=ON)\n"
    "    -a     Toggles file type (default=ANSI)\n"
    "    -f     Toggles flush after every character (default=OFF)\n"
    << endl;
  else
  {
    char pause = 1;
    char done = 0;
    char doavt0 = 0;
    char doflush = 0;
    scrinterp * interpreter = 0;
    video V;

    for (int carg = 0 ; !done && ++carg < argc ;)
    {
      char * argp = argv[carg];

      if (*argp == '-' || *argp == '/')
      {
        ++argp;

        switch(tolower(*argp))
        {
          case 'p':     // -p toggles pause between files
            pause = !pause;
            break;
          case 'a':     // -a toggles ansi/avt0 display
            doavt0 = !doavt0;
            delete interpreter;
            interpreter = 0;
            break;
          case 'f':     // -f flush each character (slows output)
            doflush = !doflush;
            break;
        }
      }
      else
      {
        DOSFileData ff;
        char * p;
        char tmpstr[128] = "";

        while ((p = strchr(argp, '/')) != 0)
          *p = '\\';
        p = strrchr(argp, '\\');
        if (p == 0)
          *(p = tmpstr) = '\0';
        else
        {
          ++p;
          int n = p - argp;
          strncpy(tmpstr, argp, n);
          p = tmpstr + n;
        }

        if (FIND_FIRST(argp, _A_NORMAL, &ff) != 0)
          cerr << "can't find file: " << argp << endl;
        else do
        {
          argp = tmpstr;
          strcpy(p, ff_name(&ff));    // Append it to path, if any

          ifstream ansifile(argp, ios::in|ios::binary);

          if (!ansifile)
            cerr << "showansi: can't open file " << argp
                 << ':' << strerror(errno) << endl;
          else
          {
            V.setattr(7);
            V.cls();
            V.gotoxy(0, 0);
            if (interpreter == 0)   // Nothing allocated yet
              interpreter = (doavt0) ? (scrinterp *)new avatar(V) :
                                       (scrinterp *)new ansisys(V);
            interpreter->flusheverychar(doflush);
            interpreter->display(ansifile);
            if (pause)
            {
              cell_t x, y;
              V.maxxy(x, y);
              V.gotoxy(0, --y);
              V.setattr(15);
              V.put(argp);
              V.put(" -- press any key to continue --");
              unsigned short k = V.getkey();
              V.gotoxy(0, y);
              V.repchr(' ', x);
              V.gotoxy(0, --y);
              if ((k & 0xff) == 27) // ESC
              {
                ++done;
                break;
              }
            }
          }
        }
        while (FIND_NEXT(&ff)==0);
        FIND_END(&ff);
      }
    }
    delete interpreter;
  }
  return 0;
}

