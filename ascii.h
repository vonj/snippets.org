/*
**  Header for conveniently dealing with ASCII control characters
*/

#undef NUL
#undef SOH
#undef STX
#undef ETX
#undef EOT
#undef ENQ
#undef ACK
#undef BEL
#undef BS_
#undef HT_
#undef LF_
#undef VT_
#undef FF_
#undef CR_
#undef SO_
#undef SI_
#undef DLE
#undef DC1
#undef DC2
#undef DC3
#undef DC4
#undef NAK
#undef SYN
#undef ETB
#undef CAN
#undef EM_
#undef SUB
#undef ESC
#undef FS_
#undef GS_
#undef RS_
#undef US_
#undef DEL

enum Cchar_T { NUL, SOH, STX, ETX, EOT, ENQ, ACK, BEL,
               BS_, HT_, LF_, VT_, FF_, CR_, SO_, SI_,
               DLE, DC1, DC2, DC3, DC4, NAK, SYN, ETB,
               CAN, EM_, SUB, ESC, FS_, GS_, RS_, US_,
               DEL = '\x7f' };
