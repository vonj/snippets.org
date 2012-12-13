/*
**  INDEX.C
**
**  This is the indexing function for creating a binary file from an ASCII
**  file formatted as follows:
**
**  Mark Corgan
**  550 Foothill Rd.
**  Gardnerville, NV 89410
**  (702) 265-2388
**  .
**  Hello World
**  123 Anywhere St.
**  Anytown, CA 12345
**  (123) 456-7890
**  .
**  etc...
**  
**  The period is what the companion LOOKUP.C looks for to indicate the end
**  of record. Of course, you could have any format you like, so long as the
**  first line is the information you are looking for. Also, there is no
**  limit to the number of lines of infomation after the first line and
**  before the period as fputs() continues until the period. Enjoy!
**
**  by Mark Corgan, 09-May-1993, and donated to the public domain
*/

#include <stdio.h>
#include <stdlib.h>
#include "errors.h"                   /* for cant()                       */
#include "indxlook.h"                 /* definitions for INDEX and LOOKUP */

struct tree_node                      /* node in tree */
{
   struct tree_node *l_ptr, *r_ptr;   /* left and right pointers */
   INDEX *data_ptr;                   /* data pointer */
};

typedef struct tree_node TREE_NODE;

void        write_index(char *txtfile, char *ndxfile);
void        save_tree(TREE_NODE * root, FILE *fp);
TREE_NODE  *make_tree(FILE *fp, long *cnt_ptr);
TREE_NODE  *insert_tree(TREE_NODE *root, INDEX *rec_ptr, long *cnt_ptr);
long        bsearch_(FILE *ifp, long first, long last, char *target);

int main(int argc, char *argv[])
{
      if (argc != 3)
      {
            puts("Usage: INDEX text_file_name index_file_name\n");
            puts("Note: The text file must consist of a number of records "
                 "separated by lines");
            puts("      containing a single period (\".\")");
            return EXIT_FAILURE;
      }
      write_index(argv[1], argv[2]);
      return EXIT_SUCCESS;
}

void write_index(char *txtfile, char *ndxfile)
{
      FILE *afp, *ifp;                    /* types of files       */
      TREE_NODE *root;                    /* index tree           */
      static INDEX header = {"!", 0};     /* dummy header node    */

      afp = cant(txtfile, "r");
      if ((root = make_tree(afp, &header.pos)) != NULL)
      {
            ifp = cant(ndxfile, "wb");
            fwrite((char *) &header, sizeof(header), 1, ifp);
            save_tree(root, ifp);
            fclose(ifp);
            printf("\n%ld records\n", header.pos);
      }
      fclose(afp);
}

/*
** Make index tree
*/

TREE_NODE *make_tree(FILE *fp,            /* file                 */
                     long *cnt_ptr)       /* count of records     */
{
      TREE_NODE *root = NULL, *temp_ptr;  /* add node to tree     */
      char line[MAX_LINE];                /* next line            */
      long start_pos = 0;
      INDEX *next_ptr;                    /* next key, pos pair   */
      /* starting with new record */
      Boolean_T new_record = True_, have_mem = True_;

      *cnt_ptr = 0;
      while (start_pos = ftell(fp), have_mem && fgets(line,sizeof(line), fp))
      {
            if (new_record)
            {
                  if ((next_ptr = (INDEX *) malloc(sizeof(INDEX))) != NULL)
                  {
                        strncpy(next_ptr->key, line, MAX_KEY);

                        next_ptr->pos = start_pos;
                        temp_ptr      = insert_tree(root, next_ptr, cnt_ptr);

                        if (temp_ptr)
                              root = temp_ptr;
                  }
                  have_mem = next_ptr && temp_ptr;
            }
            new_record = strcmp(line, END_REC) == 0;
      }
      if (!have_mem)
            fprintf(stderr, "Out of memory. Key: %s\n", line);

      return root;
}

/*
** Save the index tree to a file
*/

void save_tree(TREE_NODE *root, FILE *fp)
{
      if (root)
      {
            save_tree(root->l_ptr, fp);
            fwrite(root->data_ptr, sizeof(INDEX), 1, fp);
            save_tree(root->r_ptr, fp);
      }
}

/*
** Add record to tree
*/

TREE_NODE *insert_tree(TREE_NODE *root,         /* pointer to tree      */
                       INDEX *rec_ptr,          /* record to install    */
                       long *cnt_ptr)           /* nodes in tree        */

{
      if (root)
      {
            int cmp = strncmp(rec_ptr->key, root->data_ptr->key, MAX_KEY);

            if (cmp < 0)                        /* left side?           */
                  root->l_ptr = insert_tree(root->l_ptr, rec_ptr, cnt_ptr);
            else if (cmp > 0)                   /* right side           */
                  root->r_ptr = insert_tree(root->r_ptr, rec_ptr, cnt_ptr);
            else  fprintf(stderr, "Duplicate key: %s\n", rec_ptr->key);
      }
      else if (root = (TREE_NODE *) malloc(sizeof(TREE_NODE)), root)
      {
            root->data_ptr = rec_ptr;
            root->l_ptr = root->r_ptr = NULL;
            (*cnt_ptr)++;
      }
      return root;
}

long bsearch_(FILE *ifp, long first, long last, char *target)
{
      long pos, mid =(first + last) / 2;
      INDEX next;
      int cmp;

      if (mid < first || fseek(ifp, mid * sizeof(INDEX), 0) != 0 ||
          fread((char *) &next, sizeof(INDEX), 1, ifp) == 0)
      {
            pos = -1;
      }
      else  pos = ((cmp = strncmp(target, next.key, MAX_KEY)) == 0) ?
                  next.pos :
                  ((cmp < 0) ? bsearch_(ifp, first, mid - 1, target)
                             : bsearch_(ifp, mid + 1, last, target));
      return pos;
}
