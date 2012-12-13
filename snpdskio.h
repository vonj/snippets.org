/*
**  Header file for SNIPPETS direct disk I/O functions
*/

#ifndef SNPDKSIO__H
#define SNPDKSIO__H

#include <stddef.h>
#include <stdio.h>
#include "extkword.h"

/*
**  File: ABSDISKC.C and ABSDISK.ASM
*/

int CDECL absdisk(unsigned char  function,
                  unsigned short drive,               /* 0 = A:, etc.   */
                  size_t         number_of_sectors,
                  size_t         starting_sector,
                  void *         sector_buffer);

int AbsDiskRead(unsigned short drive,
                size_t         num_of_sectors,
                size_t         sector,
                void          *ptr);

int AbsDiskWrite(unsigned short drive,
                size_t         num_of_sectors,
                size_t         sector,
                void *ptr);

/*
**  File: HUGEREAD.C
*/

long hugeread(int fh, unsigned char FAR *buf, long size);
long hugewrite(int fh, unsigned char FAR *buf, long size);
long hugefread(FILE *fp, char FAR *buf, long size);
long hugefwrite(FILE *fp, char FAR *buf, long size);


#endif /* SNPDKSIO__H */
