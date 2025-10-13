/*******************************************************************
 * File:     transform
 * Purpose:  Transformation matrix operations based around the standard
 *           transformation matrix operations
 * Author:   RISCOS Ltd
 * Date:     26 Jun 2002
 ******************************************************************/

#ifndef TRANSFORM_H
#define TRANSFORM_H

typedef struct transform_s {
  signed long a;
  signed long b;
  signed long c;
  signed long d;
  signed long e;
  signed long f;
} transform_t;

#endif
