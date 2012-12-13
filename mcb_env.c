#include <stdio.h>
#include <dos.h>
#include <string.h>
#include <ctype.h>

#if defined(__TURBOC__) || defined(__POWERC) || defined(__ZTC__)
 #if defined(__ZTC__)
  #include <dos.h>
 #else
  #include <alloc.h>
 #endif
#else                                           /* MSC, Watcom    */
 #include <malloc.h>
#endif

#include "extkword.h"
#include "mk_fp.h"

struct EnvRec {
              unsigned EnvSeg;  /*Segment of the environment*/
              unsigned EnvLen;  /*Usable length of the environment*/
              } EnvRec;

struct EnvRec *MasterEnv(void)
{
      unsigned owner;
      unsigned mcb;
      unsigned eseg;
      static struct EnvRec env;

      env.EnvSeg = env.EnvLen = 0;
      owner = * ((unsigned FAR *) MK_FP(0, (2+4*0x2e)));

      /* int 0x2e points to command.com */

      mcb = owner -1;

      /*Mcb points to memory control block for COMMAND */

      if ( (*((char FAR *) MK_FP(mcb, 0)) != 'M') &&
           (*((unsigned FAR *) MK_FP(mcb, 1)) != owner) )
                 return (struct EnvRec *) 0;

      eseg = *((unsigned FAR *) MK_FP(owner, 0x2c));

      /* Read segment of environment from PSP of COMMAND} */
      /* Earlier versions of DOS don't store environment segment there */

      if ( !eseg )
      {

            /* Master environment is next block past COMMAND */

            mcb = owner + *((unsigned FAR *) MK_FP(mcb, 3));
            if ( (*((char FAR *) MK_FP(mcb, 0)) != 'M') &&
                 (*((unsigned FAR *) MK_FP(mcb, 1)) != owner) )
                       return (struct EnvRec *) 0;
            eseg = mcb + 1;
      }
      else  mcb = eseg-1;

      /* Return segment and length of environment */

      env.EnvSeg = eseg;
      env.EnvLen = *((unsigned FAR *) MK_FP(mcb, 3)) << 4 ;
      return &env;
}

/*
** Then a function to find the string to be replaced.   This one'll
** return a pointer to the string, or a pointer to the first (of 2)
** NUL byte at the end of the environment.
*/

char FAR *SearchEnv( char FAR *eptr, char *search )
{
      char FAR *e;
      char *s;
      while ( *eptr )
      {
            for ( s=search, e=eptr; *e && *s && (*e == *s); e++, s++ )
                  ;  /* NULL STATEMENT */
            if ( !*s )
                  break;
            while ( *eptr )
                  eptr++;           /* position to the NUL byte */
            eptr++;                      /* next string */
      }
      return eptr;
}

/*
** Now, the function to replace, add or delete.  If a value is not
** given, the string is deleted.
*/

int SetEnvStr( struct EnvRec *env, char *search, char *value )
{
      /* -Set environment string, returning true if successful */

      char FAR *envptr;
      register char FAR *p;
      char *s;
      int newlen;
      int oldlen;
      int i;

      if ( !env->EnvSeg || !search )
            return 0;

      /* get some memory for complete environment string */

      newlen = strlen(search) + sizeof((char) '\0') + strlen(value) + 2;
      if ( (s = (char *) malloc( newlen)) == NULL )
            return 0;
      for ( i = 0; *search; search++, i++ )
            s[i] = *search;
      s[i++] = '=';
      s[i] = '\0';
      envptr = SearchEnv((char FAR *) MK_FP(env->EnvSeg, 0), s );
      if ( *envptr )
      {
            for ( p = envptr, oldlen = 0; *p; oldlen++, p++ )
                  ;     /* can't use strlen() because of far pointer */
      }                 /* will set p to point to terminating NUL */

      if ( *value && (newlen > (int)env->EnvLen) ) /* not a deletion */
      {
            free( s );
            return 0;                           /* won't fit */
      }

      if ( *envptr )                            /* shift it down */
      {
            for ( ++p; (*p || *(p+1)); envptr++, p++ )
                  *envptr = *p;
            *envptr++ = '\0';
            *envptr = '\0';
      }
      if ( *value )                             /* append it */
      {
            strcat(s, value);
            while ( *s )
                  *(envptr++) = *s++;
            *envptr++ = '\0';
            *envptr = '\0';
      }
      free(s);
      return 1;
}

/*
** Ok, just to show you that I tested it :
*/

main()
{
      char vn[80];
      char va[80];
      struct EnvRec *p = MasterEnv();

      puts("enter variable name:");
      gets(vn);
      puts("enter value:");
      gets(va);
      printf("SetEnvStr returned %d\n", SetEnvStr( p, strupr(vn), va) );
      return 0;
}
