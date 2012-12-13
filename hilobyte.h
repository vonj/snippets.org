#ifndef HILOBYTE__H
#define HILOBYTE__H

#define LoByte(x) ((unsigned char)((x) & 0xff))
#define HiByte(x) ((unsigned char)((unsigned short)(x) >> 8))

#endif /* HILOBYTE__H */
