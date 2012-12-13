; By: Jeffrey Nonken 

        page    60,132
        .286

_text   segment byte public 'code'
_text   ends
_data   segment  word public 'data'
_data   ends
const   segment  word public 'const'
const   ends
_bss    segment  word public 'bss'
_bss    ends
dgroup  group   const,  _bss,   _data
        assume  cs: _text, ds: nothing

_data   segment  word public 'data'
        even
env_8087        dw      47 dup (?)
norm_8087       dw      177fh
_data   ends

_text   segment byte public 'code'
;
; This code saves the 80x87 enviroment and sets up our own. First, this
; assumes you are running an 80287; the 8087 may require more FWAIT
; operations. Second, I decided that I didn't want to handle exceptions, so
; I simply disabled them. That means that if the 80x87 gets an invalid result
; (such as divide-by-zero) the 80x87 will continue to run and will produce
; invalid results until the end of your current calculation. Anything that
; depends on the results will, of course, also be invalid. If you want
; exceptions to be handled, get documentation for the 80x87 and you will
; see how to set norm_8087 (above) to suit your needs. If you are running
; an 8087 and don't know where to put FWAIT instructions, you can always
; add one after each floating-point instruction. NOTE: FWAIT is synonymous
; to WAIT. They are the same instruction.
;
; This was written for TURBO C and will also work with MSC. It should work
; with any programming language with no more than minor changes in the
; label names or the interface. Consult your compiler manual for more detail.
; I wrote this so it would work with either the tiny or small models.
; Actually, it will probably work with any of the models. You should be
; able to assemble this with MASM and link it right in.
;
; extern save_8087();
; extern restore_8087();
;
        public  _save_8087
_save_8087      proc    near
        cli                             ; no interruptions!
        lea     bx,dgroup:env_8087      ; point to save area
        fwait                           ; make sure processor is ready
        fnsave  [bx]                    ; save the 8087 environment
        lea     bx,dgroup:norm_8087     ; point to our new 8087 setup
        mov     ax,[bx]                 ; get it
        fldcw   [bx]                    ; set it
        fwait
        sti                             ; restore interrupts
        ret
_save_8087      endp

        public  _restore_8087
_restore_8087   proc    near
        cli                             ; no interruptions!
        lea     bx,dgroup:env_8087      ; point to saved 8087 stuff
        frstor  [bx]                    ; restore the 8087 environment
        sti                             ; restore interrupts
        ret
_restore_8087   endp

_text   ends

        end
