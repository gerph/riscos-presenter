/*******************************************************************
 * File:     vdu5
 * Purpose:  Plain VDU5 text rendering
 * Author:   Justin Fletcher
 * Date:     13 Sep 2020
 ******************************************************************/

#include "gcontext.h"


/*************************************************** Gerph *********
 Function:     vdu5_findfont
 Description:  Find a font
 Parameters:   fontname-> the name of the font to find
               xsize = the size (points * 16)
               ysize = the size (points * 16)
 Returns:      font handle, or NULL if could not claim
 ******************************************************************/
void *vdu5_findfont(const char *name, int xsize, int ysize);

/*************************************************** Gerph *********
 Function:     vdu5_losefont
 Description:  Lose a font we've claimed
 Parameters:   fh = the handle to lose
 Returns:      none
 ******************************************************************/
void vdu5_losefont(font_t fh);


/*************************************************** Gerph *********
 Function:     vdu5_getem
 Description:  Return the em size
 Parameters:   fh = the font handle, or NULL for system font
 Returns:      bounds for the em
 ******************************************************************/
bounds_t vdu5_getem(font_t fh);


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
void vdu5_getstringsize(font_t, stringbounds_t *bounds,
                        int xlimit,
                        const char *str, int strsize, char split_char);

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
coords_t vdu5_paint(font_t font, int x, int y,
                    uint32_t bg, uint32_t fg,
                    const char *str, int len);

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
coords_t vdu5_paintattrib(font_t, int x0, int y0, int x1, int y1,
                          const char *str, int strsize,
                          uint32_t bg, uint32_t fg,
                          uint32_t attr);
