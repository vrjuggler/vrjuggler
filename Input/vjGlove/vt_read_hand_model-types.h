#ifndef VT_READ_HAND_MODEL_TYPES_H
#define VT_READ_HAND_MODEL_TYPES_H
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
/*  type declarations and constant defintions for                   */
/*  vt_read_hand_model.c                                            */
/*                                                                  */
/*  The SurfaceModel structure contains a boundary rep. and the ids */
/*  of associated GL display lists                                  */
/*                                                                  */
/*  ==============================================================  */
/*  --  Copyright (C) 1992, 1993 Virtual Technologies           --  */
/*                                                              --  */
/*  --    Author:  Larry Edwards                                --  */
/*  ==============================================================  */

#include <vjConfig.h>
#include <Input/vjGlove/vt_read_object-types.h>

typedef struct
{
  int palm_id;
  objstruct palm;		/* model of the main body of the hand */
  int forearm_id;
  objstruct forearm;		/* model of the forearm, strangely enough... */
  int digit_id[5][3];
  objstruct digit[5][3];	/* models of all the digits */
} SurfaceModelStruct,*SurfaceModel;

#endif				/* VT_READ_HAND_MODEL_TYPES_H */
