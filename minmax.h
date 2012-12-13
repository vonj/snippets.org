/*
**  C macros for min() and max() on any C compiler.
**
**  C++ templates for min() and max() on any C++ compiler which supports
**  templates.
**
**  Overloaded C++ functions for min() and max() on any C++ compiler which
**  doesn't support templates.
**
**  If your compiler supports both C and C++, the appropriate behavior will
**  be selected based on the value of the __cplusplus predefined macro.
*/

#ifndef MINMAX__H
#define MINMAX__H

#undef min
#undef max

/*
**  The following 2 macros are used only for C++ compilers which lack
**  templates. If you use a C++ compiler which supports templates, ignore
**  the rest of this section. If you're compiling standard C, rather than
**  C++, you may ignore the rest of this section.
**
**  Set SIGNED_DISTINCT to 1 for conforming C++ compilers that treat signed,
**  unsigned, and "normal" types as 3 distinct entities. Set SIGNED_DISTINCT
**  to 0 for older or non-conforming C++ compilers. To determine the proper
**  setting for your compiler, compile MINMAX.H as a C++ source file using
**  the "compile only" (usually -c) switch. If the compiler reports "already
**  defined" errors, you probably need to change the value of SIGNED_DISTINCT
**  to 0.
**
**  If you still get "already defined" errors after setting SIGNED_DISTINCT
**  to 0, you may either have a truly broken compiler or have some obscure
**  (e.g. Zortech/Symantec's -Ju or -Jm) switch set which causes this
**  behavior. As a last resort, you can set UNSIGNED_DISTINCT to 0 as well.
*/

#define SIGNED_DISTINCT 1
#define UNSIGNED_DISTINCT 1

/*
**  We can only use templates with newer C++ compilers compiling C++ source
*/

#if defined(__cplusplus) && __cplusplus
 #if  (defined(__SC__) && __SC__ >= 0x700) || \
      (defined(_MSC_VER) && _MSC_VER > 800) || \
      (defined(__WATCOMC__) && __WATCOMC__ >= 1000) || \
      (defined(__BORLANDC__) && __BORLANDC__ >= 0x450)

  template<class T> inline T max(T a, T b) {return (a > b) ? a : b; };
  template<class T> inline T min(T a, T b) {return (a < b) ? a : b; };

 #else /* no templates */

  /*
  ** prototypes for overloaded max() functions
  */

  inline char max(char a, char b)
  {
        return (a > b) ? a : b;
  }
  #if UNSIGNED_DISTINCT
   inline unsigned char max(unsigned char a, unsigned char b)
   {
         return (a > b) ? a : b;
   }
  #endif
  #if SIGNED_DISTINCT
   inline signed char max(signed char a, signed char b)
   {
         return (a > b) ? a : b;
   }
  #endif

  inline short max(short a, short b)
  {
        return (a > b) ? a : b;
  }
  #if UNSIGNED_DISTINCT
   inline unsigned short max(unsigned short a, unsigned short b)
   {
         return (a > b) ? a : b;
   }
  #endif
  #if SIGNED_DISTINCT
   inline signed short max(signed short a, signed short b)
   {
         return (a > b) ? a : b;
   }
  #endif

  inline int max(int a, int b)
  {
        return (a > b) ? a : b;
  }
  #if UNSIGNED_DISTINCT
   inline unsigned int max(unsigned int a, unsigned int b)
   {
         return (a > b) ? a : b;
   }
  #endif
  #if SIGNED_DISTINCT
   inline signed int max(signed int a, signed int b)
   {
         return (a > b) ? a : b;
   }
  #endif

  inline long max(long a, long b)
  {
        return (a > b) ? a : b;
  }
  #if UNSIGNED_DISTINCT
   inline unsigned long max(unsigned long a, unsigned long b)
   {
         return (a > b) ? a : b;
   }
  #endif
  #if SIGNED_DISTINCT
   inline signed long max(signed long a, signed long b)
   {
         return (a > b) ? a : b;
   }
  #endif

  inline float max(float a, float b) {return (a > b) ? a : b; }

  inline double max(double a, double b) {return (a > b) ? a : b; }

  /*
  ** prototypes for overloaded min() functions
  */

  inline char min(char a, char b)
  {
        return (a < b) ? a : b;
  }
  #if UNSIGNED_DISTINCT
   inline unsigned char min(unsigned char a, unsigned char b)
   {
         return (a < b) ? a : b;
   }
  #endif
  #if SIGNED_DISTINCT
   inline signed char min(signed char a, signed char b)
   {
         return (a < b) ? a : b;
   }
  #endif

  inline short min(short a, short b)
  {
        return (a < b) ? a : b;
  }
  #if UNSIGNED_DISTINCT
   inline unsigned short min(unsigned short a, unsigned short b)
   {
         return (a < b) ? a : b;
   }
  #endif
  #if SIGNED_DISTINCT
   inline signed short min(signed short a, signed short b)
   {
         return (a < b) ? a : b;
   }
  #endif

  inline int min(int a, int b)
  {
        return (a < b) ? a : b;
  }
  #if UNSIGNED_DISTINCT
   inline unsigned int min(unsigned int a, unsigned int b)
   {
         return (a < b) ? a : b;
   }
  #endif
  #if SIGNED_DISTINCT
   inline signed int min(signed int a, signed int b)
   {
         return (a < b) ? a : b;
   }
  #endif

  inline long min(long a, long b)
  {
        return (a < b) ? a : b;
  }
  #if UNSIGNED_DISTINCT
   inline unsigned long min(unsigned long a, unsigned long b)
   {
         return (a < b) ? a : b;
   }
  #endif
  #if SIGNED_DISTINCT
   inline signed long min(signed long a, signed long b)
   {
         return (a < b) ? a : b;
   }
  #endif

  inline float min(float a, float b) {return (a < b) ? a : b; }

  inline double min(double a, double b) {return (a < b) ? a : b; }

 #endif /* no templates */

#else /* standard C macros */

 #define min(x,y) (((x) <= (y)) ? (x) : (y))
 #define max(x,y) (((x) >= (y)) ? (x) : (y))

#endif /* __cplusplus */

#endif /* MINMAX__H */
