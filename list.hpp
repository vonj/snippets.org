////////////////////////////////////////////////////////////////
// MODULE
//  list.hpp
// CREATED
//  davidn  03 Dec 1994  23:34
//  David L. Nugent
//  This class implementation is donated to the public domain
// DESCRIPTION
//  Classes supporting linked list containers
// CLASSES/TYPES
//  class node
//    Represents a single link in a doubly linked list
//  class list
//    Base class which handles all of the linked list management
//  class iter
//    Base class for handling iteration through a linked list
//  class Node<T>
//    Template class used for containment of an arbitrary type T
//  class List<T>
//    Linked list class which is used to get/store/remove nodes
//    from a linked list containing data
//  class Iter<T>
//    Used for iteration of a List<T>
// SYNOPSIS
//  These classes allow any arbitrary type to be contained in a
//  type-safe linked list. All of the common code for list
//  management itself is contained in a common set of classes:
//  node, list and iter. Template classes derived from these
//  allow inline access to the underlying base classes via a
//  type-safe front-end.
////////////////////////////////////////////////////////////////

#if !defined( _list_h )
#define _list_h

class list;

    // Generic 'node' class

class node
{
    friend class iter;
  public:
    node( list * L =0, node * prv =0, node * nxt =0 )
      : mylist( 0 ), Prev( 0 ), Next( 0 )
    { link( L, prv, nxt ); }
    virtual ~node( void )
    { unlink( ); }
    void link( list * L, node * prv, node * nxt );
    void unlink( );
    node * prevNode( void ) const
    { return Prev; }
    node * nextNode( void ) const
    { return Next; }
  private:
    list * mylist;
    node * Prev, * Next;
};

    // template node frontend

template<class T>
class Node : public node
{
  public:
    Node( T data, list * L =0, node * prv =0, node * nxt =0 )
      : node( L, prv, nxt ), Data( data ) {}
    Node<T> * next( void ) const
    { return (Node<T> *)nextNode(); }
    Node<T> * prev( void ) const
    { return (Node<T> *)prevNode(); }
    T & ref2data( void ) const
    { return ( T & )Data; }
    T * ptr2data( void ) const
    { return ( T * )&Data; }
    T data( void ) const
    { return Data; }
  private:
    T Data;
};

    // Generic 'list' class

class list
{
    friend class node;
  public:
    list( void )
      : First( 0 ), Last( 0 ), nodes( 0 ) {}
    virtual ~list( void )
    { purge(); }
    void purge( void );
    long items( void ) const
    { return nodes; }
    void addatstart( node * n )
    { n->link( this, 0, First ); }
    void addatend( node * n )
    { n->link( this, Last, 0 ); }
    void addafter( node * n, node * prv )
    { n->link( this, prv, 0 ); }
    void addbefore( node * n, node * nxt )
    { n->link( this, 0, nxt ); }
    node * firstNode( void ) const
    { return First; }
    node * lastNode( void ) const
    { return Last; }
  protected:
    node * First, * Last;
    long nodes;
};

    // Container class List<T>

template<class T>
class List : public list
{
  public:
    List( void ) : list() {}
    Node<T> * add( T data, Node<T> * prv =0, Node<T> * nxt =0 )
    { return new Node<T>( data, this, prv, nxt ); }
    Node<T> * first( void ) const
    { return (Node<T> *)First; }
    Node<T> * last( void ) const
    { return (Node<T> *)Last; }
};

enum trOp
{
  FIRST, LAST, PREV, NEXT, CURR
};

#define TR_OK     0
#define TR_EMPTY  -2
#define TR_NOMORE -3

class iter
{
  public:
    iter( list & L )
      : mylist( L ), nptr( 0 ) {}
    iter( iter const & I )
      : mylist( I.mylist ), nptr( I.nptr ) {}
    iter & operator=( iter const & I )
    { if ( &I.mylist == &mylist ) nptr = I.nptr; return *this; }
    void reset( void )
    { nptr = 0; }
    int traverse( trOp op );
    int current( void )
    { return traverse( CURR ); }
    int first( void )
    { return traverse( FIRST ); }
    int last ( void )
    { return traverse( LAST );  }
    int prev( void )
    { return traverse( PREV );  }
    int next( void )
    { return traverse( NEXT );  }
  protected:
    list & mylist;
    node * nptr;
};

    // Iterator

template<class T>
class Iter : public iter
{
  public:
    typedef int (*comparator)( const &T, const T&);

    Iter( List<T> & L )
      : iter( L ) {}
    Iter( Iter<T> const & I )
      : iter( I ) {}
    Iter<T> & operator=( Iter<T> const & I )
    { iter::operator=( I ); return *this;  }
    List<T> & myList( void ) const
    { return ( List<T> & )mylist; }
    Node<T> * atNode( void ) const
    { return ( Node<T> * )nptr; }
    T & ref2data( void ) const
    { return atNode()->ref2data(); }
    T * ptr2data( void ) const
    { return atNode()->ptr2data(); }
    T data( void ) const
    { return atNode()->data(); }
    void addFirst( T data )
    { myList().addatstart( new Node<T>( data ) ); }
    void addLast( T data )
    { myList().addatend( new Node<T>( data ) ); }
    void addAfter( T data )
    { myList().addafter( new Node<T>( data ), nptr ); }
    void addBefore( T data )
    { myList().addbefore( new Node<T>( data ), nptr ); }
    void add( T data, trOp op );
    trOp locate( T & data, comparator compare );
    int addsorted( T data, comparator compare, int adddupe =0 );
};

template<class T> void Iter<T>::add( T data, trOp op )
{
  switch( op )
  {
  case FIRST:           addFirst( data );    break;
  case LAST:            addLast( data );     break;
  case PREV:            addBefore( data );   break;
  case CURR: case NEXT: addAfter( data );    break;
  }
}

template<class T>
trOp
Iter<T>::locate( T & data, comparator compare )
{
  register trOp rc;
  register Node<T> * n = myList().first();

  if ( n == 0 )   // Add to start of empty list
    rc = FIRST;
  else
  {
    rc = LAST;
    while ( rc == LAST && n != 0 )
    {
      int r = compare( data, n->ref2data() );
      if ( r == 0 )       // Found an exact match
        rc = CURR;
      else if ( r < 0 )   // We've gone past it
        rc = PREV;
      else
        n = n->next();
    }
  }

  nptr = n;
  return rc;

}

template<class T>
int
Iter<T>::addsorted( T data, comparator compare, int adddupe )
{
  trOp r;

  if ((( r  = locate( data, compare )) != CURR ) || adddupe )
  {
    add( data, r );
    return 1;
  }
  return 0;
}

#endif    // _list_h
