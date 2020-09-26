/*******************************************************************
 * File:        screen
 * Purpose:     Functions for accessing the screen properties
 * Author:      Gerph
 * Date:        21 Sep 2020
 ******************************************************************/

#include <stdlib.h>
#include "swis.h"

#ifdef FORTIFY
#include "fortify.h"
#endif

#include "gcontext.h"
#include "vdu.h"
#include "screen.h"


/*************************************************** Gerph *********
 Function:     screen_get_graphics_window
 Description:  Read the current clipping rectangle from the graphics
               system
 Parameters:   window-> the rectangle to fill in
 Returns:      1 if failed, 0 if successful
 ******************************************************************/
int screen_get_graphics_window(bbox_t *window)
{
    static int in[] = {
            VduVariable_GWLCol,
            VduVariable_GWBRow,
            VduVariable_GWRCol,
            VduVariable_GWTRow,
            ModeVariable_XEigFactor,
            ModeVariable_YEigFactor,
            -1
        };
    int out[6];
    _kernel_oserror *err;

    err = _swix(OS_ReadVduVariables,_INR(0,1),in,out);
    if (err)
        return 1;

    window->x0 = out[0]<<out[4];
    window->y0 = out[1]<<out[5];
    window->x1 = (out[2]+1)<<out[4];
    window->y1 = (out[3]+1)<<out[5];

    return 0;
}


/*************************************************** Gerph *********
 Function:      screen_set_graphics_window
 Description:   Set the graphics window
 Parameters:    bbox-> the window to set up
 Returns:       1 if failed, 0 if successful
 ******************************************************************/
int screen_set_graphics_window(bbox_t *window)
{
    unsigned long data[3];

    data[1]= (window->x0 & 0xFFFF) | ((window->y0 & 0xFFFF)<<16);
    data[2]= ((window->x1-1) & 0xFFFF) | (((window->y1-1) & 0xFFFF)<<16);
    data[0]= (24<<24); /* VDU code for 'set graphics window' in the top byte */
    if (_swix(OS_WriteN, _INR(0,1), ((char*)data)+3, 9))
        return 1;

    return 0;
}


/*************************************************** Gerph *********
 Function:     screen_get_size
 Description:  Read the current screen size
 Parameters:   size-> the bounds to fill in
 Returns:      1 if failed, 0 if successful
 ******************************************************************/
int screen_get_size(bounds_t *size)
{
    static int in[] = {
            ModeVariable_XWindLimit,
            ModeVariable_YWindLimit,
            ModeVariable_XEigFactor,
            ModeVariable_YEigFactor,
            -1,
        };
    int out[4];
    _kernel_oserror *err;

    err = _swix(OS_ReadVduVariables,_INR(0,1),in,out);
    if (err)
        return 1;

    size->width  = (out[0]+1)<<out[2];
    size->height = (out[1]+1)<<out[3];

    return 0;
}
