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

#ifndef _VRJ_GLOVE_H_
#define _VRJ_GLOVE_H_
//#pragma once

#include <gad/gadConfig.h>
#include <gad/Type/Input.h>
//#include <gad/Type/DeviceInterface.h>
#include <gad/Type/PositionProxy.h>
#include <vrj/Math/Vec3.h>
#include <vrj/Math/Matrix.h>

// TODO: 2 devs here is a hack until we make
//       the pinch glove act more like the flock of birds.
#define VJ_MAX_GLOVE_DEVS 2   /* The maximum number of gloves per device */

namespace vrj
{

//: This is the data stored about a glove
//
// Desc:
//  The angles are the joint angles for all fingers and the wrist
//  DIJ = Distal Interphalangeal Joint  --- finger tip
//  PIJ = Proximal "              "     --- Middle joint
//  MPJ = Metacarpo "             "     --- closest to hand
//  ABDUCT = spread of fingers
//
// YAW and PITCH apply only to WRIST
//
// xforms transfer you from one coord system to the other
// if the xforms are tied together, then they can return
// complete transformations
//
// NOTE: More docs needed here
//
//!PUBLIC_API:
class GAD_CLASS_API GloveData
{
public:
   enum { NUM_JOINTS = 4, NUM_COMPONENTS = 6 };
   enum GloveJoint
      { MPJ = 0, PIJ = 1, DIJ = 2, ABDUCT = 3, YAW = 0, PITCH = 1};
   enum GloveComponent
      { THUMB = 0, INDEX = 1, MIDDLE = 2, RING = 3, PINKY = 4, WRIST = 5};

public:
   //: Constructor
   //! POST: Initialized with zero values and identities
   GloveData();

    //: Copy Constructor
    //! POST: Initialized with the values from data
    GloveData(const GloveData &data);

   //: From the given angle data, calculate the xforms
   int calcXforms();

public:  // --- I/O Functions --- //
   //: Output the angles
   std::ostream& outputAngles(std::ostream& out) const;
   std::istream& inputAngles(std::istream& in);

public:
   float     angles[NUM_COMPONENTS][NUM_JOINTS];

   //: These are the xforms from TO the coord system of the given joint
   // Ex: xforms[0] ==> <br>
   //     base<b>T</b>mpj mpj<b>T</b>pij pij<b>T</b>dij
   Matrix  xforms[NUM_COMPONENTS][(NUM_JOINTS-1)];

   // Finger params
   //  XXX: Should put better info about hand dimensions in here
   // For now this is the translations FROM the previous joint to
   // the SPECIFIED joint.  In case of (DIJ+1), length to tip of finger
   Vec3    dims[NUM_COMPONENTS][NUM_JOINTS];
};


//------------------------------------------------------------------------
//: This is the abstract base glove class. Derived from Input.
// It specifies the interface to all glove objects in the system.
// VR Juggler will deal only with gloves using this interface.
//
// Description:
//
//-------------------------------------------------------------------------
//!PUBLIC_API:
class GAD_CLASS_API Glove : virtual public Input
{
public:
   Glove();

   virtual ~Glove() {}

   // Let constructor take care of device abilities and init
   virtual bool config(ConfigChunk* chunk)
   {return true;}

public:  // ---- GLOVE INTERFACE ---- //
   //: Return the angle of the given joint.
   // joint is one of a predefined enum type (jointType)
   float getGloveAngle(GloveData::GloveComponent component,
                       GloveData::GloveJoint joint, int devNum);

   //: This returns a vector ponting "out" of the component
   // Can be used for selection, etc.
   Vec3 getGloveVector(GloveData::GloveComponent component, int devNum);

   //: This returns the position of given components.
   // Defaults to returning the palm position.
   // Can also get finger tips.
   Matrix getGlovePos(GloveData::GloveComponent component = GloveData::WRIST, int devNum = 0);

   //: This returns a copy of the glove data struct
   GloveData getGloveData(int devNum);

protected:
   // NOTE: make sure you use the "vjInput::progress" member in the [3] slot
   // here. Then you'll not go off the end of this array.
   // TODO: VJ_MAX_GLOVE_DEVS = 2 here is a hack until we make
   //       the pinch glove act more like the flock of birds.
   GloveData mTheData[VJ_MAX_GLOVE_DEVS][3];

   //: This is the positional proxy of the glove.  It defines the location of the
   // "center" of the glove. "center" could be different for each glove type.
   // TODO: VJ_MAX_GLOVE_DEVS = 2 here is a hack until we make
   //       the pinch glove act more like the flock of birds.
   PositionProxy*  mGlovePos[VJ_MAX_GLOVE_DEVS];

};

};

#endif
