/*
**  UNISTD.H - Posix-compliant header for porting code to DOS
**             systems which lack this standard header file.
**
**  public domain by Bob Stout
*/

#ifndef __UNISTD_H      /* Use same macro as Symantec to avoid collision */
#define __UNISTD_H


#include <io.h>
#include <process.h>
#include <stdio.h>
#include "stat.h"

#ifndef F_OK
 #define F_OK    0      /* File exists          */
 #define X_OK    1      /* File is executable   */
 #define W_OK    2      /* File is writable     */
 #define R_OK    4      /* File is readable     */
#endif

#undef dirent
#undef DIR

#if !defined(__TURBOC__)
 #include <direct.h>
#else
 #include <dir.h>
#endif

#define dirent DIRENT_
#define DIR DIR_


#endif /* __UNISTD_H */
