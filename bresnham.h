/*
**  Header file for Brian Dessent's Bresenham line/circle algorithms
*/

#ifndef BRESNHAM__H
#define BRESNHAM__H

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define MAX_X (SCREEN_WIDTH-1)
#define MAX_Y (SCREEN_HEIGHT-1)

/* prototypes */

void setmode(int mode);
void plotdot(int x, int y, char c);
void bresenham_line(int x, int y, int x2, int y2, char c);
void bresenham_circle(int xc, int yc, int r, char c);

#endif /* BRESNHAM__H */
