#ifndef VT_HAND_MODEL_TYPES_H
#define VT_HAND_MODEL_TYPES_H
/*  FILE: vt_read_object-types.h                                    */
/*                                                                  */
/*      type declarations and constant definitions for              */
/*      vt_read_object.c: routines for reading in and               */
/*      building 3D object boundary representation                  */
/*                                                                  */
/*  ==============================================================  */
/*  --  Copyright (C) 1992, 1993 Virtual Technologies           --  */
/*  --  Copyright (C) 1987,88,89,90,91,92 Larry Edwards         --  */
/*  ==============================================================  */

#include <vrj/vrjConfig.h>
#include <vrj/Input/Devices/VirtualTechnologies/vt_math3d.h>

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

