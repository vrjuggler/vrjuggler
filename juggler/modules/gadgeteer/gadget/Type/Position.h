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

/////////////////////////////////////////////////////////////////////////
//
// positional tracker base class
//
////////////////////////////////////////////////////////////////////////

#ifndef _GADGET_POSITION_H_
#define _GADGET_POSITION_H_

#include <gadget/gadgetConfig.h>
#include <typeinfo>
#include <gadget/Type/Input.h>
#include <gadget/Type/PositionData.h>
#include <vrj/Math/Vec3.h>
#include <vrj/Math/Matrix.h>
#include <jccl/Plugins/PerformanceMonitor/TimeStamp.h>

namespace gadget
{

typedef struct {
  vrj::Vec3 pos, orient;   // orient - EulerZYX , 0-Z, 1-Y, 2-X ???
  //float x,y,z,azi,elev,roll;
  } POS_DATA;

//-----------------------------------------------------------------------------------
//: Position is the abstract base class that devices with digital data derive from.
//
//  Position is the base class that digital devices must derive from.  Position
//  inherits from Input, so it has pure virtual function constraints from
//  Input in the following functions: StartSampling,StopSampling,Sample,
//  and UpdateData.
//
//  Position objects have the ability to convert from the tracker's coord system
//  to the Juggler coordinate system.
//
//  Position adds one new pure virtual function, GetPosData for retreiving
//  the positional data, similar to the addition for Analog and Digital.
//-----------------------------------------------------------------------------------
//!PUBLIC_API:
class Position
{
public:

    //: Constructor
    Position();

    //: Destructor
    virtual ~Position();

    virtual bool config(jccl::ConfigChunkPtr c);

    /** Get Positional data. */
    virtual PositionData* getPositionData (int devNum = 0) = 0;

public:
    

    vrj::Matrix xformMat;   // The total xform matrix.  T*R  NOTE: Used to move from trk coord system to Juggler coord system
    vrj::Matrix rotMat;     // Only the rotation matrix
};

};

#endif
