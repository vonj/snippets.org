/*
**  MATHSTAT.C - Statistical analysis in C and C++
**
**  Public domain by Bob Stout
**
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
**
**  Uses SNIPPETS files:
**    MATHSTAT.H
**    SNIPTYPE.H
**    CAST.H
**    BINSRCH.H
**    BINSRCH.C
*/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <float.h>
#include <assert.h>
#include "mathstat.h"
#include "cast.h"
#include "binsrch.h"

/*
**  We need a local compiler-independent NaN
*/

VAR64_ stat_nan_;
static Boolean_T nan_initialized = False_;

/*
**  For functions requiring min and max data, all data are stored in an array that
**  is sized upon object creation. Data are always added to the array in sorted order.
*/


/*
**  Initialize a stat structure
**
**  Parameters: 1 - Pointer to Stat_T structure type
**              2 - Data array depth (use 0 for no data array and no filtering)
**
**  Returns: Success_ or Error_
**
**  Side effects: Allocates data array
*/

#if !(__cplusplus)

Boolean_T stat_init(Stat_T *ptr, size_t qdepth)
{
      if (!nan_initialized)
      {
            stat_nan_.v32[0].l = -1L;
            stat_nan_.v32[1].l = -1L;
            nan_initialized = True_;
      }
      ptr->count   = 0;
      ptr->len     = qdepth;
      ptr->total   = 0.0;
      ptr->total2  = 0.0;
      ptr->recip   = 0.0;
      ptr->product = 1.0;
      if (qdepth)
      {
            if (NULL == (ptr->filt = calloc(qdepth, sizeof(double))))
                  return Error_;
      }
      else
      {
            ptr->filt = NULL;
            ptr->min = FLT_MAX;
            ptr->max = FLT_MIN;
      }
      return Success_;
}

#else /* C++ */

inline Boolean_T Stat::Stat(size_t qdepth)
{
      if (!nan_initialized)
      {
            stat_nan_.v32[0].l = -1L;
            stat_nan_.v32[1].l = -1L;
            nan_initialized = True_;
      }
      count   = 0;
      len     = qdepth;
      total   = 0.0;
      total2  = 0.0;
      recip   = 0.0;
      product = 1.0;
      if (qdepth)
      {
            if (NULL == (filt = calloc(qdepth, sizeof(double))))
                  return Error_;
      }
      else
      {
            filt = NULL;
            min = FLT_MAX;
            max = FLT_MIN;
      }
      return Success_;
}

#endif

/*
**  Flush/reinitialize a stat structure
**
**  Parameters: 1 - Pointer to Stat_T structure type
**
**  Returns: Nothing
*/

#if !(__cplusplus)

void stat_flush(Stat_T *ptr)
{
      ptr->count   = 0;
      ptr->total   = 0.0;
      ptr->total2  = 0.0;
      ptr->recip   = 0.0;
      ptr->product = 1.0;
      if (0 == ptr->len)
      {
            ptr->min = FLT_MAX;
            ptr->max = FLT_MIN;
      }
}

#else /* C++ */

inline void Stat::Flush()
{
      Boolean_T retval = Success_;

      count   = 0;
      total   = 0.0;
      total2  = 0.0;
      recip   = 0.0;
      product = 1.0;
      if (0 == ptr->len)
      {
            min = FLT_MAX;
            max = FLT_MIN;
      }
}

#endif

/*
**  Closee a stat structure
**
**  Parameters: 1 - Pointer to Stat_T structure type
**
**  Returns: Nothing
**
**  Side effects: Frees the data array
*/

#if !(__cplusplus)

void stat_done(Stat_T *ptr)
{
      if (ptr->filt)
            FREE(ptr->filt);
}

#else /* C++ */

inline Boolean_T ~Stat::Stat()
{
      if (ptr->filt)
            FREE(filt);
}

#endif

/*
**  Return the number of data points
**
**  Parameters: 1 - Pointer to Stat_T structure type
**
**  Returns: Number of data points
*/

#if !(__cplusplus)

size_t stat_count(Stat_T *ptr)
{
      return ptr->count;
}

#else /* C++ */

inline size_t Stat::Count()
{
      return count;
}

#endif

