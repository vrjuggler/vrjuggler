/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */

#include <vjQuat.h>
#include <navigator.h>
#include <collider.h>
#include <vector>
#include "StopWatch.h"

class velocityNav : public navigator
{
public:
   enum navMode { GROUND=0, FLY=1 };
   velocityNav();

   // Update the interaction state
   virtual void updateInteraction();

   // Update the navigation position
   virtual void update();

   // Set the action buttons that can be used
   void setActionButtons(std::vector<std::string> action_btn_names);

   // Set the name of the pos device that is used in navigation
   void setNavPosControl(std::string wand_dev);

   // Specify the combination of keys necessary to trigger the given state
   void setAccelActionCombo(std::vector<ActionState> combo) { mAccelCombo = combo; }
   void setBrakeActionCombo(std::vector<ActionState> combo) { mBrakeCombo = combo; }
   void setStopActionCombo(std::vector<ActionState> combo) { mStopCombo = combo; }
   void setRotationActionCombo(std::vector<ActionState> combo) { mRotateCombo = combo; }
   void setResetActionCombo(std::vector<ActionState> combo) { mResetCombo = combo; }

   // set the highest velocity this nav can achieve.
   // default is 35
   void setMaxVelocity( const float& velocity = 35.0f )
   { mMaxVelocity = velocity; }

   void accelerate(const vjVec3& accel);

   //: zero all velocity
   void stop();

   bool braking() { return mBraking; }
   bool accelerating() { return mAccelerating;}
   bool rotating() { return mRotating;}
   bool stopping() { return mStopping;}
   bool reseting() { return mReseting;}

   //: damping [0...1], where 0 stops immediately, .99 brakes very slowly
   //  default is no damping (1.0f)
   void setDamping( const float& damping = 1.0f );

   void setMode(navMode new_mode)
   { mMode = new_mode; }

   vjVec3 getVelocity() const { return mVelocity; }
   float  getSpeed() const { return mVelocity.length(); }

   // resets the navigator's matrix to the origin (set by setOrigin)
   virtual void reset();

protected:
   // set the rotational acceleration matrix
   // only rotational components of this matrix are used.
   void setRotationalAcceleration( const vjMatrix& mat )
   { mRotationalAcceleration = mat;}

   // Rotate the environment
   void scaled_rotate(vjMatrix rot_mat);

private:
   vjVec3 mVelocity;
   vjVec3 mVelocityFromGravityAccumulator;
   float    mMaxVelocity;
   vjMatrix mRotationalAcceleration;
   float    mDamping;

   // digitalInterfaces
   std::vector<vjDigitalInterface*> mActionButtons;
   vjPosInterface mNavWand;

   // State combos
   std::vector<ActionState> mAccelCombo;
   std::vector<ActionState> mBrakeCombo;
   std::vector<ActionState> mStopCombo;
   std::vector<ActionState> mRotateCombo;
   std::vector<ActionState> mResetCombo;

   // State flags for the navigation
   bool  mBraking;
   bool  mAccelerating;
   bool  mRotating;
   bool  mStopping;
   bool  mReseting;

   StopWatch   stopWatch;
   navMode     mMode;
   int         mTimeHack;

   vjMatrix mOrigin;
};

velocityNav::velocityNav() :
   mOrigin(),
   mTimeHack(0),
   mDamping( 1.0f ),
   mVelocity( 0.0f, 0.0f , 0.0f ),
   mMode( velocityNav::GROUND ),
   mMaxVelocity( 55.0f )
{
   stop();
   stopWatch.start();
   stopWatch.stop();

   setNavPosControl("VJWand");         // Initialize wand device

   std::vector<std::string> def_btns(3);
   def_btns[0] = std::string("VJButton0");
   def_btns[1] = std::string("VJButton1");
   def_btns[2] = std::string("VJButton2");
   setActionButtons(def_btns);

   std::vector<navigator::ActionState> accel_combo(3), brake_combo(3), stop_combo(3), rotate_combo(3), reset_combo(3);
   accel_combo[0] = ON;    accel_combo[1] = OFF;   accel_combo[2] = OFF;    // 100
   brake_combo[0] = OFF;   brake_combo[1] = ON;    brake_combo[2] = OFF;    // 010
   stop_combo[0] = ON;     stop_combo[1] = ON;     stop_combo[2] = OFF;     // 110
   rotate_combo[0] = OFF;  rotate_combo[1] = OFF;  rotate_combo[2] = ON;    // 001
   reset_combo[0] = OFF;   reset_combo[1] = ON;    reset_combo[2] = ON;     // 011

   setAccelActionCombo(accel_combo);
   setBrakeActionCombo(brake_combo);
   setStopActionCombo(stop_combo);
   setRotationActionCombo(rotate_combo);
   setResetActionCombo(reset_combo);
}


