#ifndef VT_VIRTUAL_HAND_TYPES_H
#define VT_VIRTUAL_HAND_TYPES_H
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
/*                                                              */
/*  type declarations and constant defintions for               */
/*  vt_virtual_hand.c                                           */
/*                                                              */
/*  The VirtualHand data structure encapsulates tracker and     */
/*  glove data.                                                 */
/*                                                              */
/*  ==========================================================  */
/*  --  Copyright (C) 1992,93 Virtual Technologies          --  */
/*                                                              */
/*  --    Author: Larry Edwards                             --  */
/*  ==========================================================  */

#include <vjConfig.h>
#include <ctype.h>

#include <Input/vjGlove/vt_read_user_cal-types.h>
#include <Input/vjGlove/vt_math3d.h>
#include <Input/vjGlove/vt_init.h>
#include <Input/vjGlove/vt_glove-types.h>
#include <Input/vjGlove/vt_types.h>	/* Included for Boolean declaration */
#include <Input/vjGlove/vt_read_hand_model-types.h>


typedef struct
{
  int tracker_index,glove_index;
  CbGlove glove;
  UserGeometry user;
} VirtualHandPrivateStruct,*VirtualHandPrivate;

typedef struct _VirtualHand
{
  struct _VirtualHand *next,*prev;

  unsigned long *timestamp;
  volatile Boolean *right_hand;
  Boolean read_glove;
  int receiver;			/* NOTE: this index is one based..should */
				/* really be private ... */
  volatile float *state_vec;		/* tracker position & orientation */
  volatile float (*joint_angle)[MAX_GROUP_VALUES]; /* glove joint angles */
  volatile short DoneLoading;
  float unflexed_abduction;	/* the unflexed thumb abduction angle */
  matrix4x4 digit_xform[5][3];	/* unflexed transforms to succesive joints */
  unsigned char (*raw_glove_value)[MAX_GROUP_VALUES];

  vec3d (*geom)[MAX_GROUP_VALUES]; /* joint positions with no flex */
  SurfaceModel surface;
  Boolean visibleSurface;
  short wait;

   VirtualHandPrivateStruct *private_data; 
} VirtualHandStruct,*VirtualHand;

#endif				/* VT_VIRTUAL_HAND_TYPES_H */
