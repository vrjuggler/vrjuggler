#ifndef _VJ_COORD_
#define _VJ_COORD_

#include <vjConfig.h>
#include <iostream.h>
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
   vjCoord(vjMatrix mat);

public:
   vjVec3   pos;        // The position    - XYZ position
   vjVec3   orient;     // The orientation - XYZ Euler angles
};

#endif
