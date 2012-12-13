/*
 *  QuickMenu VidMgr demonstration application.
 *
 *  Written in June 1996 by Andrew Clarke and released to the public domain.
 *
 *  Currently assumes 80x25 video output, but may be rewritten to
 *  recognise other video dimensions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vidmgr.h"
#include "vioimage.h"

#define maxx  vm_getscreenwidth()
#define maxy  vm_getscreenheight()

#define PROG     "QuickMenu"
#define VERSION  "1.2"

#define MAXITEMS 12

typedef struct
{
    char desc[40];
    char cmd[250];
}
MENUDATA;

static VIOIMAGE sysimage;
static MENUDATA menudata[MAXITEMS];
static int menuitems, curritem, olditem;

int qmenuLoadMenu(char *fnm)
{
    FILE *fp;
    int item, line;

    fp = fopen(fnm, "r");
    if (fp == NULL)
    {
    	perror(fnm);
    	return 0;
    }

    item = 0;
    line = 1;
    while (item < MAXITEMS && !feof(fp))
    {
    	char *p;
    	
    	if (fgets(menudata[item].desc, 40, fp) == NULL)
    	{
    	    break;
    	}

    	p = strrchr(menudata[item].desc, '\n');
    	if (p != NULL)
    	{
    	    *p = '\0';
    	}

    	if (*menudata[item].desc == '\0')
    	{
            printf("%s(%d): Error! Expected description but found ''\n", fnm, line);
            return 0;
        }

    	line++;
    	if (fgets(menudata[item].cmd, 250, fp) == NULL)
        {
            printf("%s(%d): Error! Expected command but reached end of file\n", fnm, line);
            return 0;
        }

    	p = strrchr(menudata[item].cmd, '\n');
    	if (p != NULL)
    	{
    	    *p = '\0';
    	}
        
    	if (*menudata[item].cmd == '\0')
    	{
            printf("%s(%d): Error! Expected command but found ''\n", fnm, line);
            return 0;
        }

        line++;
        item++;
    }

    fclose(fp);

    menuitems = item;

    return 1;
}

void qmenuRun(void)
{
    int done, ch;
    FILE *ofp;
    char qmtmpfnm[12];
    
#if defined(OS2)
    strcpy(qmtmpfnm, "$qmtemp.cmd");
#elif defined(EMX)
    if (_osmode == DOS_MODE)
    {
        strcpy(qmtmpfnm, "$qmtemp.bat");
    }
    else
    {
        strcpy(qmtmpfnm, "$qmtemp.cmd");
    }
#else
    strcpy(qmtmpfnm, "$qmtemp.bat");
#endif

    vm_attrib(24, (char) (7 + curritem), 56, (char) (7 + curritem), vm_mkcolor(WHITE, BLACK));

    done = 0;
    while (!done)
    {
    	if (curritem != olditem)
    	{
            vm_attrib(24, (char) (7 + olditem), 56, (char) (7 + olditem), vm_mkcolor(BLACK, LIGHTGRAY));
            vm_attrib(24, (char) (7 + curritem), 56, (char) (7 + curritem), vm_mkcolor(WHITE, BLACK));
        }

        while (!vm_kbhit())
        {
            /* nada */
        }

        ch = vm_getch();
        switch (ch)
        {
        case 0x3b00:    /* F1 */
            /* nada */
            break;

        case 0x4800:    /* up arrow */
            olditem = curritem;
            if (curritem != 0)
            {
                curritem--;
            }
            break;

        case 0x5000:    /* down arrow */
            olditem = curritem;
            if (curritem != menuitems - 1)
            {
                curritem++;
            }
            break;

        case 0x4700:    /* Home */
            olditem = curritem;
            curritem = 0;
            break;

        case 0x4f00:    /* End */
            olditem = curritem;
            curritem = menuitems - 1;
            break;

        case 0x001b:    /* Escape */
        case 0x3d00:    /* F3 */
        case 0x4400:    /* F10 */
        case 0x6b00:    /* Alt+F4 */
        case 0x2d00:    /* Alt+X */
            remove(qmtmpfnm);
            done = 1;
            break;

        case 0x000d:    /* Enter */
            ofp = fopen(qmtmpfnm, "w");
            fputs("@echo off\n", ofp);
            fputs(menudata[curritem].cmd, ofp);
            fputs("\n", ofp);
            fputs("qm\n", ofp);
            fclose(ofp);
            done = 1;
            break;
            
        default:
            break;
        }
    }
}

void qmenuTerm(void)
{
    vioImageRestore(&sysimage, 1, 1);
    vioImageTerm(&sysimage);
    vm_gotoxy(vm_startup.xpos, vm_startup.ypos);
    vm_done();
}

void qmenuInit(void)
{
    int item;

    printf("\n" PROG " " VERSION "; Written in June 1996 by Andrew Clarke.\n");
    printf("Released to the public domain.\n");

    vm_init();
    vioImageDefaults(&sysimage);
    vioImageInit(&sysimage, maxx, maxy);
    vioImageSave(&sysimage, 1, 1);

    if (qmenuLoadMenu("qmenu.mnu") != 1)
    {
        vm_done();
    	exit(EXIT_FAILURE);
    }

    vm_setcursorstyle(CURSORHIDE);

    vm_attrib(22, 6, 62, 21, vm_mkcolor(LIGHTGRAY, BLACK));
    vm_paintclearbox(20, 5, 60, 20, vm_mkcolor(BLACK, LIGHTGRAY));
    vm_frame(23, 6, 57, 19, vm_mkcolor(BLACK, LIGHTGRAY), vm_frame_double);

    for (item = 0; item < menuitems; item++)
    {
        vm_puts(25, (char) (7 + item), menudata[item].desc);
    }
}

int main(void)
{
    qmenuInit();
    qmenuRun();
    qmenuTerm();
    return 0;
}
