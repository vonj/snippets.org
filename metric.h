/*************************************************
*
*         ANSI C Metric Conversion Macros
*              Mike Smith, Freeware
*   6 Grenville Road, Plymouth, England, PL4 9PX
*
*             mike@msmith.demon.co.uk
*
*   Version: 1.03
*   History: 5th of May, 1997
*
*   1.01 Taken out formulas and added values only Mon 17th April 1995
*   1.02 Error corrections Sat 13th July 1996
*   1.03 Added macros 5th of May, 1997
*
*=================================================
*  While I have done my best to assure accuracy
*     I can not confirm these are accurate.
*
*  This was done as part of a program which I
*  did and I figured while I was at it I would
*   do all of the conversions for the future.
*     These tables are copied from a book.
*
* If you find any incorrect data or can add some
*     more send them in, and the world can
*            share these routines 
*
* If you would like a free update send me a disc
*  and a stamp or request it via e-mail.
*
**************************************************/

#ifndef METRIC__H
#define METRIC__H


/******** Length Conversion Macros **********/

#define CM_TO_IN(x) (0.3937F * (x))
#define IN_TO_CM(x) (2.54F * (x))
#define IN_TO_M(x) (0.0254F * (x))
#define IN_TO_MM(x) (25.4F * (x))
#define KM_TO_MILE(x) (0.6214F * (x))
#define M_TO_IN(x) (39.37F * (x))
#define M_TO_YARD(x) (1.093611F * (x))
#define MILE_TO_KM(x) (1.609F * (x))
#define MM_TO_IN(x) (0.03937F * (x))
#define YARD_TO_M(x) (0.9144F * (x))
#define IN_TO_FT(x) ((x) / 12.0)
#define IN_TO_YARD(x) ((x) / 36.0)
#define IN_TO_MILE(x) ((x) / 63360.0)
#define FT_TO_IN(x) (12.0 * (x))
#define FT_TO_YARD(x) ((x) / 3.0)
#define FT_TO_MILE(x) ((x) / 5280.0)
#define MILE_TO_IN(x) (63360.0 * (x))
#define MILE_TO_FT(x) (5280.0 * (x))
#define MILE_TO_YARD(x) (1760.0 * (x))

/******* Weight Macros ************/

#define G_TO_OZ(x) (0.03527F * (x))
#define KG_TO_LBS(x) (2.2046F * (x))
#define LBS_TO_KG(x) (0.4545F * (x))
#define LBS_TO_OZ(x) (16.0F * (x))
#define OZ_TO_G(x) (28.3527F * (x))
#define OZ_TO_LBS(x) ((x) / 16.0)

/******* Liquid Conversion Macros ********/

#define CL_TO_OZ(x) (0.338F * (x))
#define ML_TO_OZ(x) (0.0338F * (x))
#define OZ_TO_CL(x) (2.95857988F * (x))
#define OZ_TO_ML(x) (29.5857F * (x))

/***********************************************
*
*      In case you are unaware there are:
*           16 ounces in a US quart
*           20 ounces in a UK quart
*
*  This, of course, makes gallons different
*     too.  Supplied below are US to UK
*           conversions as well.
*
************************************************/

#define KL_TO_UKGAL(x) (211.344F * (x))
#define KL_TO_USGAL(x) (264.18F * (x))
#define L_TO_UKGAL(x) (0.2199F * (x))
#define L_TO_UKQ(x) (0.945F * (x))
#define L_TO_USGAL(x) (0.26455F * (x))
#define L_TO_USQ(x) (1.0582F * (x))
#define UKGAL_TO_KL(x) (0.0047316F * (x))
#define UKGAL_TO_L(x) (4.546F * (x))
#define UKGAL_TO_USGAL(x) (1.25F * (x))
#define UKQ_TO_L(x) (1.0582F * (x))
#define UKQ_TO_USQ(x) (1.25F * (x))
#define USGAL_TO_KL(x) (0.003785F * (x))
#define USGAL_TO_L(x) (3.78F * (x))
#define USGAL_TO_UKGAL(x) (0.8F * (x))
#define USQ_TO_L(x) (0.945F * (x))
#define USQ_TO_UKQ(x) (0.8F * (x))

/**** Area Conversion Macros ****/

#define CM2_TO_IN2(x) (0.155F * (x))
#define FT2_TO_M2(x) (0.0930F * (x))
#define IN2_TO_CM2(x) (6.4516F * (x))
#define IN2_TO_MM2(x) (645.161F * (x))
#define KM2_TO_MILE2(x) (0.386F * (x))
#define M2_TO_FT2(x) (10.75F * (x))
#define MILE2_TO_KM2(x) (2.5906F * (x))
#define MM2_TO_IN2(x) (0.00155F * (x))

/******** Volume Conversion Macros *********/

#define CM3_TO_IN3(x) (0.061F * (x))
#define CM3_TO_IN3(x) (0.061F * (x))
#define IN3_TO_CC(x) (16.3934F * (x))
#define IN3_TO_CM3(x) (16.3934F * (x))
#define KM3_TO_MILE3(x) (0.25F * (x))
#define M3_TO_YARD3(x) (1.308F * (x))
#define MILE3_TO_KM3(x) (4.0F * (x))
#define YARD3_TO_M3(x) (0.764526F * (x))

#endif   /* METRIC__H */
