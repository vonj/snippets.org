/*====================================================================

    _MSC_VER        Microsoft C 6.0 and later
    __SC__          Symantec C++ and Digital Mars C++
    __ZTC__         Zortech C and C++ (also Symantec & Digital Mars)
    __GNUC__        Gnu C

    Revised:

    Original   Scott Ladd   Now obsolete
    14-Sep-93  Fred Cole    Moved MK_FP() macro to end of file to avoid
                            redefinition error when dos.h gets included
                            at the in/outport definitions for __TURBOC__
    15-Sep-93  Thad Smith   Add conditional code for TC 2.01
                            Fix findfirst/findnext support for ZTC 3.0
    15-Oct-93  Bob Stout    Revise find first/next support
    02-Dec-93  David Nugent Additions for findfirst/findnext support for
                            MSC6 (& 7) for OS/2
                            Added FIND_END macro for use under OS/2 to
                            be nice about closing the directory handle
                            DOSFileData members should be accessed via
                            the new ff_*() macros for portability
                            Note: use -DOS2 when compiling under OS/2
    03-Apr-94  Bob Stout    Add Power C support, FAR
    19-Aug-95  Bob Stout    Add NEAR, PASCAL, CDECL, and portable attributes
    06-Sep-95  Phi Nguyen   Add DOSFileTime, DOSFileDate, & supporting macros
               Bob Stout    (ff_yr/mo/day(), ff_hr/min/tsec())
    08-Sep-95  Bob Nelson   Add __GNUCC__ and PAK macro
    21-Sep-95  Bob Stout    Renamed to PC-PORT.H, revised directory stuff
    25-Sep-95  Bob Stout    Split out EXTKWORD.H
    26-Sep-95  Jerry Coffin Added Win32 support.
    21-Oct-95  Bob Stout    Resolve struct dirent incompatibilities,
                            move port I/O macros to PCHWIO.H,
                            move MK_FP() cover to SNIPTYPE.H,
                            rename to DIRPORT.H(!!!)
    15-May-96  David Nugent Added 32-bit OS/2 support (Watcom/emx)
    04-Jul-96  Bob Stout    Fixed attribute redefinition problems w/ TC 3.x
    28-Jul-96  Bob Stout    Fixed error in definition of ff_sec() in DOS
    20-Aug-96  Bob Stout    Eliminate Win32 conflicts
    05-aug-01  Bob Stout    Remove support for everything but DMC & gcc,
                            reorganize.
======================================================================*/


/* prevent multiple inclusions of this header file */

#if !defined(DIRPORT__H)
#define DIRPORT__H


#include "extkword.h"         /* Extended keywords header   */

/* make sure the structures are packed on byte boundaries */

#if defined(_MSC_VER) || defined(__SC__)
#pragma pack(1)
#endif

#ifdef __GNUC__
 #define PAK      __attribute__ ((aligned (8)))
#else
 #define PAK
#endif

/*
**  Structures to access dates and times
*/

typedef struct {
      unsigned tsecs:   5;
      unsigned mins:    6;
      unsigned hours:   5;
} DOSFileTime;

typedef struct {
      unsigned day:     5;
      unsigned month:   4;
      unsigned year:    7;
} DOSFileDate PAK;

/*
**  Correct file attributes
*/

#if defined(_WIN32) || defined(WIN32)
 #define WIN32_LEAN_AND_MEAN
 #define NOGDI
 #define NOSERVICE
 #undef INC_OLE1
 #undef INC_OLE2
 #if defined(__SC__)
  #include <win32/windows.h>
 #else     /* Assume Mingw32 */
  #include <windows.h>
  #include <io.h>
 #endif
 #include <dos.h>    /* #define's most _A_* file attribute macros    */

 #define _A_VOLID   0
 #define _A_ANY     _A_NORMAL | _A_RDONLY | _A_HIDDEN | _A_SYSTEM | \
                    _A_SUBDIR | _A_ARCH

 #define ff_name(x)     (x)->file.cFileName
 #define ff_size(x)     (x)->file.nFileSizeLow
 #define ff_attr(x)     (x)->file.dwFileAttributes
 typedef struct DIRENT_ {
       WIN32_FIND_DATA file PAK;
       HANDLE handle PAK;
 } DOSFileData;

 WORD ff_date(DOSFileData *f);
 WORD ff_yr(DOSFileData *f);
 WORD ff_mo(DOSFileData *f);
 WORD ff_day(DOSFileData *f);
 WORD ff_time(DOSFileData *f);
 WORD ff_hr(DOSFileData *f);
 WORD ff_min(DOSFileData *f);
 WORD ff_tsec(DOSFileData *f);

 int FIND_FIRST(char *spec, unsigned attr, DOSFileData *ff);
 int FIND_NEXT(DOSFileData *ff);
 int FIND_END(DOSFileData *ff);
