#ifndef _VJ_COORD_
#define _VJ_COORD_
#pragma once

#include <vjConfig.h>
#include <Math/vjVec3.h>
class vjMatrix;

//---------------------------------------------------
//: Spatial coordinate with position and orientation
//
// Position is XYZ <br>
// Orientation is XYZ Euler angles <br>
//---------------------------------------------------
class vjCoord
{
public:
   vjCoord()
   {;}

   //: Construct from a matrix
   //! ARGS: mat - A matrix to construct the coord from
   //! POST: this is constrcuted with pos and orientation
   //+       specified by the matrix.
   vjCoord(vjMatrix mat);

public:
   vjVec3   pos;        //: The position    - XYZ position
   vjVec3   orient;     //: The orientation - XYZ Euler angles
};

#endif
