        page    ,132
        title   STR_ECPY
COMMENT $
        Author: Leslie Satenstein
        A form of strcpy() where the result returned is the
        nul terminating character of the first argument. In many cases
        this function is more efficient than the equivalent
        strcpy, followed by strcat.
 
        The assembler code does a strlen followed by a memcpy,
            and makes use of the special string move instructions
            intrinsic to the intel 8088, 80186, 80286, '386 and '486s.
 
        Use as: str_ecpy(str_ecpy(target,"first part"),"Second part");
        in place of
                strcat(strcpy(target,"first part"),"Second part");
 
        One of the ways the C code appears is:
        char *_strecpy(char *target,const char *src)
        {
           return target+strlen(strcpy(target,src));
        }
        Another way is to do your own looping:
 
        char *str_ecpy(char *target,const char *src)
        {
          char *cpt,*cps;        /* many compilers optimise better
                                  * when local pointers are declared
                                  * (locals can be assigned registers)
                                  */
          cpt=target-1;
          cps=src-1;
          do
          {
            *++cpt = *++src;     /* copy first, ask questions later */
          } while(*cpt!='\0');
          return cpt;
        }

        $
 
;  Requires MASM 5.1 or later or equivalent
;
;  Assemble with:       MASM /Mx /z ...
;                       TASM /jMASM /mx /z ...

%       .MODEL  memodel,C               ;Add model support via command
                                        ;line macros, e.g.
                                        ;MASM /Dmemodel=LARGE,
                                        ;MASM /Dmemodel=SMALL, etc.
        PUBLIC str_ecpy
 
        .DATA
        .CODE
if    @DataSize
str_ecpy  proc uses si di ds,target:FAR ptr byte,src:FAR ptr byte
        les     di,src          ; load es:di with src
        mov     si,di           ; put copy to bx:si
        mov     bx,es
else
str_ecpy  proc uses si di,target:NEAR ptr byte,src:NEAR ptr byte
        mov     di,ds           ; make es same as ds
        mov     es,di
        mov     di,src
        mov     si,di           ; put copy to es:si
endif
        xor     ax,ax           ; scan for the nul at end of string
        mov     cx,-1
        repne   scasb
        not     cx              ; cx =    strlen(src)+1 ;
if    @Datasize
        les     di,target       ; this is where copy is to begin
        mov     dx,es           ; dx has segment, di has offset to target
        mov     ds,bx           ; ds:si have pointer to src
else
        mov     di,target       ; this is where copy is to begin
endif
        test    di,1            ; if we are on odd address, do one byte move
        jz      on_even
        movsb                   ; now, prepare to move words at a
        dec     cx              ;time to target
on_even:
        shr     cx,1            ; carry flag has remainder after divide by 2
        rep     movsw           ; move this many words
        jnc     fini            ; if no carry, we are finished
        movsb
fini:
        xchg    di,ax           ; set up return value, dx has seg value
        dec     ax              ; backup to the nul character
        ret

str_ecpy endp

        end
