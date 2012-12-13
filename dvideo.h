/*----------------------------[ dvideo.h ]-----------------------------*/
/*                     Hardware Interface Routines                     */
/*---------------------------------------------------------------------*/

#ifndef DVIDEO__H
#define DVIDEO__H

typedef struct
{
    int ver_major;
    int ver_minor;
    unsigned regen_buf;
    int win_rows;
    int win_cols;
} DV_INFO;

unsigned char     get_vidpage(void);
int               is_dv(void);
int               get_dvinfo(DV_INFO *dv_info);
unsigned          get_vidbase(void);
int               get_rows(void);
int               get_cols(void);
int               is_egavga(void);

#endif /* DVIDEO__H */
