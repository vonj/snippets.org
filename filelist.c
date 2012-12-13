/*
** FILELIST.C - Creates a link list of files and returns number of
**              files found.
**
** public domain by Phi Nguyen
*/

#include "filelist.h"

/*
** Free_FileList - This function simply frees each linked list item.
*/

void Free_FileList(FILELIST *p)
{
      FILELIST *next = p->next;

      while (NULL != next)
      {
            p->next = next->next;
            free(next);
            next = p->next;
      }
}

/*
** Get_FileList - This function creates a linked list of input source
**                files.  Wildcard specifications are accommodated.
*/

int Get_FileList(FILELIST *list, char **argv, int argc, int attr)
{
      int          i = 0,
                   nCount = 0;
      DOSFileData  file;
      FILELIST    *base = list,
                  *node;

      for ( ; i < argc; i++)
      {
            if (!FIND_FIRST(argv[i], attr, &file)) do
            {
                  if (NULL == (node =(FILELIST *)malloc(LIST_SIZE)))
                  {
                        Free_FileList(list);
                        list = NULL;
                        return 0;
                  }
                  base->file = file;
                  base->next = node;
                  node->next = NULL;
                  base = node;
                  nCount++;
            } while (!FIND_NEXT(&file));
      }
      return nCount;
}

#ifdef TEST

#include <stdio.h>
#define _A_FILE (_A_NORMAL|_A_RDONLY|_A_HIDDEN|_A_SYSTEM|_A_ARCH)

main(int argc, char *argv[])
{
      FILELIST  list,
               *plist = &list;
      int       i,
                nFiles;

      if (argc < 2)
      {
            puts("Usage: FILELIST filespec ....");
            return EXIT_FAILURE;
      }

      nFiles = Get_FileList(plist, argv + 1, argc - 1, _A_FILE);
      if (nFiles >= 0)
      {
            for (i = 0; i < nFiles; i++)
            {
                  printf("%-12s %8li  %2u-%02u-%02u  %2u:%02u\n",
                        ff_name(&plist->file), ff_size(&plist->file),
                        ff_mo(&plist->file), ff_day(&plist->file),
                        ff_yr(&plist->file) + 80, ff_hr(&plist->file),
                        ff_min(&plist->file));
                  plist = plist->next;
            }
            Free_FileList(&list);
      }
      else
      {
            puts("Get_FileList() failed");
            return EXIT_FAILURE;
      }
      return EXIT_SUCCESS;
}

#endif /* TEST */
