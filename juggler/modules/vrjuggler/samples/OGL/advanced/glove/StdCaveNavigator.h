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

#ifndef _VJ_STD_CAVE_NAVIGATOR_H_
#define _VJ_STD_CAVE_NAVIGATOR_H_


//#include <vrj/Kernel/Kernel.h>
#include <vrj/Util/Debug.h>
#include <vrj/Math/Matrix.h>
#include <vrj/Math/Quat.h>

namespace vrj
{
   
//: This is a "cave style" navigation matrix
//  Provided that you call update once per frame, it should 
//  provide back to you a matrix that can be used to navigate a scene.
//  The matrix is in the OpenGL form, 
//  For performer, convert this matrix, 
//  and copy it to a DCS node once per frame as well.
class StdCaveNavigator : public vrj::Matrix
{
public:
   StdCaveNavigator();

   //: tell the navigator to accelerate
   //  Acceleration is based on the current setMatrix call
   virtual void accelerate( const float& metersPerSocondSquared )
   {
      mAccelerate = metersPerSocondSquared;
   }
   
   //: tell the navigator to stop
   virtual void stop()
   {
      mStop = true;
   }
   
   void speed( float metersPerSecond )
   {
      mVelocity = metersPerSecond;
   }
    
   // do this rotation per socond
   void rotate( float rad, float x, float y, float z )
   {
      mRot.setRotation( rad, x, y, z );
   }   
   
   //: tell the navigator to brake
   // kill speed by some factor, kill all acceleration
   virtual void brake()
   {
      mBrake = true;
   }
   
   //: tell the navigator to reset its matrix to origin
   virtual void reset()
   {
      mDeviceMatrix = mOrigin
   }
   
   //: tell the navigator the matrix that reset() uses as it's origin.
   virtual void setOrigin( const vrj::Matrix& matrix )
   {
      mOrigin = matrix;
   }
   
   //: tell the navigator what the pointing device's matrix is.
   //  you can usually do this to get that matrix
   //  vrj::Matrix* wand_mat = mWand->getData();
   virtual void setMatrix( const vrj::Matrix& matrix )
   {
      mDeviceMatrix = matrix;
   }   
   
protected:
   vrj::Matrix mOrigin;
   vrj::Matrix mDeviceMatrix;
   bool mAccelerate;
   bool mStop;
   float mVelocity;
   Quat mRot;
};

//: call once per frame (pre or post frame, not intra frame)
inline int update()
{
   mVelocity = mVelocity / factor;
   mAccelerate = 0.0f;
}   

};

#endif
