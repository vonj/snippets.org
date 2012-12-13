#ifdef __WATCOMC__
 #pragma off (unreferenced);
#endif

/* testcmt.c - test getcmt.exe - C comment 1 */
int i;
main()
{
    int j;
    /* C comment 2 */
    int k;
    /* C comment 3 */
    char ch2;    /* C comment 4 */  char ch3;   // C++ comment 1
    // C++ comment 2
    char ch4;
    // C++ comment 3
    i = 0;
    return(i);
}
/* end of testcmt.c - C comment 5 */
