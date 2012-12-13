/**** init_globals(fp, names, types, ...)
**
** public domain by Raymond Gardner     Sept. 1991
**
** fp is a FILE * to the (already fopen'ed) file containing
**      initialization data
**  names is a space-separated list of names of globals (as they
**      are to appear in the data file)
**  types is a list of datatype characters, corresponding to names
**      i  for a pointer to integer
**      s  for a pointer to string (already allocated char array)
**      p  for a pointer to pointer to char (space will be malloc'd)
**    (NOTE: no whitespace allowed in types !!)
**  followed by var arg list of pointers to variables to init
*/

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include "initvars.h"

int init_globals(FILE *fp, char *names, char *types, ...)
{
    char ln[LNSZ];
    char *p;
    va_list arglist;
    char *namep, *typep, name[40], *e;
    void *argp;
    int k;

    while ( fgets(ln, LNSZ, fp) ) {             /* read init file */
        while ( isspace(ln[0]) )            /* drop leading whitespace */
            memmove(ln, ln+1, strlen(ln));
        if ( ln[0] == 0 )                       /* skip if blank line */
            continue;
        p = strchr(ln, '=');                    /* find equal sign */
        if ( p == NULL )                        /* error if none */
            return -1;  /* or continue; */
        while ( p > ln && isspace(p[-1]) ) {    /* remove whitespace */
            memmove(p-1, p, strlen(p-1));       /*   before = sign */
            --p;
        }
        *p++ = 0;                               /* plug EOS over = sign */
        while ( isspace(p[0]) )             /* remove leading space on */
            memmove(p, p+1, strlen(p));         /*    init string */
        k = strlen(p) - 1;                      /* init string length */
        if ( k < 1 )
            return -1;

        if ( p[k] != '\n' )             /* if '\n' is missing, input */
            return -1;                  /*   exceeded buffer; error return */
        p[k] = 0;                       /* plug EOS over newline */

        va_start(arglist, types);       /* setup for arglist search */

        namep = names;                  /* init ptr to var names */
        typep = types;                  /* init ptr to var types */
        while ( *namep == ' ' )         /* skip blanks before namelist */
            ++namep;
        while ( *typep ) {          /* while any typelist items left...*/

            argp = (void *)va_arg(arglist, void *); /* get var arg */

            k = strcspn(namep, " ");        /* length of namelist entry */
            memmove(name, namep, k);        /* put into name hold area */
            name[k] = 0;                    /* terminate it */
            if ( strcmp(name, ln) != 0 ) { /* if it doesn't match... */
                namep += k;                 /* get next name */
                while ( *namep == ' ' )
                    ++namep;
                ++typep;                    /* get next type */
            } else {                        /* else name is found... */
                if ( *typep == 'i' ) {      /* if it's an int, init it */
                    *(int *)argp = atoi(p);
                } else if ( *typep == 's' || *typep == 'p' ) {
                    if ( *p == '"' ) {      /* is string in quotes? */
                        ++p;                /* skip leading quote, and */
                        e = strchr(p, '"'); /* look for trailing quote */
                        if ( e )            /* terminate string if found */
                            *e = 0;
                    }
                    if ( *typep == 'p' ) {      /* if it's a char *ptr */
                        e = malloc(strlen(p) + 1); /* get space */
                        if ( e == 0 ) {         /* error if no space */
                            return -1; /* call va_end(arglist); first? */
                        }
                        *(char **)argp = e;
                        strcpy(*(char **)argp, p);  /* copy in string */
                    } else                          /* must be char array */
                        strcpy(argp, p);            /* copy in string */
                } else {
                    return -1;                      /* bad type */
                }
                break;              /* break search; get next line */
            }
        }
        va_end(arglist);
    }
    return 0;
}

#ifdef TEST

#include "errors.h"                 /* For cant()     */

int foo;
char bar[80];
int baz;
char *quux;

int main(int argc, char **argv)
{
    FILE *fp;
    int k;

    if ( argc < 2 ) {
        fprintf(stderr, "missing arg\n");
        exit(1);
    }

    fp = cant(argv[1], "r");
    k = init_globals(fp, "foo bar baz quux", "isip",
        &foo, bar, &baz, &quux);
    printf("k: %d\n", k);
    printf("foo: %d\nbar: <%s>\nbaz: %d\nquux: <%s>\n",
        foo, bar, baz, quux);
    fclose(fp);

    return 0;
}

#endif /* TEST */
