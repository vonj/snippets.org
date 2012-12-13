/********************************************************************
          The McGill Super-Duper Random Number Generator
             G. Marsaglia, K. Ananthanarayana, N. Paul
  
    Incorporating the Ziggurat method of sampling from decreasing
              or symmetric unimodal density functions.
                      G. Marsaglia, W.W. Tsang
  
                 Rewritten into C by E. Schneider
 *********************************************************************/
  
static unsigned long mcgn, srgn;
  
#define MULT 69069L
  
void rstart (long i1, long i2)
{
      mcgn = (unsigned long)((i1 == 0L) ? 0L : i1 | 1L);
      srgn = (unsigned long)((i2 == 0L) ? 0L : (i2 & 0x7FFL) | 1L);
}
  
long uni(void)
{
      unsigned long r0, r1;

      r0 = (srgn >> 15);
      r1 = srgn ^ r0;
      r0 = (r1 << 17);
      srgn = r0 ^ r1;
      mcgn = MULT * mcgn;
      r1 = mcgn ^ srgn;
      return (r1 >> 1);
}
  
long vni(void)
{
      unsigned long r0, r1;

      r0 = (srgn >> 15);
      r1 = srgn ^ r0;
      r0 = (r1 << 17);
      srgn = r0 ^ r1;
      mcgn = MULT * mcgn;
      r1 = mcgn ^ srgn;
      return r1;
}

/* 
"Anyone who consider arithmetic means of producing random number is,
 of course, in a state of sin" - John Von Neumann
*/
