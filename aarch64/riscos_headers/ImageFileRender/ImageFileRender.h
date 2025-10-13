/*******************************************************************
 * File:     ImageFileRender
 * Purpose:  Header for using ImageFileRender with tools
 * Author:   RISCOS Ltd
 * Date:     29 Jun 2002
 ******************************************************************/

#ifndef IMAGEFILERENDER_H
#define IMAGEFILERENDER_H

/* Service numbers */
#define Service_ImageFileRender_Started (0x80d40) /* R0 = API version */
#define Service_ImageFileRender_Dying   (0x80d41) /* R0 = API version */
#define Service_ImageFileRender_RendererChanged (0x80d42) /* R0 = API version,
                                                             R1 = filetype */

/* Flags for the SWIs */
#define FLAG_TRANSFORMTYPE_MASK (7)
#define TRANSFORMTYPE_SIZE      (0<<0)
#define TRANSFORMTYPE_SCALE     (1<<0)
#define TRANSFORMTYPE_TRANSFORM (2<<0)
/* Other render types are reserved */
#define FLAG_COLOURMAP          (1<<3)
#define FLAG_IGNORE_DOCUMENT_OFFSET (1<<4)
#define FLAG_RETURN_OSUNITS     (1<<5)
#define FLAG_QUALITY_SHIFT      (6)
#define FLAG_QUALITY_MASK       (15<<FLAG_QUALITY_SHIFT)

#define SEQUENCE_DEFAULT (0)
#define SEQUENCE_LAST    (-1)
#define SEQUENCE_UNKNOWN (-1)

#define QUERY_BASEDETAILS (0)

/* The way in which we build up the magic values is simple:
 *   &6699ccii where cc is the company identifier,
 *                   ii is the image identifier.
 *   &6699xxxx is used because it is less likely to crop up by chance.
 * These identifiers are used by us for allocations; private uses
 * might provide some other values, but we stick to just these numbers
 * for consistencies sake.
 */
#define IMAGEFILE_MAGIC(company,imagetype) \
                        ((0x66990000 + (company<<8)) | imagetype)

/* List of known companies */
#define IMAGEFILE_COMPANY_ROL     (1)

/* Full allocations */
#define IMAGEFILE_MAGIC_SPRITE_ROL  IMAGEFILE_MAGIC(IMAGEFILE_COMPANY_ROL,1)

#endif
