/*
 *  Written and released to the public domain by David Engel.
 *
 *  This function attempts to open a file which may be in any of
 *  several directories.  It is particularly useful for opening
 *  configuration files.  For example, PROG.EXE can easily open
 *  PROG.CFG (which is kept in the same directory) by executing:
 *
 *      cfg_file = pfopen("PROG.CFG", "r", getenv("PATH"));
 *
 *  NULL is returned if the file can't be opened.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snpdosys.h"

FILE *pfopen(const char *name, const char *mode, const char *dirs)
{
    char *ptr;
    char *tdirs;
    FILE *file = NULL;

    if (dirs == NULL || dirs[0] == '\0')
        return NULL;

    if ((tdirs = malloc(strlen(dirs)+1)) == NULL)
        return NULL;

    strcpy(tdirs, dirs);

    for (ptr = strtok(tdirs, SEP_CHARS); file == NULL && ptr != NULL;
        ptr = strtok(NULL, SEP_CHARS))
    {
        size_t len;
        char work[FILENAME_MAX];

        strcpy(work, ptr);
        len = strlen(work);
        if (len && work[len-1] != '/' && work[len-1] != '\\')
            strcat(work, "/");
        strcat(work, name);

        file = fopen(work, mode);
    }

    free(tdirs);

    return file;
}

#ifdef TEST

int main(int argc, char **argv)
{
    FILE *file;

    if (argc != 4)
    {
        fprintf(stderr, "usage: pfopen name mode dirs\n");
        exit(1);
    }

    file = pfopen(argv[1], argv[2], argv[3]);

    printf("%s \"%s\" with mode \"%s\"\n", (file == NULL) ?
        "Could not open" : "Opened", argv[1], argv[2]);

    return 0;
}

#endif /* TEST */
