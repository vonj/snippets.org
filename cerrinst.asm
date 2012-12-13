        PAGE ,132

;  Install an Interrupt 24 (DOS critical error exception) handler
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

        PUBLIC cedevdvr, cetype, ceerror, cereturn
_origvec        dd      ?
_newvec         dd      ?
cedevdvr        dd      ?
cetype          dw      ?
ceerror         dw      ?
cereturn        db      ?

        .CODE

;
;  This is our actual ISR
;
myint24:
        push    ds                      ;save registers which may be
        push    es                      ;required in case "Retry" is
        push    bx                      ;selected
        push    cx
        push    dx
        push    si
        push    di
        push    bp
        mov     word PTR cedevdvr,si    ;save device driver header address
        mov     word PTR cedevdvr+2,bp
        mov     cetype,ax               ;save error type information
        mov     ceerror,di              ;save error code information
        call    far PTR _newvec         ;call our handler
        mov     al,cereturn             ;set up return code (abort, retry...)
        pop     bp                      ;restore necessary registers
        pop     di
        pop     si
        pop     dx
        pop     cx
        pop     bx
        pop     es
        pop     ds
        iret

;
;  Call this to install  our ISR
;
;  void (_far *)() ins24(unsigned short segm, unsigned short offs);
;  void (_far *)() ins24((void _far *handler)());
;
;  Paramters (option 1) - 1 : Segment of handler
;                         2 : Offset of handler
;
;  Paramters (option 2) - 1 : Address of handler
;
;  Returns pointer to old handler
;
ins24   PROC    USES AX BX DS ES, segm:WORD, offs:WORD
        mov     ax,3524h                ;get old vector...
        int     21h
        mov     word PTR _origvec,bx
        mov     word PTR _origvec+2,es  ;...and save it
        mov     ax,offs                 ;load handler offset...
        mov     word PTR _newvec,ax
        mov     ax,segm                 ; & segment into _newvec
        mov     word PTR _newvec+2,ax
        push    cs                      ;get myint24 segment in DS
        pop     ds
        mov     dx, OFFSET myint24      ;install myint24 in int 24h
        mov     ax,2524h                ;  into Interrupt 24h
        int     21h
        mov     ax,word PTR _origvec
        mov     dx,word PTR _origvec+2
ins24   ENDP

;
;  Call this to uninstall our ISR
;
;  void redo24(void);
;
redo24  PROC    USES AX BX DS
        mov     dx, word PTR _origvec   ;restore original vector
        mov     ds, word PTR _origvec+2
        mov     ax,2524h
        int     21h
        ret
redo24  ENDP

        end
