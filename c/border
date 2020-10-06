/*******************************************************************
 * File:     border
 * Purpose:  Simple bordering
 * Author:   Justin Fletcher
 * Date:     17 Jan 2006
 ******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "swis.h"

#include "gcontext.h"
#include "border.h"

#ifdef FORTIFY
#include "fortify.h"
#endif

/* Define this to debug this file */
//#define DEBUG

#ifdef DEBUG
#define dprintf if (1) _swix(0x104,0), printf
#else
#define dprintf if (0) _swix(0x104,0), printf
#endif

static void get_shadow_colours(unsigned long colour,
                               unsigned long *lightp, unsigned long *darkp)
{
  int r,g,b;
  /* Make r,g,b into the dark colour */
#define SLAB_DEPTH (0x60)
  r = (colour >> 8) & 255;
  if (r<SLAB_DEPTH/2) r = 0;
  else if (r > 255-SLAB_DEPTH/2) r = 255-SLAB_DEPTH;
  else r = r - SLAB_DEPTH/2;

  g = (colour >> 16) & 255;
  if (g<SLAB_DEPTH/2) r = 0;
  else if (g > 255-SLAB_DEPTH/2) g = 255-SLAB_DEPTH;
  else g = g - SLAB_DEPTH/2;

  b = (colour >> 24) & 255;
  if (b<SLAB_DEPTH/2) b = 0;
  else if (b > 255-SLAB_DEPTH/2) b = 255-SLAB_DEPTH;
  else b = b - SLAB_DEPTH/2;

  *darkp = COLOUR_RGB(r,g,b);
  *lightp = COLOUR_RGB(r+SLAB_DEPTH, g+SLAB_DEPTH, b+SLAB_DEPTH);
}

