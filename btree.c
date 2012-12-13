#include "bstuff.h"

	/* Copyright (C) 2001-03 by Frank Mitchell */
	/* Under Enclosed Free-Use License */
	/* btree.c Version 0.55 (Second Release) */
	/* Contact: mitchell.frank@bigwig.net */

char*SentKP;
FILE*SpecF,*DataF,*BushyF;
DATAHR DataHr;
RECORD Record;
TREEHR T1H;
NODEHR NodeHr;
BRANCH*B1P;

int BTRINIT(void) {
	long Gen;
	char*InsKP;
	size_t Lev;
	fpos_t InsQ;
	clock_t t0,t1;
	printf("\nSTARTING BTREE.C\n");
	if((SpecF=fopen(SPECFIL,"r"))==NULL)goto BADOPENFIL;
	fscanf(SpecF,"%li",&Gen);
	fscanf(SpecF,"%li",&Gen);
	fscanf(SpecF,"%li",&Gen);
	fscanf(SpecF,"%li",&Gen);
	T1H.FullNode=(size_t)Gen;
	fscanf(SpecF,"%li",&Gen);
	T1H.LenK=(size_t)Gen;
	if(fclose(SpecF)!=0)goto BADCLOSEFIL;
	B1P=calloc(1,sizeof(BRANCH));
	B1P[0].SelKP=calloc(1,T1H.LenK);
	B1P[0].BodQP=calloc((T1H.FullNode+1),sizeof(fpos_t));
	B1P[0].BodKP=calloc((T1H.FullNode+1),T1H.LenK);
	if(B1P==NULL||B1P[0].SelKP==NULL)goto BADMALLOC;
	if(B1P[0].BodQP==NULL||B1P[0].BodKP==NULL)goto BADMALLOC;
	SentKP=malloc(T1H.LenK);
	InsKP=malloc(T1H.LenK);
	if(SentKP==NULL||InsKP==NULL)goto BADMALLOC;
	T1H.RootLev=0;
	if((BushyF=fopen(BTREEFIL,"wb+"))==NULL)goto BADOPENFIL;
	fwrite(&T1H,sizeof(TREEHR),1,BushyF);
	fflush(BushyF);
	fgetpos(BushyF,&T1H.RootQ);	/* Initial Root Posn */
	NodeHr.KeyTot=0;
	NodeHr.Lev=0;
	fwrite(&NodeHr,sizeof(NODEHR),1,BushyF);
	fwrite(B1P[0].BodQP,sizeof(fpos_t),T1H.FullNode,BushyF);
	fwrite(B1P[0].BodKP,T1H.LenK,T1H.FullNode,BushyF);
	fflush(BushyF);
	fgetpos(BushyF,&T1H.VacQ);	/* Initial Vacant Posn */
	t0=clock();
	if((DataF=fopen(DATAFIL,"rb"))==NULL)goto BADOPENFIL;
	fread(&DataHr,sizeof(DATAHR),1,DataF);
	fgetpos(DataF,&InsQ);		/* Sentinel File Posn */
	fread(&Record,sizeof(RECORD),1,DataF);
	strncpy(SentKP,Record.StrDatK,T1H.LenK);
	BUSH(SentKP,InsQ);		/* Insert Sentinel in B-Tree */
	while(1) {
		fgetpos(DataF,&InsQ);	/* Data File Posn */
		fread(&Record,sizeof(RECORD),1,DataF);
		if(feof(DataF))break;
		strncpy(InsKP,Record.StrDatK,T1H.LenK);
		BUSH(InsKP,InsQ);
	}  /* Process DATAFIL Records */
	t1=clock();
	printf("B-Tree Build clocked @ %f sec\n",(float)(t1-t0)
		/(float)(CLOCKS_PER_SEC));
	if(fclose(DataF)!=0)goto BADCLOSEFIL;
	rewind(BushyF);		/* Tree Head Posn */
	fwrite(&T1H,sizeof(TREEHR),1,BushyF);
	if(fclose(BushyF)!=0)goto BADCLOSEFIL;
	printf("# Bushy Tree Levels = %d\n",(int)T1H.RootLev+1);
	for(Lev=0;Lev<=T1H.RootLev;Lev++) {
		free(B1P[Lev].SelKP);
		free(B1P[Lev].BodQP);
		free(B1P[Lev].BodKP);
	} /* free Secondary Stores */
	free(B1P);
	free(SentKP);
	free(InsKP);
	return 1;
BADOPENFIL:printf("Can't Open File in BTRINIT\n");goto STOP;
BADCLOSEFIL:printf("Can't Close File in BTRINIT\n");goto STOP;
BADMALLOC:printf("Can't alloc Memory in BTRINIT\n");goto STOP;
STOP:exit(EXIT_FAILURE);return 0;
} /* Initialize Bushy Tree */

