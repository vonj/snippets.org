/*
**  PHONETIC.H - Snippets header file for functions to perform
**             phonetic string matching.
*/

#ifndef PHONETIC__H
#define PHONETIC__H

#include "sniptype.h"


/*
**  File SOUNDEX.C
*/

char *soundex(char *instr, char *outstr);


/*
**  File SOUNDEX4.C
*/

void  soundex4(const char *instr, char *outstr, int N);


/*
**  File SOUNDEX5.C
*/

#define SNDMAX (1 + 6 + 6*6 + 6*6*6)
#define SNDLEN (26 * SNDMAX)

int soundex5(char *instr);


/*
**  File METAPHON.C
*/

/*
**  MAXMETAPH is the length of the Metaphone code.
**
**  Four is a good compromise value for English names. For comparing words
**  which are not names or for some non-English names, use a longer code
**  length for more precise matches.
**
**  The default here is 5.
*/

#define MAXMETAPH 5

typedef enum {COMPARE, GENERATE} metaphlag;

Boolean_T metaphone(const char *Word, char *Metaph, metaphlag Flag);


/*
**  File APPROX.C
*/

void App_init(char *pattern, char *text, int degree);
void App_next(char **start, char **end, int *howclose);

#endif /* PHONETIC__H */
