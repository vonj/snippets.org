/*
** fork.c (fork.h) - fork output to multiple file(s).
**
** By Dustin Puryear <dpuryear@delphi.com>
** Placed in the Public Domain.
*/

#include "fork.h"

int main(int argc, char *argv[])
{
      LL  files;     /* Linked-list of file pointers */
      int i;

      if ( argc < 2 )
      {
            help();
            exit(EXIT_FAILURE);
      }

      files.head = files.tail = NULL;

      /*
      ** First of all, find out if the user request help or if
      ** he disabled the console.
      */

      {
            int x;   /* Use as console toggle */

            for ( x = 0, i = 1; i < argc; i++ )
                  if ( argv[i][0] == '/' )
                        switch ( argv[i][1] )
                        {
                              case '?' :
                                    help();
                                    exit(EXIT_FAILURE);
                              case 'C' :
                              case 'c' :
                                    x = 1;
                                    break;
                        }

            /*
            ** x not toggled, therefore add stdout (console) to list.
            */

            if ( !x )
                  llopen(&files, stdout);
            else
            {
                  puts("Output to files only ...");
                  fflush(stdout);
            }

      }

      /*
      ** Go through command line again. Now look to open files. If
      ** you cannot open a file, ignore it!
      */

      {
            FILE  *fptr;               /* current file opened         */

            for ( i = 1; i < argc; i++ )
                  if ( argv[i][0] != '/' )
                  {
                        switch ( argv[i][0] )
                        {
                              case '*' :    /* Overwrite mode         */
                                    fptr = fopen(argv[i] + 1, "wb");
                                    break;
                              default  :    /* Append mode            */
                                    fptr = fopen(argv[i], "ab");
                                    break;
                        }
                        if ( fptr != NULL )
                              llopen(&files, fptr);
                  }

      }

      /*
      ** Get characters from stdin and place into a buffer. When buffer
      ** becomes full, write it out to all files in list.
      */

      {
            char  buffer[MAXBUFF]; /* buffer of chars      */
            int   i;               /* buffer index         */
            int   ch;              /* character from stdin */

            i = 0;
            while ( (ch = getchar()) != EOF )
            {
                  buffer[i++] = (char) ch;
                  if ( i == MAXBUFF )
                  {
                        output(&files, buffer, i - 1);
                        i = 0;
                  }
            }

            /*
            ** If characters read, but have not been written out,
            ** write them now.
            */

            if ( i != 0 && i < MAXBUFF )
                  output(&files, buffer, i - 1);
      }

      /*
      ** Close all files and remove list.
      */

      llfree(&files);

      return (EXIT_SUCCESS);
}

/*
** llopen()
**
** Place a new node at the end of the ll, containing the file pointer.
*/

void llopen(LL *files, FILE *fptr)
{
      FILENODE *pnode;

      pnode = (FILENODE *) malloc(sizeof(FILENODE));
      if ( pnode == NULL )
      {
            perror("FORK ");
            llfree(files);
            exit(EXIT_FAILURE);
      }
      pnode->ptr = fptr;
      pnode->next = NULL;

      /*
      ** If a ll does not exist, start a new one. Otherwise, continue
      ** with the old.
      */

      if ( files->head == NULL )
            files->head = files->tail = pnode;
      else
      {
            files->tail->next = pnode;
            files->tail = pnode;
      }
}

/*
** llfree()
**
** Free the memory consumed by the linked-list and closes any open files.
*/

void llfree(LL *files)
{
      FILENODE *del;

      while ( files->head != NULL )
      {
            fclose(files->head->ptr);
            del = files->head;
            files->head = files->head->next;
            free(del);
      }
}

/*
** output()
**
** Output buffer to the file within the structure.
*/

void output(LL *files, char *buffer, int size)
{
      int      i;
      FILENODE *current;

      /*
      ** If console is still active, output to it first.
      */

      current = files->head;
      if ( current->ptr == stdout )
      {
            for ( i = 0; i <= size; i++ )
                  putchar(buffer[i]);
            fflush(stdout);
            current = current->next;
      }

      while ( current != NULL )
      {
            fwrite(buffer, sizeof(char) * size, 1, current->ptr);
            current = current->next;
      }
}

/*
** help()
**
** Show a help screen to the user.
*/

void help(void)
{
      putchar('\n');
      printf("Fork - (pd) %s v%s Dustin Puryear\n", __DATE__, VER);
      puts("This program is in Public Domain.");
      putchar('\n');
      puts("Fork piped output to the console and/or file(s).");
      putchar('\n');
      puts("[command |] fork [/?][/C] [filename.ext *filename.ext ...]");
      putchar('\n');
      puts("command       - Command that you wish to fork output.");
      puts("/?            - Call this screen.");
      puts("/C            - Do not output to console.");
      puts("filename.ext  - Fork output to this file (append).");
      puts("*filename.ext - Fork output to this file (overwrite).");
      putchar('\n');
      puts("NOTE: FORK is case insensitive.");
      putchar('\n');

      fflush(stdout);
}
