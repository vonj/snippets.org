/****************************************************************/
/*                                                              */
/*      Collect file statistics                                 */
/*                                                              */
/*      Public domain demo program for analyzing encrypted      */
/*      files. By: Bob Stout                                    */
/*                                                              */
/****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "errors.h"           /* For cant()     */

#ifdef __WATCOMC__
 #pragma off (unreferenced);
#endif
#ifdef __TURBOC__
 #pragma argsused
#endif

main(int argc, char *argv[])
{
      int i, ch, hist = 0;
      long n = 0L;
      double mean = 0., stdev = 0., ftmp;
      static unsigned bins[256];
      FILE *infile;

      infile = cant(argv[1], "rb");
      while (!feof(infile))
      {
            if (EOF == (ch = fgetc(infile)))
                  break;
            bins[ch] += 1;
            ++n;
      }
      fclose(infile);
      for (i = 0; i < 256; ++i)
      {
            mean += (double)(bins[i]);
            if (bins[i])
                  ++hist;
      }
      mean /= 256.;
      for (i = 0; i < 256; ++i)
      {
            ftmp = (double)(bins[i]) - mean;
            stdev += (ftmp * ftmp);
      }
      ftmp  = stdev / 255.;
      stdev = sqrt(ftmp);
      printf("%ld Characters were read from %s\n"
            "There are an average of %f occurrences of each character\n"
            "%d Characters out of 256 possible were used\n"
            "The standard deviation is %f\n"
            "The coefficient of variation is %f%%\n",
            n, argv[1], mean, hist, stdev, (100. * stdev) / mean);
      return EXIT_SUCCESS;
}
