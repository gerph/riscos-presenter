/*******************************************************************
 * File:     vdu5
 * Purpose:  Plain VDU5 text rendering
 * Author:   Justin Fletcher
 * Date:     13 Sep 2020
 ******************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "font.h"
#include "swis.h"
#include "gcontext.h"
#include "graphics.h"


/* Define this to debug this file */
//#undef DEBUG
//#define DEBUG

#ifdef DEBUG
#define dprintf if (1) _swix(0x104,0), printf
#else
#define dprintf if (0) _swix(0x104,0), printf
#endif

/*************************************************** Gerph *********
 Function:     vdu5_findfont
 Description:  Find a font
 Parameters:   fontname-> the name of the font to find
               xsize = the size (points * 16)
               ysize = the size (points * 16)
 Returns:      font handle, or -1 if could not claim
 ******************************************************************/
font_t vdu5_findfont(const char *name, int xsize, int ysize)
{
  uint32_t composite;
  uint32_t xeig, yeig;
#define POINT_SIZE_MAPS_TO_REGULAR_CHARACTERS (8)
  xsize = xsize * 8 / POINT_SIZE_MAPS_TO_REGULAR_CHARACTERS / 16;
  ysize = ysize * 8 / POINT_SIZE_MAPS_TO_REGULAR_CHARACTERS / 16;
  _swix(OS_ReadModeVariable, _INR(0,1)|_OUT(2), -1, 4, &xeig);
  _swix(OS_ReadModeVariable, _INR(0,1)|_OUT(2), -1, 5, &yeig);
  composite = (xeig<<28) | (yeig<<24) |
              (((uint32_t)xsize & 1023) << 12) |
              ((uint32_t)ysize & 1023);
  dprintf("Find VDU font: xeig=%lu, yeig=%lu, xsize=%i, ysize=%i\n", xeig, yeig, xsize, ysize);
  return (void*)composite;
}

/*************************************************** Gerph *********
 Function:     vdu5_losefont
 Description:  Lose a font we've claimed
 Parameters:   fh = the handle to lose
 Returns:      none
 ******************************************************************/
void vdu5_losefont(font_t fh)
{
  /* Nothing to do to lose the font */
}


/*************************************************** Gerph *********
 Function:     vdu5_getem
 Description:  Return the em size
 Parameters:   fh = the font handle, or NULL for system font
 Returns:      bounds for the em
 ******************************************************************/
bounds_t vdu5_getem(font_t fh)
{
  bounds_t bounds;
  uint32_t composite = (uint32_t) fh;
  uint32_t xeig = (composite>>28) & 0xF;
  uint32_t yeig = (composite>>24) & 0xF;
  uint32_t xsize = (composite>>12) & 0xFFF;
  uint32_t ysize = (composite>>0) & 0xFFF;

  bounds.width = xsize;
  bounds.height = ysize;
  return bounds;
}


/*************************************************** Gerph *********
 Function:     vdu5_getstringsize
 Description:  Return the size of the string into the structure
 Parameters:   fh = the font handle
               bounds-> the box to fill in
               xlimit = the widest the string may be, or -1 for unlimited
               str-> the string to read
               len = the number of characters to process, or -1 for all
               splitchar = character to split on, or 0 for none
 Returns:      none
 ******************************************************************/
void vdu5_getstringsize(font_t fh, stringbounds_t *bounds,
                        int xlimit,
                        const char *str, int strsize, char split_char)
{
  uint32_t composite = (uint32_t) fh;
  uint32_t xeig = (composite>>28) & 0xF;
  uint32_t yeig = (composite>>24) & 0xF;
  uint32_t xsize = (composite>>12) & 0xFFF;
  uint32_t ysize = (composite>>0) & 0xFFF;
  int len;

  if (strsize == -1)
    strsize = strlen(str);

  len = strsize;
  if (xlimit == -1)
    xlimit = 0x7FFFFFFF;

  if (len * xsize > xlimit)
  {
    /* we only need to worry about splitting if the string would be longer than the limit */
    if (split_char == 0)
    {
      len = xlimit / xsize; /* rounds down, which is what we want */
    }
    else
    {
      const char *searchp = str;
      len = 0;
      do {
        const char *splitp;
        splitp = strchr(searchp, split_char);
        if (splitp == NULL)
        {
            /* There was no split point before the end of the string; this should not happen,
             * because the earlier check for the length should have caught it.
             */
            len = strsize;
            break;
        }
        else
        {
            if ((splitp - str) * xsize > xlimit)
                break; /* This split is beyond the limit, so the last split we found is the one to use. */

            /* Still more space, so look for the next split point */
            searchp = splitp + 1;
        }
      } while (1);
    }
  }

  bounds->ascent = ysize * 28 / 32;
  bounds->descent = ysize - bounds->ascent;
  bounds->lbearing = 0;
  bounds->rbearing = 0;
  bounds->xoffset = len;
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
coords_t vdu5_paint(font_t fh, int x, int y,
                    uint32_t bg, uint32_t fg,
                    const char *str, int strsize)
{
  uint32_t composite = (uint32_t) fh;
  uint32_t xeig = (composite>>28) & 0xF;
  uint32_t yeig = (composite>>24) & 0xF;
  uint32_t xsize = (composite>>12) & 0xFFF;
  uint32_t ysize = (composite>>0) & 0xFFF;
  coords_t end;
  static char seq[] = {
    5, 23,17, 7,6,0,0, 0,0,0,0, 0, 4
  };
  if (strsize == -1)
    strsize = strlen(str);
  if (strsize == 0)
  {
    end.x = x;
    end.y = y;
    return end;
  }

  move(x, y + (ysize * 28 / 32));
  gcol(fg);
  seq[5] = xsize % 256;
  seq[6] = xsize / 256;
  seq[7] = ysize % 256;
  seq[8] = ysize / 256;
  if (strsize == 1)
  {
      seq[11] = *str;
      _swix(OS_WriteN, _INR(0,1), seq, 13);
  }
  else
  {
      _swix(OS_WriteN, _INR(0,1), seq, 11);
      _swix(OS_WriteN, _INR(0,1), str, strsize);
      _swix(OS_WriteI + 4, 0);
  }

  end.x = x + xsize * (strsize<<xeig);
  end.y = y;
  return end;
}


/*************************************************** Gerph *********
 Function:     vdu5_paintattrib
 Description:  Put a string, with the given attributes
 Parameters:   fonthandle = the handle to plot with
               x0,y0,x1,y1 = box to centre in
               str-> the string to use
               len = the length to use
               bg,fg = colour to use
               attrib = the attributes to use
 Returns:      coords_t for the end position
 ******************************************************************/
coords_t vdu5_paintattrib(font_t fh, int x0, int y0, int x1, int y1,
                          const char *str, int strsize,
                          uint32_t bg, uint32_t fg,
                          uint32_t attr)
{
    uint32_t composite = (uint32_t) fh;
    uint32_t ysize = (composite>>0) & 0xFFF;
    coords_t end;

    /* Correct for the _paint function wanting the baseline */
    int yb = y1 - ((ysize * 28) / 32);
    end = vdu5_paint(fh, x0, yb, bg, fg, str, strsize);
    end.y = y1;

    return end;
}
