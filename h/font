/*******************************************************************
 * File:     font
 * Purpose:  Simple font manipulation for our calendar gadget
 * Author:   Justin Fletcher
 * Date:     16 Jan 2006
 ******************************************************************/

#ifndef FONT_H
#define FONT_H

#include "gcontext.h"

/*************************************************** Gerph *********
 Function:     font_findfont
 Description:  Find a font
 Parameters:   fontname-> the name of the font to find
               xsize = the size (points * 16)
               ysize = the size (points * 16)
 Returns:      font handle, or NULL if could not claim
 ******************************************************************/
font_t font_findfont(const char *fontname, int32_t xsize, int32_t ysize);

/*************************************************** Gerph *********
 Function:     font_losefont
 Description:  Lose a font we've claimed
 Parameters:   handle = the handle to lose
 Returns:      none
 ******************************************************************/
void font_losefont(font_t handle);

/*************************************************** Gerph *********
 Function:     font_getem
 Description:  Return the em size
 Parameters:   font = the font handle, or NULL for system font
               bounds-> the box to fill in
 Returns:      bounds for the em
 ******************************************************************/
bounds_t font_getem(font_t font);

/*************************************************** Gerph *********
 Function:     font_getstringsize
 Description:  Return the size of the string into the structure
 Parameters:   handle = the font handle, or -1 for system font
               bounds-> the box to fill in
               xlimit = the widest the string may be, or -1 for unlimited
               str-> the string to read
               len = the number of characters to process, or -1 for all
               splitchar = character to split on, or 0 for none
 Returns:      none
 ******************************************************************/
void font_getstringsize(font_t handle, stringbounds_t *bounds,
                        int32_t xlimit,
                        const char *str, int32_t len, char split_char);

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
coords_t font_paint(font_t font, int32_t x, int32_t y,
                    uint32_t bg, uint32_t fg,
                    const char *str, int len);

/*************************************************** Gerph *********
 Function:     font_paintattrib
 Description:  Put a string, with the given attributes
 Parameters:   fonthandle = the handle to plot with
               x0,y0,x1,y1 = box to centre in
               str-> the string to use
               len = the length to use
               bg,fg = colour to use
               attrib = the attributes to use
 Returns:      coords_t for the end position
 ******************************************************************/
coords_t font_paintattrib(font_t fonthandle, int x0, int y0, int x1, int y1,
                          const char *str, int len,
                          uint32_t bg, uint32_t fg,
                          uint32_t attrib);

#endif
