/*
** public domain demo of cooperative multitasking using function pointers
** written 29 Nov, 1992
** by David Gersic
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define MAX_TASKS 32                /* maximum number of runnable tasks */

/* global variables */

void (*tasks[MAX_TASKS])(void);     /* pointer to list of tasks to run  */

/* prototypes */

int  schedule(void (*fp)(void));    /* add function to the task list    */
int  mark(void (*fp)(void));        /* mark task in list                */
int  kill(void (*fp)(void));        /* remove function from task list   */
void dispatcher(void);              /* dispatch a task                  */
int  set_up(void);                  /* initialization code              */
int  terminate(void);               /* shutdown code                    */

int main(void)
{
      void walk_the_dog(void);
      void floss_the_cat(void);
      void make_the_donuts(void);

      if(set_up())
      {
            fputs("Initialization failed. Exiting.\n",stderr);
            exit(1);
      }

      if(schedule(walk_the_dog))
            fputs("Could not add new function to task list.\n",stderr);
      if(schedule(floss_the_cat))
            fputs("Could not add new function to task list.\n",stderr);
      if(schedule(make_the_donuts))
            fputs("Could not add new function to task list.\n",stderr);

      while(!kbhit())  /* run 'till key hit */
            dispatcher();

      if(terminate())
      {
            fputs("Error while shutting down.\n",stderr);
            exit(1);
      }
      return(0);
}

/*
** initialize task buffer and any other 'internal' setup code
*/

int set_up(void)
{
      memset(tasks, 0x00, sizeof(tasks));
      return(0);
}

/*
** kill any tasks still in the table and shut down
*/

int terminate(void)
{
      int i;

      for(i = 0; i < MAX_TASKS; i++)
      {
            if (tasks[i])
                  kill(tasks[i]);
      }
      return(0);
}

/*
** dispatch the next runnable task
*/

void dispatcher(void)
{
      static int i = 0;

      if(tasks[i])
            tasks[i]();
      i = (i + 1) % MAX_TASKS;
}

/*
** add task to the list of runnable tasks
*/

int schedule(void (*fp)(void))
{
      int i;

      for (i = 0; i < MAX_TASKS && tasks[i]; i++)
            ;                       /* find a open slot in the table    */
      if (i < MAX_TASKS)            /* if not end of table              */
            tasks[i] = fp;          /* add task to list                 */
      else  return(1);
      return(0);
}

/*
** remove a task from the list of runnable tasks
*/

int kill(void (*fp)(void))
{
      int i;

      for (i = 0; i < MAX_TASKS; i++)
      {
            if (tasks[i] == fp)
            {
                  tasks[i] = NULL;
                  return(0);
            }
      }
      return(1);
}

void walk_the_dog(void)
{
      puts("dog");
      return;
}

void floss_the_cat(void)
{
      puts("cat");
      return;
}

void make_the_donuts(void)
{
      puts("donuts");
      return;
}
