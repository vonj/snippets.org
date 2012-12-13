/* =======================================================================
    FSM.h           Finite State machines.
                    Version 0.01x, 93-08-04.

 _____              This version is Public Domain.
 /_|__|             A.Reitsma, Delft, Nederland.
/  | \  --------------------------------------------------------------- */

struct FSMstate_entry
{
    int state ;
    int cond  ;
    int next  ;
    int (*action)(int);
};

#define STATE_LIST_END  { -1, -1, 0, NULL }
#define LIST_DEFAULT   -1

int FSMsetup( struct FSMstate_entry * StateTable );
int FSMaction( int Cond );

/* === FSM.h ========================================================== */
