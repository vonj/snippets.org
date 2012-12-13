#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

	/* Copyright (C) 2001-03 by Frank Mitchell */
	/* Under Enclosed Free-Use License */
	/* bstuff.h Version 0.55 (Second Release) */
	/* Contact: mitchell.frank@bigwig.net */

#define ZLEN 25				/* Data String Length */
#define FNAMLEN 32			/* Max Filename Length */
#define MESGFIL "MESSAGE.DAT"		/* Error Messages */
#define SPECFIL "SPECFIL.DAT"		/* User Config Spec */
#define DATAFIL "WEIRD.DAT"		/* Data File */
#define BTREEFIL "BUSHY.DAT"		/* Bushy Tree File */
#define CHECKFIL "CHECKORD.DAT"		/* Sorted Data Check */

typedef struct {
char StrkBfil[FNAMLEN];			/* String Key B-Tree Filename */
char NumkBfil[FNAMLEN];			/* Numeric Key B-Tree Filename */
} DATAHR;			/* Data File Header */

typedef struct {
unsigned Flags;				/* Record Attributes */
long RecNum;				/* Record # */
char StrDatK[ZLEN];			/* ASCII String Key */
long NumDatK;				/* Long Integer Key */
} RECORD;			/* Data File Record */

typedef struct {
fpos_t RootQ;				/* Root Node Posn */
size_t RootLev;				/* Root Node Level # */
size_t FullNode;			/* # Keys in Disk Node */
size_t LenK;				/* Key Length In Bytes */
fpos_t VacQ;				/* Vacant B-Tree End Posn */
} TREEHR;			/* Bushy Tree Header */

typedef struct {
size_t KeyTot;				/* Node Key Total */
int Lev;				/* Node Level, Leaf=0 */
} NODEHR;			/* Node Header */

typedef struct {
fpos_t NodeQ;				/* File Position Of Node */
int NodeE;				/* End Element # (=Total-1) */
int SelJ;				/* Node Body Index Selected */
char*SelKP;				/* Node Body Key Selected */
fpos_t*BodQP;				/* Node Body File Posn Store */
char*BodKP;				/* Node Body Key Store */
} BRANCH;			/* Branch Tracking At Each Level */

int DATINIT(void);				/* Data File Prep */
long KNURAN(long*,long*);			/* Knuth Virtuous Random #s */
int MAKSTR(char*,int,long*,long*);		/* Random String Keys */
int BTRINIT(void);				/* Initialize Bushy Tree */
int BUSH(char*,fpos_t);				/* Bushy Tree Build */
char*CHOPDOWN(char*,fpos_t,size_t*,FILE*,TREEHR*,BRANCH*);	/* Node Chop */
int STICKIN(char*,fpos_t*,size_t);		/* Insert Node Elements */
int NEWROOT(char*,fpos_t,size_t);		/* Create New B-Tree Root */
int COLX7F(char*,char*,size_t);			/* Custom ASCII Collation */
int TREESCAN(void);				/* Check Bushy Tree */
int NODETREK(void);				/* Scan Node Hierarchy */
int SCANZERO(char*,fpos_t);			/* Scan Level-0 For Keys */
