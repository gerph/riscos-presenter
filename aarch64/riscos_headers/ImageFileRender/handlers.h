/*******************************************************************
 * File:     handlers
 * Purpose:  Controlling the handlers that provide rendering routines
 * Author:   RISCOS Ltd
 * Date:     25 Jun 2002
 ******************************************************************/

#ifndef HANDLERS_H
#define HANDLERS_H

#include "wimp.h" /* for BBox */
#include "ImageFileRender/transform.h"
#include "ImageFileRender/colourmap.h"

#define HANDLER_API_VERSION 102

/* Bits 0+1 = the transformation type */
#define HANDLER_SUPPORTS_TRANSFORM_MASK        (3<<0)
#define HANDLER_SUPPORTS_TRANSFORM_PLAIN       (0<<0) /* can't scale/rotate */
#define HANDLER_SUPPORTS_TRANSFORM_SCALING     (1<<0) /* can scale x & y */
#define HANDLER_SUPPORTS_TRANSFORM_FULLSCALING (2<<0) /* x and y independant */
#define HANDLER_SUPPORTS_TRANSFORM_FULL        (3<<0) /* can scale/rotate/skew*/
#define HANDLER_SUPPORTS_COLOURMAP (1<<2)
#define HANDLER_REQUIRES_FULL_BBOX (1<<3)
#define HANDLER_QUALITY_MAX        (15<<4)
#define HANDLER_QUALITY_MAX_SHIFT     (4)
#define HANDLER_QUALITY_DEFAULT    (15<<8)
#define HANDLER_QUALITY_DEFAULT_SHIFT (8)
#define HANDLER_VALID              (HANDLER_SUPPORTS_TRANSFORM_MASK | \
                                    HANDLER_SUPPORTS_COLOURMAP | \
                                    HANDLER_REQUIRES_FULL_BBOX | \
                                    HANDLER_QUALITY_MAX | \
                                    HANDLER_QUALITY_DEFAULT)

/* A renderer definition */
typedef struct handlerdef_s {
  unsigned long api;
  unsigned long flags;
  int type;
  unsigned long magic;
  char *name;
  void *private;
  void (*start)(void);
  void (*stop)(void);
  void (*render)(void);
  void (*bbox)(void);
  void (*declare)(void);
  void (*info)(void);
} handlerdef_t;

/* Parameters to all renderers describing the image */
typedef struct handler_image_s {
  char *data;             /* Pointer to the data */
  unsigned long datalen;  /* Length of the data */
  void *extdata;          /* Extension data */
  unsigned long sequence; /* Sequence number */
  void *private;          /* Renderer private data (after start called) */
} handler_image_t;

/* Parameters to 'render' function */
typedef struct handler_render_s {
  unsigned long flags;      /* Rendering flags (unused presently) */
  transform_t transform;    /* Transformation to apply */
  BBox clip;                /* Clipping region in external coords */
  colourmap_t colourmap;    /* Colourmap descriptor */
} handler_render_t;

/* Parameters to 'bbox' function */
typedef struct handler_bbox_s {
  unsigned long flags;      /* Bounding box flags (unused presently) */
  transform_t transform;    /* Transformation to apply */
  BBox box;                 /* Bounding box to fill in */
} handler_bbox_t;

/* Parameters to 'declare fonts' function */
typedef struct handler_declare_s {
  unsigned long flags;        /* Declaration flags (unused presently) */
  unsigned long pdriverflags; /* Flags to pass to PDriver_DeclareFonts */
} handler_declare_t;

/* Parameters to 'info' function */
typedef struct handler_info_basedetails_s {
  unsigned long sequence;    /* Sequence number of this image (-1 if unknown) */
  unsigned long xdpi;        /* X DPI to fill in */
  unsigned long ydpi;        /* Y DPI to fill in */
  unsigned long colours;     /* Colour type to fill in */
  unsigned long flags;       /* Image flags */
#define HANDLER_BASEDETAILS_FLAG_SOLID (1<<0)
} handler_info_basedetails_t;

typedef struct handler_info_s {
  unsigned long query;
  unsigned long datalen;
  union data_u {
    handler_info_basedetails_t basedetails;
  } *data;
} handler_info_t;

/* Bad query returns an error 'pointer' which can be converted to a standard
   response.
 */
#define ERR_HANDLER_BADQUERY  ((_kernel_oserror *)1)
#define ERR_HANDLER_BADLENGTH ((_kernel_oserror *)2)

#endif
