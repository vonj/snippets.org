/*
 *  File : DEQUE.H
 *
 *  Peter Yard  02 Jan 1993.
 */

#ifndef DEQUEUE__H
#define DEQUEUE__H

#include "sniptype.h"                     /* For True_, False_    */


typedef struct nodeptr datanode;

typedef struct nodeptr {
      void        *data ;
      datanode    *prev, *next ;
} node ;

typedef struct {
      node        *head, *tail, *cursor;
      int         size, sorted, item_deleted;
} queue;

typedef  struct {
      void        *dataptr;
      node        *loc ;
} index_elt ;


int    Q_Init(queue  *q);
int    Q_Empty(queue *q);
int    Q_Size(queue *q);
int    Q_Start(queue *q);
int    Q_End(queue *q);
int    Q_PushHead(queue *q, void *d);
int    Q_PushTail(queue *q, void *d);
void  *Q_First(queue *q);
void  *Q_Last(queue *q);
void  *Q_PopHead(queue *q);
void  *Q_PopTail(queue *q);
void  *Q_Next(queue *q);
void  *Q_Previous(queue *q);
void  *Q_DelCur(queue *q);
void  *Q_Get(queue *q);
int    Q_Put(queue *q, void *data);
int    Q_Sort(queue *q, int (*Comp)(const void *, const void *));
int    Q_Find(queue *q, void *data,
              int (*Comp)(const void *, const void *));
void  *Q_Seek(queue *q, void *data,
              int (*Comp)(const void *, const void *));
int    Q_Insert(queue *q, void *data,
                int (*Comp)(const void *, const void *));

#endif /* DEQUEUE__H */
