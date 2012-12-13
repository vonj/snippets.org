/////////////////////////////////////////////////////////////
// MODULE
//  list.cpp
// CREATED
//  davidn  03 Dec 1994  23:59
//  David L. Nugent
//  This class implementation is donated to the public domain
// DESCRIPTION
//  Implementation of class node, list & iter
// FUNCTIONS
//  node::unlink()
//    Destroys linkage from a list and removes it from a
//    linked list
//  node::link()
//    Links a node into a linked list
//  list::purge()
//    Removes all linked list entries
//  iter::traverse()
//    Provides full traversal functions for nodes in a
//    linked list
/////////////////////////////////////////////////////////////

// Implementation of class list & friends

#include "list.hpp"

void
node::unlink()
{
  if ( mylist )
  {
      // Unlink from previous
    if ( Prev )
      Prev->Next = Next;
    else
      mylist->First = Next;

      // Unlink from next
    if ( Next )
      Next->Prev = Prev;
    else
      mylist->Last = Prev;

    mylist->nodes--;
    mylist = 0;
    Prev = Next = 0;
  }
}

void
node::link( list * L, node * prv, node * nxt )
{

    // If currently linked, then unlink it

  if ( mylist )
    unlink();

    // Link it to the list

  if ( L )
  {

    mylist = L;

    // Add after previous

    if ( prv )
    {

      Prev = prv;
      Next = prv->Next;
    }

    // Add before next

    else if ( nxt )
    {

      Next = nxt;
      Prev = nxt->Prev;
    }

    // Else just add to end

    else
    {

      Next = 0;
      Prev = L->Last;
    }

    if ( Prev )
      Prev->Next = this;
    else
      L->First = this;

    if ( Next )
      Next->Prev = this;
    else
      L->Last = this;

    mylist->nodes++;
  }
}


void
list::purge( void )
{
  while ( First )
    delete First;
}


int
iter::traverse( trOp op )
{
  if ( mylist.firstNode() == 0 )
    return TR_EMPTY;

  switch ( op )
  {

  case NEXT:
    if ( nptr )
    {

      nptr = nptr->Next;
      break;
    }

  case FIRST:
    nptr = mylist.firstNode();
    break;


  case PREV:
    if ( nptr )
    {
      nptr = nptr->Prev;
      break;
    }

  case LAST:
    nptr = mylist.lastNode();
    break;

  case CURR:
    break;

  }

  return ( nptr ) ? TR_OK : TR_NOMORE;
}