// Set the action buttons that can be used
void velocityNav::setActionButtons(std::vector<std::string> action_btn_names)
{
   // Make sure list is long enough
   while(action_btn_names.size() > mActionButtons.size())
   {
      mActionButtons.push_back(new vjDigitalInterface());
   }

   // Update all the entries
   for(int i=0;i<action_btn_names.size();i++)
   {
      mActionButtons[i]->init(action_btn_names[i]);
   }
}

// Set the name of the pos device that is used in navigation
void velocityNav::setNavPosControl(std::string wand_dev)
{
   vjDEBUG(vjDBG_ALL,0) << clrOutNORM(clrGREEN,"Setting Nav Pos Control: ") << wand_dev << endl << vjDEBUG_FLUSH;
   mNavWand.init(wand_dev);
}

void velocityNav::updateInteraction()
{
   mAccelerating = checkForAction(mActionButtons,mAccelCombo);
   mBraking = checkForAction(mActionButtons,mBrakeCombo);
   mStopping = checkForAction(mActionButtons,mStopCombo);
   mReseting = checkForAction(mActionButtons,mResetCombo);
   mRotating = checkForAction(mActionButtons,mRotateCombo);

   // Braking
   if(mBraking)
   { mDamping = 0.85; }
   else
   { mDamping = 1.0f; }

   // Stopping -- NOTE: This must go after braking
   if(mStopping)
   {
      setDamping(0.0f);
   }

   // Accelerate
   if(mAccelerating)
   { accelerate(vjVec3(0,0,-1)); }

   // Reseting
   if(mReseting)
      navigator::reset();

   // Rotating
   vjMatrix rot_mat = *(mNavWand->getData());
   rot_mat.setTrans(0,0,0);

   setRotationalAcceleration(rot_mat);
}

