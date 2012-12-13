/*****************************************************************************
   This code is based upon the program SETPATH.PAS (located in BPROGA) by
   David Dubois [71401,747]

   This Turbo C version is written by Peter Thomas [75716,2377]

   This series of routines are designed to Locate, Retrieve, Update, and
   Remove "Variables" from the MASTER copy of the DOS Environment table.
   The routines have been written in a manner that avoids linking any
   EXTERNAL routines for string manipulation, and thus should be independent
   of memory model being used.

   Be careful that changes made to the Environment with these routines
   ONLY OCCUR IN THE MASTER COPY, and that if COMMAND.COM is spawned
   from a routine that has changed the environment, NO CHANGES WILL BE
   SEEN IN THE ENVIRONMENT. This is most apparent when this program is run
   in the INTEGRATED environment: changes made by this technique will
   not appear if the "OS Shell" is invoked, and will only appear on exit
   from TC.

   For full documentation on the techniques used here can be found in the
   file COMENV.ARC located in LIB 2 of BPROGA on Compuserve.

   As David Dubois says:

  I hereby dedicate this knowledge to the public domain. Feel free to use
  it, but if you do, please mention my name. There are no guarantees, and
  in fact, I wouldn't bet a dollar that it will work every time.

  That this works at all is based on experimental, rather than properly
  documented, evidence. There are no guarantees. But then, its free.

*****************************************************************************/

#include <stdio.h>
#include <dos.h>
#include "extkword.h"
#include "mk_fp.h"

#if defined(__ZTC__) && (!defined(__SC__) || __SC__ < 0x700)
 #error ZTC, SC 6.x not supported - huge pointers required!
#endif

#ifdef __TURBOC__
 #include <alloc.h>
 #define Fmalloc farmalloc
 #define Ffree farfree
#else
 #include <malloc.h>
 #include <stdlib.h>
 #define Fmalloc _fmalloc
 #define Ffree _ffree
 #ifndef MK_FP
  #define MK_FP(seg,offset) \
        ((void far *)(((unsigned long)(seg)<<16) | (unsigned)(offset)))
 #endif
#endif

/*
 *   Mstr_FindEnvironment:
 *     Scans for the "Master" Environment area, and returns
 *     a pointer to it, and the size of the environment.
*/
void Mstr_FindEnvironment ( char FAR **Env , unsigned *EnvSize )
{
   unsigned int FAR *CommandSeg, FAR *TempSeg ;
   char FAR *BlockSeg ;

   /*
    *  Scan through PSP's looking for a block that is its own father.
    *  This block is the PSP of COMMAND.COM
   */
  TempSeg = MK_FP ( _psp , 0 ) ;
   do
   {
     CommandSeg = TempSeg ;
     TempSeg = MK_FP ( *(TempSeg+8) , 0 ) ;
   }
   while ( TempSeg != CommandSeg ) ;

   /*
    *  Scan forward through memory looking for the correct MSB.
    *  This will have COMMAND.COM's PSP as owner, and begin with
    *  the character M
   */
   BlockSeg = (char FAR *)CommandSeg ;
   do
   {
    BlockSeg = MK_FP ( FP_SEG(BlockSeg)+1 , 0 ) ;
   }
   while ( ( *(unsigned int FAR *)(BlockSeg+1) != FP_SEG ( CommandSeg ) ) ||
           ( *BlockSeg != 'M' ) ) ;

   /*
    *  The environment is the NEXT segment of memory
    *  and bytes 4 and 5 are the size in paragraphs
   */
   *Env = MK_FP ( FP_SEG(BlockSeg)+1 , 0 ) ;
   *EnvSize = 16 * *(unsigned int FAR *)(BlockSeg+3) ;
}

