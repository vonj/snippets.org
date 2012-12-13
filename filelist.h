/*
**  FILELIST.H
*/

#ifndef FILELIST_H
#define FILELIST_H

#include <stdlib.h>
#include <string.h>
#include "dirport.h"

typedef struct _filelist {
      DOSFileData file;
      struct _filelist *next;
} FILELIST;

#define LIST_SIZE sizeof(FILELIST)

int Get_FileList(FILELIST *list, char **argv, int agrc, int attr);
void Free_FileList(FILELIST *list);

#endif /* FILELIST_H */
