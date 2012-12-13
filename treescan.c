#include "bstuff.h"

	/* Copyright (C) 2001-03 by Frank Mitchell */
	/* Under Enclosed Free-Use License */
	/* treescan.c Version 0.55 (Second Release) */
	/* Contact: mitchell.frank@bigwig.net */

char*SntKP;
FILE*DatF,*BTreeF;
DATAHR DatHd;
RECORD Recd;
TREEHR T2H;
BRANCH*B2P;

int TREESCAN(void) {
	int wasfound;
	char*DatKP;
	size_t Lev;
	fpos_t DataQ;
	clock_t t0,t1;
	printf("\nSTARTING TREESCAN.C\n");
	if((BTreeF=fopen(BTREEFIL,"rb"))==NULL)goto BADOPENFIL;
	fread(&T2H,sizeof(TREEHR),1,BTreeF);
	B2P=calloc((T2H.RootLev+1),sizeof(BRANCH));
	if(B2P==NULL)goto BADMALLOC;
	for(Lev=0;Lev<=T2H.RootLev;Lev++) {
		B2P[Lev].SelKP=calloc(1,T2H.LenK);
		B2P[Lev].BodQP=calloc(T2H.FullNode,sizeof(fpos_t));
		B2P[Lev].BodKP=calloc(T2H.FullNode,T2H.LenK);
		if(B2P[Lev].SelKP==NULL)goto BADMALLOC;
		if(B2P[Lev].BodQP==NULL)goto BADMALLOC;
		if(B2P[Lev].BodKP==NULL)goto BADMALLOC;
	} /* Secondary Stores For Each Level */
	SntKP=malloc(T2H.LenK);
	DatKP=malloc(T2H.LenK);
	if(SntKP==NULL||DatKP==NULL)goto BADMALLOC;
	if((DatF=fopen(DATAFIL,"rb"))==NULL)goto BADOPENFIL;
	fread(&DatHd,sizeof(DATAHR),1,DatF);
	fread(&Recd,sizeof(RECORD),1,DatF);	/* Sentinel Keys */
	strncpy(SntKP,Recd.StrDatK,T2H.LenK);
	t0=clock();
	NODETREK();
	t1=clock();
	printf("B-Tree Scan clocked @ %f sec\n",(float)(t1-t0)
		/(float)(CLOCKS_PER_SEC));
	rewind(DatF);
	t0=clock();
	fread(&DatHd,sizeof(DATAHR),1,DatF);
	while(1) {
		fgetpos(DatF,&DataQ);
		fread(&Recd,sizeof(RECORD),1,DatF);
		if(feof(DatF))break;
		strncpy(DatKP,Recd.StrDatK,T2H.LenK);
		wasfound=SCANZERO(DatKP,DataQ);
		if(wasfound==0)goto DATNOTLOC;
	}  /* Scan Through All Records */
	t1=clock();
	printf("Data File Scan clocked @ %f sec\n",(float)(t1-t0)
		/(float)(CLOCKS_PER_SEC));
	if(fclose(DatF)!=0)goto BADCLOSEFIL;
	if(fclose(BTreeF)!=0)goto BADCLOSEFIL;
	for(Lev=0;Lev<=T2H.RootLev;Lev++) {
		free(B2P[Lev].SelKP);
		free(B2P[Lev].BodQP);
		free(B2P[Lev].BodKP);
	} /* free Secondary Stores */
	free(B2P);
	free(SntKP);
	free(DatKP);
	return 1;
BADOPENFIL:printf("Can't Open File in TREESCAN\n");goto STOP;
BADCLOSEFIL:printf("Can't Close File in TREESCAN\n");goto STOP;
BADMALLOC:printf("Can't alloc Memory in TREESCAN\n");goto STOP;
DATNOTLOC:printf("Data Record Not Located in TREESCAN\n");goto STOP;
STOP:exit(EXIT_FAILURE);return 0;
} /* Check Bushy Tree Index */

