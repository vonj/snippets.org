/*
**  Challenge: Write the smallest self-duplicating program, not
**  reading the source file, which successfully exits and is strictly
**  conforming Standard C.
**
**  Public domain response by Thad Smith
*/

#include<stdio.h>
main(){char*c="\\\"#include<stdio.h>%cmain(){char*c=%c%c%c%.102s%cn%c;printf(c+2,c[102],c[1],*c,*c,c,*c,c[1]);exit(0);}\n";printf(c+2,c[102],c[1],*c,*c,c,*c,c[1]);exit(0);}