/*
**  Add a value for statistical analysis
**
**  Parameters: 1 - Datum to add
**              2 - Pointer to Stat_T structure type
**
**  Returns: Error_ if data array is full, else Success_
**
**  Side effects: Inserts the new datum into the data array in sorted order
*/

#if !(__cplusplus)

Boolean_T stat_add(double datum, Stat_T *ptr)
{
      int high, low, idx;

      if ((0 == ptr->len) || (ptr->count < ptr->len))
      {
            ptr->total   += datum;
            ptr->total2  += datum * datum;
            ptr->recip   += 1.0 / datum;
            ptr->product *= datum;

        /*
        **  If a data array is being used, maintain it
        */

            if (ptr->filt)
            {
                  /*
                  ** Place the data in sorted order
                  **
                  ** If the array is empty or the datum is a new max, simply append it
                  */
                  
                  if ((0 == ptr->count) || (ptr->filt[ptr->count - 1] <= datum))
                        ptr->filt[ptr->count++] = datum;
                  
                  /*
                  ** If the datum is a new min, shift the array up and insert it at the beginning.
                  */
                  
                  else if (ptr->filt[0] >= datum)
                  {
                        memmove(&ptr->filt[1], &ptr->filt[0], (ptr->count++) * sizeof(datum));
                        ptr->filt[0] = datum;
                  }
                  
                  /*
                  ** Use a binary search to insert the datum into the array
                  */
                  
                  else
                  {
                        for (low = 0, high = ptr->count - 1;  high - low > 1; )
                        {
                              idx = (high+low) / 2;
                              if (datum == ptr->filt[idx])
                                    break;
                              else if (datum < ptr->filt[idx])
                                    high = idx;
                              else  low  = idx;
                        }
                        if (datum != ptr->filt[idx])
                              idx = high;
                        memmove(&ptr->filt[idx+1], &ptr->filt[idx], (ptr->count++ - idx) * sizeof(datum));
                        ptr->filt[idx] = datum;
                  }
                  return Success_;
            }
            else
            {
                  ptr->count++;
                  if (datum > ptr->max)
                        ptr->max = datum;
                  if (datum < ptr->min)
                        ptr->min = datum;
            }
      }
      else  return Error_;
}

#else /* C++ */

Boolean_T Stat::Add(double datum)
{
      int high, low, idx;

        if ((0 == ptr->len) || (ptr->count < ptr->len))
      {
            total   += datum;
            total2  += datum * datum;
            recip   += 1.0 / datum;
            product *= datum;

            /*
            **  If a data array is being used, maintain it
            */

            if (filt)
            {
                  /*
                  ** Place the data in sorted order
                  **
                  ** If the array is empty or the datum is a new max, simply append it
                  */
                  
                  if ((0 == count) || (filt[count - 1] <= datum))
                        filt[count++] = datum;
                  
                  /*
                  ** If the datum is a new min, shift the array up and insert it at the beginning.
                  */
                  
                  else if (filt[0] >= datum)
                  {
                        memmove(&filt[1], &filt[0], (count++) * sizeof(datum));
                        filt[0] = datum;
                  }
                  
                  /*
                  ** Use a binary search to insert the datum into the array
                  */

                  else
                  {
                        for (low = 0, high = count - 1;  high - low > 1; )
                        {
                              idx = (high+low) / 2;
                              if (datum == filt[idx])
                                    break;
                              else if (datum < filt[idx])
                                    high = idx;
                              else  low  = idx;
                        }
                        if (datum != filt[idx])
                              idx = high;
                        memmove(&filt[idx+1], &filt[idx], (count++ - idx) * sizeof(datum));
                        filt[idx] = datum;
                  }
                  return Success_;
            }
            else
            {
                  count++;
                  if (datum > max)
                        max = datum;
                  if (datum < min)
                        min = datum;
            }
      }
      else  return Error_;
}

#endif

/*
**  Delete a value from a statistical analysis
**
**  Parameters: 1 - Datum to add
**              2 - Pointer to Stat_T structure type
**
**  Returns: Error_ if data array is empty or doesn't contain the datum, else Success_
**
**  Side effects: Removes the datum from the data array, leaving it in sorted order
*/

#if !(__cplusplus)