int BUSH(char*InsKP,fpos_t InsQ) {
	int more;
	size_t Lev;
	Lev=T1H.RootLev;
	CHOPDOWN(InsKP,T1H.RootQ,&Lev,BushyF,&T1H,B1P);
	more=1;		/* Insertion Needed */
	while(Lev<=T1H.RootLev) {
		if(more)more=STICKIN(InsKP,&InsQ,Lev);
		Lev++;
	} /* Go Back Up Tree Inserting As Required */
	if(more)NEWROOT(InsKP,InsQ,T1H.RootLev);
	return 1;
} /* Build Bushy Tree */

char*CHOPDOWN(char*DatKP,fpos_t NextQ,size_t*LevP,
		FILE*TxF,TREEHR*TxP,BRANCH*BxP) {
	int Lev,lb,ub,j;
	char*GenKP;
	NODEHR LevHead;
	Lev=*LevP;
	while(Lev!=-1) {
		if(BxP[Lev].NodeQ!=NextQ) {
			BxP[Lev].NodeQ=NextQ;
			fsetpos(TxF,&NextQ);
			fread(&LevHead,sizeof(NODEHR),1,TxF);
			if(LevHead.Lev!=Lev)goto WRONGLEVEL;	/* Useful! */
			BxP[Lev].NodeE=LevHead.KeyTot-1;	/* End Elem */
			fread(BxP[Lev].BodQP,sizeof(fpos_t),TxP->FullNode,TxF);
			fread(BxP[Lev].BodKP,TxP->LenK,LevHead.KeyTot,TxF);
		} /* Need To Read From Disk */
		lb=-1;
		ub=(DatKP==NULL)?0:BxP[Lev].NodeE+1;
		while(ub-lb!=1) {
			j=(ub+lb)>>1;
			GenKP=BxP[Lev].BodKP+TxP->LenK*j;
			if(COLX7F(GenKP,DatKP,TxP->LenK)<0)lb=j;
			else ub=j;
		} /* Binary Chop to Insert Position ub */
		BxP[Lev].SelJ=ub;
		GenKP=BxP[Lev].BodKP+TxP->LenK*ub;
		strncpy(BxP[Lev].SelKP,GenKP,TxP->LenK);
		NextQ=BxP[Lev].BodQP[ub];
		Lev--;
	} /* Binary Chop Down To Level-0 */
	*LevP=0;
	return BxP[0].SelKP;
WRONGLEVEL:printf("Wrong Node Level in CHOPDOWN\n");goto STOP;
STOP:exit(EXIT_FAILURE);return 0;
} /* Find Data File Position */

int STICKIN(char*InsKP,fpos_t*InsQP,size_t Lev) {
	int j,more;
	char*GenKP;
	size_t HiTot,LoTot;
	for(j=B1P[Lev].NodeE+1;j>B1P[Lev].SelJ;j--) {
		B1P[Lev].BodQP[j]=B1P[Lev].BodQP[j-1];
		GenKP=B1P[Lev].BodKP+T1H.LenK*j;
		strncpy(GenKP,GenKP-T1H.LenK,T1H.LenK);
	} /* Shift Key File Posn & Char Array */
	B1P[Lev].BodQP[B1P[Lev].SelJ]=*InsQP;	/* Insert File Posn */
	GenKP=B1P[Lev].BodKP+T1H.LenK*B1P[Lev].SelJ;
	strncpy(GenKP,InsKP,T1H.LenK);		/* Insert Key Chars */
	B1P[Lev].NodeE++;
	NodeHr.KeyTot=B1P[Lev].NodeE+1;
	NodeHr.Lev=Lev;
	if(NodeHr.KeyTot!=T1H.FullNode+1) {
		fsetpos(BushyF,&B1P[Lev].NodeQ);
		fwrite(&NodeHr,sizeof(NODEHR),1,BushyF);
		fwrite(B1P[Lev].BodQP,sizeof(fpos_t),T1H.FullNode,BushyF);
		GenKP=B1P[Lev].BodKP;
		fwrite(GenKP,T1H.LenK,NodeHr.KeyTot,BushyF);
		fflush(BushyF);
		more=0;		/* No More Insertions */
	} /* Rewrite Original Node */
	else {
		HiTot=(T1H.FullNode+1)>>1;	/* Can Modify For End-Nodes */
		LoTot=T1H.FullNode+1-HiTot;
		NodeHr.KeyTot=HiTot;		/* For Hi Half in Same Posn */
		fsetpos(BushyF,&B1P[Lev].NodeQ);
		fwrite(&NodeHr,sizeof(NODEHR),1,BushyF);
		fwrite(B1P[Lev].BodQP+LoTot,sizeof(fpos_t),T1H.FullNode,BushyF);
		GenKP=B1P[Lev].BodKP+T1H.LenK*LoTot;
		fwrite(GenKP,T1H.LenK,HiTot,BushyF);
		more=1;			/* Lo Half Needs Insertion In Parent */
		B1P[Lev].NodeQ=T1H.VacQ;	/* Store Lo Half New Posn */
		B1P[Lev].NodeE=LoTot-1;
		NodeHr.KeyTot=LoTot;
		GenKP=B1P[Lev].BodKP+T1H.LenK*(LoTot-1);
		strncpy(InsKP,GenKP,T1H.LenK);		/* Lo Max Key Insert */
		*InsQP=T1H.VacQ;			/* File Posn Insert */
		fsetpos(BushyF,InsQP);
		fwrite(&NodeHr,sizeof(NODEHR),1,BushyF);
		fwrite(B1P[Lev].BodQP,sizeof(fpos_t),T1H.FullNode,BushyF);
		GenKP=B1P[Lev].BodKP;
		fwrite(GenKP,T1H.LenK,T1H.FullNode,BushyF);
		fflush(BushyF);
		fgetpos(BushyF,&T1H.VacQ);
	}/* Split Node So Hi Half With Old Max Key Retains Old Position */
	return more;
} /* Add Elem To Node, Splitting If Needed */

