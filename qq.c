/************************************************************************/
/*                                                                      */
/*  QQ.C - Implements a general-pupose queuing mechanism.               */
/*                                                                      */
/*  Public domain by Bob Stout                                          */
/*                                                                      */
/*  What makes this package general purpose is the fact that all data   */
/*  are stored cast to unsigned long, allowing any integer or pointer   */
/*  data type to be used interchangeably.                               */
/*                                                                      */
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "qq.h"


/*
**  QQopen() - Queue initialization function.
**
**  Parameters: 1 - Address of a queue of type struct QQ
**              2 - Number of entries (each will be cast to unsigned long)
**
**  Returns: Success_ or Error_
**
**  Side effects: Allocated memory, zeros head and tail pointers,
**                sets length, clears full flag.
*/

Boolean_T QQopen(struct QQ **queue, size_t len)
{
      if (NULL == (*queue = malloc(sizeof(struct QQ))))
            return Error_;
      if (NULL == ((*queue)->queue = calloc(len, sizeof(unsigned long))))
            return Error_;

      (*queue)->len  = len;
      (*queue)->head = (*queue)->tail = 0;
      (*queue)->full = False_;
      return Success_;
}

/*
**  QQclose() - Close a queue.
**
**  Parameters: 1 - Address of a queue of type struct QQ
**
**  Returns: Nothing.
**
**  Side effects: Deallocates memory.
*/

void QQclose(struct QQ **queue)
{
      FREE(*queue);
}

/*
**  QQenqueue() - FIFO enqueuing function.
**
**  Parameters: 1 - Address of a queue of type struct QQ
**              2 - Datum to enqueue, cast to unsigned long
**
**  Returns: Success_ or Error_
**
**  Side effects: Adjusts head pointer.
*/

Boolean_T QQenqueue(struct QQ *queue, unsigned long datum)
{
      int diff;

      if (queue->full)
            return Error_;

      diff = (int)queue->head - (int)queue->tail;
      if (diff < 0)
            diff += (int)queue->len;

      if (diff >= (int)queue->len)
            return Error_;

      queue->queue[queue->head++] = datum;
      if (queue->head >= queue->len)
            queue->head = 0;
      if (queue->head == queue->tail)
            queue->full = True_;
      return Success_;
}

/*
**  QQdequeue() - FIFO dequeuing function
**
**  Parameters: 1 - Address of a queue of type struct QQ
**
**  Returns: Oldest datum in queue or QQ_EMPTY.
**
**  Side effects: Adjusts tail pointer
*/

unsigned long QQdequeue(struct QQ *queue)
{
      unsigned long retval;

      if ((queue->head == queue->tail) && !queue->full)
            return QQ_EMPTY;

      retval = queue->queue[queue->tail++];
      if (queue->tail >= queue->len)
            queue->tail = 0;
      queue->full = False_;
      return retval;
}

/*
**  QQpeekF() - FIFO peek function - CAUTION: returns the oldest data in the
**              queue without removing it from the queue.
**
**  Parameters: 1 - Address of a queue of type struct QQ
**
**  Returns: Oldest datum in queue or QQ_EMPTY.
**
**  Side effects: None.
*/

unsigned long QQpeekF(struct QQ *queue)
{
      if ((queue->head == queue->tail) && !queue->full)
            return QQ_EMPTY;

      return queue->queue[queue->tail];
}

/*
**  QQpush() - LIFO enqueuing function.
**
**  Parameters: 1 - Address of a queue of type struct QQ
**              2 - Datum to enqueue, cast to unsigned long
**
**  Returns: Success_ or Error_
**
**  Side effects: Adjusts head pointer.
**
**  Notes: Wrapper for QQenqueue().
*/

Boolean_T QQpush(struct QQ *queue, unsigned long datum)
{
      return QQenqueue(queue, datum);
}

/*
**  QQpop() - LIFO dequeuing function
**
**  Parameters: 1 - Address of a queue of type struct QQ
**
**  Returns: Newest datum in queue or QQ_EMPTY.
**
**  Side effects: Adjusts head pointer
*/

unsigned long QQpop(struct QQ *queue)
{
      unsigned long retval;

      if ((queue->head == queue->tail) && !queue->full)
            return QQ_EMPTY;

      queue->head -= 1;
      if (queue->head < 0 || queue->head > queue->len)
            queue->head = queue->len - 1;
      retval = queue->queue[queue->head];
      queue->full = False_;
      return retval;
}

/*
**  QQpeekL() - LIFO peek function - CAUTION: returns the newest data in the
**              queue without removing it from the queue.
**
**  Parameters: 1 - Address of a queue of type struct QQ
**
**  Returns: Newest datum in queue or QQ_EMPTY.
**
**  Side effects: None.
*/