Boolean_T stat_delete(double datum, Stat_T *ptr)
{
      if (ptr->count)
      {
            int idx;

            ptr->total   -= datum;
            ptr->total2  -= datum * datum;
            ptr->recip   -= 1.0 / datum;
            ptr->product /= datum;

            /*
            **  If a data array is being used, maintain it
            */

            if (ptr->filt)
            {
                  /*
                  **  Remove low data by physically shifting the array. This is laborious,
                  **  but no more so than implementing a decent sort on a circular buffer.
                  */
                  
                  if (datum == ptr->filt[0])
                        memmove(&ptr->filt[0], &ptr->filt[1], (--ptr->count) * sizeof(double));
                  
                  /*
                  **  Remove high data. Actually, if the datum being removed was at the top
                  **  of the array, it may be effectively wiped out by decremented the count,
                  **  so the only reamining task to locate data within the array and delete it.
                  **  Again, this is done by physically shifting data in the array.
                  */
                  
                  else if (datum != ptr->filt[ptr->count-1])
                  {
                        if (-1 == (idx = BinSearchD(datum, &ptr->filt[0], ptr->count)))
                              return Error_;
                        memmove(&ptr->filt[idx], &ptr->filt[idx+1], (--ptr->count - idx) * sizeof(double));
                  }
                  else  --(ptr->count);
                  
                  return Success_;
            }
      }
      return Error_;
}

#else /* C++ */

Boolean_T Stat::Delete(double datum)
{
      if (count)
      {
            int idx;

            total   -= datum;
            meantmp  = Mean();
            total2  -= datum * datum;
            recip   -= 1.0 / datum;
            product /= datum;
            
            /*
            **  If a data array is being used, maintain it
            */

            if (filt)
            {
                  /*
                  **  Remove low data by physically shifting the array. This is laborious,
                  **  but no more so than implementing a decent sort on a circular buffer.
                  */
                  
                  if (datum == filt[0])
                        memmove(&filt[0], &filt[1], (--count) * sizeof(float));

                  /*
                  **  Remove high data. Actually, if the datum being removed was at the top
                  **  of the array, it may be effectively wiped out by decremented the count,
                  **  so the only reamining task to locate data within the array and delete it.
                  **  Again, this is done by physically shifting data in the array.
                  */
                  
                  else if (datum != filt[count-1])
                  {
                        if (-1 == (idx = BinSearchF(datum, &filt[0], count)))
                              return Error_;
                        memmove(&filt[idx], &filt[idx+1], (--count - idx) * sizeof(datum));
                  }
                  else  --count;
                  
                  return Success_;
            }
      }
      return Error_;
}

#endif

/*
**  Return the minimum datum
**
**  Parameters: 1 - Pointer to Stat_T structure type
**
**  Returns: Minimum datum
**
**  Note: If no data, return STAT_NaN
*/

#if !(__cplusplus)

double stat_min(Stat_T *ptr)
{
      if (ptr->count)
      {
            if (ptr->filt)
                  return ptr->filt[0];
            else  return ptr->min;
      }
      else  return STAT_NaN;
}

#else /* C++ */

inline double Stat::Min()
{
      if (count)
      {
            if (filt)
                  return filt[0];
            else  return min;
      }
      else  return STAT_NaN;
}

#endif

/*
**  Return the maximum datum
**
**  Parameters: 1 - Pointer to Stat_T structure type
**
**  Returns: Maximum datum
**
**  Note: If no data, return STAT_NaN
*/

#if !(__cplusplus)

double stat_max(Stat_T *ptr)
{
      if (ptr->count)
      {
            if (ptr->filt)
                  return ptr->filt[ptr->count - 1];
            else  return ptr->max;
      }
      else  return STAT_NaN;
}

#else /* C++ */

inline double Stat::Max()
{
      if (count)
      {
            if (filt)
                  return filt[count - 1];
            else  return max;
      }
      else  return STAT_NaN;
}

#endif

/*
**  Return the error (%) for the minimum datum
**
**  Parameters: 1 - Pointer to Stat_T structure type
**
**  Returns: Percentage error of the minimmum datum
**
**  Note: If no data, return STAT_NaN
*/

#if !(__cplusplus)

