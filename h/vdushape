/*******************************************************************
 * File:     vdushape
 * Purpose:  Plain shape rendering (VDU calls)
 * Author:   Justin Fletcher
 * Date:     13 Sep 2020
 ******************************************************************/

#include "gcontext.h"
#include "graphics.h"


/*************************************************** Gerph *********
 Function:     gcontext_vdu_rectfill
 Description:  Fill a rectangle
 Parameters:   col = the colour to fill with
               x0,y0 = bottom left
               x1,y1 = top right
 Returns:      none
 ******************************************************************/
void gcontext_vdu_rectfill(uint32_t colour,
                           int x0, int y0, int x1, int y1);

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
                          int x0, int y0, int x1, int y1, int x2, int y2);

/*************************************************** Gerph *********
 Function:     gcontext_vdu_linestart
 Description:  Start a line group
 Parameters:   col = the colour to draw with
 Returns:      none
 ******************************************************************/
void gcontext_vdu_linestart(uint32_t colour);

/*************************************************** Gerph *********
 Function:     gcontext_vdu_lineend
 Description:  End a line group
 Parameters:   none
 Returns:      none
 ******************************************************************/
void gcontext_vdu_lineend(void);

/*************************************************** Gerph *********
 Function:     gcontext_vdu_lineend
 Description:  End a line group
 Parameters:   none
 Returns:      none
 ******************************************************************/
void gcontext_vdu_line(int x0, int y0, int x1, int y1);


/* Fill routines only for debugging */
void gcontext_vdu_fill_start(uint32_t fill_colour);
void gcontext_vdu_fill_move(int x, int y);
void gcontext_vdu_fill_line(int x, int y);
void gcontext_vdu_fill_close(void);
void gcontext_vdu_fill_end(void);
