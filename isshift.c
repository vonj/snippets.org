/*-------------------------[ IsShift ]--------------------------*/
/*             Determine whether a shift key is depressed       */
/*                public domain snippet by Jeff Dunlop          */
/*  Revisions:                                                  */
/*  30-Mar-96  Ed Blackman  OS/2 mods, added #defines for keys, */
/*                          wrapped platform dependant actions  */
/*                          in a macro, added some discussion   */
/*--------------------------------------------------------------*/
/* local:                                                       */
/*      kf =                                                    */
/*          DOS: pointer to bios shift key area                 */
/*          OS2: pointer to previously stored key info struct   */
/* return:                                                      */
/*      non-zero if shift key is depressed                      */
/*--------------------------------------------------------------*/

#include "extkword.h"
#include "snipkbio.h"
#include "mk_fp.h"

#ifdef __OS2__
KBDKEYINFO ki;                            /* holds key status */
#endif /* !__OS2__ */

int IsLeftShift(void)
{
      unsigned short FAR* kf = peekkey();

      return (*kf & LEFT_SHIFT);
}

int IsRightShift(void)
{
      unsigned short FAR* kf = peekkey();

      return (*kf & RIGHT_SHIFT);
}

int IsShift(void)
{
      return (IsLeftShift()) || (IsRightShift());
}

int IsLeftAlt(void)
{
      unsigned short FAR* kf = peekkey();

      return (*kf & EITHER_ALT) && (*kf & LEFT_ALT);
}

int IsRightAlt(void)
{
      unsigned short FAR* kf = peekkey();

/* 30-Mar-96 - EBB: In DOS there is no direct way to tell if the right
** Alt key is depressed. DOS sets a status bit if either Alt key was
** pressed, and another if the left Alt key was pressed.  By
** elimination, we can say that the right Alt key was pressed if one of
** the Alt keys was pressed and it wasn't the left one.
*/
    return (*kf & EITHER_ALT) && !(*kf & LEFT_ALT);
}

int IsAlt(void)
{
      return (IsLeftAlt()) || (IsRightAlt());
}

int IsLeftCtl(void)
{
      unsigned short FAR* kf = peekkey();

      return (*kf & EITHER_CTL) && (*kf & LEFT_CTL);
}

int IsRightCtl(void)
{
    unsigned short FAR* kf = peekkey();

/* 30-Mar-96 - EBB: The discussion about the right Alt key in
** IsRightAlt() also applies to the right Ctl key
*/

      return (*kf & EITHER_CTL) && !(*kf & LEFT_CTL);
}

int IsCtl(void)
{
      return (IsLeftCtl()) || (IsRightCtl());
}

int IsSysRq(void)
{
      unsigned short FAR* kf = peekkey();

      return (*kf & SYSRQ);
}
