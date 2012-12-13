/****************************************************************************
* @(#) MODULINF.h
* @(#)      Header defining a structure for tracking & status information.
*
*
*** Corrections *************************************************************
*
*
*****************************************************************************
*@(#)1993 Erik Bachmann (E-mail: ebp@dde.dk)
*
* Released to public domain 27-Oct-95
*****************************************************************************/

#if !defined(MODULINF_H)      /* IF not yet defined (to avoid repetitions) */
#define MODULINF_H                                          /* Define */

struct tyModulInfo
{
      char  *pszModuleTag ;                           /* tagModulInf    */
      char  *pszModuleName ;                          /* __FILE__       */
      char  *pszCreateDate ;                          /* 1993-06-11     */
      char  *pszChangedDate ;                         /* __DATE__       */
      char  *pszChangedTime ;                         /* __TIME__       */
      char  *pszCopyright ;                           /* text           */
} ;

#endif


#define MODULEINF( _MODULEDATE, _MODULENAME ) \
      static struct tyModulInfo stModulInfo = \
      { \
            "tagModulInf", \
            __FILE__,\
             _MODULEDATE,\
             __DATE__,\
            __TIME__, \
            _MODULENAME \
      }

#define PROGRAMINF( _PROGNAME, _PROGVER, _MODULEDATE, _COPYRIGHT ) \
      const char  far   *PROGNAME   =     _PROGNAME ; \
      const char  far   *PROGVER          =     _PROGVER ; \
      struct tyModulInfo stModulInfo = \
      { \
            "tagModulInf",\
             __FILE__,\
            _MODULEDATE,\
             __DATE__,\
            __TIME__, \
            _COPYRIGHT \
      }
