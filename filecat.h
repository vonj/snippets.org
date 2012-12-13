/*
**  SNIPPETS header for FILECAT.C and MAXLINES.C
*/

unsigned int max_line(char ** str_array);             /* MAXLINES.C     */
int filecat(char * dest_file,                         /* FILECAT.C      */
            char * src_file,
            int line_max,
            int max_lines);
