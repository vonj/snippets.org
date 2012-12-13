; cpuid.asm
;
; This module contains a C callable routine which returns a 16-bit
; integer (in AX) which indicates the type of CPU on which the
; program is running.  The low eight bits (AL) contain a number
; corresponding to the family number (e.g. 0 = 8086, 1 = 80186,
; 2 = 80286, etc.).  The high eight bits (AH) contain a collection
; of bit flags which are defined below.
;
;       written on Thu  10-31-1996  by Edward J. Beroset
;               and released to the public domain by the author
;
%       .MODEL  memodel,C               ;Add model support via command
                                        ;line macros, e.g.
                                        ;MASM /Dmemodel=LARGE,
                                        ;TASM /Dmemodel=SMALL, etc.

        .8086
        PUBLIC cpu_id

;
; using MASM 6.11       Ml /c /Fl CPUID.ASM
;
; using TASM 4.00       TASM CPUID.ASM
;
; using older assemblers, you may have to use the following equate
; and eliminate the .586 directive
;
;CPUID equ "dw 0a20fh"
;
; bit flags for high eight bits of return value
;
HAS_NPU         equ     01h
IS386_287       equ     02h
IS386SX         equ     04h
CYRIX           equ     08h
NEC             equ     10h
NEXGEN          equ     20h
AMD             equ     40h
UMC             equ     80h

        .code

cpu_id  proc
        push    bx
        push    cx
        push    dx
        push    bp
        mov     bp,sp
        xor     dx,dx                   ; result = 0 (UNKNOWN)
;**********************************************************************
; The Cyrix test
;
;   Cyrix processors do not alter the AF (Aux carry) bit when
;   executing an XOR.  Intel CPUs (and, I think, all the others)
;   clear the AF flag while executing an XOR AL,AL.
;
;**********************************************************************
TestCyrix:
        mov     al,0fh                  ;
        aas                             ; set AF flag
        xor     al,al                   ; only Cyrix leaves AF set
        aas                             ;
        jnc     Test8086                ;
        or      dh,CYRIX                ; it's at least an 80386 clone
        jmp     Test486                 ;
;**********************************************************************
;
; The 80186 or under test
;
;   On <80286 CPUs, the SP register was decremented *before* being
;   pushed onto the stack.  All later CPUs do it correctly.
; 
;**********************************************************************
Test8086:
        push    sp                      ; Q: is it an 8086, 80188, or
        pop     ax                      ;
        cmp     ax,bp                   ;
        je      Test286                 ;   N: it's at least a 286
;**********************************************************************
; The V20/V30 test
;
;   NEC's CPUs set the state of ZF (the Zero flag) correctly after
;   a MUL.  Intel's CPUs do not -- officially the state of ZF is
;   "undefined" after a MUL or IMUL.
;
;**********************************************************************
TestV20:
        xor     al,al                   ; clear the zero flag
        mov     al,1                    ;
        mul     al                      ;
        jnz     Test186                 ;
        or      dh,NEC                  ; it's a V20 or a V30
;**********************************************************************
; The 80186 test
;
;   On the 80186, shifts only use the five least significant bits,
;   while the 8086 uses all 8, so a request to shift 32 bits will
;   be requested as a shift of zero bits on the 80186.
;
;**********************************************************************
Test186:
        mov     al,01h                  ;
        mov     cl,32                   ; shift right by 33 bits
        shr     al,cl                   ;
        mov     dl,al                   ; al = 0 for 86, al = 1 for 186
longTestNpu:
        jmp     TestNpu                 ;

;**********************************************************************
; The 286 test
;   Bits 12-15 (the top four) of the flags register are all set to
;   0's on a 286 and can't be set to 1's.
;
;**********************************************************************
Test286:
        .286
        mov     dl,2                    ; it's at least a 286
        pushf                           ; save the flags
        pop     ax                      ; fetch 'em into AX
        or      ah,0f0h                 ; try setting those high bits
        push    ax                      ;
        popf                            ; run it through the flags reg
        pushf                           ;
        pop     ax                      ; now check the results
        and     ah,0F0h                 ; Q: are bits clear?
        jz      longTestNpu             ;   Y: it's a 286

;**********************************************************************
; The 386 test
;
;   The AC (Alignment Check) bit was introduced on the 486.  This
;   bit can't be toggled on the 386.
;
;**********************************************************************
Test386:
        .386
        mov     dl,3                    ; it's at least a 386
        pushfd                          ; assure enough stack space
        cli
        and     sp, NOT 3               ; align stack to avoid AC fault
        pushfd                          ;
        pop     cx                      ;
        pop     ax                      ;
        mov     bx,ax                   ; save a copy
        xor     al,4                    ; flip AC bit
        push    ax                      ;
        push    cx                      ;
        popfd                           ;
        pushfd                          ;
        pop     cx                      ;
        pop     ax                      ;
        and     al,4                    ;
        sti
        xor     al,bl                   ; Q: did AC bit change?
        jnz     Test486                 ;   N: it's a 386
        .386P
