/*******************************************************************
 * File:     font
 * Purpose:  Simple font manipulation for our calendar gadget
 * Author:   Justin Fletcher
 * Date:     16 Jan 2006
 ******************************************************************/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "swis.h"

#ifdef FORTIFY
#include "fortify.h"
#endif

#include "font.h"
#include "gcontext.h"

/* Define this to debug this file */
#undef DEBUG
//#define DEBUG

#ifdef DEBUG
#define dprintf if (1) _swix(0x104,0), printf
#else
#define dprintf if (0) _swix(0x104,0), printf
#endif

#define Font_OSUnit (400)

#define FONT_XTOOS(x) (((x)+Font_OSUnit-1)/Font_OSUnit)
#define FONT_YTOOS(x) (((x)+Font_OSUnit-1)/Font_OSUnit)
#define FONT_OSTOX(x) ((x)*Font_OSUnit)
#define FONT_OSTOY(x) ((x)*Font_OSUnit)

struct font_s {
    int32_t handle;
    int32_t xsize, ysize;
    int32_t ascent, descent;

    bounds_t em; /* This is the width and height of an em */
};


/*************************************************** Gerph *********
 Function:     font_findfont
 Description:  Find a font
 Parameters:   fontname-> the name of the font to find
               xsize = the size (points * 16)
               ysize = the size (points * 16)
 Returns:      font handle, or NULL if could not claim
 ******************************************************************/
font_t font_findfont(const char *fontname, int xsize, int ysize)
{
  _kernel_oserror *err;
  font_t font;
  int32_t handle;
  err = _swix(Font_FindFont, _INR(1,5)|_OUT(0), fontname, xsize, ysize, 0, 0,
                                                &handle);
  if (err)
  {
    dprintf("FAILED: Could not find font %s\n", fontname);
    free(font);
    return NULL;
  }
  font = calloc(1, sizeof(*font));
  if (font == NULL)
  {
    _swix(Font_LoseFont, _IN(0), handle);
    return NULL;
  }
  font->handle = handle;
  font->xsize = xsize;
  font->ysize = ysize;
  font->ascent = 0;
  font->descent = 0;

  /* Values that aren't cached yet */
  font->em.width = -1;
  font->em.height = -1;
  return font;
}

/*************************************************** Gerph *********
 Function:     font_losefont
 Description:  Lose a font we've claimed
 Parameters:   handle = the handle to lose
 Returns:      none
 ******************************************************************/
void font_losefont(font_t font)
{
  if (font == NULL)
    return;
  _swix(Font_LoseFont, _IN(0), font->handle);
  free(font);
}

/*************************************************** Gerph *********
 Function:     font_paint
 Description:  Paint our font somewhere
 Parameters:   handle = the font handle
               x = the x position to plot at (OS units)
               y = the y position to plot at (OS units)
                   (of the left base line)
               bg,fg = colours to use
               str-> the string to plot
               len = the number of characters to plot, or -1 for all
 Returns:      coords_t for the end position
 ******************************************************************/
coords_t font_paint(font_t font, int x, int y,
                    uint32_t bg, uint32_t fg,
                    const char *str, int len)
{
  _kernel_oserror *err;
  coords_t end = {0};
  if (len==-1)
    len = strlen(str);

  if (font == NULL)
  {
    /* No font data, so just the system font text string */
    _swix(0x105,0);
    _swix(ColourTrans_SetGCOL,_IN(0)|_INR(3,4), fg,0,0);
    _swix(OS_Plot,_INR(0,2),4,x,y+28-2);
    _swix(OS_WriteN,_INR(0,1),str,len);
    _swix(0x104,0);
    end.x += len * 16;
    end.y = y;
  }
  else
  {
    _swix(ColourTrans_SetFontColours,_INR(0,3), font->handle,
                                                bg,fg, 14);
    err = _swix(Font_Paint, _INR(0,7)|_OUTR(3,4),
                            font->handle, str,
                            (1<<8) | (1<<7) | (1<<4) | (1<<11),
                            x, y, NULL, NULL, len,
                            &end.x, &end.y);
    if (err)
    {
      dprintf("FAILED: Error writing string\n");
      end.x = x;
      end.y = y;
    }
  }
  return end;
}


