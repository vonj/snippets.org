/*
**  Tests for popular PC compilers and versions
*/

#include <stdio.h>

main()
{
      int version;

#if defined(__ZTC__)
 #if defined(__SC__)
      printf("Symantec C++ ver. %x.%x\n", __SC__ >> 8, __SC__ & 0xff);
 #else
      printf("Zortech C++ ver. %x.%xr%x\n",
            __ZTC__ >> 8, (__ZTC__ >> 4) & 0xf, __ZTC__ & 0xf);
 #endif
#elif defined(__WATCOMC__)
      printf("Watcom C/C++ ver. %d.%d\n",
            __WATCOMC__ / 100, __WATCOMC__ % 100);
#elif defined(__TURBOC__)
      version = __TURBOC__;
      if (0x295 > version)
      {
            printf("Borland Turbo C ver. %x.%02x\n",
                  version >> 8, version & 0xff);
      }
      else if (0x400 <= version)
      {
 #if defined(__BORLANDC__)
            printf("Borland C++ ver. %x.%x\n",
                  (version >> 8) - 1, (version & 0xff) >> 4);
 #else
            printf("Borland Turbo C++ ver. %x.%x\n",
                  (version >> 8) - 1, (version & 0xff) >> 4);
 #endif
      }
      else if (0x297 > version)
            printf("Borland Turbo C++ ver. 1.%02x\n", version - 0x295);
      else  printf("Borland C++ ver. 2.%02x\n", version - 0x297);
#elif defined(_MSC_VER)
      printf("Microsoft C(/C++) ver. %d.%d\n",
            _MSC_VER / 100, _MSC_VER % 100);
#elif defined(_QC)
      printf("Microsoft Quick C ver. %d.%d\n", _QC / 100, _QC % 100);
#elif defined(__POWERC)
      printf ("MIX Power C ver. %d.%d.%d\n",
            __POWERC/100, (__POWERC / 10) % 10, __POWERC % 10);
#elif defined(__GNUC__)
 #ifdef __EMX__
      printf("GNU/EMX C version %d.%d\n", __GNUC__, __GNUC_MINOR__);
  #else
      printf("GNU C version %d.%d\n", __GNUC__, __GNUC_MINOR__);
 #endif
#elif defined(__IBMC__) || defined (__IBMCPP__)
 #ifdef __IBMC__
      version = __IBMC__;
 #else
      version = __IBMCPP__;
 #endif
      if (version >= 300)
            printf ("IBM VisualAge C++ %d.%d\n",
                  version / 100, version % 100);
      else  printf ("IBM C Set++ %d.%d\n", version / 100, version % 100);
#else
      puts("Unknown compiler!");
#endif
      return 0;
}