;**********************************************************************
; The 386SX test
;
;   On the 386SX, the ET (Extension Type) bit of CR0 is permanently
;   set to 1 and can't be toggled.  On the 386DX this bit can be
;   cleared.
;**********************************************************************
        mov     eax,cr0
        mov     bl,al                   ; save correct value
        and     al,not 10h              ; try clearing ET bit
        mov     cr0,eax                 ;
        mov     eax,cr0                 ; read back ET bit
        xchg    bl,al                   ; patch in the correct value
        mov     cr0,eax                 ;
        test    bl,10h                  ; Q: was bit cleared?
        jz      TestNpu                 ;  Y: it's a DX
        or      dh,IS386SX              ;  N: it's probably an SX

;**********************************************************************
; The 486 test
;
;   Try toggling the ID bit in EFLAGS.  If the flag can't be toggled,
;   it's a 486.
;
; Note:
;   This one isn't completely reliable -- I've heard that the NexGen
;   CPU's don't make it through this one even though they have all
;   the Pentium instructions.
;**********************************************************************
Test486:
        .486
        pushfd
        pop     cx
        pop     bx
        mov     dl,4                    ;
        mov     ax,bx                   ;
        xor     al,20h                  ; flip EFLAGS ID bit
        push    ax                      ;
        push    cx                      ;
        popfd                           ;
        pushfd                          ;
        pop     cx                      ;
        pop     ax                      ;
        and     al,20h                  ; check ID bit
        xor     al,bl                   ; Q: did ID bit change?
        jz      TestNpu                 ;   N: it's a 486

;**********************************************************************
; The Pentium+ tests
;
;   First, we issue a CPUID instruction with EAX=0 to get back the
;   manufacturer's name string.  (We only check the first letter.)
;
;**********************************************************************
PentPlus:
        .586
        push    dx                      ;
        xor     eax,eax                 ;
        cpuid                           ;
        pop     dx                      ;
        cmp     bl,'G'                  ; Q: GenuineIntel?
        jz      WhatPent                ;   Y: what kind?
        or      dh,CYRIX                ; assume Cyrix for now
        cmp     bl,'C'                  ;
        jz      WhatPent                ;
        xor     dh,(CYRIX OR AMD)       ;
        cmp     bl,'A'                  ;
        jz      WhatPent                ;
        xor     dh,(AMD OR NEXGEN)      ;
        cmp     bl,'N'                  ;
        jz      WhatPent                ;
        xor     dh,(NEXGEN OR UMC)      ; assume it's UMC
        cmp     bl,'U'                  ;
        jz      WhatPent                ;
        xor     dh,UMC                  ; we don't know who made it!
;**********************************************************************
; The Pentium+ tests (part II)
;
;   This test simply gets the family information via the CPUID
;   instruction
;
;**********************************************************************
WhatPent:
        push    edx                     ;
        xor     eax,eax                 ;
        inc     al                      ;
        cpuid                           ;
        pop     edx                     ;
        and     ah,0fh                  ;
        mov     dl,ah                   ; put family code in DL

;**********************************************************************
; The NPU test
;
;   We reset the NPU (using the non-wait versions of the instruction, of
;   course!), put a non-zero value on the stack, then write the NPU
;   status word to that stack location.  Then we check for zero, which
;   is what would be there if there were an NPU.
;
;**********************************************************************
TestNpu:
        .8087
        .8086
        mov     sp,bp                   ; restore stack
        fninit                          ; init but don't wait
        mov     ax,0EdEdh               ;
        push    ax                      ; put non-zero value on stack
        fnstsw  word ptr [bp-2]         ; save NPU status word
        pop     ax                      ;
        or      ax,ax                   ; Q: was status = 0?
        jnz     finish                  ;   N: no NPu
        or      dh,HAS_NPU              ;   Y: has NPU

;**********************************************************************
; The 386/287 combo test
;
;   Since the 386 can be paired with either a 387 or 287, we check to
;   see if the NPU believes that +infinity equals -infinity.  The 387
;   says they're equal, while the 287 doesn't.
;
;**********************************************************************
        cmp     dl,3                    ; Q: is CPU a 386?
        jnz     finish                  ;  N: no need to check infinities
        fld1                            ; load 1
        fldz                            ; load 0
        fdiv                            ; calculate infinity! (1/0)
        fld     st                      ; duplicate it
        fchs                            ; change signs of top inf
        fcompp                          ; identical?
        push    ax                      ;
        fstsw   word ptr [bp-2]         ;
        pop     ax                      ;
        test    ah,40h                  ; Q: does NPU say they're equal?
        jz      finish                  ;  N: it's a 387
        or      dh,IS386_287            ;
finish:
        mov     ax,dx                   ; put our return value in place
        pop     bp                      ; clean up stack
        pop     dx                      ;
        pop     cx                      ;
        pop     bx                      ;

        ret                             ;
cpu_id  endp

        END
