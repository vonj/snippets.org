/*
**          MFL.LIB  function source code
**  Copyright 1986, S.E. Margison
**
**  Modified 1989-93; Copyright Robert B.Stout dba MicroFirm as part of the
**  MicroFirm Function Library (MFL).
**                                                                      
**  The user is granted a free limited license to use this source file  
**  to create royalty-free programs, subject to the terms of the        
**  license restrictions specified in the LICENSE.MFL file.             
**                                                                      
**  FUNCTION: fopenp, fopend, fopeng
**
**  Perform fopen() in path, stated environment, or both.
**  Current directory is always attempted first.
**  For fopeng() environment variable is searched before path
**  DRIVE and DIRECTORY strings may NOT be specified as part
**  of the filename.
*/

#include <stdio.h>
#include "snipfile.h"

FILE *fopenp(char *name, char *mode)
{
        char *newname;

        if (NULL != (newname = pexists(name)))
                return(fopen(newname, mode));
        else    return NULL;
}

FILE *fopend(char *name, char *mode, char *envar)
{
        char *newname;

        if (NULL != (newname = dexists(name, envar)))
                return(fopen(newname, mode));
        else    return NULL;
}

FILE *fopeng(char *name, char *mode, char *envar)
{
        char *newname;

        if (NULL != (newname = gexists(name, envar)))
                return(fopen(newname, mode));
        else    return NULL;
}
