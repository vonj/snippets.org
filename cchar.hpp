//
// Classed char
//
// puts array of characters on the heap using new without having
// to delete.

/*
 * I hereby give this source code to the public domain.
 * You may use this file for any purpose whatsoever without royalty.
 * However, you also accept that I bear no responsibility for the
 * fitness for any particular use, expressed or implied, or that this
 * will even work.
 *
 * If you make any changes to this file, please let me know so that
 * I may incorporate those changes into the originals.
 *
 * Reachable at:
 *   mcbride@ee.ualberta.ca
 *   mcbride@tower.bohica.net
 *   darin.mcbride@tower.alive.ampr.ab.ca
 *   Sysop of 1:342/708 (fido)
 *           40:6494/2004 (ibm)
 *           77:2/0 (fire)
 *          100:1403/0 (rpg)
 */

#ifndef __CHAR_ON_HEAP_HPP
#define __CHAR_ON_HEAP_HPP

#include <string.h>

#define ASSERT(x) Assert(x) // use to point to your assert function type
// use '#define ASSERT(x)' to remove assertions (DANGEROUS!)

// if your compiler requires a specific ostream& operator<< for this
// class, uncomment the following line, or define it in the makefile/project
// #define REQUIRES_OSTREAM_INSERTER_OPERATOR

#ifdef REQUIRES_OSTREAM_INSERTER_OPERATOR
#include <iostream.h>
#endif

class cchar
{
#ifdef REQUIRES_OSTREAM_INSERTER_OPERATOR
    friend ostream& operator << (ostream& os, cchar& cc);
#endif
   private:
       char* pchar; // the pointer to the character(s)

       // following routines are provided to easily change the
       // method for allocation.  For example, to change from
       // C++'s new/delete to farmalloc/farfree, just change these
       // functions
       void Allocate(size_t n)
       {
           ASSERT(pchar == NULL);
           pchar = new char[n];
       }
       void DeAllocate()
       {
           ASSERT(pchar != NULL);
           delete[] pchar;
           pchar = NULL;
       }

   public:
       cchar(int n = 0) : pchar(NULL)
       {
           if (n > 0)
               Allocate(n); // grab enough memory
       }
       cchar(const char* psz) : pchar(NULL)
       {
           if (psz != NULL) {
               Allocate(strlen(psz) + 1);
               strcpy(pchar, psz);
           }
       }
       cchar(const cchar& cch) : pchar(NULL)
       {
           if (cch.pchar != NULL) {
               Allocate(strlen(cch.pchar) + 1);
               strcpy(pchar, cch.pchar);
           }
       }
       cchar& operator=(const char* psz)
       {
           if (psz != pchar) { // ensure we aren't copying over ourselves
               if (pchar != NULL)
                   DeAllocate();
               if (psz != NULL) {
                   Allocate(strlen(psz) + 1);
                   strcpy (pchar, psz);
               } else {
                   pchar = NULL;
               }
           }
           return *this;
       }
       cchar& operator=(const cchar& cch)
       {
           // use the above operator= to do the rest - code reuse?
           return *this = cch.pchar;
       }
       ~cchar()
       {
           if (pchar != NULL)
               DeAllocate();
       }
       // for those cases that you really need to test validity...
       // and the standard 'char_ptr == NULL' isn't working.  <sigh>
       const int IsValid() const
       {
           return pchar != NULL;
       }
       operator char*() const
       {
           return pchar;
       }
       operator const char*() const
       {
           return pchar;
       }
       const char* operator()() const
       {
           return pchar;
       }
       char* operator()()
       {
           return pchar;
       }
       char operator()(int n) const
       {
           ASSERT(pchar != NULL);
           return pchar[n];
       }
       char& operator()(int n)
       {
           ASSERT(pchar != NULL);
           return pchar[n];
       }
       char& operator[](int n)
       {
           ASSERT(pchar != NULL);
           return pchar[n];
       }
       char operator[](int n) const
       {
           ASSERT(pchar != NULL);
           return pchar[n];
       }
};

#ifdef REQUIRES_OSTREAM_INSERTER_OPERATOR
inline ostream& operator << (ostream& os, cchar& cc)
{
   return os << cc.pchar;
}
#endif

#endif
