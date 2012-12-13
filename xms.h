/*
** XMS.H
**
** Header file for Extended Memory routines in XMS.C.
*/

#ifndef _XMS_H
#define _XMS_H

int  XMSinit(void);
int  XMSversion(void);
long XMScoreleft(void);
int  XMSfree(unsigned int handle);
long XMSmemcpy(unsigned int desthandle, long destoff,
                 unsigned int srchandle, long srcoff, long n);
int  DOStoXMSmove(unsigned int desthandle, long destoff,
                 const char *src, int n);
int  XMStoDOSmove(char *dest, unsigned int srchandle, long srcoff, int n);

unsigned int XMSalloc(long size);

#endif
