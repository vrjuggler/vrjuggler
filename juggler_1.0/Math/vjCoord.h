/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


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
