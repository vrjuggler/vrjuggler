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

#ifndef _VRJ_PROJECTION_H_
#define _VRJ_PROJECTION_H_

#include <vrj/vrjConfig.h>
#include <vrj/Math/Matrix.h>
#include <vrj/Display/Frustum.h>
#include <vrj/Util/Debug.h>
#include <vrj/Math/Vec3.h>
#include <vrj/Input/Type/Position.h>

namespace vrj
{

//------------------------------------------------------------------
//: Pure virtual base class for viewport definitions.
//
// Responsible for storing and computing projection
//  information based upon an eye positions
// This class is an abstract base class for other classes
//  that actually compute the projections.
//
// @author Allen Bierbaum
//  Date: 9-8-97
//------------------------------------------------------------------
class VJ_CLASS_API Projection
{
public:
   // Eye and type
   enum
   {LEFT = 1, RIGHT = 2, SURFACE, SIM};

public:
   Projection()
   {
      mType = -1;
      mEye = 0;
      mFocusPlaneDist = 1.0f;
   }

   virtual void config(ConfigChunk* chunk)
   {;}

   void setEye(int _eye)
   { mEye = _eye; }

   int getEye()
   { return mEye;}

   virtual void calcViewMatrix(Matrix& eyePos) = 0;

   //: Helper to the frustum apex and corners in model coordinates
   //!NOTE: This function is meant for debugging purposes
   //!POST: The given vars contain the values of the frustums
   //+ corners in model space.
   void getFrustumApexAndCorners(Vec3& apex, Vec3& ur, Vec3& lr, Vec3& ul, Vec3& ll);

   //: Virtual output oporators.
   // Every class derived from us shoudl just define this, and
   // the opertetor<< will "just work"
   virtual std::ostream& outStream(std::ostream& out);

   friend VJ_API(std::ostream&) operator<<(std::ostream& out, Projection& proj);


public:
   Matrix    mViewMat;
   Frustum   mFrustum;

protected:
   int mEye;
   int mType;

   float       mFocusPlaneDist;     // Basically the distance to the surface.  Needed for drawing.

protected:     // Statics
   static float mNearDist;
   static float mFarDist;    // Near far distances

public:
   static void setNearFar(float near_val, float far_val);
};

};

#endif
