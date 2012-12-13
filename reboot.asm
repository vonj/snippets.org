; by: David Nugent via FidoNet ECPROG echo
;
; works reliably under protected mode environments
;
; modified by Bob Stout
;
;  Requires MASM 5.1 or later or equivalent
;
;  Assemble with:       MASM /Mx /z ...
;                       TASM /jMASM /mx /z ...
;

%       .MODEL  memodel,C               ;Add model support via
                                        ;command line macros, e.g.
                                        ;MASM /Dmemodel=LARGE
 
BIOS_POST  equ    0472H         ; POST (warm boot) flag

        .CODE
;
; void reboot(int warm);        /* 0 => cold boot, else warm boot */
;

        PUBLIC  reboot

reboot  PROC      warm:WORD
        xor BX,BX
        mov ES,BX

        mov AX,warm             ; AL=1 for warm boot, 0 for cold
        cmp AL,1
        jnz @NcB
        mov AX,1234H            ; Avoid POST
@NcB:
        mov ES:[BIOS_POST],AX   ; Install flag
 
        cli                     ; Reboot
        xor AX,AX
        mov DS,AX
        mov ES,AX
        mov SS,AX
        mov SP,AX
@cP:
        in AL,64H               ; Wait on AT keyboard controller
        test AL,2
        jne @cP
 
        mov ah,0d               ; DOS function 0Dh: flush disk buffers
        int 21                  ; (this will also flush most caches)

        xor AL,AL               ; Try reset lines
        out 64H,AL
        mov ax,0000
@Dly1:
        dec ax                  ; Delay loop:  wait for reset signal
        cmp ax,0000
        jnz @Dly1

        mov AL,0FEh
        out 64H,AL
        mov ax,0000
@Dly2:
        dec ax
        cmp ax,0000
        jnz @Dly2               ; Still going?  hardware reset failed

        mov AX,0002H            ; Jump to reset vector
        push AX                 ; via IRET
        mov AX,00000H
        push AX
        mov AX,0FFFFH
        push AX
        iret

reboot  ENDP    

        end
