/************************************************************************
 This random number generator originally appeared in "Toward a Universal
 Random Number Generator" by George Marsaglia and Arif Zaman.
 Florida State University Report: FSU-SCRI-87-50 (1987)

 It was later modified by F. James and published in "A Review of Pseudo-
 random Number Generators"

 Converted from FORTRAN to C by Phil Linttell, James F. Hickling
 Management Consultants Ltd, Aug. 14, 1989.

 THIS IS THE BEST KNOWN RANDOM NUMBER GENERATOR AVAILABLE.
       (However, a newly discovered technique can yield
         a period of 10^600. But that is still in the development stage.)

 It passes ALL of the tests for random number generators and has a period
   of 2^144, is completely portable (gives bit identical results on all
   machines with at least 24-bit mantissas in the floating point
   representation).

 The algorithm is a combination of a Fibonacci sequence (with lags of 97
   and 33, and operation "subtraction plus one, modulo one") and an
   "arithmetic sequence" (using subtraction).

 On a Vax 11/780, this random number generator can produce a number in
    13 microseconds.
************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define TRUE    1
#define FALSE   0

float u[97], c, cd, cm;
int i97, j97, test;

int rmarin(int ij, int kl);
int ranmar(float rvec[], int len);


int main()
{

        float temp[100];
        int i;
        int ij, kl, len;

        /*These are the seeds needed to produce the test case results*/

        ij = 1802;
        kl = 9373;

        /*Do the initialization*/

        if (1 == rmarin(ij,kl))
                return 1;

        /*Generate 20000 random numbers*/

        len = 100;
        for ( i=0; i<=199 ; i++)
                if (1 == ranmar(temp, len))
                        return 1;
    
        /*If the random number generator is working properly,
          the next six random numbers should be:

            6533892.0  14220222.0   7275067.0
            6172232.0   8354498.0  10633180.0
        */

        len = 6;
        if (1 == ranmar(temp, len))
                return 1;

        for ( i=0; i<=5; i++)
                printf("%12.1f\n",4096.0*4096.0*temp[i]);

        return 0;
}


/************************************************************************
 This is the initialization routine for the random number generator RANMAR()
 NOTE: The seed variables can have values between:    0 <= IJ <= 31328
                                                      0 <= KL <= 30081
 The random number sequences created by these two seeds are of sufficient
 length to complete an entire calculation with. For example, if several
 different groups are working on different parts of the same calculation,
 each group could be assigned its own IJ seed. This would leave each group
 with 30000 choices for the second seed. That is to say, this random
 number generator can create 900 million different subsequences -- with
 each subsequence having a length of approximately 10^30.

 Use IJ = 1802 & KL = 9373 to test the random number generator. The
 subroutine RANMAR should be used to generate 20000 random numbers.
 Then display the next six random numbers generated multiplied by 4096*4096
 If the random number generator is working properly, the random numbers
 should be:
           6533892.0  14220222.0   7275067.0
           6172232.0   8354498.0  10633180.0
************************************************************************/

int rmarin(int ij, int kl)
{

        float s, t;
        int i, j, k, l, m;
        int ii, jj;

        /* Change FALSE to TRUE in the next statement to test the
           random routine.*/

        test = TRUE;

        if ( ( ij < 0 || ij > 31328 ) ||
                ( kl < 0 || kl > 30081 ) )
        {
                printf ("RMARIN: The first random number seed must have a "
                        "value between 0 and 31328\n");
                printf ("        The second random number seed must have a "
                        "value between 0 and 30081");
                return 1;
        }

        i = (int)fmod(ij/177.0, 177.0) + 2;
        j = (int)fmod(ij      , 177.0) + 2;
        k = (int)fmod(kl/169.0, 178.0) + 1;
        l = (int)fmod(kl      , 169.0);

        for ( ii=0; ii<=96; ii++ )
        {
                s = (float)0.0;
                t = (float)0.5;
                for ( jj=0; jj<=23; jj++ )
                {
                        m = (int)fmod( fmod(i*j,179.0)*k , 179.0 );
                        i = j;
                        j = k;
                        k = m;
                        l = (int)fmod( 53.0*l+1.0 , 169.0 );
                        if ( fmod(l*m,64.0) >= 32)
                                s = s + t;
                        t = (float)(0.5 * t);
                }
                u[ii] = s;
        }

        c  = (float)(  362436.0 / 16777216.0);
        cd = (float)( 7654321.0 / 16777216.0);
        cm = (float)(16777213.0 / 16777216.0);

        i97 = 96;
        j97 = 32;

        test = TRUE;

        return 0;
}

int ranmar(float rvec[], int len)
{
        float uni;
        int ivec;

        if ( !test )
        {
                printf ("RANMAR: Call the initialization routine (RMARIN) "
                        "before calling RANMAR.\n");
                return 1;
        }

        for ( ivec=0; ivec < len; ivec++)
        {
                uni = u[i97] - u[j97];
                if ( uni < 0.0F )
                        uni = uni + 1.0;
                u[i97] = uni;
                i97--;
                if ( i97 < 0 )
                        i97 = 96;
                j97--;
                if ( j97 < 0 )
                        j97 = 96;
                c = c - cd;
                if ( c < 0.0F )
                        c = c + cm;
                uni = uni - c;
                if ( uni < 0.0F )
                        uni = uni + 1.0;
                rvec[ivec] = uni;
        }
        return 0;
}

/* I use the following procedure in TC to generate seeds:

  The sow() procedure calculates two seeds for use with the random number
  generator from the system clock.  I decided how to do this myself, and
  I am sure that there must be better ways to select seeds; hopefully,
  however, this is good enough.  The first seed is calculated from the values
  for second, minute, hour, and year-day; weighted with the second most
  significant and year-day least significant.  The second seed weights the
  values in reverse.
*/

void sow( seed1, seed2 )
int *seed1, *seed2;
{
        struct tm *tm_now;
        float s_sig, s_insig, maxs_sig, maxs_insig;
        time_t secs_now;
        int s, m, h, d, s1, s2;

        time(&secs_now);
        tm_now = localtime(&secs_now);

        s = tm_now->tm_sec + 1;
        m = tm_now->tm_min + 1;
        h = tm_now->tm_hour + 1;
        d = tm_now->tm_yday + 1;

        maxs_sig   = (float)(60.0 + 60.0/60.0 + 24.0/60.0/60.0 +
              366.0/24.0/60.0/60.0);
        maxs_insig = (float)(60.0 + 60.0*60.0 + 24.0*60.0*60.0 +
              366.0*24.0*60.0*60.0);

        s_sig      = (float)(s + m/60.0 + h/60.0/60.0 + d/24.0/60.0/60.0);
        s_insig    = (float)(s + m*60.0 + h*60.0*60.0 + d*24.0*60.0*60.0);

        s1 = (int)(s_sig   / maxs_sig   * 31328.0);
        s2 = (int)(s_insig / maxs_insig * 30081.0);

        *seed1 = s1;
        *seed2 = s2;
}
