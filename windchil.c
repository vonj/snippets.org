/*
**  Wind Chill for exposed human skin, expressed as a function of wind
**  speed in Miles per Hour and temperature in degrees Fahrenheit.
**
**  Public domain from numerous published references.
*/

#include <math.h>
#include "windchil.h"
 
double wind_chill(int wind_speed, int temp)
{
      if (4 > wind_speed)
            return (double)temp;
      else
      {
            return (((10.45 + (6.686112 * sqrt((double) wind_speed))
                  - (.447041 * wind_speed)) / 22.034 * (temp - 91.4)) + 91.4);
      }
}