/*
 *   Mstr_getenv:
 *     Scans the "Master" Environment for a given "sub string"
 *     and returns a pointer to it.
 *     Similar to Turbo routine "getenv" but uses the Master copy of the
 *     environment table.
*/
char FAR *Mstr_getenv (char FAR *Env , char FAR *name)
{
   char FAR *Sub_Env, FAR *str1, FAR *str2 ;

   /*
    *  Start at the beginning of the environment
   */
   Sub_Env = Env ;

   /*
    *  While the "sub string" we're looking at is non-zero
   */
   for ( ; *Sub_Env ; )
   {
     /*
      *  Simulate a "strcmp" on the "sub string" of the environment
      *  and the string we're looking for
     */
     for ( str1 = Sub_Env , str2 = name ;
           (*str1) && (*str2) && ( *str1 == *str2) ;
           str1++ , str2++ ) ;
     /*
      *  If we reached the end of the string we're looking for
      *  we've found the correct portion of the environment.
      *  Return the ptr to the start of this "sub string"
     */
     if ( !*str2 )
       return ( Sub_Env ) ;

     /*
      *  Otherwise, advance to the next "sub string" in the environment
      *  by performing a "strchr" function
     */
     for ( ; *(Sub_Env++) ; ) ;
   }

   /*
    *  Obviously, the string is not present in the environment.
    *  Return this fact.
   */
  return ( NULL ) ;
}

/*
 *   Mstr_delenv:
 *     Scans the "Master" Environment for a given "sub string"
 *     and removes it.
*/
int Mstr_delenv (char FAR *Env , unsigned EnvSize , char FAR *name)
{
   char FAR *Sub_Env , FAR *New_Env ;
   char HUGE *Dest , FAR *Src , HUGE *End_Env ;

   int Done  ;
   unsigned Ctr ;

   /*
    *  Allocate a chunk of storage to act as a "working" copy of
    *  the Environment table
   */
   New_Env = Fmalloc ( EnvSize ) ;

   /*
    *  Copy the data from the Master to Working copy of the
    *  Environment table.
    *  Simulates a "memcpy" function.
   */
   for ( Src = Env , Dest = (char FAR *)New_Env , Ctr = 0 ;
         Ctr < EnvSize ;
         *(Dest++) = *(Src++) , Ctr++ ) ;

   /*
    *  Scan the working copy of the environment for the desired
    *  sub string
   */
   Sub_Env = Mstr_getenv ( New_Env , name ) ;

   if ( Sub_Env == NULL )
   {
     /*
      *  If not found, do nothing
     */
     Done = -1 ;
   } else {
     /*
      *  Locate the end of the string to delete
      *  Simulate a "strchr" call
     */
     for ( Src = Sub_Env ; *(Src++) ; ) ;

     /*
      *  Move the rest of the environment back over the "sub string"
      *  being deleted.
      *  Simulated "memcpy" function.
      *  Huge pointers used for pointer comparison purposes.
     */
     for ( Dest = (char HUGE *)Sub_Env , End_Env = (char HUGE *) (New_Env + EnvSize ) ;
         ( Dest < End_Env ) ;
         *(Dest++) = *(Src++) ) ;

     /*
      *  Copy the data from the Working to Master copy of the
      *  Environment table.
      *  Simulates a "memcpy" function.
     */
     for ( Src = New_Env , Dest = (char HUGE *)Env , Ctr = 0 ;
         Ctr < EnvSize ;
         *(Dest++) = *(Src++) , Ctr++ ) ;

     /*
      *  Signal all done
     */
     Done = 0 ;
   }

   /*
    *  Free all working storage
   */
   Ffree ( New_Env ) ;

   return ( Done ) ;
}

