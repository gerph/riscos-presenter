/*******************************************************************
 * File:     WebColoursSWIs
 * Purpose:  Colour management for Web formats
 * Author:   Justin Fletcher
 * Date:     13 May 2000
 ******************************************************************/

#ifndef WEBCOLOURS_H
#define WEBCOLOURS_H

/* SWI Numbers */
#undef WebColours_HTML4ToName
#define WebColours_HTML4ToName    (0x00054700)
#undef WebColours_HTML4ToNumber
#define WebColours_HTML4ToNumber  (0x00054701)
#undef WebColours_CSSToName
#define WebColours_CSSToName      (0x00054702)
#undef WebColours_CSSToNumber
#define WebColours_CSSToNumber    (0x00054703)

/* Flags for conversions */
#define WebColoursFlag_UseNamed         (1<<0)
#define WebColoursFlag_ZeroTerminated   (1<<1)
#define WebColoursFlag_CSS3             (1<<8)

#endif
