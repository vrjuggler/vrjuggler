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


#ifndef _VJ_COORD_
#define _VJ_COORD_
//#pragma once

#include <vjConfig.h>
#include <Math/vjVec3.h>

namespace vrj
{
   
class Matrix;

//---------------------------------------------------
//: Spatial coordinate with position and orientation
//
// Position is XYZ <br>
// Orientation is XYZ Euler angles <br>
//!PUBLIC_API:
//---------------------------------------------------
class VJ_CLASS_API Coord
{
public:
   Coord()
   {;}

   //: Construct from a matrix
   //! ARGS: mat - A matrix used to construct the coord
   //! POST: this is constructed with pos and orientation
   //+       specified by the matrix.
   Coord(Matrix mat);

public:
   Vec3   pos;        //: The position    - XYZ position
   Vec3   orient;     //: The orientation - XYZ Euler angles
};

};
#endif