double stat_minerror(Stat_T *ptr)
{
      double Mean = stat_mean(ptr);

      if (ptr->count)
            return 100.0 * (stat_min(ptr) - Mean) / Mean;
      else  return STAT_NaN;
}

#else /* C++ */

double Stat::Minerror()
{
      double mean_ = Mean();

      if (ptr->count)
            return 100.0 * (Min() - mean) / mean;
      else  return STAT_NaN;
}

#endif

/*
**  Return the error (%) for the maximum datum
**
**  Parameters: 1 - Pointer to Stat_T structure type
**
**  Returns: Percentage error of the minimmum datum
**
**  Note: If no data, return STAT_NaN
*/

#if !(__cplusplus)

double stat_maxerror(Stat_T *ptr)
{
      double Mean = stat_mean(ptr);

      if (ptr->count)
            return 100.0 * (stat_max(ptr) - Mean) / Mean;
      else  return STAT_NaN;
}

#else /* C++ */

double Stat::Maxerror()
{
      double mean_ = Mean();

      if (ptr->count)
            return 100.0 * (Max() - mean) / mean;
      else  return STAT_NaN;
}

#endif

/*
**  Compute the arithmetic mean
**
**  Parameters: 1 - Pointer to Stat_T structure type
**
**  Returns: Arithmetic mean of the data
**
**  Note: If no data, return STAT_NaN
*/

#if !(__cplusplus)

double stat_mean(Stat_T *ptr)
{
      if (ptr->count)
            return (ptr->total / ptr->count);
      else  return STAT_NaN;
}

#else /* C++ */

double Stat::Mean()
{
      if (count)
            return (total / count);
      else  return STAT_NaN;
}

#endif

/*
**  Compute the geometric mean
**
**  Parameters: 1 - Pointer to Stat_T structure type
**
**  Returns: Geometric mean of the data
**
**  Note: If no data, return STAT_NaN
*/

#if !(__cplusplus)

double stat_gmean(Stat_T *ptr)
{
      if (ptr->count)
            return pow(ptr->product, 1.0 / ptr->count);
      else  return STAT_NaN;
}

#else /* C++ */

double Stat::Gmean()
{
      if (count)
            return pow(product, 1.0 / count);
      else  return STAT_NaN;
}

#endif

/*
**  Compute the harmonic mean
**
**  Parameters: 1 - Pointer to Stat_T structure type
**
**  Returns: Harmonic mean of the data
**
**  Note: If no data, return STAT_NaN
*/

#if !(__cplusplus)

double stat_hmean(Stat_T *ptr)
{
      if (ptr->count)
            return ptr->count / ptr->recip;
      else  return STAT_NaN;
}

#else /* C++ */

double Stat::Hmean()
{
      if (count)
            return count / recip;
      else  return STAT_NaN;
}

#endif

/*
**  Compute the standard deviation of the population
**
**  Parameters: 1 - Pointer to Stat_T structure type
**
**  Returns: Standard deviation of the population
**
**  Note: If no data, return STAT_NaN
*/

#if !(__cplusplus)

double stat_stddevP(Stat_T *ptr)
{
      if (ptr->count)
            return sqrt(stat_varP(ptr));
      else  return STAT_NaN;
}

#else /* C++ */

double Stat::StddevP()
{
      if (count)
            return sqrt(VarP);
      else  return STAT_NaN;
}

#endif

/*
**  Compute the standard deviation of the sampled data
**
**  Parameters: 1 - Pointer to Stat_T structure type
**
**  Returns: Standard deviation of the sampled data
**
**  Note: If insufficient data, return STAT_NaN
*/

#if !(__cplusplus)

double stat_stddevS(Stat_T *ptr)
{
      if (ptr->count >= 2)
            return sqrt(stat_varS(ptr));
      else  return STAT_NaN;
}

#else /* C++ */

double Stat::StddevS()
{
      if (count >= 2)
            return sqrt(VarS);
      else  return STAT_NaN;
}

#endif

/*
**  Compute the variance of the population
**
**  Parameters: 1 - Pointer to Stat_T structure type
**
**  Returns: Variance of the population
**
**  Note: If no data, return STAT_NaN
**
**  Note: If no data, return STAT_NaN
*/

