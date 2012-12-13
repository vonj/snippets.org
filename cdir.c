/*
**                          CDIR.C
**
** Written By:    Lynn R. Lively
** Date Written:  9/18/91
**
** Purpose: To provide a change directory facility that will cross
**          drive/partition boundaries. Never did understand why
**          MSDOS cd wouldn't do this already.
**
**-----------------------------------------------------------------
** I hereby place this work into the Public Domain. It may be used
** for any legal purpose public or private. Use this material at
** your own risk. I accept no responsibility for the accuracy or
** usability of the information contained herein. Neither do I
** accept liability for any possible damage caused by use of this
** material. However, should you have a problem, question, or
** suggestion I would be glad to help in any way that I can. You
** can reach me at (H) 713-893-7875 or (W) 713-591-6611 x 149.
**-----------------------------------------------------------------
*/

/*
**                          Change History
**
**  Rev #   Date       By      Description of change
**  1.00  | 09/18/91 | LRL | Original Version
**  1.01  | 09/18/91 | RBS | Added MSC, ZTC support for SNIPPETS
**-----------------------------------------------------------------
** Directory of initials:
** Initials          Name
** LRL        Lynn R. Lively
** RBS        Bob Stout
*/


#include <stdio.h>
#include <string.h>

#if defined(__TURBOC__) || defined(__POWERC)
 #include <dir.h>
#else
 #include <dos.h>
 #include <direct.h>

 #ifdef __ZTC__
  #define _dos_getdrive(d) dos_getdrive(d)
  #define _dos_setdrive(d,m) dos_setdrive(d,m)
 #endif

 #if defined(__ZTC__) || defined(__WATCOMC__)
  #define drive_t unsigned
 #else
  #define drive_t int
 #endif

 #if defined(_MSC_VER) || defined(__WATCOMC__)
 drive_t getdisk(void)
 {
       drive_t drive;

       _dos_getdrive(&drive);
       return drive - 1;
 }

 drive_t setdisk(drive_t drive)
 {
       drive_t max_drives;

       _dos_setdrive(drive + 1, &max_drives);
       return max_drives - 1;
 }
 #endif
#endif

main (int argc, char * argv[])
{
      int d;
      int max_d;

      char wk_str[128];

      if (argc > 1)
      {
            strupr (argv[1]);
            if (argv[1][1] == ':')
            {
                  /*
                  ** Find out what the maximum drive number can be.
                  */

                  max_d = getdisk ();
                  max_d = setdisk (max_d);

                  d = argv[1][0] - 'A';
                  if (d < max_d)
                  {
                        /*
                        ** If the drive specification was valid position
                        ** to it and then do a change directory.
                        */

                        setdisk (d);
                        chdir (argv[1]);
                  }
                  else
                  {
                        puts ("Invalid drive specification");
                        return -1;
                  }
            }
            else
            {
                  /*
                  ** If the argument has no drive spec just do a regular
                  ** change directory.
                  */

                  chdir (argv[1]);
            }
      }
      else
      {
            /*
            ** If no arguments are passed, return the current working
            ** directory path just like MSDOS cd does.
            */

            puts (getcwd (wk_str, sizeof (wk_str)));
      }
      return 0;
}
