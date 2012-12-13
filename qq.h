/************************************************************************/
/*                                                                      */
/*  QQ.H - Contains declarations for general-purpose queue functions.   */
/*                                                                      */
/*  Public domain by Bob Stout                                          */
/*                                                                      */
/*  What makes this package general purpose is the fact that all data   */
/*  are stored cast to unsigned long, allowing any integer or pointer   */
/*  data type to be used interchangeably.                               */
/*                                                                      */
/************************************************************************/

#ifndef QQ_H
#define QQ_H

#include <stddef.h>
#include "sniptype.h"

#define QQ_EMPTY        ((unsigned long)(-1L))

struct QQ {
      unsigned long     *queue;
      size_t             len;
      size_t             head;
      size_t             tail;
      Boolean_T          full;
};

/*
**  File: QQ.C prototypes
*/

Boolean_T     QQopen(struct QQ **queue, size_t len);
void          QQclose(struct QQ **queue);

/*
**  FIFO Functions
*/

Boolean_T     QQenqueue(struct QQ *queue, unsigned long datum);
unsigned long QQdequeue(struct QQ *queue);
unsigned long QQpeekF(struct QQ *queue);

/*
**  LIFO Functions
*/

Boolean_T     QQpush(struct QQ *queue, unsigned long datum);
unsigned long QQpop(struct QQ *queue);
unsigned long QQpeekL(struct QQ *queue);

/*
**  Maintenance Functions
*/

void          QQqflush(struct QQ *queue);
Boolean_T     QQisfull(struct QQ *queue);
Boolean_T     QQisempty(struct QQ *queue);
size_t        QQnum_enqueued(struct QQ *queue);

#endif /* QQ_H */
