/************************************************************************/
/*                                                                      */
/*  QQD.H - Contains declarations for double-ended queue functions for  */
/*         doubles.                                                     */
/*                                                                      */
/*  Public domain by Bob Stout                                          */
/*                                                                      */
/************************************************************************/

#ifndef QQD_H
#define QQD_H

#include <stddef.h>
#include "sniptype.h"

extern const VAR64_     QQD_NaN;
#define QQD_EMPTY       QQD_NaN.d   /* NaN  */

struct QQD {
      double    *queue;
      size_t    len;
      size_t    head;
      size_t    tail;
      Boolean_T full;
};

/*
**  File: QQ.C prototypes
*/

Boolean_T   QQDopen(struct QQD **queue, size_t len);
void        QQDclose(struct QQD **queue);

/*
**  FIFO Functions
*/

Boolean_T   QQDenqueue(struct QQD *queue, double datum);
double      QQDdequeue(struct QQD *queue);
double      QQDpeekF(struct QQD *queue);

/*
**  LIFO Functions
*/

Boolean_T   QQDpush(struct QQD *queue, double datum);
double      QQDpop(struct QQD *queue);
double      QQDpeekL(struct QQD *queue);

/*
**  Maintenance Functions
*/

void        QQDqflush(struct QQD *queue);
Boolean_T   QQDisfull(struct QQD *queue);
Boolean_T   QQDisempty(struct QQD *queue);
size_t      QQDnum_enqueued(struct QQD *queue);

void        QQDdump(struct QQD *queue);

#endif /* QQD_H */
