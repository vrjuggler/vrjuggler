/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#ifndef _MATRIX_NAVIGATOR_H_
#define _MATRIX_NAVIGATOR_H_


//#include <vrj/Kernel/Kernel.h>
#include <vrj/Math/Coord.h>
//#include <vrj/Util/Debug.h>

#include <velocityNav.h>

//: This is a "cave style" navigation matrix
//  Provided that you call update once per frame, it should
//  give back to you a matrix that can be used to navigate a scene.
//  The matrix is in the OpenGL form,
//  For performer, convert this matrix,
//  and copy it to a DCS node once per frame as well.
class CaveNavigator : public vrj::Matrix
{
public:
   CaveNavigator();

   velocityNav* getNavigator()
   { return &mVNav; }

   //: call once per frame (pre or post frame, not intra frame)
   virtual int update();

   //: tell the navigator to rotate
   //  Rotation is based on the current setMatrix call
   virtual void rotate( const bool& state = true )
   {
      mNowRotating = state;
   }

   //: tell the navigator to accelerate forward
   virtual void accelerate( const bool& state = true )
   {
      mNowAccelerating = state;
   }

   //: tell the navigator to stop
   virtual void stop( const bool& state = true )
   {
       mNowStopping = state;
   }

   //: tell the navigator to reverse acceleration
   virtual void reverse( const bool& state = true )
   {
      mNowReversing = state;
   }

   //: tell the navigator to brake (or decelerate)
   virtual void brake( const bool& state = true )
   {
       mNowBraking = state;
   }

   //: tell the navigator to reset its matrix to origin
   virtual void reset( const bool& state = true )
   {
      mNowResetting = state;
   }

   //: tell the navigator the matrix that reset() uses as it's origin.
   virtual void setOrigin( const vrj::Matrix& matrix )
   {
      mVNav.setOrigin( matrix );
   }

   float speed() const
   { return mVNav.getSpeed(); }

   virtual void heading( float& pitch, float& yaw, float& roll )
   {
      mVNav.heading( pitch, yaw, roll );
   }

   enum Gravity
   {
      ON, OFF
   };

   void setGravity( Gravity mode )
   {
      switch (mode)
      {
         case OFF:
            mVNav.setMode( velocityNav::FLY );
            break;
         case ON:
            mVNav.setMode( velocityNav::DRIVE );
            break;
      }
   }

   //: tell the navigator what the pointing device's matrix is.
   //  you can usually do this to get that matrix
   //  vrj::Matrix* wand_mat = mWand->getData();
   virtual void setMatrix( const vrj::Matrix& matrix )
   {
      mDeviceMatrix = matrix;
   }

private:
   velocityNav          mVNav;      // My navigator

protected:
   vrj::Matrix mDeviceMatrix;
   bool     mNowAccelerating;
   bool     mNowRotating;
   bool     mNowStopping;
   bool     mNowBraking;
   bool     mNowReversing;
   bool     mNowResetting;
};



CaveNavigator::CaveNavigator() : vrj::Matrix(), mNowReversing(false), mNowStopping(false), mNowAccelerating( false ), mNowRotating( false )
{
   mVNav.setRotAxis(false, true, false);
   this->setGravity( ON );
}

//: call once per frame (pre or post frame, not intra frame)
int CaveNavigator::update()
{
   mVNav.setRotationalAcceleration( mDeviceMatrix );
   mVNav.update();

   if (true == mNowAccelerating)
   {
      // magic number!  acceleration of 10 units per second
      mVNav.accelerate( vrj::Vec3(0,0,-10.0f) );
   }

   if (true == mNowStopping)
   {
      mVNav.stop();
   }

   if (true == mNowBraking)
   {
      // magic number!  i tweaked it to work well. (dampened stop)
      mVNav.setDamping( 0.85 );
   }

   if (false == mNowStopping && false == mNowBraking)
   {
      // magic number!  total damping (instant stop.)
      mVNav.setDamping( 1.0f );
   }

   if (true == mNowReversing)
   {
      // magic number!  decceleration of .9 units per second
      mVNav.accelerateBackward( 0.9f );
   }

   if (true == mNowResetting)
   {
      mVNav.reset();
   }

   // Set the matrix to the navigation matrix
   vrj::Matrix cur_pos,world_pos;
   cur_pos = mVNav.getCurPos();  // Invert because we want to move the world
   //cerr << "Set Pos: " << Coord(cur_pos).pos << endl;
   world_pos.invert( cur_pos );
   this->copy( world_pos );


   return 1;
}

#endif
