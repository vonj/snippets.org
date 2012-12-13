/*****************************************************************************
* @(#)BACSTD.h
*
* @(#)Headerfile for the BACSTD library.
*
* This library is avalable for the following memorymodels:
* Small (bacstd_S.lib), Medium (bacstd_M.lib), Compact (bacstd_C.lib), 
* Large (bacstd_L.lib) and Huge (bacstd_H.lib)
*
* BACSTD.lib contains several basic routines for BORLAND C and TURBO C
*
* For further details see the textfile: BACSTD.TXT
*
*-----------------------------------------------------------------------------
*
******************************************************************************
*@(#)1991-1995 Erik Bachmann (E-mail: ebp@dde.dk)
*
* Released to public domain 27-Oct-95
******************************************************************************/

#include <stdio.h>                     /* FILE */
#include <stdarg.h>                    /* va_list */
#include "modulinf.h"

/*** DEFINITIONS *****************************************/
#define _cdecl cdecl
                                       /* Library => EXTERN */
#if !defined(BACSTD_LIB_H)
#  define   BACSTD_LIB_H


#  if defined(__TINY__) || defined(__SMALL__)
#     define _CfnTYPE near             /* neardef for functions */
#     define _CdtTYPE near             /* neardef for data */
#  else
#     define _CfnTYPE far              /* fardef for functions */
#     define _CdtTYPE far              /* fardef for data */
#  endif


/*** GLOBAL DATA *****************************************/
   enum BOOLEAN   { FALSE, TRUE };

   #define  complement(base)  ~base + 1
   
#endif


/*** PROTOTYPES *******************************************/

/* E_HANDL.C */
int handler ( int errval , int ax , int bp , int si );

/* STRCASE.C */
unsigned char _CfnTYPE *strcase ( unsigned char *pszStr , 
         unsigned char *pszOrder );

/* STRREPC.C */
int _CfnTYPE strrepc ( char *pszStr , char cFrom , char cTo );

/* MODULUS.C */
int _CfnTYPE modulus10 ( char *pszBase );
int _CfnTYPE modulus11 ( char *pszBase );
int _CfnTYPE check_modulus10 ( char *pszBase );
int _CfnTYPE check_modulus11 ( char *pszBase );

/* REPSTR.C */
int _CfnTYPE repstr ( FILE *fpIn , FILE *fpOut , char *PatternTable [][2 ]);

/* CMPSTR.C */
int far cmpstr ( unsigned char *pszStr1 , unsigned char *pszStr2 , 
         unsigned char *pszOrder );
int far tcmpstr ( unsigned char *pszStr1 , unsigned char *pszStr2 , 
         unsigned char *pszOrder , unsigned char *pszMask , 
         unsigned char *pszTrunc );
void state ( int iOrder );

/* STRNSUB.C */
char _CfnTYPE *strnsub ( char *pszString , char *pszPattern , 
         char *pszReplacement , int iMaxLength );

/* TIME__.C */
char _CfnTYPE *time_mac_conv ( char *pszTime );

/* DATE__.C */
char far *date_mac_conv ( char *pszDate );

/* STRTRIM.C */
int _CfnTYPE strtrimr ( char *pszStr );
int _CfnTYPE strtriml ( char *pszStr );
int _CfnTYPE strtrim ( char *pszStr );

/* STRTRIMC.C */
int _CfnTYPE strtrimcr ( char *szStr , char *szSet );
int _CfnTYPE strtrimcl ( char *szStr , char *szSet );
int _CfnTYPE strtrimc ( char *szStr , char *szSet );
int _CfnTYPE rep_last_char ( char *pszStr , char cChar1 , char cChar2 );