#if !(__cplusplus)

double stat_varP(Stat_T *ptr)
{
      if (ptr->count)
            return ((ptr->count * ptr->total2) - (ptr->total * ptr->total)) / (ptr->count * ptr->count);
      else  return STAT_NaN;
}

#else /* C++ */

inline double Stat::VarP()
{
      if (count)
            retval = ((count * total2) - (total * total)) / (count * count);
      else  return STAT_NaN;
}

#endif

/*
**  Compute the variance of the sample
**
**  Parameters: 1 - Pointer to Stat_T structure type
**
**  Returns: Variance of the sampled data
**
**  Note: If insufficient data, return STAT_NaN
*/

#if !(__cplusplus)

double stat_varS(Stat_T *ptr)
{
      if (ptr->count >= 2)
            return ((ptr->count * ptr->total2) - (ptr->total * ptr->total)) / ((ptr->count - 1) * (ptr->count - 1));
      else  return STAT_NaN;
}

#else /* C++ */

inline double Stat::VarS()
{
      if (ptr->count >= 2)
            return ((count * total2) - (total * total)) / ((count - 1) * (count - 1));
      else  return STAT_NaN;
}

#endif

/*
**  Compute the coefficient of variation (percentage) for the population
**
**  Parameters: 1 - Pointer to Stat_T structure type
**
**  Returns: Coefficient of variation of the population
**
**  Note: If no data, return STAT_NaN
*/

#if !(__cplusplus)

double stat_varcoeffP(Stat_T *ptr)
{
      if (ptr->count)
            return (stat_varP(ptr) / stat_mean(ptr)) * 100.0;
      else  return STAT_NaN;
}

#else /* C++ */

double Stat::VarcoeffP()
{
      if (count)
            return (Varp(ptr) / Mean(ptr)) * 100.0;
      else  return STAT_NaN;
}

#endif

/*
**  Compute the coefficient of variation (percentage) for the sample
**
**  Parameters: 1 - Pointer to Stat_T structure type
**
**  Returns: Coefficient of variation of the sampled data
**
**  Note: If no data, return STAT_NaN
*/

#if !(__cplusplus)

double stat_varcoeffS(Stat_T *ptr)
{
      if (ptr->count >= 2)
            return (stat_stddevS(ptr) / stat_mean(ptr)) * 100.0;
      else  return STAT_NaN;
}

#else /* C++ */

double Stat::VarcoeffS()
{
      if (ptr->count >= 2)
            return (StddevS() / Mean()) * 100.0;
      else  return STAT_NaN;
}

#endif

/*
**  Notes about filters:
**
**  Sampled data are always subject to spurious random effects from noise or simple
**  sampling errors. To improve the quality of the data population, you use a filter.
**  A filter is simply a device which removes spurious data. After running a filter,
**  the quantity of your data may be reduced, but its quality is improved.
**
**  So it is with the filters here. Each acts on the stored data to selectively remove
**  specific data with the goal of reducing the variance of the remaining population.
**  After running the filter, new statistics may be run on the remaining data set, the
**  most common of which are arithmetic mean and standard deviation/variance.
**
**  Three specific filter types are currently implemented, although more are planned:
**
**  1.  An "Olympic" filter derives its nickname from a scoring technique used in the
**      Olympic games where the high and low data are removed from consideration. This
**      is a time-tested and amazingly effective technique which is especially well
**      adapted to resource-restricted embedded systems.
**
**  2.  A median filter simply removes all but a fixed-width window of data surrounding
**      the mid-point of an array of the sampled data. For example, if you have a
**      population of 10 samples and impose a median filter with a window width of 4,
**      the lowest 3 and highest 3 data would be removed.
**
**  3.  Combining elements of both the above, an "Olympiad" filter is an iterative
**      Olympic filter, with  smart windowing. You call an Olympiad filter with a
**      target value for the maximum tolerable coefficient of variation (expressed as
**      a percentage). The Olympiad filter will iteratively remove high and low data
**      until either the target is achieved or no more data may be removed. The only
**      disadvantage to the Olympiad filter is that by using the coefficient of
**      variation, it inherently uses the arithmetic mean of the data. Should you wish
**      to filter using the geometric or harmonic mean, you would have to create your
**      own function which iteratively called the Olympic filter.
**
**
**  Implementation notes:
**
**  As written, these functions are meant to be general purpose. As such, they use
**  techniques which might not be suitable for specific environments. In particular,
**  many embedded systems cannot afford to use dynamic memory allocation. It is left
**  as an exercise to the reader to adapt these as required for a specific application.
**
**  The original Olympic filter module used fixed registers for min and max data to
**  date. This is extremely fast and efficient, but inadequate for more advanced
**  filtering techniques. The current scheme saves all data entered into the analysis
**  in sorted order, so the min and max are always available at the top and bottom of
**  the array. The price is that the data array must be maintained for each datum
**  entry and deletion. To return to the original system, you must replace the data
**  array with min and max registers. Without the data array, only the Olympic filter
**  should be used and even it can no longer be used iteratively.
**
**  The min and max data are still saved if no data array is defined. However, an
**  analysis with a NULL data array pointer will not support filtering or removal of
**  data from the anlysis.
*/

