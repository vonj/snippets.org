/*  tiresize.c -  Quick & dirty 'C' program to calculate the actual
 *                dimensions (in English Units) of an automobile tire
 *                given the size specification in the form: 265/75-16
 *
 *                where:  265  is width of tire in millimeters
 *                         75  is ratio of sidewall height to width
 *                             (or aspect ratio)
 *                         16  is the diameter of wheel (rim) in inches
 *
 *                Furthermore, this program will calculate the tire
 *                revolutions per mile driven.
 *
 *        Author: Michael K. Demski
 * Email Address: 75324.3574@compuserve.com
 *          Date: 09/08/96
 *        Notice: Donated to the Public Domain, use at your own risk.
 *   Inspired by: http://www.powerdog.com/tiresize.cgi
 */

#include <stdio.h>
#include <stdlib.h>

#define MY_PI 3.14159

int main(int argc, char *argv[])
{
      float w,    /* tire width in millimeters                        */
            ar,   /* ratio of sidewall height to width (aspect ratio) */
            wd,   /* wheel diameter in inches                         */
            sh,   /* tire sidewall height                             */
            d,    /* total diameter of (wheel + tire)                 */
            c,    /* total circumference                              */
            rev;  /* revolutions per mile                             */

      if (4 != argc)
      {
            puts("\nUsage: TIRESIZE <width> <aspect ratio> <diameter>");
            puts("\n       Example:  For a tire of size: 265/75-16\n");
            puts("       Command line:   TIRESIZE 265 75 16\n");

            return EXIT_FAILURE;
      }
      else
      {
            w  = atof(argv[1]);
            ar = atof(argv[2]);
            wd = atof(argv[3]);
      }

      if (ar > 0.0)                /* ensure no divide by zero error      */
      {
            sh = w * (ar / 100.0); /* tire sidewall height in millimeters */
      }
      else
      {
            puts("Invalid input, please try again.");
            return EXIT_FAILURE;
      }

      sh /= (10.0 * 2.54);       /* sidewall height to inches (2.54cm/in) */
      d   = (sh * 2.0) + wd;     /* total diameter of "wheel + tire" (in) */
      c   = 2 * MY_PI * (d / 2.0);      /* total circumference (in)       */
      w  /= (2.54 * 10.0);       /* convert width to inches               */

      if (c > 0.0)               /* ensure no divide by zero error        */
      {
            rev = (5280.0 * 12.0) / c;       /* revolutions per mile      */
      }
      else
      {
            puts("Invalid input, please try again.");
            return EXIT_FAILURE;
      }

      printf("\n          Tire Width: %7.2f in\n", w);
      printf("     Sidewall Height: %7.2f in\n", sh);
      printf("       Tire Diameter: %7.2f in\n", d);
      printf("       Circumference: %7.2f in\n", c);
      printf("Revolutions per mile: %7.2f\n", rev);

      printf("\nA tire: (%s/%.0f-%.0f) is ", argv[1], ar, wd);
      printf("(%.2f) inches tall and (%.2f) inches wide.\n", d, w);

      return EXIT_SUCCESS;
}
