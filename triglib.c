/******************************************************/
/* Filespec  :  triglib.c triglib.h                   */
/* Date      :  February 21 1997                      */
/* Time      :  14:11                                 */
/* Revision  :  1.0C                                  */
/* Update    :                                        */
/******************************************************/
/* Programmer:  Nigel Traves                          */
/* Address   :  5 Breamer Road, Collingham, Newark,   */
/*           :  Notts, U.K.                           */
/* Post Code :  NG23 7PN                              */
/******************************************************/
/* Released to the Public Domain                      */
/******************************************************/

#include <math.h>
#include "triglib.h"

#undef  PI
#define PI              3.14159265358979323846
#define PI_Times2       6.28318530717958647692        /* PI * 2 */
#define PI_Divided_By2  1.57079632679489661923;       /* PI / 2 */

double asinh(double x)
{
      return (log(x + sqrt(x * x + 1.0)));
}

double acosh(double x)
{
      return (log(x + sqrt(x * x - 1.0)));
}

double atanh(double x)
{
      return (log((1.0 + x) / (1.0 - x)) / 2.0);
}

void normalize_radians(double *radians)
{
      while (*radians > PI)
            *radians -= PI_Times2;
      while (*radians < -PI)
            *radians += PI_Times2;
}

void normalize_degrees(double *degrees)
{
      while (*degrees > 180.0)
            *degrees -= 360.0;
      while (*degrees < -180.0)
            *degrees += 360.0;
}

void normalize_grade(double *grade)
{
      while (*grade > 200.0)
            *grade -= 400.0;
      while (*grade < -200.0)
            *grade += 400.0;
}

double radians_to_degrees(double radians)
{
      normalize_radians(&radians);
      return (radians * (180.0 / PI));
}

double radians_to_grade(double radians)
{
      normalize_radians(&radians);
      return (radians * (200.0 / PI));
}

double degrees_to_radians(double degrees)
{
      normalize_degrees(&degrees);
      return (degrees * (PI / 180.0));
}

double degrees_to_grade(double degrees)
{
      normalize_degrees(&degrees);
      return (degrees * 1.11111111111111111111);
}

double grade_to_radians(double grade)
{
      normalize_grade(&grade);
      return (grade * (PI / 200.0));
}

double grade_to_degrees(double grade)
{
      normalize_grade(&grade);
      return (grade * 0.9);
}

double sine(double angle, enum angle_type atype)
{
      switch (atype)
      {
      case RAD:
            normalize_radians(&angle);
            return sin(angle);
            break;

      case DEG:
            return sin(degrees_to_radians(angle));
            break;

      case GRAD:
            return sin(grade_to_radians(angle));
      }
}

double cosine(double angle, enum angle_type atype)
{
      switch (atype)
      {
      case RAD:
            normalize_radians(&angle);
            return cos(angle);
            break;

      case DEG:
            return cos(degrees_to_radians(angle));
            break;

      case GRAD:
            return cos(grade_to_radians(angle));
      }
}

double tangent(double angle, enum angle_type atype)
{
      switch (atype)
      {
      case RAD:
            normalize_radians(&angle);
            return tan(angle);
            break;

      case DEG:
            return tan(degrees_to_radians(angle));
            break;

      case GRAD:
            return tan(grade_to_radians(angle));
      }
}

double secant(double angle, enum angle_type atype)
{
      return (1.0 / cosine(angle, atype));
}

double cosecant(double angle, enum angle_type atype)
{
      return (1.0 / sine(angle, atype));
}

double cotangent(double angle, enum angle_type atype)
{
      return (1.0 / tangent(angle, atype));
}

double arc_sine(double x, enum angle_type out_type)
{
      switch (out_type)
      {
      case RAD:
            return asin(x);
            break;

      case DEG:
            return radians_to_degrees(asin(x));
            break;

      case GRAD:
            return radians_to_grade(asin(x));
      }
}