/*** Begin filters ******************************************************************/

/*
**  "Olympic filter" - toss out the high and low data
**
**  Parameters: 1 - Pointer to Stat_T structure type
**
**  Returns: Error_ if less than 3 data, else Success_
*/

#if !(__cplusplus)

Boolean_T stat_olympic(Stat_T *ptr)
{
      Boolean_T retval;

      if (!ptr->filt || ptr->count < 3)
            return Error_;

      stat_delete(ptr->filt[0], ptr);
      stat_delete(ptr->filt[ptr->count - 1], ptr);

      return Success_;
}

#else /* C++ */

Boolean_T Stat::Olympic()
{
      if (!filt || count < 3)
            return Error_;

      Delete(filt[0]);
      Delete(filt[count - 1]);

      return Success_;
}

#endif

/*
**  Iterative "Olympic" filter - toss out the high and low data to reach a desired
**  coefficient of variation for the population. Functionally, it may be considered
**  as a "smart" median filter which auto-adjusts its window.
**
**  Parameters: 1 - Pointer to Stat_T structure type
**              2 - Target coefficient of variation
**
**  Returns: Error_ if target is not achieved, else Success_
**
**  Note: Since this filter uses the coefficient of variation, it uses the arithmetic
**        mean for its target. If you want to use either the geometric or harmonic
**        mean instead, you can do it by explicitly iterating calls to the Opympic
**        filter instead.
*/

#if !(__cplusplus)

Boolean_T stat_olympiad(Stat_T *ptr, double CoefVar)
{
      double v;

      do
      {
            if (!ptr->filt || ptr->count < 3)
                  return Error_;

            stat_delete(ptr->filt[0], ptr);
            stat_delete(ptr->filt[ptr->count - 1], ptr);

            v = stat_varcoeffP(ptr);
      } while (v > CoefVar);

      return Success_;
}

#else /* C++ */

Boolean_T Stat::Olympiad(double CoefVar)
{
      double v;

      do
      {
            if (!filt || count < 3)
                  return Error_;

            Delete(filt[0]);
            Delete(filt[count - 1]);

            v = VarcoeffP();
      } while (v > CoefVar);

      return Success_;
}

#endif

/*
**  Median filter
**
**  Parameters: 1 - Pointer to Stat_T structure type
**
**  Returns: Nothing
**
**  Side effects: Removes all data outside the specified window from the data array
*/

#if !(__cplusplus)

Boolean_T stat_median(Stat_T *ptr, size_t window)
{
      size_t i, n;

      if (!ptr->filt || (ptr->count < 1) || (ptr->count < window))
            return Error_;

      for (i = 0, n = (ptr->count - window) / 2; i < n; ++i)
            stat_delete(ptr->filt[0], ptr);

      for (i = window; ptr->count >= i; ++i)
            stat_delete(ptr->filt[window], ptr);

      return Success_;
}

#else /* C++ */

Boolean_T Stat::Median(Stat_T *ptr, size_t window)
{
      size_t i, n;

      if (!filt || (count < 1) || (count < window))
            return Error_;

      for (i = 0, n = (count - window) / 2; i < n; ++i)
            stat_delete(filt[0], ptr);

      for (i = window; count >= i; ++i)
            stat_delete(filt[window], ptr);

      return Success_;
}

