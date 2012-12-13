        page    55, 132

;
;  ABSDISK.ASM
;
;  Originally published as part of The MicroFirm Function Library
;  This version released to the public domain by the author, Bob Stout
;
;  Requires MASM 5.1 or later or equivalent
;
;  Assemble with:       MASM /Mx /z ...
;                       TASM /jMASM /mx /z ...
;

%       .MODEL  memodel,C               ;Add model support via
                                        ;command line macro,
                                        ;e.g. MASM /Dmemodel=LARGE ...
        extrn   _osmajor:BYTE
        public  absdisk

        .DATA
start   dw      ?
fill    dw      0
number  dw      ?
buf     dw      ?,?

        .CODE
absdisk PROC USES SI DI BP, func:BYTE, drive:WORD, num_sec:WORD, start_sec:WORD, buffer:PTR
        mov     AX,drive        ;Get drive number in AL
        mov     AH,_osmajor     ;Load OS version in AH
        mov     CX,num_sec      ;Set up regs for DOS 3 call
        mov     DX,start_sec
    IF  @DataSize
        push    DS              ;Save DS in L & C models
        lds     BX,buffer
    ELSE
        mov     BX,buffer
    ENDIF
        cmp     AH,4            ;DOS 4+?
        jb      doint           ;No, skip it
        mov     start,DX        ;Yes, fill in DCB structure
        mov     number,CX
        mov     buf,BX
        mov     buf+2,DS
        mov     cx,-1
    IF  @DataSize               ;Point to DCB
        mov     BX,@Data
        mov     DS,BX
    ENDIF
        mov     bx,OFFSET start
doint:  mov     AH,func         ;Read or Write?
        cmp     AH,25h
        jne     skip_1
        int     25h             ;Read sector
        jmp     skip_3
skip_1: cmp     AH,26h
        jne     skip_2
        int     26h             ;Write sector
        jmp     skip_3
skip_2: stc                     ;Invalid command
        mov     AX,-1
skip_3: jc      bye             ;Error?
        mov     AX,0            ;No, return SUCCESS
bye:    add     SP,2            ;Int 25h leave the flags on the stack
    IF  @DataSize
        pop     DS              ;Restore DS in L & C models
    ENDIF
        ret

absdisk ENDP  

        end
