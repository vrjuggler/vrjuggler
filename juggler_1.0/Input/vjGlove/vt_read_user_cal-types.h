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
/*  Type declarations for vt_read_user_cal.c                    */
/*                                                              */
/*  ==========================================================  */
/*  --  Copyright (C) Virtual Technologies 1990,91,92,93    --  */
/*  --                                                      --  */
/*  --    Author:  William L. Chapin                        --  */
/*  ==========================================================  */

#ifndef VT_READ_USER_CAL_TYPES_H
#define VT_READ_USER_CAL_TYPES_H

#include <vjConfig.h>
#include <Input/vjGlove/vt_math3d.h>
#include <Input/vjGlove/vt_glove-types.h>

typedef struct
{
  int fingers,joints;
  Boolean right_hand;
  vec3d geom[MAX_SENSOR_GROUPS][MAX_GROUP_VALUES];
  vec3d pisiform;
  float knuckle_radius[MAX_SENSOR_GROUPS][MAX_GROUP_VALUES];
  vec3d radi_ulna[8], forearm[8];
  vec3d thumb_roll_axis;
  vec3d thumb_mcp_axis;
  float thumb_roll;
  float hand_roll;
  vec3d wrist_offset;		/* offset of wrist from tracker receiver */
} UserGeometryStruct,*UserGeometry;

#endif /* VT_READ_USER_CAL_TYPES_H */
