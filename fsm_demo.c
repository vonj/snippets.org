/*
**  code snippet demonstrating a finite state machine (FSM)
*/

typedef enum {s0,s1,s2,s3,s4,...,sn,sexit} state;

state nextstate;
int done = 0;

nextstate = s0;  /* set up to start with the first state */
while(!done)
   switch(nextstate)
      {
         case s0:
            nextstate = do_state_0();
            break;
         case s1:
            nextstate = do_state_1();
            break;
         case s2:
            nextstate = do_state_2();
            break;
         case s3:
             .
             .
             .
             .
         case sn:
            nextstate = do_state_n();
            break;
         case sexit:
            done = TRUE;
            break;
         default:
            /*  some sort of unknown state */
            break;
      }
