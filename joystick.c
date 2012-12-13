/*
**  JOYSTICK.C
**
**  Joystick support functions
**
**  Public domain demo by Bob Stout
*/

#include <dos.h>
#include "snpdosys.h"

struct joystick JoyStick;

/*
**  read_joystick()
**
**  returns Success_ or Error_
**
**  fills in global JoyStick structure
*/

Boolean_T read_joystick(void)
{
        union REGS regs;

        regs.h.ah = 0x84;                       /* Read the switches    */
        regs.x.dx = 0;
        int86(0x15, &regs, &regs);
        if (regs.x.cflag)
                return Error_;
        JoyStick.switch_0 = TOBOOL(regs.h.al & 0x10);
        JoyStick.switch_1 = TOBOOL(regs.h.al & 0x20);
        JoyStick.switch_2 = TOBOOL(regs.h.al & 0x40);
        JoyStick.switch_3 = TOBOOL(regs.h.al & 0x80);

        regs.h.ah = 0x84;                       /* Read positions       */
        regs.x.dx = 1;
        int86(0x15, &regs, &regs);
        if (regs.x.cflag)
                return Error_;
        JoyStick.pos_Ax = regs.x.ax;
        JoyStick.pos_Ay = regs.x.bx;
        JoyStick.pos_Bx = regs.x.cx;
        JoyStick.pos_By = regs.x.dx;

        return Success_;
}
