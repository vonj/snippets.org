        PAGE    55,132
        .LIST
;
;       Interrupt 2Eh Call
;
;               From information originally published in
;               PC magazine, April 28, 1987. Requires
;               MASM 5.1 or later.
;
;               Adapted by Bob Stout.
;
;       NOTES:  INT 2Eh passes a formatted command line
;               directly to the resident portion of
;               COMMAND.COM for execution. It functions
;               similarly to the 'EXEC' function in DOS
;               but is generally quicker. This is an
;               undocumented DOS function and is subject
;               to change in future releases of DOS. It
;               also aborts any .BAT file which invokes
;               a program which uses it. Use with care!
;
;  Assemble with:       MASM /Mx /z ...
;                       TASM /jMASM /mx /z ...
;
 
%       .MODEL  memodel,C               ;Add model support via
                                        ;command line macros, e.g.
                                        ;MASM /Mx /Dmemodel=LARGE
 
        .CODE
 
        PUBLIC  _Int_2E
 
_Int_2E PROC    USES SI DI DS ES, command:PTR
        Mov     CS:SaveSP,SP
        Mov     CS:SaveSS,SS
    IF @DataSize
        Lds     SI,command
    ELSE
        Mov     SI,command
    Endif
 
        Int     2Eh
 
        Mov     AX,CS:SaveSS
        Mov     SS,AX
        Mov     SP,CS:SaveSP
        Ret
 
SaveSS  Dw      ?
SaveSP  Dw      ?
 
_Int_2E ENDP
 
        End