int NODETREK(void) {
	int j,m;
	char*PrevKP,*ScanKP;
	size_t Lev;
	fpos_t ScanQ;
	FILE*CheckF;
	PrevKP=malloc(T2H.LenK);
	if(PrevKP==NULL)goto BADMALLOC;
	strncpy(PrevKP,"",T2H.LenK);	/* Previous Key String Comparator */
	if((CheckF=fopen(CHECKFIL,"w"))==NULL)goto BADOPENFIL;
	ScanKP=PrevKP;
	ScanQ=T2H.RootQ;
	Lev=T2H.RootLev;
	do { /* Endless Loop */
		CHOPDOWN(NULL,ScanQ,&Lev,BTreeF,&T2H,B2P);
		for(j=0;j<=B2P[0].NodeE;j++) {
			ScanQ=B2P[0].BodQP[j];
			ScanKP=B2P[0].BodKP+T2H.LenK*j;
			m=COLX7F(PrevKP,ScanKP,T2H.LenK);
			if(m>0)goto WRONGORDER;
			fsetpos(DatF,&ScanQ);
			fread(&Recd,sizeof(RECORD),1,DatF);
			if(strncmp(Recd.StrDatK,ScanKP,T2H.LenK)!=0)
				goto WRONGDATREC;
			fprintf(CheckF,"%10ld  ",Recd.RecNum);
			fprintf(CheckF,"%-25s  ",Recd.StrDatK);
			fprintf(CheckF,"%+11ld\n",Recd.NumDatK);
			strncpy(PrevKP,ScanKP,T2H.LenK);
		} /* Level-0 Scan */
		strncpy(B2P[0].SelKP,ScanKP,T2H.LenK);
		do { /* Up Tree For Node Transition */
			Lev++;
			if(Lev>T2H.RootLev) {
				Lev--;
				if(strncmp(B2P[Lev].SelKP,SntKP,T2H.LenK)!=0)
					goto NOSENTINEL;
				if(fclose(CheckF)!=0)goto BADCLOSEFIL;
				free(PrevKP);
				return 1;
			} /* Finish Here */
			if(strncmp(B2P[Lev].SelKP,B2P[Lev-1].SelKP,T2H.LenK))
				goto WRONGKEYMAX;
		} while(B2P[Lev].SelJ==B2P[Lev].NodeE);
		B2P[Lev].SelJ++;
		ScanKP=B2P[Lev].BodKP+T2H.LenK*B2P[Lev].SelJ;
		strncpy(B2P[Lev].SelKP,ScanKP,T2H.LenK);
		ScanQ=B2P[Lev].BodQP[B2P[Lev].SelJ];
		Lev--;
	} while(1);	/* CHOPDOWN Next */
BADMALLOC:printf("Can't alloc Memory in NODETREK\n");goto STOP;
BADOPENFIL:printf("Can't Open Check File in NODETREK\n");goto STOP;
BADCLOSEFIL:printf("Can't Close Check File in NODETREK\n");goto STOP;
WRONGORDER:printf("Node Keys Out Of Order in NODETREK\n");goto STOP;
WRONGKEYMAX:printf("Parent Key Max Wrong in NODETREK\n");goto STOP;
WRONGDATREC:printf("Tree Finds Wrong Record in NODETREK\n");goto STOP;
NOSENTINEL:printf("Max Key Not Sentinel in NODETREK\n");goto STOP;
STOP:exit(EXIT_FAILURE);return 0;
} /* Scan All Nodes In Bushy Tree */

int SCANZERO(char*DatKP,fpos_t DataQ) {
	int j;
	char *ScanKP;
	size_t Lev;
	fpos_t NextQ;
	NextQ=T2H.RootQ;
	Lev=T2H.RootLev;
	CHOPDOWN(DatKP,NextQ,&Lev,BTreeF,&T2H,B2P);
	do { /* Endless Loop */
		j=B2P[0].SelJ;
		while(j<=B2P[0].NodeE) {
			ScanKP=B2P[0].BodKP+T2H.LenK*j;
			if(strncmp(DatKP,ScanKP,T2H.LenK))return 0;
			if(B2P[0].BodQP[j]==DataQ)return 1;
			j++;
		} /* Scan Level-0 Node For Duplicate Keys */
		do { /* Up Tree For Node Transition */
			Lev++;
			if(Lev>T2H.RootLev)return 0;	/* Root Ended */
		} while(B2P[Lev].SelJ==B2P[Lev].NodeE);
		B2P[Lev].SelJ++;
		NextQ=B2P[Lev].BodQP[B2P[Lev].SelJ];
		Lev--;
		CHOPDOWN(NULL,NextQ,&Lev,BTreeF,&T2H,B2P);
	} while(1);
} /* Binary Chop Down To Level-0 Then Scan For Duplicate Keys */
