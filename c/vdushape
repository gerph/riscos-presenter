/*******************************************************************
 * File:     vdushape
 * Purpose:  Plain shape rendering (VDU calls)
 * Author:   Justin Fletcher
 * Date:     13 Sep 2020
 ******************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "gcontext.h"
#include "graphics.h"

/* Define this to debug printed output */
//#undef DEBUG
//#define DEBUG

/* Define this to add the routines for 'fill' parameters so that the
   region is outlined */
/* #define DEBUG_FILL */

/* Define this to draw rectangles as 'x' characters */
/* #define DEBUG_RECTASX */

#ifdef DEBUG
#define dprintf if (1) _swix(0x104,0), printf
#else
#define dprintf if (0) _swix(0x104,0), printf
#endif


/*************************************************** Gerph *********
 Function:     gcontext_vdu_rectfill
 Description:  Fill a rectangle
 Parameters:   col = the colour to fill with
               x0,y0 = bottom left
               x1,y1 = top right
 Returns:      none
 ******************************************************************/
void gcontext_vdu_rectfill(uint32_t colour,
                           int x0, int y0, int x1, int y1)
{
#ifdef DEBUG_RECTASX
  line_start(colour);
  line(x0,y0,x1,y1);
  line(x0,y1,x1,y0);
  line_end();
#else
  gcol(colour);
  rect(x0,y0,x1,y1);
#endif
}

/*************************************************** Gerph *********
 Function:     gcontext_vdu_trifill
 Description:  Fill a triangle
 Parameters:   col = the colour to fill with
               x0,y0 = first
               x1,y1 = second
               x2,y2 = third
 Returns:      none
 ******************************************************************/
void gcontext_vdu_trifill(uint32_t colour,
                          int x0, int y0, int x1, int y1, int x2, int y2)
{
  gcol(colour);
  move(x0,y0);
  move(x1,y1);
  tri(x2,y2);
}

/*************************************************** Gerph *********
 Function:     gcontext_vdu_linestart
 Description:  Start a line group
 Parameters:   col = the colour to draw with
 Returns:      none
 ******************************************************************/
void gcontext_vdu_linestart(uint32_t colour)
{
  gcol(colour);
}

/*************************************************** Gerph *********
 Function:     gcontext_vdu_lineend
 Description:  End a line group
 Parameters:   none
 Returns:      none
 ******************************************************************/
void gcontext_vdu_lineend(void)
{
  /* Nothing */
}

/*************************************************** Gerph *********
 Function:     gcontext_vdu_lineend
 Description:  End a line group
 Parameters:   none
 Returns:      none
 ******************************************************************/
void gcontext_vdu_line(int x0, int y0, int x1, int y1)
{
  move(x0,y0);
  draw(x1,y1);
}


/* Fill routines only for debugging */
void gcontext_vdu_fill_start(uint32_t fill_colour)
{
  gcol(fill_colour);
}
void gcontext_vdu_fill_move(int x, int y)
{
  move(x,y);
}
void gcontext_vdu_fill_line(int x, int y)
{
  draw(x,y);
}
void gcontext_vdu_fill_close(void)
{
  /* None */
}
void gcontext_vdu_fill_end(void)
{
  /* None */
}