#endif

/*** End filters ********************************************************************/

/*
**  Debugging functions
*/

/*
**  Debugging function to dump the Stat_T registers
*/

#if !(__cplusplus)

void stat_dump(Stat_T *ptr)
{
      printf("stat_dump(): count   = %d\n", ptr->count);
      printf("             total   = %d\n", ptr->total);
      printf("             total2  = %f\n", ptr->total2);
      printf("             recip   = %f\n", ptr->recip);
      printf("             product = %f\n", ptr->product);
      printf("             filt    = %p\n", ptr->filt);
      printf("             min     = %p\n", ptr->min);
      printf("             max     = %p\n", ptr->max);
}

#else /* C++ */

void Stat::dump()
{
      cout << "dump(): count   = " << count << endl;
      cout << "        total   = " << total << endl;
      cout << "        total2  = " << total2 << endl;
      cout << "        recip   = " << recip << endl;
      cout << "        product = " << product << endl;
      cout << "        filt    = " << filt << endl;
      cout << "        min     = " << min << endl;
      cout << "        max     = " << min << endl;
}

#endif

/*
**  Debugging function to dump the data array
*/

#if !(__cplusplus)

void stat_qdump(Stat_T *ptr)
{
      size_t i;

      puts(">>DATA ARRAY");
      for (i = 0; i < ptr->count; ++i)
            printf("filt[%02d]=%f\n", i, ptr->filt[i]);
}

#else /* C++ */

void Stat::qdump()
{
      size_t i;

      cout << ">>DATA ARRAY" << endl;
      for (i = 0; i < ptr->count; ++i)
            cout << "filt[" << i << "]=" << i, ptr->filt[i] << endl;
}

#endif

/*
**  Test harness begins here. Define TEST macro to build stand-alone.
*/

#ifdef TEST

#include <stdlib.h>

#if !(__cplusplus)

#include <stdio.h>

int main(int argc, char *argv[])
{
      Stat_T data;
      size_t i;

      stat_init(&data, 20);
      while (--argc)
      {
            double ftmp;

            ftmp = atof(*(++argv));
            stat_add(ftmp, &data);
      }

      puts("\nBefore \"Olympic\" filtering\n");
      printf("Minimum datum             = %g\n", stat_min(&data));
      printf("Maximum datum             = %g\n", stat_max(&data));
      printf("Number of samples         = %d\n", stat_count(&data));
      printf("Arithmetic mean           = %g\n", stat_mean(&data));
      printf("Geometric mean            = %g\n", stat_gmean(&data));
      printf("Harmonic mean             = %g\n", stat_hmean(&data));
      printf("Standard deviation (N)    = %g\n", stat_stddevP(&data));
      printf("Standard deviation (N-1)  = %g\n", stat_stddevS(&data));
      printf("Variance (population)     = %g\n", stat_varP(&data));
      printf("Variance (sample)         = %g\n", stat_varS(&data));
      printf("Population coeff. of var. = %g%%\n", stat_varcoeffP(&data));
      printf("Sample coeff. of var.     = %g%%\n", stat_varcoeffS(&data));
/*      qdump(&data);*/

      i = data.count - 2;
      if (Success_ == stat_median(&data, i))
            printf("\nMedian filter (window=%d)  = %g\n", i, stat_mean(&data));
      else  printf("Median filter failed, window=%d\n", i);
/*      qdump(&data);*/

      printf("\nDeleting sample #%d\n", data.count / 2);
      stat_delete(data.filt[data.count / 2], &data);
/*      qdump(&data);*/

      puts("\nAfter \"Olympic\" filtering\n");
      printf("stat_olympic() returned %s\n", stat_olympic(&data) ?
            "ERROR" : "SUCCESS");
      printf("Minimum datum             = %g\n", stat_min(&data));
      printf("Maximum datum             = %g\n", stat_max(&data));
      printf("Number of samples         = %d\n", stat_count(&data));
      printf("Arithmetic mean           = %g\n", stat_mean(&data));
      printf("Geometric mean            = %g\n", stat_gmean(&data));
      printf("Harmonic mean             = %g\n", stat_hmean(&data));
      printf("Standard deviation (N)    = %g\n", stat_stddevP(&data));
      printf("Standard deviation (N-1)  = %g\n", stat_stddevS(&data));
      printf("Variance (population)     = %g\n", stat_varP(&data));
      printf("Variance (sample)         = %g\n", stat_varS(&data));
      printf("Population coeff. of var. = %g%%\n", stat_varcoeffP(&data));
      printf("Sample coeff. of var.     = %g%%\n", stat_varcoeffS(&data));
/*      qdump(&data);*/

      puts("\nAfter \"Olympiad\" filtering, CoeffVar = 5.0\n");
      printf("stat_olympiad() returned %s\n", stat_olympiad(&data, 5.0) ?
             "ERROR" : "SUCCESS");
      printf("Minimum datum             = %g\n", stat_min(&data));
      printf("Maximum datum             = %g\n", stat_max(&data));
      printf("Number of samples         = %d\n", stat_count(&data));
      printf("Arithmetic mean           = %g\n", stat_mean(&data));
      printf("Geometric mean            = %g\n", stat_gmean(&data));
      printf("Harmonic mean             = %g\n", stat_hmean(&data));
      printf("Standard deviation (N)    = %g\n", stat_stddevP(&data));
      printf("Standard deviation (N-1)  = %g\n", stat_stddevS(&data));
      printf("Variance (population)     = %g\n", stat_varP(&data));
      printf("Variance (sample)         = %g\n", stat_varS(&data));
      printf("Population coeff. of var. = %g%%\n", stat_varcoeffP(&data));
      printf("Sample coeff. of var.     = %g%%\n", stat_varcoeffS(&data));
/*      qdump(&data);*/

      return EXIT_SUCCESS;
}

