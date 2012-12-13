        page    55, 132

;  FUNCTIONS: dosgetch/doswaitch
;
;  Donated to the public domain 96-11-12 by Tom Torfs (2:292/516)
;
;  dosgetch() attempts to read a character from stdin. Returns the
;  character read or EOF (-1) if no character is available. Extended keys
;  are returned in two parts: first a zero and then the actual code.
;
;  doswaitch() works alike but waits if no character is available.
;
;  Requires MASM 5.1 or later or equivalent
;
;  Assemble with:       MASM /Mx /z ...
;                       TASM /jMASM /mx /z ...
;

%       .MODEL  memodel,C               ;Add model support via
                                        ;command line macros, e.g.
                                        ;MASM /Dmemodel=LARGE

        .CODE
;
; int dosgetch(void) - comments see above
;

        PUBLIC  dosgetch

dosgetch        PROC

        push dx
        mov     ah, 6                   ; direct console I/O
        mov     dl, 255                 ; read a character
        int     21h                     ; let DOS do the job
        pop dx

        jz      nochar                  ; character ready ?

        xor     ah,ah                   ; clear high byte ax
        ret                             ; return

nochar:

        mov     ax, -1                  ; return EOF
        ret                             ; return

dosgetch        ENDP    

;
; int doswaitch(void) - comments see above
;

        PUBLIC  doswaitch

doswaitch       PROC

waitloop:

        call    dosgetch               ; try to read character
        
        cmp     ax, -1                 ; available ?
        je waitloop                    ; no -> repeat

        ret                            ; return

doswaitch       ENDP    

        end
