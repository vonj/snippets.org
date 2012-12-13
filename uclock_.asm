        page    55,132
        title   UCLOCK_

;  Requires MASM 5.1 or later or equivalent
;
;  Assemble with:       MASM /Mx /z ...
;                       TASM /jMASM /mx /z ...

%       .MODEL  memodel,C               ;Add model support via command
                                        ;line macros, e.g.
                                        ;MASM /Dmemodel=LARGE,
                                        ;MASM /Dmemodel=SMALL, etc.

        PUBLIC Uclock_, SetMode2_


Uclock_T        STRUC
        count   DW      ?
        lticks  DW      ?
        hticks  DW      ?
Uclock_T        ENDS

        .DATA
        .CODE

;=============================================================================
; This code is excerpted from:
;
; File:         PCTIM002.TXT
; Description:  FAQ / Application Note: Timing on the PC under DOS
; Author:               Kris Heidenstrom (kheidens@actrix.gen.nz)
; Version:              19950816, Release 2
;
; For additional information, ftp:
; oak.oakland.edu//SimTel/msdos/info/pctim*.zip.
;
; Quoting Section 1.3:
;
; This document (including sample code and programs) is Copyright (c)
; 1994-1995 by K. Heidenstrom.
;
; ...
;
; The sample code and sample programs may be freely used in any commercial or
; non-commercial software.
;=============================================================================


;=============================================================================
; Excerpted and adapted from Sample program #16
;
; typedef struct {
;       unsigned int part;
;       unsigned long ticks;
;       } Uclock_T;
;
; void Uclock_(Uclock_T *uclock_now);
;
; Func:   Return absolute timestamp (48-bit) in
;         units of 0.83809534452us since midnight
;         in the current day (range 000000000000h
;         to 001800AFFFFFh) using BIOS tick count
;         variable and CTC channel zero count in
;         progress, assuming CTC channel 0 is
;         operating in mode 2 with a reload value
;         of 0 (65536 divisor).
;
; In:     Pointer to Uclock_T buffer to be filled in
;
; Out:    Nothing
;
; Note:   This routine briefly disables then
;         enables then disables interrupts
;         regardless of the state of the
;         interrupt flag on entry.
;         It restores the original interrupt
;         flag state on exit.
;=============================================================================


Uclock_  PROC USES BX CX DX SI DI ES,clock:ptr Uclock_T
        pushf                   ; Preserve interrupt flag
        xor     ax,ax           ; Zero
        mov     es,ax           ; Address low memory with ES
        cli
        mov     si,es:[46Ch]    ; Loword of tick count
        mov     di,es:[46Eh]    ; Hiword of tick count
        mov     al,0            ; Latch count for CTC channel 0
        out     43h,al          ; Send it
        jmp     SHORT $+2       ; Delay
        in      al,40h          ; Get lobyte of count
        mov     ah,al           ; Save in AH
        jmp     SHORT $+2       ; Delay
        in      al,40h          ; Get hibyte of count
        sti                     ; Make sure interrupts are enabled now
        xchg    al,ah           ; Get bytes the right way round
        nop                     ; Sniff for interrupt
        neg     ax              ; Convert to ascending count
        cli                     ; No interrupts again for reading count
        mov     dx,es:[46Ch]    ; Loword of tick count again
        mov     bx,es:[46Eh]    ; Hiword of tick count again
        popf                    ; Restore original interrupt flag
        cmp     dx,si           ; Did tick count change?
        je      GotIt           ; If not, just return second tick count

        test    ax,ax           ; Is tick count low or high?
        jns     GotIt           ; If low, read was just past interrupt

        mov     dx,si           ; If high, previous tick count is right
        mov     bx,di           ; Get hiword of tick count too

GotIt:
if    @DataSize
        les     di,clock
        mov     word ptr es:[di+count],ax
        mov     word ptr es:[di+lticks],dx
        mov     word ptr es:[di+hticks],bx
else
        mov     di,clock
        mov     word ptr [di+count],ax
        mov     word ptr [di+lticks],dx
        mov     word ptr [di+hticks],bx
endif
        ret

Uclock_ ENDP

; Excerpted and translated from Sample Program #15
;
; void SetMode2_(void);

SetMode2_       PROC USES DX ES BP
        mov     bp,sp
        xor     ax,ax                    ; Wait for tick count to change
        mov     es,ax
        mov     ax,word ptr es:[46Ch]
        mov     word ptr [bp-2],ax
@wait:
        mov     ax,word ptr es:[46Ch]
        cmp     ax,word ptr [bp-2]
        je      short @wait
        pushf                             ; Mask interrupts
        cli
        mov     dx,43h                    ; Set channel 0, mode 2
        mov     al,34h
        out     dx,al
        mov     dx,40h                    ; Low word of divisor (0=>65536)
        mov     al,0
        out     dx,al
        jmp     SHORT $+2
        out     dx,al                     ; High word of divisor (0=>65536)
        popf                              ; Restore interrupt mask
        ret

SetMode2_       endp

        end
