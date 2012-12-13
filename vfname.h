/*
**  Configuration header file for VFNAME.C
*/

/*
**  Rules for MS/PC-DOS
*/

#define FNAME_MAX       12
#define FNAME_SIZE_MAX  8
#define FNAME_SIZE_MIN  1
#define FEXT_SIZE_MAX   3
#define FEXT_SIZE_MIN   0
#define MULTIPLE_EXTS   False_
#define PATH_SEP_CHARS  "\\/"
#define INVALID_CHARS   " ,;:|<>\"+=[]"
#define WILDCARD_CHARS  "*?"
#define DEVICE_LIST_3   {"AUX","CON","PRN","NUL",\
                        "aux","con","prn","nul",NULL}
#define DEVICE_LIST_4   {"LPT1","LPT2","LPT3","COM1","COM2","COM3","COM4",\
                        "lpt1","lpt2","lpt3","com1","com2","com3","com4",NULL}
#define DEVICE_LIST_6   {"CLOCK$","clock$",NULL}

#if 0
 /*
 **  Rules for Posix
 */

 #define FNAME_MAX      _PC_NAME_MAX
 #define FNAME_SIZE_MAX _PC_NAME_MAX
 #define FNAME_SIZE_MIN 0
 #define FEXT_SIZE_MAX  _PC_NAME_MAX
 #define FEXT_SIZE_MIN  0
 #define MULTIPLE_EXTS  True_
 #define PATH_SEP_CHARS "/"
 #define INVALID_CHARS  " ,;:|<>\"\'+=(){}!@#%&!~`"
 #define WILDCARD_CHARS "*?[]^$"
 #define DEVICE_LIST_3  {"nul",NULL}
 #define DEVICE_LIST_4  {"",NULL}
 #define DEVICE_LIST_6  {"",NULL}
#endif
