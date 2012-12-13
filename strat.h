/*
**  strat.h 10-5-91  Robert Mashlan, public domain
**
**  header file for strat.c
**
*/

#ifndef STRAT__H
#define STRAT__H

const enum {
      FIRST_FIT_LOW,
      BEST_FIT_LOW,
      LAST_FIT_LOW,
      FIRST_FIT_HIGH = 0x80, /* these strategies available only in DOS 5.0 */
      BEST_FIT_HIGH,
      LAST_FIT_HIGH,
      FIRST_FIT_HIGHONLY = 0x40,
      BEST_FIT_HIGHONLY,
      LAST_FIT_HIGHONLY
      };

int get_alloc_strat(void);
int set_alloc_strat( int strat );
int set_handle_count( unsigned nhandles );

#endif /* STRAT__H */
