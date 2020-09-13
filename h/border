/*******************************************************************
 * File:     border
 * Purpose:  Simple bordering
 * Author:   Justin Fletcher
 * Date:     17 Jan 2006
 ******************************************************************/

#ifndef BORDER_H
#define BORDER_H

typedef enum bordertype_e {
  bt_none,
  bt_solid,
  bt_double,
  bt_inset,
  bt_outset,
  bt_ridge,
  bt_groove
} bordertype_t;

/* The width of each edge */
typedef struct borderedges_s {
  int top, right, bottom, left;
} borderedges_t;

/*************************************************** Gerph *********
 Function:     border_draw
 Description:  Draw a border around a region
 Parameters:   type = the type of border we should draw
               edges = a block describing the size of the edges
               x,y,width,height = the thing that we're bordering,
                                      top left corner
               face = the lighter colour, or COLOUR_NONE
               opp = the darker colour, or COLOUR_NONE
 Returns:      none
 ******************************************************************/
void border_draw(bordertype_t type,
                 borderedges_t *edges,
                 int x, int y, int width, int height,
                 unsigned long face, unsigned long opp);

/*************************************************** Gerph *********
 Function:     border_drawsimple
 Description:  Draw a simplified border
 Parameters:   type = the type of border we should draw
               size = the border size
               x,y,width,height = the thing that we're bordering,
                                      top left corner
               face = the lighter colour, or COLOUR_NONE
               opp = the darker colour, or COLOUR_NONE
 Returns:      none
 ******************************************************************/
void border_drawsimple(bordertype_t type, int size,
                       int x, int y, int width, int height,
                       unsigned long face, unsigned long opp);

#endif
