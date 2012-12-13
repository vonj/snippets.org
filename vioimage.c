/*
 *  VIOIMAGE.C; VidMgr routines for saving and restoring text images.
 *              Release 1.2.
 *
 *  This module written in May 1996 by Andrew Clarke and released to the
 *  public domain.
 */

#include <stdlib.h>
#include "vidmgr.h"
#include "vioimage.h"

void vioImageDefaults(VIOIMAGE * v)
{
    v->width = 0;
    v->height = 0;
    v->image = NULL;
}

int vioImageInit(VIOIMAGE * v, char width, char height)
{
    v->image = malloc(width * height * 2);
    if (v->image)
    {
        v->width = width;
        v->height = height;
        return 1;
    }
    else
    {
        return 0;
    }
}

int vioImageTerm(VIOIMAGE * v)
{
    if (v->image)
    {
        free(v->image);
        return 1;
    }
    else
    {
        return 0;
    }
}

int vioImageSave(VIOIMAGE * v, char x, char y)
{
    if (v->image)
    {
        vm_gettext(x, y, v->width, v->height, v->image);
        return 1;
    }
    else
    {
        return 0;
    }
}

int vioImageRestore(VIOIMAGE * v, char x, char y)
{
    if (v->image)
    {
        vm_puttext(x, y, v->width, v->height, v->image);
        return 1;
    }
    else
    {
        return 0;
    }
}
