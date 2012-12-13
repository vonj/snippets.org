/************************************************************************/
/*                                                                      */
/*  QQD.C - Implements a double-ended queuing mechanism for doubles.    */
/*                                                                      */
/*  Public domain by Bob Stout                                          */
/*                                                                      */
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "qqd.h"

const VAR64_ QQD_NaN = {-1L};           /* NaN bit pattern (all 1's)    */

/*
**  QQDopen() - Queue initialization function.
**
**  Parameters: 1 - Address of a queue of type struct QQ
**              2 - Number of entries
**
**  Returns: Success_ or Error_
**
**  Side effects: Allocated memory, zeros head and tail pointers,
**                sets length, clears full flag.
*/

Boolean_T QQDopen(struct QQD **queue, size_t len)
{
      if (NULL == (*queue = malloc(sizeof(struct QQD))))
            return Error_;
      if (NULL == ((*queue)->queue = calloc(len, sizeof(double))))
            return Error_;

      (*queue)->len  = len;
      (*queue)->head = (*queue)->tail = 0;
      (*queue)->full = False_;
      return Success_;
}

/*
**  QQDclose() - Close a queue.
**
**  Parameters: 1 - Address of a queue of type struct QQD
**
**  Returns: Nothing.
**
**  Side effects: Deallocates memory.
*/

void QQDclose(struct QQD **queue)
{
      FREE(*queue);
}

/*
**  QQDenqueue() - FIFO enqueuing function.
**
**  Parameters: 1 - Address of a queue of type struct QQD
**              2 - Double datum to enqueue
**
**  Returns: Success_ or Error_
**
**  Side effects: Adjusts head pointer.
*/

Boolean_T QQDenqueue(struct QQD *queue, double datum)
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
**  QQDdequeue() - FIFO dequeuing function
**
**  Parameters: 1 - Address of a queue of type struct QQD
**
**  Returns: Oldest datum in queue or QQD_EMPTY.
**
**  Side effects: Adjusts tail pointer
*/

double QQDdequeue(struct QQD *queue)
{
      double retval;

      if ((queue->head == queue->tail) && !queue->full)
            return QQD_EMPTY;

      retval = queue->queue[queue->tail++];
      if (queue->tail >= queue->len)
            queue->tail = 0;
      queue->full = False_;
      return retval;
}

/*
**  QQDpeekF() - FIFO peek function - CAUTION: returns the oldest data in the
**              queue without removing it from the queue.
**
**  Parameters: 1 - Address of a queue of type struct QQD
**
**  Returns: Oldest datum in queue or QQD_EMPTY.
**
**  Side effects: None.
*/

double QQDpeekF(struct QQD *queue)
{
      if ((queue->head == queue->tail) && !queue->full)
            return QQD_EMPTY;

      return queue->queue[queue->tail];
}

/*
**  QQDpush() - LIFO enqueuing function.
**
**  Parameters: 1 - Address of a queue of type struct QQD
**              2 - Double datum to enqueue
**
**  Returns: Success_ or Error_
**
**  Side effects: Adjusts head pointer.
**
**  Notes: Wrapper for QQDenqueue().
*/

Boolean_T QQDpush(struct QQD *queue, double datum)
{
      return QQDenqueue(queue, datum);
}

/*
**  QQDpop() - LIFO dequeuing function
**
**  Parameters: 1 - Address of a queue of type struct QQD
**
**  Returns: Newest datum in queue or QQD_EMPTY.
**
**  Side effects: Adjusts head pointer
*/

double QQDpop(struct QQD *queue)
{
      double retval;

      if ((queue->head == queue->tail) && !queue->full)
            return QQD_EMPTY;

      queue->head -= 1;
      if (queue->head < 0 || queue->head > queue->len)
            queue->head = queue->len - 1;
      retval = queue->queue[queue->head];
      queue->full = False_;
      return retval;
}

/*
**  QQDpeekL() - LIFO peek function - CAUTION: returns the newest data in the
**              queue without removing it from the queue.
**
**  Parameters: 1 - Address of a queue of type struct QQD
**
**  Returns: Newest datum in queue or QQD_EMPTY.
**
**  Side effects: None.
*/

double QQDpeekL(struct QQD *queue)
{
      size_t tmp_head;

      if ((queue->head == queue->tail) && !queue->full)
            return QQD_EMPTY;

      tmp_head = queue->head - 1;
      if (tmp_head < 0 || tmp_head > queue->len)
            tmp_head = queue->len - 1;

      return queue->queue[tmp_head];
}

/*
**  QQDisfull() - Function to determine is a queue is full.
**
**  Parameters: 1 - Address of a queue of type struct QQD
**
**  Returns: True_ if queue is full, False_ if not full.
*/

Boolean_T QQDisfull(struct QQD *queue)
{
      return queue->full;
}