double arc_cosine(double x, enum angle_type out_type)
{
      switch (out_type)
      {
      case RAD:
            return acos(x);
            break;

      case DEG:
            return radians_to_degrees(acos(x));
            break;

      case GRAD:
            return radians_to_grade(acos(x));
      }
}

double arc_tangent(double x, enum angle_type out_type)
{
      switch (out_type)
      {
      case RAD:
            return atan(x);
            break;

      case DEG:
            return radians_to_degrees(atan(x));
            break;

      case GRAD:
            return radians_to_grade(atan(x));
      }
}

double arc_secant(double x, enum angle_type out_type)
{
      return arc_cosine((1.0 / x), out_type);
}

double arc_cosecant(double x, enum angle_type out_type)
{
      return arc_sine((1.0 / x), out_type);
}

double arc_cotangent(double x, enum angle_type out_type)
{
      return arc_tangent((1.0 / x), out_type);
}

double hyperbolic_sine(double angle, enum angle_type atype)
{
      switch (atype)
      {
      case RAD:
            normalize_radians(&angle);
            return sinh(angle);
            break;

      case DEG:
            return sinh(degrees_to_radians(angle));
            break;

      case GRAD:
            return sinh(grade_to_radians(angle));
      }
}

double hyperbolic_cosine(double angle, enum angle_type atype)
{
      switch (atype)
      {
      case RAD:
            normalize_radians(&angle);
            return cosh(angle);
            break;

      case DEG:
            return cosh(degrees_to_radians(angle));
            break;

      case GRAD:
            return cosh(grade_to_radians(angle));
      }
}

double hyperbolic_tangent(double angle, enum angle_type atype)
{
      switch (atype)
      {
      case RAD:
            normalize_radians(&angle);
            return tanh(angle);
            break;

      case DEG:
            return tanh(degrees_to_radians(angle));
            break;

      case GRAD:
            return tanh(grade_to_radians(angle));
      }
}

double hyperbolic_secant(double angle, enum angle_type atype)
{
      return (1.0 / hyperbolic_cosine(angle, atype));
}

double hyperbolic_cosecant(double angle, enum angle_type atype)
{
      return (1.0 / hyperbolic_sine(angle, atype));
}

double hyperbolic_cotangent(double angle, enum angle_type atype)
{
      return (1.0 / hyperbolic_tangent(angle, atype));
}

double hyperbolic_arc_sine(double x, enum angle_type out_type)
{
      switch (out_type)
      {
      case RAD:
            return asinh(x);
            break;

      case DEG:
            return radians_to_degrees(asinh(x));
            break;

      case GRAD:
            return radians_to_grade(asinh(x));
      }
}

double hyperbolic_arc_cosine(double x, enum angle_type out_type)
{
      switch (out_type)
      {
      case RAD:
            return acosh(x);
            break;

      case DEG:
            return radians_to_degrees(acosh(x));
            break;

      case GRAD:
            return radians_to_grade(acosh(x));
      }
}

double hyperbolic_arc_tangent(double x, enum angle_type out_type)
{
      switch (out_type)
      {
      case RAD:
            return atanh(x);
            break;

      case DEG:
            return radians_to_degrees(atanh(x));
            break;

      case GRAD:
            return radians_to_grade(atanh(x));
      }
}

double hyperbolic_arc_secant(double x, enum angle_type out_type)
{
      return hyperbolic_arc_cosine((1.0 / x), out_type);
}

double hyperbolic_arc_cosecant(double x, enum angle_type out_type)
{
      return hyperbolic_arc_sine((1.0 / x), out_type);
}

double hyperbolic_arc_cotangent(double x, enum angle_type out_type)
{
      return hyperbolic_arc_tangent((1.0 / x), out_type);
}

double sech(double x)
{
      return (2.0 / (exp(x) + exp(-x)));
}

double csch(double x)
{
      return (2.0 / (exp(x) - exp(-x)));
}

double coth(double x)
{
      return (exp(-x) / (exp(x) - exp(-x)) * 2.0 + 1.0);
}

double acoth(double x)
{
      return (log((x + 1.0) / (x - 1.0)) / 2.0);
}
