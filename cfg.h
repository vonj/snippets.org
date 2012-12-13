/* =======================================================================
    CFG.h       Configuration file handler.
                A. Reitsma, Delft, The Netherlands.
                v1.00  94-07-09  Public Domain.
 ---------------------------------------------------------------------- */

struct CfgStrings
{
    char * name ;
    char * data ;
};

int CfgRead( char * Filename, struct CfgStrings * CfgInfo );

/* ==== CFG.h end ===================================================== */