#else /* C++ */

#include <iostreams.h>

int main(int argc, char *argv[])
{
      class Stat data(20);
      char *str;

      while (--argc)
      {
            double ftmp;

            ftmp = atof(*(++argv));
            data.Add(ftmp);
      }

      cout << endl << "Before \"Olympic\" filtering\n" << endl << endl;
      cout << "Minimum datum             = " << data.Min()       << endl;
      cout << "Maximum datum             = " << data.Max()       << endl;
      cout << "Number of samples         = " << data.Count()     << endl;
      cout << "Arithmetic mean           = " << data.Mean()      << endl;
      cout << "Geometric mean            = " << data.Gmean()     << endl;
      cout << "Harmonic mean             = " << data.Hmean()     << endl;
      cout << "Standard deviation (N)    = " << data.StddevP()   << endl;
      cout << "Standard deviation (N-1)  = " << data.StddevS()   << endl;
      cout << "Variance (population)     = " << data.VarP()       << endl;
      cout << "Variance (sample)         = " << data.VarS()       << endl;
      cout << "Population coeff. of var. = " << data.VarcoeffP() << "%" << endl;
      cout << "Sample coeff. of var.     = " << data.VarcoeffS() << "%" << endl;

      if (Success_ == data.Olympic())
            str = "SUCCESS";
      else  str = "ERROR";

      cout << endl << "After \"Olympic\" filtering" << endl << endl;
      cout << "data.Olympic() returned " << str << endl;
      cout << "Minimum datum             = " << data.Min()       << endl;
      cout << "Maximum datum             = " << data.Max()       << endl;
      cout << "Number of samples         = " << data.Count()     << endl;
      cout << "Arithmetic mean           = " << data.Mean()      << endl;
      cout << "Geometric mean            = " << data.Gmean()     << endl;
      cout << "Harmonic mean             = " << data.Hmean()     << endl;
      cout << "Standard deviation (N)    = " << data.StddevP()   << endl;
      cout << "Standard deviation (N-1)  = " << data.StddevS()   << endl;
      cout << "Variance (population)     = " << data.VarP()       << endl;
      cout << "Variance (sample)         = " << data.VarS()       << endl;
      cout << "Population coeff. of var. = " << data.VarcoeffP() << "%" << endl;
      cout << "Sample coeff. of var.     = " << data.VarcoeffS() << "%" << endl;

      return EXIT_SUCCESS;
}

#endif /* C/C++ */

#endif
