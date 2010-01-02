/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/
#ifndef _VELOCITY_NAV_H
#define _VELOCITY_NAV_H

#include <navigator.h>
#include <vector>
#include <vpr/Util/Interval.h>
#include <gadget/Type/PositionInterface.h>
//#include "StopWatch.h"

class velocityNav : public navigator
{
public:
   enum navMode { DRIVE=0, FLY=1 };
   velocityNav();

   // Empty virtual destructor to make GCC happy.
   virtual ~velocityNav (void) {
      /* Do nothing. */ ;
   }

   // Update the interaction state
   virtual void updateInteraction();

   // Update the navigation position
   virtual void update();

   // Set the action buttons that can be used
   void setActionButtons(std::vector<std::string> action_btn_names);

   // Set the name of the pos device that is used in navigation
   void setNavPosControl(std::string wand_dev);

   // Specify the combination of keys necessary to trigger the given state
   void setAccelForwardActionCombo(std::vector<ActionState> combo)
   {
      mAccelForwardCombo = combo;
   }

   void setBrakeActionCombo(std::vector<ActionState> combo)
   {
      mBrakeCombo = combo;
   }

   void setStopActionCombo(std::vector<ActionState> combo)
   {
      mStopCombo = combo;
   }

   void setRotationActionCombo(std::vector<ActionState> combo)
   {
      mRotateCombo = combo;
   }

   void setResetActionCombo(std::vector<ActionState> combo)
   {
      mResetCombo = combo;
   }

   // pass in velocity to add per second
   void setAccel(const float& vel_per_sec)
   {
      mAcceleration = vel_per_sec;
   }

   // set the highest velocity this nav can achieve.
   // default is 35
   void setMaxVelocity(const float& velocity = 50.0f)
   {
      mMaxVelocity = velocity;
   }

   //: arbitrary acceleration in any direction.
   //  nice for straefing, rising or diving. :)
   //  accel = velocity per second
   void accelerate(const gmtl::Vec3f& accel);

   //: zero all velocity
   void stop();

   // TODO: add other directions to accelerate. (since it's hard coded to
   // 0,0,-x in updateInteraction)
   bool acceleratingForward()
   {
      return mAcceleratingForward;
   }

   bool braking()
   {
      return mBraking;
   }

   bool rotating()
   {
      return mRotating;
   }

   bool stopping()
   {
      return mStopping;
   }

   bool resetting()
   {
      return mResetting;
   }

   //: damping [0...1], where 0 stops immediately, .99 brakes very slowly
   //  default is no damping (1.0f)
   void setDamping( const float& damping = 1.0f );

   void setMode(navMode new_mode)
   {
      mMode = new_mode;
   }

   virtual gmtl::Vec3f getVelocity() const
   {
      return mVelocity;
   }

   virtual float  getSpeed() const
   {
      return gmtl::length(mVelocity);
   }

   // resets the navigator's matrix to the origin (set by setOrigin)
   virtual void reset();

protected:
   // set the rotational acceleration matrix
   // only rotational components of this matrix are used.
   void setRotationalAcceleration(const gmtl::Matrix44f& mat)
   {
      mRotationalAcceleration = mat;
   }

   // Rotate the environment
   void scaled_rotate(gmtl::Matrix44f rot_mat);

   // util func... used to convert gravity from meters...
   void meters2feet(const gmtl::Vec3f& gravityInMeters,
                    gmtl::Vec3f& gravityInFeet)
   {
      float meters_to_feet = (100.0f / 0.3048f) * 0.01f;
      //float convtofeet = (meters / .3048 * 100.0f) * .01;
      gravityInFeet = gravityInMeters * meters_to_feet;
   }

   enum Units
   {
      METERS, FEET
   };

   void setUnits(Units units = FEET)
   {
      mUnits = units;
   }

private:
   gmtl::Vec3f mVelocity;
   gmtl::Vec3f mVelocityFromGravityAccumulator;
   float    mMaxVelocity;
   gmtl::Matrix44f mRotationalAcceleration;
   float    mDamping;
   float    mAcceleration;

   // digitalInterfaces
   std::vector<gadget::DigitalInterface*> mActionButtons;
   gadget::PositionInterface mNavWand;

   // State combos
   std::vector<ActionState> mAccelForwardCombo;
   std::vector<ActionState> mBrakeCombo;
   std::vector<ActionState> mStopCombo;
   std::vector<ActionState> mRotateCombo;
   std::vector<ActionState> mResetCombo;

   // State flags for the navigation
   bool  mBraking;
   bool  mAcceleratingForward;
   bool  mRotating;
   bool  mStopping;
   bool  mResetting;

   float mTimeDelta;
   vpr::Interval mLastTimeStamp;

   Units       mUnits;
   //StopWatch   stopWatch;
   navMode     mMode;
   int         mTimeHack;
};


#endif
