/** BODYMASS.C
 *  Calculate body mass index (BMI) for given height and weight.
 *  According to U.S. federal guidelines, a BMI of 24 or less is
 *  desirable. Anything higher is considered overweight.
 *  Donated to the public domain, October 17, 1996.
 **/

short BodyMassIndex(unsigned short height, unsigned short weight)
{
      /* Returns the Body Mass Index (BMI) for height in inches and
      ** weight in pounds. BMI is weight in kilograms divided by height
      ** in meters squared. Returns -1 if invalid height entered.
      */

      /* Define the metric conversion constants... */

#define LBperKG    2.2046f
#define INCHperM  39.37f
#define CFACTOR   ((INCHperM * INCHperM) / LBperKG)

      /* Make sure height is not 0 and is 'reasonable' (100?) */

      if(height < 1 || height > 100)
            return -1;

      return (short) ((((float) weight * CFACTOR) / (height * height)) +
                      0.5f);
}

#ifdef TEST

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
      short bmi, h, w;

      if(argc < 3)
      {
            printf("Usage: bodymass height-in-inches weight-in-pounds\n");
            return EXIT_FAILURE;
      }
      h = atoi(argv[1]);
      w = atoi(argv[2]);
      if (h < 20 || h > 100)
      {
            printf("Height %d out of range!\n", h);
            return EXIT_FAILURE;
      }
      if (w < 20)
      {
            printf("Weight %d out of range!\n", w);
            return EXIT_FAILURE;
      }
      bmi = BodyMassIndex(h, w);
      printf("The Body Mass Index for height %d inches "
             "and weight %d pounds is %d\n", h, w, bmi);
      if (bmi < 25)
            printf("Congratulations! Your index is within the "
                   "recommended range.\n");
      else
      {
            while((bmi = BodyMassIndex(h, --w)) > 24)
                  ;
            printf("Your index is above the recommended level of 24.\n"
                   "To reach that level your weight must drop to %d "
                   "pounds.\n", w);
      }
      return EXIT_SUCCESS;
}

#endif