int NEWROOT(char*InsKP,fpos_t InsQ,size_t Lev) {
	Lev++;
	B1P=realloc(B1P,(Lev+1)*sizeof(BRANCH));
	B1P[Lev].SelKP=calloc(1,T1H.LenK);
	B1P[Lev].BodQP=calloc((T1H.FullNode+1),sizeof(fpos_t));
	B1P[Lev].BodKP=calloc((T1H.FullNode+1),T1H.LenK);
	if(B1P==NULL||B1P[Lev].SelKP==NULL)goto BADMALLOC;
	if(B1P[Lev].BodQP==NULL||B1P[Lev].BodKP==NULL)goto BADMALLOC;
	B1P[Lev].NodeQ=T1H.VacQ;	/* New Root Posn */
	B1P[Lev].NodeE=1;		/* Always 2 Elems */
	B1P[Lev].BodQP[0]=InsQ;
	strncpy(B1P[Lev].BodKP,InsKP,T1H.LenK);
	B1P[Lev].BodQP[1]=T1H.RootQ;	/* Old Root Posn With Sentinel */
	strncpy(B1P[Lev].BodKP+T1H.LenK,SentKP,T1H.LenK);
	NodeHr.KeyTot=2;
	NodeHr.Lev=Lev;
	fsetpos(BushyF,&B1P[Lev].NodeQ);
	fwrite(&NodeHr,sizeof(NODEHR),1,BushyF);
	fwrite(B1P[Lev].BodQP,sizeof(fpos_t),T1H.FullNode,BushyF);
	fwrite(B1P[Lev].BodKP,T1H.LenK,T1H.FullNode,BushyF);
	fflush(BushyF);
	fgetpos(BushyF,&T1H.VacQ);
	rewind(BushyF);
	T1H.RootQ=B1P[Lev].NodeQ;
	T1H.RootLev=Lev;
	fwrite(&T1H,sizeof(TREEHR),1,BushyF);
	fflush(BushyF);
	return 1;
BADMALLOC:printf("Can't alloc Memory in NEWROOT\n");goto STOP;
STOP:exit(EXIT_FAILURE);return 0;
} /* Create New Bushy Tree Root */

int COLX7F(char*StrA,char*StrB,size_t Lenx) {
	int a,b,cpr=0;
	char*PA,*PB;
	size_t k;
	static const int ascol[128]=
		{0,4,6,5,2,19,18,25,30,31,28,27,26,29,20,21
		,1,11,12,13,14,17,22,3,15,23,16,24,7,8,9,10
		,32,60,41,64,62,63,52,55,47,46,58,57,40,36,37,35
		,65,66,67,68,69,70,71,72,73,74,38,39,49,50,48,59
		,61,75,77,79,81,83,85,87,89,91,93,95,97,99,101,103
	,105,107,109,111,113,115,117,119,121,123,125,45,34,44,54,33
	,56,76,78,80,82,84,86,88,90,92,94,96,98,100,102,104
	,106,108,110,112,114,116,118,120,122,124,126,43,51,42,53,127};
	PA=StrA;PB=StrB;
	for(k=0;k<Lenx;k++) {
		a=(int)*PA;b=(int)*PB;
		cpr=ascol[a]-ascol[b];
		if(cpr!=0)break;
		if(a==0 || b==0)break;
		PA++;PB++;
	} /* Compare Character Weights */
	return cpr;
}/* Customized ASCII Collation */

