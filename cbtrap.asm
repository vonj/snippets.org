        PAGE ,132

;  Install a custom Interrupt 1b (Ctrl-Break exception) handler
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

        .DATA?
_origvec        dd      ?

        .DATA

        public  cbrcvd

cbrcvd  dw      0

        .CODE

;
;  This is our actual ISR
;
myint1b:
        mov     ax,-1
        mov     cbrcvd,ax
        iret

;
;  Call this to install  our ISR
;
ins1b   PROC    USES AX BX DS ES
        mov     ax,351bh                ;get old vector...
        int     21h
        mov     word PTR _origvec,bx
        mov     word PTR _origvec+2,es  ;...and save it

        push    cs                      ;get myint1b segment in DS
        pop     ds
        mov     dx, OFFSET myint1b      ;install myint1b in int 1bh
        mov     ax,251bh
        int     21h
        ret
ins1b   ENDP

;
;  Call this to uninstall our ISR
;
redo1b  PROC    USES AX BX DS
        mov     dx, word PTR _origvec   ;restore original vector
        mov     ds, word PTR _origvec+2
        mov     ax,251bh
        int     21h
        ret
redo1b  ENDP

        end
