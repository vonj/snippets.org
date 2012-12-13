/*
** This source is released to the Public
** domain on December 16, 1992.
**
** Curtis Paris
** Internet: cparis@comtch.spk.wa.usa
**
** modified 12-Mar-94 by Bob Stout - Added pc-port.h from SNIPPETS
*/

#include <stdio.h>
#include <dos.h>
#include <ctype.h>
#include "extkword.h"

#define MODEMIO_INIT
#include "modemio.h"

void (INTERRUPT FAR *old_modem_isr)(void);

/*********************************************************************/
void FAR interrupt modem_isr(void)
{
  unsigned char c;

  enable();

  if (modem_buffer_count<1024) {
    c=inp(modem_base);
    if (((c==XON) || (c==XOFF)) && (modem_xon_xoff)) {
      switch(c) {
        case XON :modem_pause=0; break;
        case XOFF:modem_pause=1; break;
      }
    } else {
      modem_pause=0;
      modem_buffer[modem_buffer_head++]=c;
      if (modem_buffer_head>=MAX_BUFFER) modem_buffer_head=0;
      modem_buffer_count++;
    }
    modem_overflow=0;
  } else {
    modem_overflow=1;
  }
  disable();
  outp(0x20,0x20);
}

int com_carrier(void)
{
  int x;

  if (!modem_open) return(0);
  if ((inp(modem_base+6) & 0x80)==128) return(1);

  for (x=0; x<500; x++) {
    if ((inp(modem_base+6) & 0x80)==128) return(1);
  }
  return(0);
}

int com_ch_ready(void)
{
  if (!modem_open) return(0);
  if (modem_buffer_count!=0) return(1);
  return(0);
}

int com_read_ch(void)
/* This returns EOF if the port hasn't been opened, or if no character
 * is waiting.  Otherwise it returns the next character from the port.
 */
{
  unsigned char ch;

  if (!modem_open) return(EOF);
  if (!com_ch_ready()) return(EOF);
  ch=modem_buffer[modem_buffer_tail];
  modem_buffer[modem_buffer_tail]=0;
  modem_buffer_count--;
  if (++modem_buffer_tail>=MAX_BUFFER) modem_buffer_tail=0;
  return(ch);
}

void com_send_ch(unsigned char ch)
{
  if (!modem_open) return;
  outp(modem_base+4,0x0B);
  if (modem_rts_cts) {
    while((inp(modem_base+6) & 0x10)!=0x10) ; /* Wait for Clear to Send */
  }
  while((inp(modem_base+5) & 0x20)!=0x20) ;
  if (modem_xon_xoff) {
    while((modem_pause) && (com_carrier())) ;
  }
  outp(modem_base,ch);
}


void com_parity(char p)
{
  int x, newb=0;

  if (!modem_open) return;
  x=inp(modem_base+3);

  newb=((x>>6)<<6)+((x<<5)>>5); /* Get rid of old parity */

  switch(toupper(p)) {
    case 'N':newb+=0x00; break; /* None  */
    case 'O':newb+=0x08;break;  /* Odd   */
    case 'E':newb+=0x18; break; /* Even  */
    case 'M':newb+=0x28;break;  /* Mark  */
    case 'S':newb+=0x38;break;  /* Space */
  }

  outp(modem_base+3, newb);
}

void com_data_bits(unsigned char bits)
{
  int x, newb=0;

  if (!modem_open) return;
  x=inp(modem_base+3);

    newb=((x>>2)<<2); /* Get rid of the old Data Bits */

  switch(bits) {
    case 5 :newb+=0x00; break;
    case 6 :newb+=0x01; break;
    case 7 :newb+=0x02; break;
    default:newb+=0x03; break;
  }

  outp(modem_base+3,newb);
}

void com_stop_bits(unsigned char bits)
{
  int x, newb=0;

  if (!modem_open) return;
  x=inp(modem_base+3);

  newb=((x<<6)>>6)+((x>>5)<<5); /* Kill the old Stop Bits */

  if (bits==2) newb+=0x04; /* Only check for 2, assume 1 otherwise */

  outp(modem_base+3,newb);
}

void com_speed(long speed)
{
  int x;
    char l, m;
    int d;

  if (!modem_open) return;

  x=inp(modem_base+3); /* Read In Old Stats */

  if ((x & 0x80)!=0x80) outp(modem_base+3,x+0x80); /* Set DLab On */

  d=(int)(115200L/speed);
  l=d & 0xFF;
  m=(d >> 8) & 0xFF;

  outp(modem_base+0,l);
  outp(modem_base+1,m);

  outp(modem_base+3,x); /* Restore the DLAB bit */
}

int com_open(int comport, long speed, int data_bit, unsigned char parity,
      unsigned char stop_bit)
{
  int x;

  disable();
  if (modem_open) com_close();
  modem_port=comport;

  switch(modem_port) {
    case 1:modem_base=0x3F8; modem_irq=4; modem_vect=0x0C; break;
    case 2:modem_base=0x2F8; modem_irq=3; modem_vect=0x0B; break;
    case 3:modem_base=0x3E8; modem_irq=4; modem_vect=0x0B; break;
    case 4:modem_base=0x2E8; modem_irq=3; modem_vect=0x0C; break;
    case 5:break;
    default:modem_base=0x3F8; modem_irq=4; modem_vect=0x0C; break;
  }

  outp(modem_base+1,0x00);
  if (inp(modem_base+1)!=0) {
    enable();
    return(0);
  }

  /* Set up the Interrupt Info */
  old_modem_ier=inp(modem_base+1);
  outp(modem_base+1,0x01);

  old_modem_isr=(void (INTERRUPT FAR *)(void))getvect(modem_vect);
  setvect(modem_vect,modem_isr);

  if (modem_rts_cts) {
    outp(modem_base+4,0x0B);
  } else {
    outp(modem_base+4,0x09);
  }

  old_modem_imr=inp(I8088_IMR);
  outp(I8088_IMR,old_modem_imr & ((1 << modem_irq) ^ 0x00FF));

  for (x=1; x<=5; x++) inp(modem_base+x);

  modem_open=1;

  modem_buffer_count=0;
  modem_buffer_head=0;
  modem_buffer_tail=0;

  com_speed(speed);
  com_data_bits((unsigned char)data_bit);
  com_parity(parity);
  com_stop_bits(stop_bit);
  enable();
  return(1);
}

void com_close(void)
{
  if (!modem_open) return;

  outp(modem_base+1,old_modem_ier);
  outp(I8088_IMR, old_modem_imr);

  setvect(modem_vect, old_modem_isr);
  outp(0x20,0x20);
  modem_open=0;
}