unsigned long QQpeekL(struct QQ *queue)
{
      size_t tmp_head;

      if ((queue->head == queue->tail) && !queue->full)
            return QQ_EMPTY;

      tmp_head = queue->head - 1;
      if (tmp_head < 0 || tmp_head > queue->len)
            tmp_head = queue->len - 1;

      return queue->queue[tmp_head];
}

/*
**  QQisfull() - Function to determine is a queue is full.
**
**  Parameters: 1 - Address of a queue of type struct QQ
**
**  Returns: True_ if queue is full, False_ if not full.
*/

Boolean_T QQisfull(struct QQ *queue)
{
      return queue->full;
}

/*
**  QQisempty() - Function to determine is a queue is empty.
**
**  Parameters: 1 -Address of a queue of type struct QQ
**
**  Returns: True_ if queue is empty, False_ if not empty.
*/

int QQisempty(struct QQ *queue)
{
      return ((queue->head == queue->tail) && !queue->full);
}

/*
**  QQflush() - FIFO queue flushing function.
**
**  Parameters: 1 - Address of a queue of type struct QQ
**
**  Returns: N.A.
**
**  Side effects: Adjusts head and tail pointers
*/

void QQqflush(struct QQ *queue)
{
      queue->head = queue->tail = 0;
      queue->full = False_;
}

/*
**  QQnum_enqueued() - FIFO queue function to return the number of queue
**                     entries.
**
**  Parameters: 1 - Address of a queue of type struct QQ
**
**  Returns: Number of data in the queue
**
**  Side effects: Adjusts head and tail pointers
*/

size_t QQnum_enqueued(struct QQ *queue)
{
      int diff;

      if (queue->full)
            return queue->len;

      diff = (int)queue->head - (int)queue->tail;
      if (diff < 0)
            diff += (int)queue->len;

      if (diff >= (int)queue->len)
            return queue->len;
      else  return (size_t)diff;
}

#ifdef TEST

main()
{
      int i;
      struct QQ *queue, *stack;

      QQopen(&queue, 10);

      printf("QQnum_enqueued() returned %d\n\n",
             QQnum_enqueued(queue));

      for (i = 0; i < 11; ++i)
      {
            int n = 5 * (i + 1);

            printf("QQenqueue(%d) returned %d\n", n,
                   QQenqueue(queue, n));
      }

      printf("\nQQnum_enqueued() returned %d\n\n",
             QQnum_enqueued(queue));

      for (i = 0; i < 7; ++i)
      {
            printf("QQpeekF() returned %d\n", QQpeekF(queue));
            printf("QQdequeue() returned %d\n",
                   QQdequeue(queue));
            printf("QQnum_enqueued() returned %d\n",
                   QQnum_enqueued(queue));
      }

      for (i = 0; i < 2; ++i)
      {
            int n = 3 * (i + 1);

            printf("QQenqueue(%d) returned %d\n", n,
                   QQenqueue(queue, n));
      }

      printf("\nQQnum_enqueued() returned %d\n\n",
             QQnum_enqueued(queue));

      for (i = 0; i < 7; ++i)
      {
            printf("QQpeekF() returned %d\n", QQpeekF(queue));
            printf("QQdequeue() returned %d\n",
                   QQdequeue(queue));
            printf("QQnum_enqueued() returned %d\n",
                   QQnum_enqueued(queue));
      }

      QQqflush(queue);

      printf("QQnum_enqueued() returned %d\n\n",
             QQnum_enqueued(queue));

      for (i = 0; i < 11; ++i)
      {
            int n = 5 * (i + 1);

            printf("QQpush(%d) returned %d\n", n,
                   QQpush(queue, n));
      }

      printf("\nQQnum_enqueued() returned %d\n\n",
             QQnum_enqueued(queue));

      for (i = 0; i < 7; ++i)
      {
            printf("QQpeekL() returned %d\n", QQpeekL(queue));
            printf("QQpop() returned %d\n",
                   QQpop(queue));
            printf("QQnum_enqueued() returned %d\n",
                   QQnum_enqueued(queue));
      }

      for (i = 0; i < 2; ++i)
      {
            int n = 3 * (i + 1);

            printf("QQpush(%d) returned %d\n", n,
                   QQpush(queue, n));
      }

      printf("\nQQnum_enqueued() returned %d\n\n",
             QQnum_enqueued(queue));

      for (i = 0; i < 7; ++i)
      {
            printf("QQpeekL() returned %d\n", QQpeekL(queue));
            printf("QQpop() returned %d\n",
                   QQpop(queue));
            printf("QQnum_enqueued() returned %d\n",
                   QQnum_enqueued(queue));
      }

      QQclose(&queue);

      return 0;
}

#endif /* TEST */
