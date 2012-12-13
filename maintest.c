#include "bstuff.h"

	/* Copyright (C) 2001-03 by Frank Mitchell */
	/* Under Enclosed Free-Use License */
	/* maintest.c Version 0.55 (Second Release) */
	/* Contact: mitchell.frank@bigwig.net */

int main(void) {
	DATINIT();
	BTRINIT();
	TREESCAN();
	return 1;
} /* Driver */

int DATINIT(void) {
	long Gen,kl,NRecs,Xknu=43019L,Yknu=71633L;
	char X7F[]={'\x7F','\0'};
	size_t NodeSize,KeyLen;
	FILE *MesgF,*SpecF,*DataF;
	DATAHR DataHr;
	RECORD Record;
	if((MesgF=fopen(MESGFIL,"w"))==NULL)goto BADOPENFIL;
	printf("\nSTARTING MAINTEST.C\n");
	if((SpecF=fopen(SPECFIL,"r+"))==NULL) {		/* No Spec File Yet */
		if((SpecF=fopen(SPECFIL,"w"))==NULL)goto BADOPENFIL;
		printf("Enter # Records in WEIRD.DAT [Min=2]\n");
		scanf("%li",&Gen);
		NRecs=(Gen<2)?2:Gen;
		printf("Enter Node Size [Min=3]\n");
		scanf("%li",&Gen);
		NodeSize=(Gen<3)?3:(size_t)Gen;
		printf("Enter Key Length [Min=1]\n");
		scanf("%li",&Gen);
		KeyLen=(Gen<1)?1:(size_t)Gen;
	} /* Initial Run Specification */
	else {
		printf("CAN USE PREVIOUS VALUES [Enter 0]\n");
		fscanf(SpecF,"%li",&Xknu);
		fscanf(SpecF,"%li",&Yknu);
		fscanf(SpecF,"%li",&NRecs);
		fscanf(SpecF,"%li",&Gen);
		NodeSize=(size_t)Gen;
		fscanf(SpecF,"%li",&Gen);
		KeyLen=(size_t)Gen;
		printf("Enter Random Seed #1 [0< %li <2147483647]\n",Xknu);
		scanf("%li",&Gen);
		Xknu=(Gen<1)?Xknu:Gen;
		printf("Enter Random Seed #2 [0< %li <2147483399]\n",Yknu);
		scanf("%li",&Gen);
		Yknu=(Gen<1)?Yknu:Gen;
		printf("Enter # Records in WEIRD.DAT [1< %li <Many]\n",NRecs);
		scanf("%li",&Gen);
		NRecs=(Gen<2)?NRecs:Gen;
		printf("Enter Node Size [2< %u <~127]\n",(unsigned)NodeSize);
		scanf("%li",&Gen);
		NodeSize=(Gen<3)?NodeSize:(size_t)Gen;
		printf("Enter Key Length [0< %u <Many]\n",(unsigned)KeyLen);
		scanf("%li",&Gen);
		KeyLen=(Gen<1)?KeyLen:(size_t)Gen;
	} /* Vary Specification From Previous Run */
	printf("USER INPUT ENDS HERE\n");
	rewind(SpecF);
	fprintf(SpecF," %li \n",Xknu);
	fprintf(SpecF," %li \n",Yknu);
	fprintf(SpecF," %li \n",NRecs);
	fprintf(SpecF," %li \n",(long)NodeSize);
	fprintf(SpecF," %li \n",(long)KeyLen);
	if(fclose(SpecF)!=0)goto BADCLOSEFIL;
	Record.Flags=16384|4|1;			/* Sys + Read + Hide */
	Record.RecNum=0;			/* Sentinel Record #0 */
	strncpy(Record.StrDatK,X7F,ZLEN);	/* String Sentinel */
	Record.NumDatK=LONG_MAX;		/* long int Sentinel */
	if((DataF=fopen(DATAFIL,"wb"))==NULL)goto BADOPENFIL;
	fwrite(&DataHr,sizeof(DATAHR),1,DataF);
	fwrite(&Record,sizeof(RECORD),1,DataF);
	for(kl=1;kl<NRecs;kl++) {
		Record.Flags=0;
		Record.RecNum=kl;
		MAKSTR(Record.StrDatK,ZLEN,&Xknu,&Yknu);
		Record.NumDatK=KNURAN(&Xknu,&Yknu);
		fwrite(&Record,sizeof(RECORD),1,DataF);
	} /* Non-Sentinel Records */
	if(fclose(DataF)!=0)goto BADCLOSEFIL;
	if((DataF=fopen(DATAFIL,"rb"))==NULL)goto BADOPENFIL;
	fread(&DataHr,sizeof(DATAHR),1,DataF);
	for(kl=0;kl<8;kl++) {
		fread(&Record,sizeof(RECORD),1,DataF);
		fprintf(MesgF,"%-4X  ",Record.Flags);
		fprintf(MesgF,"%10ld  ",Record.RecNum);
		fprintf(MesgF,"%-25s  ",Record.StrDatK);
		fprintf(MesgF,"%+11ld\n",Record.NumDatK);
	} /* List 8 Records */
	if(fclose(DataF)!=0)goto BADCLOSEFIL;
	if(fclose(MesgF)!=0)goto BADCLOSEFIL;
	return 1;
BADOPENFIL:printf("Can't Open File in DATINIT\n");goto STOP;
BADCLOSEFIL:printf("Can't Close File in DATINIT\n");goto STOP;
STOP:exit(EXIT_FAILURE);return 0;
} /* Initialize Data File */

int MAKSTR(char*Buffer,int Len,long*XknuP,long*YknuP) {
	int k,Lenb,m;
	long RanL;
	float a;
	static const float POW31=2147483648.0; 	/* Max Knuth Integer +1 */
		RanL=KNURAN(XknuP,YknuP);
		a=RanL/POW31;
		Lenb=(int)(Len-a*0.5*Len);	/* Max = Len-1 */
		Lenb=(Lenb<2)?2:Lenb;
	for(k=0;k<Lenb;k++) {
		RanL=KNURAN(XknuP,YknuP);
		a=RanL/POW31;
		m=(int)(48.0+a*62.0);
		if(m>57)m+=7;
		if(m>90)m+=6;
		Buffer[k]=(char)m;
	} /* Random Distrib Alpha-Numeric <\x7F */
	Buffer[Lenb]='\0';
	return Lenb;
} /* Generate Random String */
