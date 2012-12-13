/*
** CHECKEXE.C - checksum protection for executable files
**
** by: Bob Jarvis
*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "crc.h"

static struct {
      unsigned char marker[16];
      unsigned long checksum;
} marker_struct = {"CHECKEXE MARKER",0L};

void checkexe(char *fname)
{
      FILE *fptr;
      unsigned int c;
      int first_time = 0, i;
      char buffer[14];
      unsigned long chksum = 0L;
      unsigned long l;
      unsigned long marker_offset;
      unsigned char *charptr;
      time_t tm;

      fptr = fopen(fname,"r+b");
      if(fptr == NULL)
      {
            fprintf(stderr,"checkexe : unable to open input file '%s'\n",
                  fname);
            exit(99);
      }

      setvbuf(fptr, NULL, _IOFBF, 32767);    /* try to get a 32K buffer */

   /* 
    * If this is the first time the check has been run, scan the entire file
    * to find the marker.  Otherwise proceed.
    */

      if(marker_struct.checksum == 0L)
      {
            first_time = 1;

            c = fgetc(fptr);
            while(!feof(fptr))
            {
                  if(c == (unsigned int)marker_struct.marker[0])
                  {
                        fread(buffer,sizeof(buffer),1,fptr);
                        if(memcmp(buffer,&marker_struct.marker[1],
                              sizeof(buffer)) == 0)
                        {
                              marker_offset = ftell(fptr) + 1L;
                              break;
                        }
                        fseek(fptr,-13L,SEEK_CUR);
                  }

                  c = fgetc(fptr);
            }

            if(feof(fptr))
            {
                  fprintf(stderr,"checkexe : unable to locate marker\n");
                  exit(99);
            }

      /* Change the marker field to random values */

            tm = time(NULL);

            srand((unsigned int)tm);

            for(i = 0 ; i < sizeof(marker_struct.marker) ; ++i)
                  marker_struct.marker[i] = (unsigned char) rand();

            fseek(fptr,marker_offset - sizeof(marker_struct.marker),SEEK_SET);

            fwrite(marker_struct.marker,sizeof(marker_struct.marker),1,fptr);
      }

   /* Calculate the checksum for the entire file */

      rewind(fptr);

      c = fgetc(fptr);
      for(l = 0 ; !feof(fptr) ; ++l)
      {
            chksum += (unsigned long)c;
            c = fgetc(fptr);
      }

      if(first_time)
      {
            marker_struct.checksum = chksum;
            fseek(fptr,marker_offset,SEEK_SET);
            fwrite(&marker_struct.checksum,sizeof(unsigned long),1,fptr);
      }
      else
      {
            charptr = (unsigned char*) &marker_struct.checksum;

            for(i = 0 ; i < sizeof(marker_struct.checksum) ; ++i)
                  chksum -= (unsigned long)(charptr[i]);

            if(chksum != marker_struct.checksum)
            {
                  fprintf(stderr, "\acheckexe : %s has been altered, "
                        "possibly by a virus\n", fname);
                  exit(99);
            }
      }

      fclose(fptr);
      return;
}

#ifdef TEST

#ifdef __WATCOMC__
 #pragma off (unreferenced);
#endif
#ifdef __TURBOC__
 #pragma argsused
#endif

main(int argc, char *argv[])
{
      checkexe(argv[0]);
      return EXIT_SUCCESS;
}

#endif /* TEST */
