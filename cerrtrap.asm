        PAGE ,132

;  A sample Interrupt 24 (DOS critical error exception) handler
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

        EXTRN cedevdvr:dword, cetype:word, ceerror:word, cereturn:byte
        EXTRN read_err:far, write_err:far, term_err:far
        EXTRN no_paper:far, fixup_ret:far, FAT_err:far

        ;NOTE:  All the above routines MUST set cereturn to:
        ;       0 - Ignore
        ;       1 - Retry
        ;       2 - Abort
        ;       3 - Fail (DOS 3.3 and later)

        .DATA?

        PUBLIC  osver, rmvbl, exerr, locus, class, suggest
osver   db ?
rmvbl   db ?
exerr   dw ?
locus   db ?
class   db ?
suggest db ?

        .CODE

;
;  This is called by myint24
;
;  extern int (*read_err)(),
;             (*write_err)(),
;             (*term_err)(),
;             (*no_paper)(),
;             (*fixup_ret)(),
;             (*FAT_err)();
;
;  Each returns: 0 - Ignore
;                1 - Retry
;                2 - Abort
;                3 - Fail (DOS 3.3 and later)
;
mynew24 PROC    USES BX
        mov     ah,030h                 ;get DOS version number
        int     21
        or      al,al                   ;zero means DOS 1.x
        jnz     NotDOS1
        mov     al,1
NotDOS1:
        mov     osver,al                ;save DOS version
        mov     ax,cetype               ;get type of exception...
        mov     bx,ax                   ; & save it for later
        and     ax,80h                  ;disk error?
        jnz     NotDiskErr              ;no, continue
        cmp     al,1                    ;yes, DOS 1.x?
        jz      wrong_DOS               ;yes, can't check for removable media
        mov     ah,-1                   ;no, assume removable media
        test    word PTR cedevdvr,0800h ;is the media removable?
        jz      removable
        xor     ah,ah                   ;no, flag fixed media
removable:
        mov     rmvbl,ah                ;save media type
        cmp     al,3                    ;DOS 3.0 or greater?
        jb      wrong_DOS               ;no, skip it
        push    bx                      ;yes, save cetype info...
        push    ds                      ; & other regs
        push    es
        push    dx
        push    si
        push    di
        push    bp
        mov     ah,59h                  ;get extended error info
        int     21
        pop     bp                      ;restore regs
        pop     di
        pop     si
        pop     dx
        pop     es
        pop     ds
        mov     exerr,ax                ;save extended error code...
        mov     locus,ch                ; locus...
        mov     class,bh                ;  class...
        mov     suggest,bl              ;   & suggested action
        pop     bx                      ;restore cetype info
wrong_DOS:
        mov     ax,bx                   ;get exception type
        and     ax,06h                  ;FAT problems?
        cmp     ax,02h
        jnz     ok_fat                  ;no, continue
        jmp     far PTR FAT_err         ;yes, handle it
ok_fat:
        mov     ax,bx                   ;get exception type
        and     ax,01h                  ;handle read and write separately
        jz      rd_err
        jmp     far PTR write_err
rd_err:
        jmp     far PTR read_err
NotDiskErr:
        test    ceerror,0009h           ;printer out of paper?
        jnz     not_eop                 ;no, continue
        jmp     far PTR no_paper        ;yes, handle it
not_eop:
        test    word PTR cedevdvr,8000h ;character device?
        jnz     unknown                 ;no, continue
        jmp     far PTR term_err        ;yes, assume bad terminal I/O

;
;  If we get here, we haven't identified the error. We now call fixup_ret()
;  to resolve which action to take. This will usually involve the information
;  in exerr qualified by the version of DOS in use and is best left to coding
;  in a higher level language like C.
;
;  NOTE: It is IMPERATIVE that the return value of fixup_ret() default to 2
;        to insure that if all else fails, the critcal error handler aborts!
;

unknown:
        call    far PTR fixup_ret       ;unknown error - handle loose ends...
        xor     ah,ah                   ; & return
        mov     cereturn,al
        ret
mynew24 ENDP

        end