/*************************************************** Gerph *********
 Function:     font_getem
 Description:  Return the em size
 Parameters:   font = the font handle, or NULL for system font
               bounds-> the box to fill in
 Returns:      bounds for the em
 ******************************************************************/
bounds_t font_getem(font_t font)
{
  bounds_t bounds;
  stringbounds_t stringbounds;
  if (font == NULL)
  {
    bounds.width = 16;
    bounds.height = 32;
    return bounds;
  }

  if (font->em.width != -1)
  {
    /* Return cached value */
    return font->em;
  }

  font_getstringsize(font, &stringbounds, -1, "M", 1, 0);
  dprintf("String bounds for M were %i, %i, %i\n", stringbounds.lbearing, stringbounds.xoffset, stringbounds.rbearing);
  font->em.width = -stringbounds.lbearing + stringbounds.xoffset + stringbounds.rbearing;
  font->em.height = stringbounds.ascent + stringbounds.descent;
  return font->em;
}


/*************************************************** Gerph *********
 Function:     font_getstringsize
 Description:  Return the size of the string into the structure
 Parameters:   font = the font handle, or NULL for system font
               bounds-> the box to fill in
               xlimit = the widest the string may be in OS units, or -1 for unlimited
               str-> the string to read
               len = the number of characters to process, or -1 for all
               splitchar = character to split on, or 0 for none
 Returns:      none
 ******************************************************************/
void font_getstringsize(font_t font, stringbounds_t *bounds,
                        int32_t xlimit,
                        const char *str, int len, char split_char)
{
  _kernel_oserror *err;
  if (len==-1)
    len = strlen(str);

  if (font == NULL)
  {
    bounds->ascent = 28;
    bounds->descent = 6;
    bounds->lbearing = 0;
    bounds->rbearing = 0;
    bounds->xoffset = len * 16;
    return;
  }

  /* Get the ascent/descent */
  if (font->ascent == 0)
  {
      dprintf("Cache the ascent/descent\n");
      err = _swix(Font_ReadInfo, _IN(0)|_OUT(2)|_OUT(4),
                                 font->handle,
                                 &font->descent,
                                 &font->ascent);
      if (err)
      {
        font->ascent = 0;
        font->descent = 0;
        dprintf("FAILED: Could not read the font information\n");
      }
      else
      {
        dprintf("font ascent = %i\n", font->ascent);
        dprintf("font descent = %i\n", font->descent);
      }
  }

  bounds->ascent = font->ascent;
  bounds->descent = -font->descent;
  dprintf("descent = %i\n", bounds->descent);
  dprintf("ascent = %i\n", bounds->ascent);

  if (xlimit == -1)
    xlimit = 0x7FFFFFFF;
  else
    xlimit = FONT_OSTOX(xlimit);

  /* Obtain the size of the string */
  {
    char *endp = NULL;
    int32_t width = 0;
    int32_t block[9];
    block[0]=0;
    block[1]=0;
    block[2]=0;
    block[3]=0;
    block[4] = split_char ? split_char : -1;
    err = _swix(Font_ScanString, _INR(0,7)|_OUT(1)|_OUT(3),
                                 font->handle, str,
                                 (1<<8)|(1<<5)|(1<<18)|(1<<7),
                                 xlimit, 1<<30,
                                 block, NULL,
                                 len,
                                 &endp,
                                 &width);
    if (err)
    {
      dprintf("FAILED: Error scanning : %s\n", err->errmess);
      bounds->lbearing = 0;
      bounds->rbearing = 0;
      bounds->xoffset = 0;
      bounds->charoffset = 0;
    }
    else
    {
      bounds->lbearing  = FONT_XTOOS(-block[5]);
      bounds->rbearing  = FONT_XTOOS(block[7]-width);
      bounds->xoffset   = FONT_XTOOS(width);
      bounds->charoffset = endp - str;
    }
    dprintf("font_getstringsize lbearing=%i,rbearing=%i,xoffset=%i,charoffset=%i\n",
            bounds->lbearing, bounds->rbearing, bounds->xoffset, bounds->charoffset);
  }
}

