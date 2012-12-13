/*
**  MATHSTAT.H - Header file for statistical analysis in C and C++
**
**  Public domain by Bob Stout
*/

#include <stddef.h>
#include <math.h>
#include "sniptype.h"
#include "binsrch.h"

#if !(__cplusplus)

/*
**  We need a local compiler-independent NaN
*/

extern VAR64_ stat_nan_;
#define STAT_NaN stat_nan_.d

/*
**  Structure to hold statistical analysis data
**
**  total   = Total of all data added
**  len     = Maximum number of data points (0 if data arrray not used)
**  total2  = Total of squares of all data added
**  product = Product of all data added
**  recip   = Total of the reciprocals of all data added
**  count   = Number of data elements under analysis
**  filt    = The sorted data array supports min, max, and filter functionality
**  min     = Minimum datum (if data arrray not used)
**  max     = Maximum Datum (if data arrray not used)
*/

typedef struct {
      size_t count;         /* Number of items in the analysis          */
      size_t len;           /* Maximum Number of items in the analysis  */
      double total;         /* Total of data                            */
      double total2;        /* Sum of swaures of data                   */
      double product;       /* Product of data                          */
      double recip;         /* Sum of reciprocals of data               */
      double *filt;         /* Data array (NULL disallows filtering)    */
      double min;           /* Min datum (if data array not used)       */
      double max;           /* Min datum (if data array not used)       */
} Stat_T;

/*
**  Provided functions:
**  1.  Open/Initialize
**  2.  Close
**  3.  Flush/Reinitialize
**  4.  Add datum
**  5.  Delete datum
**  6.  "Olympic" filter - discards high and low data.
**  7.  "Olympiad" filter - an auto-windowing median filter, it iteratively discards
**      high and low data until a target coefficient of variation is achieved.
**  8.  Median Filter - discards data outside of a window around the median datum.
**  9.  Minumum datum
**  10. Maximum datum
**  11. Minimum error - percent deivation from arithmetic mean
**  12. Maximum error - percent deivation from arithmetic mean
**  13. Arithmetic mean
**  14. Geometric mean
**  15. Harmonic mean
**  16. Standard deviation (population)
**  17. Standard deviation (sample)
**  18. Variance (population)
**  19. Variance (sample)
**  20. Coefficient of variation (population)
**  21. Coefficient of variation (sample)
*/

Boolean_T   stat_init(Stat_T *ptr, size_t qdepth);
void        stat_flush(Stat_T *ptr);
void        stat_done(Stat_T *ptr);
size_t      stat_count(Stat_T *ptr);
Boolean_T   stat_add(double datum, Stat_T *ptr);
Boolean_T   stat_delete(double datum, Stat_T *ptr);
Boolean_T   stat_olympic(Stat_T *ptr);
Boolean_T   stat_olympiad(Stat_T *ptr, double CoefVar);
Boolean_T   stat_median(Stat_T *ptr, size_t window);
double      stat_min(Stat_T *ptr);
double      stat_max(Stat_T *ptr);
double      stat_minerror(Stat_T *ptr);
double      stat_maxerror(Stat_T *ptr);
double      stat_mean(Stat_T *ptr);
double      stat_gmean(Stat_T *ptr);
double      stat_hmean(Stat_T *ptr);
double      stat_stddevP(Stat_T *ptr);
double      stat_stddevS(Stat_T *ptr);
double      stat_varP(Stat_T *ptr);
double      stat_varS(Stat_T *ptr);
double      stat_varcoeffP(Stat_T *ptr);
double      stat_varcoeffS(Stat_T *ptr);

/*
**  Debugging functions
*/

void stat_dump(Stat_T *ptr);
void stat_qdump(Stat_T *ptr);

#else /* is C++ */

class Stat {
      private:
            size_t            count;
            size_t            len;
            double            total;
            double            total2;
            double            product;
            double            recip;
            double            *filt;
            double            min;
            double            max;
      public:
            inline Boolean_T  Stat(size_t qdepth);
            void              Flush();
            inline size_t     Count();
            Boolean_T         Add(double datum);
            Boolean_T         Delete(double datum);
            Boolean_T         Olympic();
            Boolean_T         Olympiad(double CoefVar);
            Boolean_T         Median(double *median, Stat_T *ptr, size_t window);
            inline double     Min();
            inline double     Max();
            double            Minerror();
            double            Maxerror();
            double            Mean();
            double            Gmean();
            double            Hmean();
            double            StddevP();
            double            StddevS();
            inline double     VarP();
            inline double     VarS();
            double            VarcoeffP();
            double            VarcoeffS();

            /*
            **  Debugging functions
            */

            void dump();
            void qdump();
};

#endif
