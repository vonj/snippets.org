/*
** UPDATE.H - Header for UPDATE.C
*/

#ifndef UPDATE__H
#define UPDATE__H

#include <io.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sniptype.h"

Boolean_T update_file(char *szFname, char *szOther);

#define ren2bak(x) update_file(x, NULL)

#endif /* UPDATE__H */