/*************************************************** Gerph *********
 Function:     font_paintattrib
 Description:  Put a string, with the given attributes
 Parameters:   font = the handle to plot with
               x0,y0,x1,y1 = box to centre in
               str-> the string to use
               len = the length to use
               bg,fg = colour to use
               attrib = the attributes to use
 Returns:      coords_t for the end position
 ******************************************************************/
coords_t font_paintattrib(font_t font, int32_t x0, int32_t y0, int32_t x1, int32_t y1,
                          const char *str, int32_t len,
                          uint32_t bg, uint32_t fg,
                          uint32_t attrib)
{
  stringbounds_t bounds;
  int32_t strwidth, strheight;
  int32_t x,y;
  coords_t end = {0};

  font_getstringsize(font, &bounds, -1, str, len, 0);
  strwidth = bounds.lbearing + bounds.rbearing + bounds.xoffset;
  strheight = bounds.ascent + bounds.descent;
  switch (attrib & FONTATTRIB_ALIGN_HMASK)
  {
    default:
    case FONTATTRIB_ALIGN_HLEFT:
retry_left:
      x = x0 + bounds.lbearing;
      /* FIXME: Check for too big on right */
      break;

    case FONTATTRIB_ALIGN_HRIGHT:
      x = x1 - strwidth + bounds.lbearing;
      /* FIXME: Check for too big on left */
      break;

    case FONTATTRIB_ALIGN_HCENTRE:
      x = (x0+x1-strwidth)/2+bounds.lbearing;
      if (x < x0)
        goto retry_left; /* Too big, pretend left aligned and truncate */
      break;
  }
  end.x = x + bounds.xoffset;

  switch (attrib & FONTATTRIB_ALIGN_VMASK)
  {
    default:
    case FONTATTRIB_ALIGN_VTOP:
      y = y1 - strheight + bounds.descent;
      if (y < y0)
        return end; /* It doesn't fit, so give up */
      end.y = y1;
      break;

    case FONTATTRIB_ALIGN_VBOTTOM:
      y = y0 + bounds.descent;
      if (y+bounds.ascent > y1)
        return end; /* It doesn't fit, so give up */
      end.y = y0;
      break;

    case FONTATTRIB_ALIGN_VCENTRE:
      y = (y0+y1-strheight)/2+bounds.descent;
      end.y = y - bounds.descent;
      break;
  }
  /* x,y now the left of the baseline to plot at */

  dprintf("Plot string %p at %i, %i (bg=%08lx, fg=%08lx) -> %i, %i\n", str, x, y, bg, fg, end.x, end.y);

  font_paint(font, x, y, bg, fg, str, len);

  if (attrib & FONTATTRIB_UNDERLINE)
  {
    line_start(fg);
    line(x,y, x+bounds.xoffset, y);
    line_end();
  }

#if 0
  gcol(cal->fg);
  move(x0,y0);
  draw((x0+x1-strwidth)/2+bounds.lbearing,
       (y0+y1-strheight)/2+bounds.descent);
  draw((x0+x1-strwidth)/2+bounds.lbearing+bounds.xoffset,
       (y0+y1-strheight)/2+bounds.descent);
  draw((x0+x1-strwidth)/2+bounds.lbearing+bounds.xoffset,
       (y0+y1-strheight)/2+bounds.descent + bounds.ascent);
  draw((x0+x1-strwidth)/2+bounds.lbearing+bounds.xoffset,
       (y0+y1-strheight)/2);
#endif

    return end;
}