/*
**  QQDisempty() - Function to determine is a queue is empty.
**
**  Parameters: 1 -Address of a queue of type struct QQD
**
**  Returns: True_ if queue is empty, False_ if not empty.
*/

int QQDisempty(struct QQD *queue)
{
      return ((queue->head == queue->tail) && !queue->full);
}

/*
**  QQDflush() - FIFO queue flushing function.
**
**  Parameters: 1 - Address of a queue of type struct QQD
**
**  Returns: N.A.
**
**  Side effects: Adjusts head and tail pointers
*/

void QQDqflush(struct QQD *queue)
{
      queue->head = queue->tail = 0;
      queue->full = False_;
}

/*
**  QQDnum_enqueued() - FIFO queue function to return the number of queue
**                     entries.
**
**  Parameters: 1 - Address of a queue of type struct QQD
**
**  Returns: Number of data in the queue
**
**  Side effects: Adjusts head and tail pointers
*/

size_t QQDnum_enqueued(struct QQD *queue)
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

/*
**  QQDdump() - Utility function to dump (print) the contents of a queue
**
**  Parameters: 1 - Address of a queue of type struct QQD
**
**  Returns: N.A.
**
**  Side effects: None
*/

void QQDdump(struct QQD *queue)
{
      size_t i, tailtmp, qlen;

      if (QQDisempty(queue))
      {
            puts("QQDdump(): Queue is empty\n");
            return;
      }
      else    puts("QQDdump(): Dumping queue, tail first");

      for (i = 0, qlen = queue->len, tailtmp = queue->tail; i < QQDnum_enqueued(queue); ++i)
      {
            printf("%2d: %f\n", i, queue->queue[tailtmp++]);
            if (tailtmp >= qlen)
                  tailtmp = 0;
      }
}

#ifdef TEST

main()
{
      int i;
      struct QQD *queue, *stack;

      QQDopen(&queue, 10);

      printf("QQDnum_enqueued() returned %d\n\n",
             QQDnum_enqueued(queue));

      for (i = 0; i < 11; ++i)
      {
            int n = 5 * (i + 1);

            printf("QQDenqueue(%d) returned %d\n", n,
                   QQDenqueue(queue, (double)n));
      }

      printf("\nQQDnum_enqueued() returned %d\n\n",
             QQDnum_enqueued(queue));

      for (i = 0; i < 7; ++i)
      {
            printf("QQDpeekF() returned %f\n", QQDpeekF(queue));
            printf("QQDdequeue() returned %f\n",
                   QQDdequeue(queue));
            printf("QQDnum_enqueued() returned %d\n",
                   QQDnum_enqueued(queue));
      }

      for (i = 0; i < 2; ++i)
      {
            int n = 3 * (i + 1);

            printf("QQDenqueue(%d) returned %d\n", n,
                   QQDenqueue(queue, (double)n));
      }

      printf("\nQQDnum_enqueued() returned %d\n\n",
             QQDnum_enqueued(queue));

      for (i = 0; i < 7; ++i)
      {
            printf("QQDpeekF() returned %f\n", QQDpeekF(queue));
            printf("QQDdequeue() returned %f\n",
                   QQDdequeue(queue));
            printf("QQDnum_enqueued() returned %d\n",
                   QQDnum_enqueued(queue));
      }

      QQDqflush(queue);

      printf("QQDnum_enqueued() returned %d\n\n",
             QQDnum_enqueued(queue));

      for (i = 0; i < 11; ++i)
      {
            int n = 5 * (i + 1);

            printf("QQDpush(%d) returned %d\n", n,
                   QQDpush(queue, (double)n));
      }

      printf("\nQQDnum_enqueued() returned %d\n\n",
             QQDnum_enqueued(queue));

      for (i = 0; i < 7; ++i)
      {
            printf("QQDpeekL() returned %f\n", QQDpeekL(queue));
            printf("QQDpop() returned %f\n",
                   QQDpop(queue));
            printf("QQDnum_enqueued() returned %d\n",
                   QQDnum_enqueued(queue));
      }

      for (i = 0; i < 2; ++i)
      {
            int n = 3 * (i + 1);

            printf("QQDpush(%d) returned %d\n", n,
                   QQDpush(queue, (double)n));
      }

      printf("\nQQDnum_enqueued() returned %d\n\n",
             QQDnum_enqueued(queue));

      for (i = 0; i < 7; ++i)
      {
            printf("QQDpeekL() returned %f\n", QQDpeekL(queue));
            printf("QQDpop() returned %f\n",
                   QQDpop(queue));
            printf("QQDnum_enqueued() returned %d\n",
                   QQDnum_enqueued(queue));
      }

      QQDclose(&queue);

      return 0;
}

#endif /* TEST */
