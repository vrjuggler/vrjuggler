#ifndef VT_HAND_MODEL_TYPES_H
#define VT_HAND_MODEL_TYPES_H
/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/
/*                                                                  */
/*      type declarations and constant definitions for              */
/*      vt_read_object.c: routines for reading in and               */
/*      building 3D object boundary representation                  */
/*                                                                  */
/*  ==============================================================  */
/*  --  Copyright (C) 1992, 1993 Virtual Technologies           --  */
/*  --  Copyright (C) 1987,88,89,90,91,92 Larry Edwards         --  */
/*  ==============================================================  */

#include <vjConfig.h>
#include <Input/vjGlove/vt_math3d.h>

#ifndef NULL
#define NULL 0
#endif

typedef struct
{
  struct loopsegtype *loopseg0;
  vec4d norm;
  vec4d pos;
} vertstruct,*vertptr;

typedef struct
{
  vertptr vertex1,vertex2;
  struct loopsegtype *loopseg1,*loopseg2;
  float dihedral_angle;
} edgestruct,*edgeptr;

typedef struct loopsegtype
{
  vertptr vert;
  edgeptr edge;
  struct facetype *parentface;
} loopsegstruct,*loopsegptr;

typedef struct facetype
{
  int numloopsegs;
  loopsegptr loopsegs;
  struct objtype *parentobj;
  vec4d norm;
} facestruct,*faceptr;

typedef struct objtype
{
  int numfaces;
  faceptr faces;
  int numedges;
  edgeptr edges;
  int numverts;
  vertptr vertices;
  matrix4x4 xform,invxform;
} objstruct,*objptr;

#endif /* VT_HAND_MODEL_TYPES_H */

