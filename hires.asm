1. 80 x 50 on VGA
=================
       mov     ax,1202h                ; select 400 scan line mode
       mov     bl,30h
       int     10h
       mov     ax,3                    ; select 80 x 25 16 colour mode
       int     10h
       mov     ax,1112h                ; load 8x8 character set into RAM
       mov     bl,0                    
       int     10h                    

2. 80 x 43 on EGA
=================
       mov     ax,3                    ; establish 350 scan line mode
       int     10h                     ; and 80 x 25 16 colour mode
       mov     ax,1112h                ; 8x8 character set
       mov     bl,0
       int     10h
       mov     ax,40h                  ; update cursor size/ pointers
       mov     es,ax
       mov     dx,es:[63h]
       mov     ax,060ah
       out     dx,ax
       mov     ax,000bh
       out     dx,ax
       mov     ax,12h                  ; set up new prtscr routine
       mov     bl,20h
       int     10h

Warnings:
=========
DOS's ANSI.SYS prior to DOS 5 has no comprehension of screens having more
than 25 lines! 

Reference:
"Programmer's Guide to PC and PS/2 Video Systems": author Richard Wilton.
Microsoft Press 1988.  ISBN 1-55615-103-9
