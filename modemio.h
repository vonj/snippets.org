/*                                           */
/* This source is released to the Public     */
/* domain on December 16, 1992.              */
/*                                           */
/* Curtis Paris                              */
/* Internet: cparis@comtch.spk.wa.usa        */
/*                                           */

#ifndef MODEMIO__H
#define MODEMIO__H

#include "pchwio.h"

#define MAX_BUFFER 1024 /* Max Input Buffer */

#define I8088_IMR  0x21
#define XON        0x11
#define XOFF       0x13

#ifdef MODEMIO_INIT

/*** Modem Buffer Information ***/
int           modem_buffer_count=0;
unsigned char modem_buffer[MAX_BUFFER];
int           modem_buffer_head=0,
              modem_buffer_tail=0;

/*** Misc. Modem Status Information ***/
int modem_rts_cts=1;   /* RTS/CTS variable 0=Off, 1=On */
int modem_xon_xoff=1;  /* XON/XOFF variable 0=off, 1=on */
int modem_open=0;      /* IS the port open variable, do not change */
int modem_port=0;      /* What port is it on, 1-5 */
int modem_base=0;      /* The ports BASE address */
int modem_irq =0;      /* The IRQ */
int modem_vect=0;      /* The Interrupt Vector */
int modem_overflow=0;  /* Modem Overflow Alert */
int modem_pause=0;     /* Is it paused for XON/XOFF */

/*** Old Port Interrupt Holders ***/
int old_modem_lcr, old_modem_imr,
    old_modem_ier, old_modem_status;

#else

/*** Modem Buffer Information ***/
extern int           modem_buffer_count;
extern unsigned char modem_buffer[MAX_BUFFER];
extern int           modem_buffer_head,
                     modem_buffer_tail;

/*** Misc. Modem Status Information ***/
extern int modem_rts_cts;
extern int modem_xon_xoff;
extern int modem_open;
extern int modem_port;
extern int modem_base;
extern int modem_vect;
extern int modem_overflow;
extern int modem_pause;

/*** Old Port Interrupt Holders ***/
extern int old_modem_lcr, old_modem_imr,
       old_modem_ier, old_modem_status;

#endif

int com_carrier(void);

int  com_ch_ready(void);
int  com_read_ch(void);
void com_send_ch(unsigned char ch);

void com_parity(char p);
void com_data_bits(unsigned char bits);
void com_stop_bits(unsigned char bits);
void com_speed(long speed);

int com_open(int comport, long speed, int data_bit, unsigned char parity,
      unsigned char stop_bit);
void com_close(void);

#endif /* MODEMIO__H */
