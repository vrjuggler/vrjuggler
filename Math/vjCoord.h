/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */


#ifndef _VJ_COORD_
#define _VJ_COORD_
//#pragma once

#include <vjConfig.h>
#include <Math/vjVec3.h>
class vjMatrix;

//---------------------------------------------------
//: Spatial coordinate with position and orientation
//
// Position is XYZ <br>
// Orientation is XYZ Euler angles <br>
//!PUBLIC_API:
//---------------------------------------------------
class vjCoord
{
public:
   vjCoord()
   {;}

   //: Construct from a matrix
   //! ARGS: mat - A matrix used to construct the coord
   //! POST: this is constructed with pos and orientation
   //+       specified by the matrix.
   vjCoord(vjMatrix mat);

public:
   vjVec3   pos;        //: The position    - XYZ position
   vjVec3   orient;     //: The orientation - XYZ Euler angles
};

#endif
