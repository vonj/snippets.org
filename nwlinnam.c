/*
**  NWLINNAME.C - Novell Netware: getNwLoginName
**  This should work for Advanced NetWare (2.x) -> NetWare386 3.x & 4.x
**
**  Written by david nugent, public domain
*/

/* #define TEST */            /* Uncomment to compile test main()       */

#include <string.h>
#include <process.h>
#include <dos.h>
#include <errno.h>
#include "snpdosys.h"

#define addrOffset(val)     (unsigned short)(((long)(val)) & 0xfff)
    /* Invoke Novell Netware API.  */
#define callNetware(func, reqbuf, repbuf, inregs, segregs) \
    { \
        (inregs)->h.ah = (func); \
        (inregs)->x.si = addrOffset(reqbuf); \
        (inregs)->x.di = addrOffset(repbuf); \
        intdosx(inregs, inregs, segregs); \
    }

#if defined(_MSC_VER) || defined(_QC) || defined(__WATCOM__)
    #pragma pack(1)
#elif defined(__ZTC__)
    #pragma ZTC align 1
#elif defined(__TURBOC__) && (__TURBOC__ > 0x202)
    #pragma option -a-
#endif

/*
**  Get Connection Information E3(16)
*/

struct _conninfo
{
    unsigned short len;
    unsigned char func;
    unsigned char cno;
};

struct _connrep
{
    unsigned short len;
    unsigned long objectID;
    unsigned short objecttype;
    char objectname[48];
    unsigned char logintime[7];
    unsigned char reserved[39];
};

#if defined(_MSC_VER) || defined(_QC) || defined(__WATCOM__)
    #pragma pack()
#elif defined(__ZTC__)
    #pragma ZTC align 2
#elif defined(__TURBOC__) && (__TURBOC__ > 0x202)
    #pragma option -a
#endif

int getNwLoginName (char * namebuf)
{
      union REGS r;

      r.x.ax = 0xDC00;    /* Get Connection Number */
      intdos(&r, &r);

      /*
      ** If the connection number is in range 1-100 (ie. valid),
      ** get Connection Information and retrieve the user name.
      */

      if (r.h.al <= 0 ||  r.h.al > 100)
            return errno = EINVAL;

      else
      {
            struct SREGS s;
            static struct _connrep connrep;
            static struct _conninfo conninfo;

            conninfo.len = sizeof(conninfo) - sizeof(conninfo.len);
            conninfo.func = 0x16;   /* Get connection information */
            conninfo.cno = r.h.al;
            connrep.len = sizeof(connrep) - sizeof(connrep.len);
            segread(&s);
            s.es = s.ds;    /* ds:si=request buffer, es:di=reply buffer */

            /* login info */

            callNetware(0xE3, &conninfo, &connrep, &r, &s);
            if (r.h.al == 0)
            {
                  strcpy (namebuf, connrep.objectname);
                  return 0;
            }
            return r.h.al;
      }
}

#if defined(TEST)

#include <stdio.h>

int
main()
{
      char loginname[48];
      int rc = getNwLoginName (loginname);

      if (rc == 0)
            printf ("Your login name is '%s'\n", loginname);
      else  printf ("Error %d calling Netware, %s\n", rc, strerror(errno));
      return 0;
}

#endif /* TEST */