#elif defined(__MSDOS__) || defined(MSDOS)
 #define _A_ANY  _A_NORMAL | _A_RDONLY | _A_HIDDEN | _A_SYSTEM | _A_VOLID | \
                 _A_SUBDIR | _A_ARCH

#elif defined(__unix__)

#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>

 typedef struct DIRENT_ {
       DIR *dirp PAK;
       struct {
             char              attrib PAK;
             DOSFileTime       time PAK;
             DOSFileDate       date PAK;
             long              d_size PAK;
             char              d_name[FILENAME_MAX] PAK;
             char              d_fname[FILENAME_MAX] PAK;
		 char              d_path[FILENAME_MAX] PAK;
		 unsigned		 d_attr PAK;
       } f;
 } DOSFileData;

 #define ff_name(x)     (x)->f.d_name
 #define ff_size(x)     (x)->f.d_size
 #define ff_attr(x)     (x)->f.attrib
 #define ff_date(x)     (x)->f.date
 #define ff_yr(x)       (x)->f.date.year
 #define ff_mo(x)       (x)->f.date.month
 #define ff_day(x)      (x)->f.date.day
 #define ff_time(x)     (x)->f.time
 #define ff_hr(x)       (x)->f.time.hours
 #define ff_min(x)      (x)->f.time.mins
 #define ff_sec(x)      (x)->f.time.tsecs

 #define _A_NORMAL      0
 #define _A_RDONLY      1
 #define _A_HIDDEN      2
 #define _A_SYSTEM      4
 #define _A_VOLID       8
 #define _A_SUBDIR      16
 #define _A_ARCH        32
 #define _A_EXECUTABLE  64
 #define _A_ANY _A_NORMAL | _A_RDONLY | _A_HIDDEN | _A_SYSTEM | _A_VOLID | \
                _A_SUBDIR | _A_ARCH | _A_EXECUTABLE
#endif

/*--------------------------------------------------------------------
    Directory search macros and data structures

    DOSFileData         MS-DOS file data structure
    DOSFileDate         MS-DOS file date structure
    DOSFileTime         MS-DOS file time structure
    FIND_FIRST          MS-DOS function 0x4E -- find first matching spec
    FIND_NEXT           MS-DOS function 0x4F -- find subsequent files
----------------------------------------------------------------------*/

/*
**  Use this structure in place of compiler-defined file structure.
**  Always use the ff_xxxx() macros to access structure members.
*/

#if  !defined(WIN32) && !defined(_WIN32) && !defined(__unix__) /* Not Win32 or Unix - i.e. DOS */

 typedef struct DIRENT_ {
      char              reserved[21];
      char              attrib;
      DOSFileTime       time;
      DOSFileDate       date;
      long              d_size;
      char              d_name[13];
      } DOSFileData;

 #define ff_name(x)     (x)->d_name
 #define ff_size(x)     (x)->d_size
 #define ff_attr(x)     (x)->attrib
 #define ff_date(x)     (x)->date
 #define ff_yr(x)       (x)->date.year
 #define ff_mo(x)       (x)->date.month
 #define ff_day(x)      (x)->date.day
 #define ff_time(x)     (x)->time
 #define ff_hr(x)       (x)->time.hours
 #define ff_min(x)      (x)->time.mins
 #define ff_sec(x)      (x)->time.tsecs
#endif

/* include proper header files and create macros */

#if defined (_MSC_VER) || defined(__SC__)
 #if  !(defined(_WIN32) || defined(WIN32))
  #include "dos.h"
  #define FIND_FIRST(spec,attr,buf) _dos_findfirst(spec,attr, \
        (struct find_t *)buf)
  #define FIND_NEXT(buf) _dos_findnext((struct find_t *)buf)
  #define FIND_END(buf)
 #endif
#endif

/* set structure alignment to default */

#if defined (_MSC_VER) || defined(__SC__)
 #pragma pack()
#endif

#endif /* DIRPORT__H */
