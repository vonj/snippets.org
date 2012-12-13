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

#ifndef TRIGLIB_H
#define TRIGLIB_H

/********************************************************************/
/* This library is concerned entirely with angles in general and    */
/* trigonometric functions in particular.                           */
/********************************************************************/

enum angle_type {RAD, DEG, GRAD};

/********************************************************************/
/* The following three routines 'normalize' the supplied angle to   */
/* be within limits appropiate for the trigonemetric routines.      */
/* normalize_radians ensures that the supplied angle is between -PI */
/* and +PI, normalize_degrees between -180.0 and +180.0 and         */
/* normalize_grade between -200.0 and +200.0.  NOTE - ALL the       */
/* normal trigonometric functions normalize the angle before use,   */
/* and the inverse functions after.                                 */
/********************************************************************/

void normalize_radians(double *radians);
void normalize_degrees(double *degrees);
void normalize_grade(double *grade);

/********************************************************************/
/* These six routines enable conversion, of angles, between         */
/* radians, degrees and grade.  NOTE there is no need to normalize  */
/* the angle to be converted before calling any of these routines   */
/* as they all call the appropriate normalisation routine.          */
/********************************************************************/

double radians_to_degrees(double radians);
double radians_to_grade(double radians);
double degrees_to_radians(double degrees);
double degrees_to_grade(double degrees);
double grade_to_radians(double grade);
double grade_to_degrees(double grade);

/********************************************************************/
/* The following six routines are the normal trigonometric          */
/* functions.                                                       */
/********************************************************************/

double sine(double angle, enum angle_type atype);
double cosine(double angle, enum angle_type atype);
double tangent(double angle, enum angle_type atype);
double secant(double angle, enum angle_type atype);
double cosecant(double angle, enum angle_type atype);
double cotangent(double angle, enum angle_type atype);

/********************************************************************/
/* The following six routines are the normal inverse trigonometric  */
/* functions.                                                       */
/********************************************************************/

double arc_sine(double x, enum angle_type outtype);
double arc_cosine(double x, enum angle_type outtype);
double arc_tangent(double x, enum angle_type outtype);
double arc_secant(double x, enum angle_type outtype);
double arc_cosecant(double x, enum angle_type outtype);
double arc_cotangent(double x, enum angle_type outtype);

/********************************************************************/
/* The following six routines are the hyperbolic trigonometric      */
/* functions.                                                       */
/********************************************************************/

double hyperbolic_sine(double angle, enum angle_type atype);
double hyperbolic_cosine(double angle, enum angle_type atype);
double hyperbolic_tangent(double angle, enum angle_type atype);
double hyperbolic_secant(double angle, enum angle_type atype);
double hyperbolic_cosecant(double angle, enum angle_type atype);
double hyperbolic_cotangent(double angle, enum angle_type atype);

/********************************************************************/
/* The following six routines are the hyperbolic inverse            */
/* trigonometric functions.                                         */
/********************************************************************/

double hyperbolic_arc_sine(double x, enum angle_type outtype);
double hyperbolic_arc_cosine(double x, enum angle_type outtype);
double hyperbolic_arc_tangent(double x, enum angle_type outtype);
double hyperbolic_arc_secant(double x, enum angle_type outtype);
double hyperbolic_arc_cosecant(double x, enum angle_type outtype);
double hyperbolic_arc_cotangent(double x, enum angle_type outtype);

/********************************************************************/
/* The following four routines "complete" the standard library      */
/* functions.                                                       */
/********************************************************************/

double sech(double x);
double csch(double x);
double coth(double x);
double acoth(double x);

#endif