static void border_intdraw(borderedges_t *edges,
                           int x, int y, int width, int height,
                           unsigned long tl, unsigned long br)
{
  int more;
#if 0
  borderedges_t left;
  borderedges_t coord;
  y-=1; width-=1; height-=1;
  coord.top = y;
  coord.bottom = y-height;
  coord.left = x;
  coord.right = x + width;
  left = *edges;
  if (tl == br)
    gcol(tl);
  do
  {
    move(      coord.left,   coord.bottom);
    if (tl!=br)
      gcol(br);
    drawnofirst(coord.right,  coord.bottom);
    drawnofirst(coord.right,  coord.top);
    if (tl!=br)
      gcol(tl);
    drawnofirst(coord.left,   coord.top);
    drawnofirst(coord.left,   coord.bottom);
    more=0;
    if (--left.top > 0) left.top--, coord.top-=2, more=1;
    if (--left.right > 0) left.right--, coord.right-=2, more=1;
    if (--left.bottom > 0) left.bottom--, coord.bottom+=2, more=1;
    if (--left.left > 0) left.left--, coord.left+=2, more=1;
  } while (more);
#else
  borderedges_t outer;
  borderedges_t inner;
  outer.top = y;
  outer.bottom = y-height;
  outer.left = x;
  outer.right = x + width;
  inner.top = outer.top - edges->top;
  inner.right = outer.right - edges->right;
  inner.bottom = outer.bottom + edges->bottom;
  inner.left = outer.left + edges->left;
  dprintf("border_intdraw: %08lx/%08lx\n", tl,br);
  if (tl == br)
  {
    /* It's a 'solid' external border; no slab effect, so we can optimise
       the fill a little */
    if (fill_start(tl))
    {
      /* We can use a path */
      /* 0-------------------1
         | 5---------------6 |
         | |               | |
         | |               | |
         | |               | |
         | 8---------------7 |
         3-------------------2 */
      fill_move(outer.left,  outer.top);     /* 0 */
      fill_line(outer.right, outer.top);     /* 1 */
      fill_line(outer.right, outer.bottom);  /* 2 */
      fill_line(outer.left,  outer.bottom);  /* 3 */
      fill_line(outer.left,  outer.top);     /* 0 */
      fill_close();

      fill_move(inner.left,  inner.top);     /* 4 */
      fill_line(inner.right, inner.top);     /* 5 */
      fill_line(inner.right, inner.bottom);  /* 6 */
      fill_line(inner.left,  inner.bottom);  /* 7 */
      fill_line(inner.left,  inner.top);     /* 4 */
      fill_close();
      fill_end();
    }
    else
    {
      /* Have to do things the simple way */
      /* 0-+---------------3-+
         | 4---------------+ |
         | |               | |
         | |               | |
         | |               | |
         | +---------------+-2
         +-1-----------------+ */
      rect_fill(tl, outer.left, outer.top, inner.left, outer.bottom); /* 0->1 */
      rect_fill(tl, inner.left, outer.bottom, outer.right, inner.bottom); /* 1->2 */
      rect_fill(tl, outer.right, inner.bottom, inner.right, outer.top); /* 2->3 */
      rect_fill(tl, inner.right, outer.top, inner.left, inner.top); /* 2->3 */
    }
  }
  else
  {
    /* It's a slabbed block */
    if (fill_start(tl))
    {
      /*  0-------------5
          | 3----------4
          | |
          | |
          | 2
          1/
       */
      fill_move(outer.left,  outer.top);     /* 0 */
      fill_line(outer.left,  outer.bottom);  /* 1 */
      fill_line(inner.left,  inner.bottom);  /* 2 */
      fill_line(inner.left,  inner.top);     /* 3 */
      fill_line(inner.right, inner.top);     /* 4 */
      fill_line(outer.right, outer.top);     /* 5 */
      fill_line(outer.left,  outer.top);     /* 0 */
      fill_close();
      fill_end();

      /*              /1
                     2 |
                     | |
                     | |
           4---------3 |
         5/------------0
       */
      fill_start(br);
      fill_move(outer.right,  outer.bottom); /* 0 */
      fill_line(outer.right,  outer.top);    /* 1 */
      fill_line(inner.right,  inner.top);    /* 2 */
      fill_line(inner.right,  inner.bottom); /* 3 */
      fill_line(inner.left,   inner.bottom); /* 4 */
      fill_line(outer.left,   outer.bottom); /* 5 */
      fill_line(outer.right,  outer.bottom); /* 0 */
      fill_close();
      fill_end();
    }
    else
    {
      /*  2-------------4
          | 3----------5
          | |
          | |
          | 1
          0/
       */
      tri_fill(tl, outer.left,  outer.bottom, /* 0 */
                   inner.left,  inner.bottom, /* 1 */
                   outer.left,  outer.top     /* 2 */);

      tri_fill(tl, inner.left,  inner.bottom, /* 1 */
                   outer.left,  outer.top,    /* 2 */
                   inner.left,  inner.top     /* 3 */);

      tri_fill(tl, outer.left,  outer.top,    /* 2 */
                   inner.left,  inner.top,    /* 3 */
                   outer.right, outer.top   /* 4 */ );

      tri_fill(tl, inner.left,  inner.top,    /* 3 */
                   outer.right, outer.top,    /* 4 */
                   inner.right, inner.top     /* 5 */);

      /*              /0
                     1 |
                     | |
                     | |
           5---------3 |
         4/------------2
       */
      tri_fill(br, outer.right,  outer.top,    /* 0 */
                   inner.right,  inner.top,    /* 1 */
                   outer.right,  outer.bottom  /* 2 */);

      tri_fill(br, inner.right,  inner.top,    /* 1 */
                   outer.right,  outer.bottom, /* 2 */
                   inner.right,  inner.bottom  /* 3 */);

      tri_fill(br, outer.right,  outer.bottom, /* 2 */
                   inner.right,  inner.bottom, /* 3 */
                   outer.left,   outer.bottom  /* 4 */ );

      tri_fill(br, inner.right,  inner.bottom, /* 3 */
                   outer.left,   outer.bottom, /* 4 */
                   inner.left,   inner.bottom  /* 5 */);
    }
  }
#endif
}

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
                 unsigned long face, unsigned long opp)
{
  int hasborder;
  hasborder = (edges->top > 0 ||
               edges->right > 0 ||
               edges->bottom > 0 ||
               edges->left > 0);
  if (!hasborder || type == bt_none)
    return;

  if (face == COLOUR_NONE && opp == COLOUR_NONE)
    return;

  if (face == COLOUR_NONE || opp == COLOUR_NONE)
  {
    unsigned long colour;
    if (face == COLOUR_NONE) colour = opp;
    else                     colour = face;
    if (type != bt_solid &&
        type != bt_double)
      get_shadow_colours(colour, &face, &opp);
    else
      face = opp = colour;
  }

  /* dprintf("type = %i, face = %08lx, opp = %08lx\n", type, face, opp); */

  switch (type)
  {
    default:
    case bt_solid:
      dprintf("solid\n");
      border_intdraw(edges, x,y,width,height, opp, opp);
      break;

    case bt_double:
      {
        borderedges_t inner;
        borderedges_t outer;
        outer.top = (edges->top+1)/3;
        outer.left = (edges->left+1)/3;
        outer.right = (edges->right+1)/3;
        outer.bottom = (edges->bottom+1)/3;
        inner = outer;
        border_intdraw(&outer, x,y,width,height, opp, opp);
        x+= edges->left - inner.left;
        y-= edges->top - inner.top;
        width -=  (edges->right - inner.right) + (edges->left - inner.left);
        height -=  (edges->bottom - inner.bottom) + (edges->top - inner.top);
        border_intdraw(&inner, x,y,width,height, opp, opp);
      }
      break;

    case bt_inset:
      border_intdraw(edges, x,y,width,height, opp, face);
      break;

    case bt_outset:
      border_intdraw(edges, x,y,width,height, face, opp);
      break;

    case bt_ridge:
      {
        borderedges_t inner;
        borderedges_t outer;
        outer.top = edges->top/2;
        outer.left = edges->left/2;
        outer.right = edges->right/2;
        outer.bottom = edges->bottom/2;
        inner = outer;
        border_intdraw(&outer, x,y,width,height, face, opp);
        x+= inner.left;
        y-= inner.top;
        width -=  inner.right + inner.left;
        height -=  inner.bottom + inner.top;
        border_intdraw(&inner, x,y,width,height, opp, face);
      }
      break;

    case bt_groove:
      {
        borderedges_t inner;
        borderedges_t outer;
        outer.top = edges->top/2;
        outer.left = edges->left/2;
        outer.right = edges->right/2;
        outer.bottom = edges->bottom/2;
        inner = outer;
        border_intdraw(&outer, x,y,width,height, opp, face);
        x+= inner.left;
        y-= inner.top;
        width -= inner.right + inner.left;
        height -=  inner.bottom + inner.top;
        border_intdraw(&inner, x,y,width,height, face, opp);
      }
      break;
  }
}

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
                       unsigned long face, unsigned long opp)
{
  borderedges_t edges;
  edges.top = edges.left = edges.right = edges.bottom = size;
  border_draw(type,&edges,x,y,width,height,face,opp);
}