/*
 *   Mstr_putenv:
 *     Adds/Replaces a given "sub string" in the Master Environment.
 *     Similar to Turbo routine "putenv" but uses the Master copy of the
 *     environment table.
*/
int Mstr_putenv (char FAR *Env , unsigned EnvSize , char FAR *name )
{
   char FAR *Sub_Env , FAR *Temp_Name ;
   char HUGE *Dest , FAR *Src , HUGE *End_Env ;
   int Done ;

   /*
    *  Allocate a chunk of storage to create the Variable name to add
    *  to the Environment table
   */
   Temp_Name = Fmalloc ( 256 ) ;

   /*
    *  Extract only the Name portion of the data to add to the Environment
   */
   for ( Src = name , Dest = Temp_Name ;
         *Src && ( *Src != '=' ) ;
        *(Dest++) = *(Src++) ) ;

   /*
    *  Ensure that the resulting name is well formed.
   */
   *(Dest++) = '=' ;
   *Dest = 0 ;

   /*
    *  Delete this sub string if found in the environment
   */
   Mstr_delenv ( Env , EnvSize , Temp_Name ) ;

   /*
    *  Locate the END of the Master table by locating a zero length
    *  String in it
   */
   Sub_Env = Env ;
   for ( ; *Sub_Env ; )
   {
     for ( ; *(Sub_Env++) ; ) ;
   }

   /*
    *  Add the new string to the END of the existing environment, with
    *  truncation IF needed
   */
   for ( Dest = (char HUGE *)(Sub_Env) , Src = name , End_Env = (char HUGE *) (Env + EnvSize ) ;
         ( Dest < End_Env ) && (*Src) ;
         *(Dest++) = *(Src++) ) ;

   Done = -1 ;
   if ( !*Src )
   {
     /*
      *  If the string to add was FULLY added, ensure that the
      *  newly updated environment is properly finished
     */
     Done = 0 ;
     *(Dest++) = 0 ;
     *Dest = 0 ;
   }

   /*
    *  As a real safety measure, ensure that the FINAL two bytes of the
    *  Environment are both 0. This will finish the last string AND
    *  ensure that a zero length string is also present
   */
   *(End_Env-1) = 0 ;
  *(End_Env-2) = 0 ;

   /*
    *  Free all working storage
   */
   Ffree ( Temp_Name ) ;

   return ( Done ) ;
}

main()
{
  char FAR *Env ;
   unsigned EnvSize ;

   /*
    *  Locate the Master Table
   */
   Mstr_FindEnvironment ( &Env, &EnvSize ) ;

   /*
    *  Describe what we've just found
   */
   printf ( "Env = %Fp Size = %u\n\n" , Env , EnvSize ) ;

   /*
    *  Search for, and display LOCATIONS of PATH, FRED and BERT
   */
   printf ( "Search for PATH= returns %Fp\n",  Mstr_getenv ( Env , "PATH=" ) ) ;
   printf ( "Search for FRED= returns %Fp\n",  Mstr_getenv ( Env , "FRED=" ) ) ;
   printf ( "Search for BERT= returns %Fp\n",  Mstr_getenv ( Env , "BERT=" ) ) ;

   /*
    *  Add FRED and BERT to the environment
   */
   Mstr_putenv ( Env , EnvSize , "FRED=fred" ) ;
   Mstr_putenv ( Env , EnvSize , "BERT=bert" ) ;

   printf ( "\nAdded FRED and BERT to environment\n" ) ;

   /*
    *  Search for, and display LOCATIONS of PATH ,FRED and BERT
   */
   printf ( "Search for PATH= returns %Fp\n",  Mstr_getenv ( Env , "PATH=" ) ) ;
   printf ( "Search for FRED= returns %Fp\n",  Mstr_getenv ( Env , "FRED=" ) ) ;
   printf ( "Search for BERT= returns %Fp\n",  Mstr_getenv ( Env , "BERT=" ) ) ;

   /*
    *  Delete FRED from the environment
   */
   Mstr_delenv ( Env , EnvSize , "FRED=" ) ;

   printf ( "\nDeleted FRED= from the environment\n" ) ;

   /*
    *  Search for, and display LOCATIONS of PATH, FRED, and BERT
   */
   printf ( "Search for PATH= returns %Fp\n",  Mstr_getenv ( Env , "PATH=" ) ) ;
   printf ( "Search for FRED= returns %Fp\n",  Mstr_getenv ( Env , "FRED=" ) ) ;
   printf ( "Search for BERT= returns %Fp\n",  Mstr_getenv ( Env , "BERT=" ) ) ;

   printf ( "\nIssue the DOS command SET and see that BERT is now set\n" ) ;

   return 0;
}
