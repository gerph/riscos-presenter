/*******************************************************************
 * File:     colourmap
 * Purpose:  Defines a colour map descriptor
 * Author:   RISCOS Ltd
 * Date:     29 Jun 2002
 ******************************************************************/

#ifndef COLOURMAP_H
#define COLOURMAP_H

typedef struct colourmap_s {
  void *workspace;
  void (*function)(void);
} colourmap_t;

#endif
