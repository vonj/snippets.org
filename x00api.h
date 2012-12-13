/*
 *  X00API.H - X00 FOSSIL driver HLLAPI
 *
 *   Created by R.F. Pels.
 *   modified by Bob Stout
 *   Placed in the public domain by R.F. Pels.
 */

#ifndef __X00API_H                        /*  Prevent double inclusion  */
#define __X00API_H

/*  FOSSIL specifics */

#define X00_VECTOR          0x14
#define X00_IDOFFSET        6

#if defined(__TURBOC__) || defined(__POWERC)
 #define _far far
#endif

/*  FOSSIL function request codes */

#define SET_BAUDRATE        0x0000
#define TX_CHAR             0x0100
#define RX_CHAR             0x0200
#define STATUS              0x0300
#define INITIALIZE          0x0400
#define DEINITIALIZE        0x0500
#define RAISE_DTR           0x0601
#define LOWER_DTR           0x0600
#define GET_SYS_INFO        0x0700
#define FLUSH_OUTPUT        0x0800
#define PURGE_OUTPUT        0x0900
#define PURGE_INPUT         0x0A00
#define TX_CHAR_NOWAIT      0x0B00
#define PEEK_AHEAD_INPUT    0x0C00
#define PEEK_AHEAD_KBD      0x0D00
#define READ_KBD            0x0E00
#define FLOW_CONTROL        0x0F00
#define CTLC_CTLK_CHECK     0x1000
#define SET_CUP             0x1100
#define GET_CUP             0x1200
#define WRITE_ANSI_CHAR     0x1300
#define ENABLE_WATCHDOG     0x1401
#define DISABLE_WATCHDOG    0x1400
#define WRITE_BIOS_CHAR     0x1500
#define INSERT_TICK_FUNC    0x1601
#define DELETE_TICK_FUNC    0x1600
#define BOOT_MACHINE        0x1700
#define READ_BLOCK          0x1800
#define WRITE_BLOCK         0x1900
#define START_BREAK_SIGNAL  0x1A01
#define STOP_BREAK_SIGNAL   0x1A00
#define GET_DRIVER_INFO     0x1B00
#define INSTALL_APPENDAGE   0x7e00
#define REMOVE_APPENDAGE    0x7f00

/* port numbers and variable type of portnumber in calls */

#define PORTCOM1            0
#define PORTCOM2            1
#define PORTCOM3            2
#define PORTCOM4            3
#define PORTCOM5            4
#define PORTCOM6            5
#define PORTCOM7            6
#define PORTCOM8            7
#define PORTSPECIAL         0x00ff

typedef unsigned int PORT;

/* defines components of baud settings call */

#define BAUD300             0x40
#define BAUD600             0x60
#define BAUD1200            0x80
#define BAUD2400            0xa0
#define BAUD4800            0xc0
#define BAUD9600            0xe0
#define BAUD19200           0x00
#define BAUD38400           0x20
#define PARITYNONE          0x00
#define PARITYODD           0x08
#define PARITYNONEALT       0x10
#define PARITYEVEN          0x18
#define STOP1BIT            0x00
#define STOP2BIT            0x04
#define WORD5BIT            0x00
#define WORD6BIT            0x01
#define WORD7BIT            0x02
#define WORD8BIT            0x03
#define FIDOSETTING         PARITYNONE | STOP1BIT | WORD8BIT
#define OPUSSETTING         PARITYNONE | STOP1BIT | WORD8BIT
#define SEADOGSETTING       PARITYNONE | STOP1BIT | WORD8BIT

/* Error numbers */

#define X00_OK              0
#define X00_NOT_HERE        100
#define X00_CHAR_NOT_SENT   101
#define X00_NO_INPUT        0xffff
#define X00_NO_KEY          0xffff
#define X00_INS_TICK        104
#define X00_DEL_TICK        105
#define X00_INS_APP         106
#define X00_REM_APP         107
#define X00_DTR_HIGH        108
#define X00_DTR_LOW         109


/* FOSSIL initcall result type */

#define X00_PRESENT         0x1954

typedef struct {
        unsigned int result;
        unsigned char max_function;
        unsigned char revision;
        } FOSSILINIT;

/* FOSSIL status return type: all fields are 1 if condition exists */

