/*
**  BORDCOLR.C - set the border color
**  by: Bob Jarvis
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>

char *usage = "BORDCOLR - sets the border color\n"
              "Parameter: color to set - one of\n"
              "\tBLK - black\n"
              "\tBLU - blue\n"
              "\tGRN - green\n"
              "\tCYN - cyan\n"
              "\tRED - red\n"
              "\tMAG - magenta\n"
              "\tBRN - brown\n"
              "\tLTG - light gray\n"
              "\tDKG - dark gray\n"
              "\tLTB - light blue\n"
              "\tLGN - light green\n"
              "\tLTC - light cyan\n"
              "\tLTR - light red\n"
              "\tLTM - light magenta\n"
              "\tYEL - yellow\n"
              "\tWHT - white";

#define BLACK     0
#define BLUE      1
#define GREEN     2
#define CYAN      3
#define RED       4
#define MAGENTA   5
#define BROWN     6
#define LTGRAY    7
#define DKGRAY    8
#define LTBLUE    9
#define LTGREEN   10
#define LTCYAN    11
#define LTRED     12
#define LTMAGENTA 13
#define YELLOW    14
#define WHITE     15

void set_border_color(int color)
{
      union REGS regs;

      printf("color = %d\n", color);

      regs.h.ah = 0x0B;
      regs.h.bh = 0;
      regs.h.bl = color;

      int86(0x10, &regs, &regs);
}

main(int argc, char *argv[])
{
      int color; 

      if(argc < 2)
      {
            printf(usage);
            return EXIT_SUCCESS;
      }

      if(stricmp(argv[1], "BLK") == 0)
            color = BLACK;
      else  if(stricmp(argv[1], "BLU") == 0)
            color = BLUE;
      else  if(stricmp(argv[1], "GRN") == 0)
            color = GREEN;
      else  if(stricmp(argv[1], "CYN") == 0)
            color = CYAN;
      else  if(stricmp(argv[1], "RED") == 0)
            color = RED;
      else  if(stricmp(argv[1], "MAG") == 0)
            color = MAGENTA;
      else  if(stricmp(argv[1], "BRN") == 0)
            color = BROWN;
      else  if(stricmp(argv[1], "LTG") == 0)
            color = LTGRAY;
      else  if(stricmp(argv[1], "DKG") == 0)
            color = DKGRAY;
      else  if(stricmp(argv[1], "LTB") == 0)
            color = LTBLUE;
      else  if(stricmp(argv[1], "LGN") == 0)
            color = LTGREEN;
      else  if(stricmp(argv[1], "LTC") == 0)
            color = LTCYAN;
      else  if(stricmp(argv[1], "LTR") == 0)
            color = LTRED;
      else  if(stricmp(argv[1], "LTM") == 0)
            color = LTMAGENTA;
      else  if(stricmp(argv[1], "YEL") == 0)
            color = YELLOW;
      else  if(stricmp(argv[1], "WHT") == 0)
            color = WHITE;
      else
      {
            printf(usage);
            return EXIT_SUCCESS;
      }

      set_border_color(color);
      return EXIT_SUCCESS;
}
