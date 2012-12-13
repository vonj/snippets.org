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

        .DATA?
_origvec        dd      ?
_newvec         dd      ?

        .CODE

;
;  This is our actual ISR
;

myint23:
        call    dword PTR _newvec       ;call our handler
        iret

;
;  Call this to install  our ISR
;

ins23   PROC    USES AX BX DS ES, offs:WORD, segm:WORD
        mov     ax,3523h                ;get old vector...
        int     21h
        mov     word PTR _origvec,bx
        mov     word PTR _origvec+2,es  ;...and save it
        mov     ax,offs                 ;load handler offset...
        mov     word PTR _newvec,ax
        mov     ax,segm                 ; & segment into _newvec
        mov     word PTR _newvec+2,ax
        push    cs                      ;get myint23 segment in DS
        pop     ds
        mov     dx, OFFSET myint23      ;install myint23 in int 23h
        mov     ax,2523h
        int     21h
        ret
ins23   ENDP

;
;  Call this to uninstall our ISR
;

redo23  PROC    USES AX BX DS
        mov     dx, word PTR _origvec   ;restore original vector
        mov     ds, word PTR _origvec+2
        mov     ax,2523h
        int     21h
        ret
redo23  ENDP

        end
