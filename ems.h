/*
** EMS.H
**
** Header file Expanded Memory Routines
*/

#ifndef EMS_H_
#define EMS_H_

#define EMS_PAGE_SIZE   16384   /* Each page is this size */

unsigned int EMSbaseaddress(void);
int  EMSversion(void);
int  EMSstatus(void);
int  EMSpages(void);
int  EMSalloc(int pages);
int  EMSfree(int handle);
int  EMSmap(int bank, int handle, int page);

#endif
