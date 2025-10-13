/*******************************************************************
 * File:     fontmap
 * Purpose:  FontMap module constants
 * Author:   Justin Fletcher
 * Date:     08 Jan 2005
 ******************************************************************/

#ifndef FONTMAPCONSTS_H
#define FONTMAPCONSTS_H

/* SWI numbers */
#ifndef FontMap_00
#define FontMap_00 (0x00057f00)
#define FontMap_Translate         (0x00057f00)
#endif /* FontMap_00 */

typedef enum fontmapitalic_e {
  fmi_dontcare=0, /* Makes orthogonal sense that 'don't care' be 0 */
  fmi_plain=1,
  fmi_italic=2
} fontmapitalic_t;

/* Note that Panose 1.0 has a slightly different order for these -
    Very Light, Light, Thin, Book, Medium, Demi, Bold, Heavy, Black, Nord.
    Note in particular the ordering of Thin and the Light pair being
    reversed.
 */
typedef enum fontmapweight_s {
  fmw_dontcare=0,
  fmw_thin=100,
  fmw_extralight=200,
  fmw_light=300,
  fmw_regular=400,
  fmw_medium=500,
  fmw_semibold=600,
  fmw_bold=700,
  fmw_extrabold=800,
  fmw_black=900,
  /* Special markers to indicate that we want to modify the current type */
  /* Note: These are powers of 2 to make it easier to load them, not for any
           other reason. NOT used at present */
  fmw_lighter=1024,
  fmw_bolder=2048
} fontmapweight_t;

/* Note that this has no comparable values on Windows; it is partially
   supported as a Panose 1.0 mapping, (values 5,6,7,8). */
typedef enum fontmapstretch_s {
  fms_dontcare=0,
  fms_ultracondensed=100,
  fms_extracondensed=200,
  fms_condensed=300,
  fms_semicondensed=400,
  fms_normal=500,
  fms_semiexpanded=600,
  fms_expanded=700,
  fms_extraexpanded=800,
  fms_ultraexpanded=900
} fontmapstretch_t;

typedef struct fontinst_s {
  char *name;
  fontmapitalic_t italic;
  fontmapweight_t weight;
  fontmapstretch_t stretch;
} fontinst_t;

/* Flags for FontMap_Translate */
#define FONTMAP_SYS_RO    (0)
#define FONTMAP_SYS_WIN   (1)
#define FONTMAP_SYS_MAX   (2)
#define FONTMAP_SYS_MASK  (7)
#define FONTMAP_SRC_SHIFT (0)
#define FONTMAP_DST_SHIFT (3)

/* Flags which we can pass to FontMap_Translate */
#define FontMap_Translate_ValidFlags (FONTMAP_MAX-1)

#endif
