/*******************************************************************
 * File:     gcontext
 * Purpose:  Graphics context operations
 * Author:   Justin Fletcher
 * Date:     11 Jun 2006
 ******************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "gcontext.h"
#include "graphics.h"
#include "font.h"
#include "vdu5.h"
#include "vdushape.h"

/* Define this to debug printed output */
//#undef DEBUG
//#define DEBUG

#ifdef DEBUG
#define dprintf if (1) _swix(0x104,0), printf
#else
#define dprintf if (0) _swix(0x104,0), printf
#endif

gcontext_t gcontext;

/*************************************************** Gerph *********
 Function:     gcontext_generic_rectoutline
 Description:  Outline a rectangle
 Parameters:   col = the colour to fill with
               x0,y0 = bottom left
               x1,y1 = top right
 Returns:      none
 ******************************************************************/
static void gcontext_generic_rectoutline(uint32_t colour,
                                         int x0, int y0, int x1, int y1)
{
    line_start(colour);
    line(x0,y0,x0,y1);
    line(x0,y1,x1,y1);
    line(x1,y1,x1,y0);
    line(x1,y0,x0,y0);
    line_end();
}


/*************************************************** Gerph *********
 Function:     gcontext_initvdu
 Description:  Initialise the graphics context for the VDU system
 Parameters:   fonts = 1 if we should use fonts
 Returns:      none
 ******************************************************************/
void gcontext_initvdu(int fonts)
{
  gcontext.rectangle_fill = gcontext_vdu_rectfill;
  gcontext.rectangle_outline = gcontext_generic_rectoutline;
  gcontext.line_start = gcontext_vdu_linestart;
  gcontext.line_line = gcontext_vdu_line;
  gcontext.line_end = gcontext_vdu_lineend;
  gcontext.fill_start = NULL; /* We don't have any 'fill' routines */
  gcontext.triangle_fill = gcontext_vdu_trifill;
  if (!fonts)
  {
    gcontext.text_findfont = (font_t(*)(const char *, int, int))vdu5_findfont;
    gcontext.text_losefont = (void(*)(font_t))vdu5_losefont;
    gcontext.text_getstringsize = (void(*)(font_t, stringbounds_t *, int,
                                           const char *, int, char))(vdu5_getstringsize);
    gcontext.text_getemsize = vdu5_getem;
    gcontext.text_paint = (coords_t(*)(font_t,int,int,
                                       uint32_t,uint32_t,
                                       const char *,int))(vdu5_paint);
    gcontext.text_paintattrib = (coords_t(*)(font_t,int,int,int,int,
                                             const char *,int,
                                             uint32_t,uint32_t,
                                             uint32_t))(vdu5_paintattrib);
  }
  else
  {
    gcontext.text_findfont = (font_t(*)(const char *, int32_t, int32_t))font_findfont;
    gcontext.text_losefont = (void(*)(font_t))font_losefont;
    gcontext.text_getstringsize = (void(*)(font_t, stringbounds_t *, int32_t,
                                           const char *, int32_t, char))(font_getstringsize);
    gcontext.text_getemsize = font_getem;
    gcontext.text_paint = (coords_t(*)(font_t,int,int,
                                       uint32_t,uint32_t,
                                       const char *,int))(font_paint);
    gcontext.text_paintattrib = (coords_t(*)(font_t,int32_t,int32_t,int32_t,int32_t,
                                             const char *,int32_t,uint32_t,uint32_t,
                                             uint32_t))(font_paintattrib);
  }
#ifdef DEBUG_FILL
  gcontext.fill_start = gcontext_vdu_fill_start;
  gcontext.fill_move = gcontext_vdu_fill_move;
  gcontext.fill_line = gcontext_vdu_fill_line;
  gcontext.fill_close = gcontext_vdu_fill_close;
  gcontext.fill_end = gcontext_vdu_fill_end;
#endif
}
