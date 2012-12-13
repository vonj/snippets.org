/* - skiplist.c --------------------------------------------------- *
**
** Once upon a time Skiplist were invented by William Pugh at the
** University of Maryland (according to Bruce Schneier, DrDobbs,
** january 1994). They are an elegant alternative approach to balanced
** binary threes.
**
** Unfortunately the implementation shown in that same article did
** not yield the expected results, so I decided to "roll my own"
** rather than looking for errors in somebody elses code.
**
** The result (surprise?) looked remarkably like the original version,
** except that it worked with my compiler, that is, untill I turned on
** the optimizer! (Lattice 5.06.02)
**
** I then started to tweak the code, changing while to do/while or
** variable++; to ++variable; in order to fool the optimizer into
** doing what I intended to be done. (All very esoteric ;)
**
** I have since then tested with gcc (DecStation) which works, and
** also cc -O3 (DecStation) which now works (This last one one was
** a beast!!)
**
** If your compiler/optimizer still cannot digest this,
** then please let me know,
**
** else tweak this file to be useful in your own projects
** and (if you like?) send me a "postcard" :-)
**
** 17.08.1996 // Jens M Andreasen <jens-and@dsv.su.se>
**
** In a message of 20-Aug-1996, Jens M Andreasen wrote:
**
** My skiplist.c listing is hereby explicitly given the status of freeware.
** Distribute as is with the following additions ...
**
** ------------------------------------------------------------------------
**
** Note that the integers used for key and value are assumed to be 32 bit.
**
** Note also that the randomgenerator is a temporary hack. The original
** implementation has a better one. And do take a look at the alternative
** approches to DUPLICATEFOUND while you are at it:
*/

#include<limits.h>
#include<stdlib.h>
#include<stdio.h>

#define LEVELMAX 15
#define keytype int
#define valuetype int
#define KEYMAX INT_MAX

typedef struct structnode node;
typedef struct structnode{
      int key;
      int value;
      node *next[1];
} *pt_node;


/* ---------------------------------------------------------------- */

#define New(type) malloc(sizeof (type))
#define NewArray(type,x) malloc(sizeof(type) * (x))
#define NewNode(randomlevel)\
 malloc(sizeof(node) + (randomlevel) * sizeof(pt_node))

int insertnode(node **List, int key, int value);
#define DUPLICATEFOUND /* return -1 */

int findnode(node **List, int key);
#define INITVALUE 0

int deletenode(node **List, int key);

void findall(node **List);/* under construction */

node **newlist(void);

int main(void);


/* ---------------------------------------------------------------- */



int insertnode(node **List, int key, int value)
{
      int r,level = LEVELMAX;
      node *temp  = List[0], **bTrack[LEVELMAX+1];

      if (temp->key > key)
      {
            do
                  bTrack[level] = List;
            while (--level >= 0);
      }
      else
      {
            do
            {
                  while((temp = List[level]), (temp->key < key))
                  {
                        List = temp->next;
                  }
                  bTrack[level] = List;
            } while (--level >= 0);

            if (temp->key == key)
            {
                  DUPLICATEFOUND;
                  temp->value = value;
                  return 0;
            }
      }

      r = rand(); /* not so random, but works for now ... */
      level = 0;

      while (r & 0x4000)
      {
            r <<= 1;
            ++level;
      }

      temp = NewNode(level);
      if (!temp)
      {
            printf(" Out of memory at key: %d",key);
            exit(1);
      }

      temp->value = value;
      temp->key   = key;

      do{
            temp->next[level] = bTrack[level][level];
            bTrack[level][level] = temp;
      } while (--level >= 0);

      return 1;
 }

int findnode(node **List, int key)
{
      int  level = LEVELMAX;
      node *temp = List[0];

      if (temp->key > key)
            return INITVALUE;

      do
      {
            while(temp = List[level], (temp->key < key))
            {
                  List = temp->next;
            }
      } while (--level >= 0);

      if (temp->key == key)
      {
            return temp->value;
      }
      else  return INITVALUE;
 }



int deletenode(node **List, int key)
{
      int level  = LEVELMAX;
      node *temp = List[0], **bTrack[LEVELMAX + 1];

      if ((temp->key > key)||(KEYMAX == key))
            return 0;

      do
      {
            while(bTrack[level] = List, temp = List[level], (temp->key < key))
            {
                  List=temp->next;
            }

      } while (--level >= 0);

      if (temp->key == key)
      {
            level=0;
            do
            {
                  bTrack[level][level] = temp->next[level];
            } while (++level, bTrack[level][level]->key == key);

            free(temp);
            return -1;
      }
      return 0;
 }



void findall(node **List)                 /* just testing ... */
{
      while (List && List[0])
      {
            ++List[0]->value;
            --List[0]->value;

            List = List[0]->next;
      }
}

node **newlist(void)
{
      int l = LEVELMAX;
      node *NIL,

      **L = NewArray(pt_node, LEVELMAX + 1);

      NIL = NewNode(LEVELMAX);

      NIL->key   = KEYMAX;
      NIL->value = INITVALUE;
      do
      {
            NIL->next[l] = NULL;
      } while (l--);

      l=LEVELMAX;
      do
      {
            L[l] = NIL;
      } while (l--);

      return L;
 }


/* ---------------------------------------------------------------- */


int main(void)
{
      int i;
      node **L;

      if (sizeof(int) < 4)
      {
            puts("This code must be compiled with 32-bit ints!");
            return EXIT_FAILURE;
      }

      L=newlist();

      puts(" DOWN");

      for(i = 100000; i >= 0; i -=2)
            insertnode(L, i, i);

      puts(" UP");
      for(i = 1; i < 100000; i += 2)
            insertnode(L, i, i);

      deletenode(L, 40);

      puts(" FIND");
      for(i = -2; i <= 100002; ++i)
            findnode(L, i);

      puts(" FAST");
      findall(L);

      puts("SAMPLES");
      printf(" %d", findnode(L, -10));

      printf(" %d,", findnode(L, 0));
      printf(" %d", findnode(L, 1));
      printf(" %d", findnode(L, 2));
      printf(" %d", findnode(L, 39));
      printf(" %d", findnode(L, 40));
      printf(" %d", findnode(L, 41));
      printf(" %d", findnode(L, 42));
      printf(" %d", findnode(L, 99999));
      printf(" %d", findnode(L, 100000));

      printf(", %d", findnode(L, 100001));
      printf(" %d", findnode(L, 100008));

      puts(" DONE");

      return EXIT_SUCCESS;
}
