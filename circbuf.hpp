//
//  CIRCBUF.HPP - Header file for circular buffer C++ functions
//
//  by Bob Stout, originally published in C/C++ Users Journal
//  Use freely
//

#ifndef _CIRCBUF_DEFINED_
#define _CIRCBUF_DEFINED_

#include <stdlib.h>
#include <string.h>

typedef enum {ASSIGNED = -1, ERROR = -1, SUCCESS, FALSE = 0, TRUE} Boolean_T;

template<class T> class cbuf_t
{
private:
      size_t      next;       // Wrap-around pointer to next datum
      size_t      current;    // Wrap-around pointer to current datum
      Boolean_T   full;       // TRUE when buffer has filled
      Boolean_T   ready;      // TRUE after init() call...
                              //  ASSIGNED  after import() call...
                              //   else FALSE

protected:
      size_t      size;       // Number of elements
      T         * buf;        // Pointer to circular buffer

public:
      cbuf_t()           { buf = NULL; next = current = 0;
                           full = ready = FALSE; };
      cbuf_t(size_t len) { init(len); };
      ~cbuf_t()          { if (TRUE == ready) delete buf; };

      void        init(size_t);
      Boolean_T   add(T);
      Boolean_T   fetch(T *);
      Boolean_T   data_ready() { return full; };
      Boolean_T   data_avail() { return (next != current); };
      void        data_used()  { current = next; };
      Boolean_T   clear();
      Boolean_T   export(T **, size_t *);
      void        import(T *, size_t);
};

/************************************************************************/
/*                                                                      */
/*  init() Circular buffer initializer.                                 */
/*                                                                      */
/*  Arguments: 1 - Number of elements in the circular buffer.           */
/*                                                                      */
/************************************************************************/

template<class T> void cbuf_t<T>::init(size_t len)
{
      if (TRUE == ready)
            delete buf;
      else  ready = TRUE;
      size = len;
      buf  = new T[size];
      clear();
}

/************************************************************************/
/*                                                                      */
/*  clear() - Clear a circular buffer.                                  */
/*                                                                      */
/*  Returns: ERROR if buffer has not been initialized, else SUCCESS.    */
/*                                                                      */
/************************************************************************/

template<class T> Boolean_T cbuf_t<T>::clear()
{
      if (!ready)
            return ERROR;
      memset(buf, 0, size * sizeof(T));
      next = 0;
      full = FALSE;
      return SUCCESS;
}

/************************************************************************/
/*                                                                      */
/*  add() - Function to add data to a circular buffer.                  */
/*                                                                      */
/*  Arguments: 1 - Data to add.                                         */
/*                                                                      */
/*  Returns: TRUE if buffer has been filled,                            */
/*           FALSE if buffer has not filled,                            */
/*           ERROR if buffer has not been initialized.                  */
/*                                                                      */
/************************************************************************/

template<class T> Boolean_T cbuf_t<T>::add(T data)
{
      if (!ready)
            return ERROR;
      buf[next] = data;
      if (size <= ++next)
      {
            next = 0;
            full = TRUE;
      }
      if (next == current)
            ++current;
      return full;
}

/************************************************************************/
/*                                                                      */
/*  fetch() - Function to retrieve data from a circular buffer.         */
/*                                                                      */
/*  Arguments: 1 - Pointer to data storage.                             */
/*                                                                      */
/*  Returns: SUCCESS if datum retrieved, else ERROR.                    */
/*                                                                      */
/************************************************************************/

template<class T> Boolean_T cbuf_t<T>::fetch(T *data)
{
      if (!ready || next == current)
            return ERROR;
      *data = buf[current];
      if (size <= ++current)
            current = 0;
      return SUCCESS;
}

/************************************************************************/
/*                                                                      */
/*  export() - Publish a circular buffers location and length.          */
/*                                                                      */
/*  Arguments: 1 - Storage for buffer pointer.                          */
/*             2 - Storage for size of buffer.                          */
/*                                                                      */
/*  Returns: ERROR if buffer has not been initialized, else SUCCESS.    */
/*                                                                      */
/************************************************************************/

template<class T> Boolean_T cbuf_t<T>::export(T **buffer, size_t *len)
{
      if (!ready)
            return ERROR;
      *buffer = buf;
      *len    = size;
      return SUCCESS;
}

/************************************************************************/
/*                                                                      */
/*  import() - Assign an existing circular buffer to a cbuf_t object.   */
/*                                                                      */
/*  Arguments: 1 - Buffer to assign.                                    */
/*             2 - Size of buffer.                                      */
/*                                                                      */
/************************************************************************/

template<class T> void cbuf_t<T>::import(T *buffer, size_t len)
{
      if (TRUE == ready)
            delete buf;
      buf   = buffer;
      size  = len;
      ready = ASSIGNED;
}

#endif // _CIRCBUF_DEFINED_
