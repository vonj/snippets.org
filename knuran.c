long KNURAN(long*,long*);	/* Knuth Virtuous Random #s */

#define MM 2147483647L		/* Mersenne Prime */
#define AA 48271L		/* Spectral Success */
#define QQ 44488L		/* (long)(MM/AA) */
#define RR 3399L		/* (MM%AA) Important: RR<QQ */
#define MMM 2147483399L		/* Non-Mersenne Prime */
#define AAA 40692L		/* Spectral Success */
#define QQQ 52774L		/* (long)(MMM/AAA) */
#define RRR 3791L		/* (MMM%AAA) Again: RRR<QQQ */

long KNURAN(long *Xknu,long *Yknu) {
	long xx,yy,Z;
	xx=*Xknu; yy=*Yknu;
	xx=AA*(xx%QQ)-RR*(long)(xx/QQ);
	if(xx<0) xx+=MM;
	yy=AAA*(yy%QQQ)-RRR*(long)(yy/QQQ);
	if (yy<0) yy+=MMM;
	Z=xx-yy;
	if (Z<=0) Z+=MM;
	*Xknu=xx; *Yknu=yy;
	return Z;
} /* Initialize Seeds 0<Xknu<MM, 0<Yknu<MMM. Gives 0<Z<2**31.
Virtuous Random Integer Generator from Donald E Knuth of
Stanford University. See: "The Art Of Computer Programming"
Volume 2: "Seminumerical Algorithms", ISBN 0-201-89684-2 */

