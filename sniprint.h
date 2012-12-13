/*
**  Header file for SNIPPETS PC printer control functions
*/

#ifndef SNIPRINT__H
#define SNIPRINT__H

#include <stdio.h>            /* For FILE */

/*
**  ASSIGNPRN.C
*/

#define NUM_OF_PRNTRS 6

extern FILE *printer[NUM_OF_PRNTRS];

int assign_printer(int number, char *device);

/*
**  CHANGPRN.C
*/

typedef enum {LPT1, LPT2, LPT3, COM1, COM2, CON} PrintDevice;

int change_prn(PrintDevice device);

/*
**  PRTOGGLE.C
*/

int prtoggle(void);

/*
**  PRTSCRN.C
*/

int PrtScrnStat(void);
int PrtScrn(void);

/*
**  PRTSTAT.C
*/

struct PrStatus {
      unsigned int timeout  : 1;
      unsigned int unused   : 2;
      unsigned int IOerror  : 1;
      unsigned int selected : 1;
      unsigned int paperout : 1;
      unsigned int ack      : 1;
      unsigned int notbusy  : 1;
};

int prtstat(unsigned int);


#endif /* SNIPRINT__H */
