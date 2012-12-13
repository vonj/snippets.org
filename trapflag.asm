        PAGE ,132

;  Install a custom Interrupt 23 (Ctrl-C exception) handler
;
;  Public domain by Bob Stout
;
;  Requires MASM 5.1 or later or equivalent
;
;  Assemble with:       MASM /Mx /z ...
;                       TASM /jMASM /mx /z ...

%       .MODEL  memodel,C               ;Add model support via command
                                        ;line macros, e.g.
                                        ;MASM /Dmemodel=LARGE

kbstatseg       equ     40h
kbstatofs       equ     17h
ctl_on          equ     0100b
alt_on          equ     1000b
sk_c            equ     2eh
EOI             equ     20h
PIC             equ     20h
kb_inp          equ     60h
kb_outp         equ     61h

        .CODE

_oldvec09       dd      ?
_oldvec1b       dd      ?

        public  ccrcvd

ccrcvd          dw      0

;
;  This is our actual ISR
;
myint09:
        push    ax                      ;save AX...
        pushf                           ;  ...& flags
        in      al,kb_inp               ;get scan code
        cmp     al,sk_c                 ;'C'?
        jne     do_old                  ;no, forget it

        push    ax                      ;yes, save it...
        push    es                      ;...& ES reg
        mov     ax,kbstatseg            ;read keyboard status from 40:17
        mov     es,ax
        mov     al,es:kbstatofs
        test    al,ctl_on               ;Ctrl pressed?
        pop     es                      ;(restore AX, ES)
        pop     ax
        jz      do_old                  ;no, forget it

        in      al,kb_outp              ;yes, toggle keyboard acknowledge line
        mov     ah,al
        or      al,80h
        out     kb_outp,al
        xchg    al,ah
        out     kb_outp,al
        cli
        mov     ax,EOI                  ;send end-of-interrupt code
        out     PIC,al
        mov     ax,1
        mov     CS:ccrcvd,ax
        pop     ax                      ;discard original flags...
        pop     ax                      ; ...& AX
        iret                            ;all done
do_old:
        popf                            ;restore flags...
        pop     ax                      ; ...& AX
        jmp     dword PTR CS:_oldvec09  ;call our handler

;
;  To avoid keyboard confusion, trap Ctrl-Break separately
;
myint1b:
        push    ax
        pushf
        mov     ax,-1
        mov     CS:ccrcvd,ax
        popf
        pop     ax
        iret

;
;  Call this to uninstall our ISR
;
undo09  PROC    USES DX DS AX
        mov     dx, word PTR CS:_oldvec09 ;restore original keyboard vector
        mov     ds, word PTR CS:_oldvec09+2
        mov     ax,2509h
        int     21h

        mov     dx, word PTR CS:_oldvec1b ;restore original keyboard vector
        mov     ds, word PTR CS:_oldvec1b+2
        mov     ax,251bh
        int     21h

        ret
undo09  ENDP

;
;  Call this to install  our ISR
;
ins09   PROC    USES AX BX DS ES

        mov     ax,3509h                ;get old keyboard ISR vector...
        int     21h
        mov     word PTR CS:_oldvec09,bx
        mov     word PTR CS:_oldvec09+2,es ;...and save it

        mov     ax,351bh                ;get old exit vector...
        int     21h
        mov     word PTR CS:_oldvec1b,bx
        mov     word PTR CS:_oldvec1b+2,es ;...and save it

        push    cs                      ;get myint09 segment in DS
        pop     ds
        mov     dx, OFFSET myint09      ;install myint09 in int 09h
        mov     ax,2509h
        int     21h

        push    cs                      ;get myint1b segment in DS
        pop     ds
        mov     dx, OFFSET myint1b      ;install myint1b in int 1bh
        mov     ax,251bh
        int     21h

        ret
ins09   ENDP

        end