typedef union {
        struct STATUSBITS {
            unsigned                   : 3;
            unsigned alwayshigh        : 1;
            unsigned                   : 3;
            unsigned carrier_detect    : 1;
            unsigned chars_in_input    : 1;
            unsigned input_overrun     : 1;
            unsigned                   : 3;
            unsigned output_not_full   : 1;
            unsigned output_empty      : 1;
            unsigned                   : 1;
            } statusbits;
      unsigned int statusword;
      } FOSSILSTATUS;

/* FOSSIL info type */

typedef struct {
        unsigned int  size;
        unsigned char version;
        unsigned char revision;
        unsigned int  ofs_fossil_id;
        unsigned int  seg_fossil_id;
        unsigned int  input_size;
        unsigned int  input_avail;
        unsigned int  output_size;
        unsigned int  output_avail;
        unsigned char screen_width;
        unsigned char screen_height;
        unsigned char baud_rate_mask;
        } FOSSILINFO;

/* FOSSIL system info type */

typedef struct {
        unsigned char tick_number;
        unsigned char ticks_per_second;
        unsigned int  approx_ms_per_tick;
        } FOSSILSYSINFO;

/* FOSSIL flow control type */

typedef union {
        struct FLOWBITS {
            unsigned  xonxoff         : 1;
            unsigned  ctsrts          : 1;
            unsigned  remote_xonxoff  : 1;
            unsigned                  : 5;
            } flowbits;
      unsigned char flowword;
      } FOSSILFLOWCTRL;

/* FOSSIL checks control type */

#define X00_CTLCK           0x0001
#define X00_NO_CTLCK        0x0000

typedef union {
        struct CHECKBITS {
            unsigned  ctlc_ctlk         : 1;
            unsigned  stop_transmitter  : 1;
            unsigned                    : 6;
            } checkbits;
      unsigned char checkword;
      } FOSSILCTLCCTLK;

/* Macros to use with x00_boot_machine() */

#define COLDBOOT        0
#define WARMBOOT        1

/* Macros to use with x00_read_kbd() */

#define ISFNKEY(x)      (((x) & 0x00FF) == 0)
#define FNKEYCODE(x)    ((x) >> 8)

#if defined(__cplusplus) && __cplusplus
 extern "C" {
#endif

unsigned int  x00_detect(void);
FOSSILSTATUS  x00_set(unsigned char set, PORT port);
FOSSILSTATUS  x00_tx_char(unsigned char c, PORT port);
unsigned char x00_rx_char(PORT port);
FOSSILSTATUS  x00_status(PORT port);
FOSSILINIT    x00_init(PORT port, unsigned char _far *ctlc_flagbyte);
void          x00_deinit(PORT port);
unsigned int  x00_raise_dtr(PORT port);
unsigned int  x00_lower_dtr(PORT port);
FOSSILSYSINFO x00_sysinfo(void);
void          x00_flush_output(PORT port);
void          x00_purge_output(PORT port);
void          x00_purge_input(PORT port);
unsigned int  x00_tx_char_nowait(unsigned char c, PORT port);
unsigned int  x00_peek_ahead_input(PORT port);
unsigned int  x00_peek_ahead_kbd(void);
unsigned int  x00_read_kbd(void);
void          x00_flow_control(FOSSILFLOWCTRL f, PORT port);
unsigned int  x00_ctlc_ctlk_check(FOSSILCTLCCTLK c, PORT port);
void          x00_set_cup(unsigned char row, unsigned char col);
void          x00_get_cup(unsigned char *row, unsigned char *col);
void          x00_write_ANSI_char(unsigned char c);
void          x00_enable_watchdog(PORT port);
void          x00_disable_watchdog(PORT port);
void          x00_write_BIOS_char(unsigned char c);
unsigned int  x00_insert_tick_func(void (_far *tickfunc)());
unsigned int  x00_delete_tick_func(void (_far *tickfunc)());
void          x00_boot_machine(unsigned int boottype);
unsigned int  x00_read_block(unsigned int count, void _far *buf, PORT port);
unsigned int  x00_write_block(unsigned int count, void _far *buf,
PORT port);
void          x00_start_break_signal(PORT port);
void          x00_stop_break_signal(PORT port);
unsigned int  x00_get_driverinfo(void _far *buf, PORT port);
unsigned int  x00_install_appendage(unsigned char appcode,
                  void (_far *appfunc)());
unsigned int  x00_remove_appendage(unsigned char appcode,
                  void (_far *appfunc)());

#if defined(__cplusplus) && __cplusplus
 }
#endif

#endif /* __X00API_H */
