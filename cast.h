/*
**  public domain demo by Bob Stout
*/

#ifndef CAST__H
#define CAST__H

#define CAST(new_type,old_object) (*((new_type *)(&(old_object))))

#if 0
*************************************************************************
*                                                                       *
*       /* Example of CAST macro at work        */                      *
*                                                                       *
*       union {                                                         *
*               char    ch[4];                                          *
*               int     i[2];                                           *
*       } my_union;                                                     *
*                                                                       *
*       long    longvar;                                                *
*                                                                       *
*               longvar = (long)my_union;       /* Illegal cast */      *
*               longvar = CAST(long, my_union); /* Legal cast   */      *
*                                                                       *
*************************************************************************
#endif  /* 0 */

#endif /* CAST__H */
