/*
** KILLFF.C - A program was written to strip out all the Form Feeds
** in text files.
**
** Public domain by Erik VanRiper, 12/22/91
** Modified by Bob Stout, 17 Feb 93
**
** Reads a text file and makes a duplicate with NO Form Feed
** characters! The default action is to create a duplicate without
** Form Feeds, then remove the original and rename the duplicate,
** although an explicit output file name may be specified.
**
** Form Feed characters are replaced with newline characters ('\n').
** Since ANSI mandates that fwrite() will translate newlines when
** a stream is opened in text (non-binary) mode, these will appear
** in the output file in a format appropriate to the implementation,
** e.g. CRLF pairs on PC's.
**
** Usage: KILLFF filename [newname]
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BSIZ 32768U             /* max size of read/write buffer */

main(int argc, char *argv[])
{
   FILE *in, *out;              /* input and output files        */
   char name[80],               /* name of file to be fixed      */
        temp[80],               /* output file name              */
        *buf;                   /* buffer we will use to write   */
   size_t bad,                  /* check to see if write ok      */
          num;                  /* number of bytes read          */
   int retval  = EXIT_SUCCESS,  /* return value                  */
       tmpflag = 0;             /* non-zero if tmpnam() used     */

   printf("\nKILL FORM FEEDS by Erik VanRiper & Bob Stout\n\n");

   if(argc < 2)                              /* usage info       */
   {
      puts("Usage: KILLFF input_file [output_file]");
      puts("\nIf no output file is given, the input file will "
           "be replaced.");
      return retval;                         /* return to OS     */
   }

   strcpy(name,argv[1]);                     /* input filename   */
   if(argc == 3) strcpy(temp,argv[2]);       /* outfile name     */
   else
   {
      tmpnam(temp);
      tmpflag = -1;
   }

   if((in = fopen(name,"r")) == NULL)        /* Open in file     */
   {
      printf("\nCan't Open Input File %s",name);
      return (retval = EXIT_FAILURE);        /* return to OS     */
   }
   if((out = fopen(temp,"wt")) == NULL)      /* open out file    */
   {
      printf("\nCan't Open Output File %s",temp);
      fclose(in);                            /* close in file    */
      return (retval = EXIT_FAILURE);        /* return to OS     */
   }

   if((buf = malloc(BSIZ)) == NULL)     /* malloc a large buffer */
   {
      printf("\nOut of memory\n");
      return (retval = EXIT_FAILURE);        /* return to OS     */
   }

   printf("Input file: %s Output file: %s\n",
      name,tmpflag ? name : temp);

   /* read in file while chars to read */

   while (0 < (num = fread(buf,sizeof(char),BSIZ,in)))
   {
      size_t i;

      for (i = 0; i < num; ++i)              /* look for FF      */
         if ('\f' == buf[i])
            buf[i] = '\n';                  /* change to newline */

      bad=fwrite(buf,sizeof(char),num,out);  /* write out buf    */
      if(bad != num)                         /* error            */
      {
         printf("\nCan't Write to %s ", temp);
         retval = EXIT_FAILURE;              /* return to OS     */
         break;
      }
   }
   fclose(in);                               /* close in file    */
   fclose(out);                              /* close out file   */
   free(buf);                                /* free memory      */
   if (tmpflag)
   {
      if (remove(name))
      {
         printf("Can't rename %s\n", name);
         printf("Converted file is named %s\n", temp);
      }
      else
         rename(temp, name);
   }
   printf("\nDone!");                        /* Finished         */
   return retval;                            /* return to OS     */
}
/*

List this source file to test this program!

New page

New page

All done

*/
