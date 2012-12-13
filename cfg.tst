; =============================================================
; Sample CS2QWK.cfg file with some usage comments.
;
; This one has been used during testing and reflects my setup.
;
; Each line consists of a keyword, white space plus an optional
; '=' (can be in any position, as shown) and a value.
; A semicolon starts a comment. Blank lines are ignored. In the
; current version unrecognized keywords are also ignored.
; Keywords are not case sensitive.
; For CS2QWK the keyword "WORKPATH" is optional. It defaults to
; the current directory. The rest MUST be present and correct.
; Leading and trailing white space is stripped from the values.
; The values for "Archiver" and "Unarchiver" may contain
; options appended to their values.
; E.g. for arj as unarchiver:   unarchiver arj e

; file names and paths:
WorkPath=   h:\                    ; my ram-disk
Splitfile = splitted               ; no extension!
replyfile  =f:\comm\contrast.rep   ; full path
msg_file    contrast.msg           ; name only

; used executables:
archiver   =c:\comm\ol158\pkzip    ; need THAT version
unarchiver= pkunzip                ; any version in PATH is OK
splitter=   Csplit1                ; need version 1

; ==== CS2QWK.cfg end =========================================