void velocityNav::update()
{
   stopWatch.stop();
   stopWatch.start();

   if (mTimeHack < 3)
   {
      stopWatch.timeInstant = 0;
      ++mTimeHack;
   }

   //////////////////////////////////
   // do navigations...
   //////////////////////////////////
   // Define axes, in Juggler/OpenGL coordinate system (right handed)
   vjVec3         trackerZaxis(0.0f, 0.0f, 1.0f);
   vjVec3         trackerXaxis(1.0f, 0.0f, 0.0f);
   vjVec3         trackerYaxis(0.0f, 1.0f, 0.0f);
   const vjVec3   gravity( 0.0f, -9.8f, 0.0f ); //9.8 m/s (METERS), this should be differnet for apps in FEET!

   // get the rotation that takes the scene from localspace (user) to modelspace
   //vjMatrix currentRotation = mCurPos;
   //currentRotation.setTrans( 0.0f, 0.0f, 0.0f ); // zero out the translation...

   // get the inverse, we'll need it.
   //vjMatrix IcurrentRotation;
   //IcurrentRotation.invert( currentRotation );

   if ((mAllowRot) && (mRotating))
   {
      this->scaled_rotate( mRotationalAcceleration );     // Interpolates the rotation, and updates mCurPos matrix
   }

   if (mMode == GROUND)
   {
      // get the axes of the tracking/pointing device
      // NOTE: constrain to the Y axis in GROUND mode (no flying/hopping or diving faster than gravity allows)
      vjMatrix constrainedToY;
      //mRotationalAcceleration.constrainRotAxis( false, true, false, constrainedToY );
      constrainedToY.makeXYZEuler(0,mRotationalAcceleration.getYRot(),0);     // Only allow Yaw (rot y)

      trackerZaxis.xformVec( constrainedToY, trackerZaxis);
      trackerXaxis.xformVec( constrainedToY, trackerXaxis);
      trackerYaxis.xformVec( constrainedToY, trackerYaxis);
   }
   else
   {
      // get the axes of the tracking/pointing device
      trackerZaxis.xformVec( mRotationalAcceleration, trackerZaxis);
      trackerXaxis.xformVec( mRotationalAcceleration, trackerXaxis);
      trackerYaxis.xformVec( mRotationalAcceleration, trackerYaxis);
   }

   // this is used to accumulate velocity added by navigation
   vjVec3   velocityAccumulator( 0.0f, 0.0f, 0.0f );

   if (mMode == GROUND)
   {
      // add the velocity this timeslice/frame by the acceleration from gravity.
      velocityAccumulator += mVelocityFromGravityAccumulator;

      // recalculate the current downward velocity from gravity.
      // this vector then is accumulated with the rest of the velocity vectors each frame.
      mVelocityFromGravityAccumulator += (gravity * stopWatch.timeInstant);
   }
   if (mAllowTrans)
   {
      // add velocity with respect to the tracking/pointing device
      velocityAccumulator += (trackerZaxis * mVelocity[2]); // forward/reverse   |reletive to tracker
      velocityAccumulator += (trackerXaxis * mVelocity[0]); // strafe            |reletive to tracker
      velocityAccumulator += (trackerYaxis * mVelocity[1]); // rise/dive         |reletive to tracker
   }

   // Get rid of some velocity due to damping in the system
   mVelocity -= (mVelocity * (1.0f - mDamping));

   // navigation just calculated navigator's next velocity
   // now convert accumulated velocity to distance traveled this frame (by cancelling out time)
   // NOTE: this is not the final distance, since we still have to do collision correction.
   vjVec3 distanceToMove = velocityAccumulator * stopWatch.timeInstant;

   // --- TRANSLATION and COLLISION DETECTION --- //
   bool     did_collide;               // Did we collide with anything
   vjVec3   total_correction;          // The total correction on the movement (in modelspace)
   navTranslate(distanceToMove,did_collide,total_correction);

   if(did_collide)      // If we hit something, stop falling
   {
      mVelocityFromGravityAccumulator.set( 0.0f, 0.0f, 0.0f );
   }
}

void velocityNav::scaled_rotate(vjMatrix rot_mat)
{
   //: Confused by quaternions???
   //  All this does is scale the angle of rotation back by about %4
   //  Quaternions allow you to interpolate between rotations,
   //  thus getting a scale factor of the original (with respect to identity)

   // I should scale the rotation matrix here
   // I should also compute a relative rotation from the head here as well
   vjMatrix    transform, transformIdent;
   vjQuat      source_rot, goal_rot, slerp_rot;

   // Create an identity quaternion to rotate from
   transformIdent.makeIdent();

   // Create the goal rotation quaternion (the goal is the input matrix)
   source_rot.makeQuat(transformIdent);
   goal_rot.makeQuat(rot_mat);

   // If we don't have two identity matrices, then interpolate between them
   if(transformIdent != rot_mat)
   {
      slerp_rot.slerp( 0.04, source_rot, goal_rot );    // Transform part way there
      transform.makeQuaternion( slerp_rot );            // Create the transform matrix to use
   }
   else
   {
      transform.makeIdent();
   }
   navigator::navRotate(transform);                   // update the mCurPos navigation matrix
}


void velocityNav::accelerate(const vjVec3& accel)
{
   if(mVelocity.length() < mMaxVelocity)
   {
      mVelocity += (accel * stopWatch.timeInstant);
   }
}


void velocityNav::stop()
{
   this->setDamping( 0.0f );
}

void velocityNav::setDamping( const float& damping )
{
   mDamping = damping;
   //cout<<"Setting damping coef to: "<<mDamping<<"\n"<<flush;
}

void velocityNav::reset()
{
   mVelocity.set( 0,0,0 );
   mVelocityFromGravityAccumulator.set( 0,0,0 );
   mRotationalAcceleration.makeIdent();
   navigator::reset();
}

