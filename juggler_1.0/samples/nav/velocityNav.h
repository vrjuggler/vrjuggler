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

   virtual void update();

   // set the rotational acceleration matrix
   // only rotational components of this matrix are used.
   void setRotationalAcceleration( const vjMatrix& mat )
   {
      mRotationalAcceleration = mat;
   }


   // set the highest velocity this nav can achieve.
   // default is 35
   void setMaxVelocity( const float& velocity = 35.0f )
   {
      mMaxVelocity = velocity;
   }

   // add forward velocity  (accelerate)
   void accelerateForward( const float& accel );

   // add backward velocity (decelerate/reverse)
   void accelerateBackward( const float& accel );

   // add left velocity (Strafe left)
   void accelerateLeft( const float& accel );

   // add right velocity (strafe right)
   void accelerateRight( const float& accel );

   // add up velocity (rise/climb)
   void accelerateUp( const float& accel );

   // add down velocity (fall/dive)
   void accelerateDown( const float& accel );

   //: zero all velocity
   void stop();

   //: damping [0...1], where 0 stops immediately, .99 brakes very slowly
   //  default none (1.0f)
   void setDamping( const float& damping = 1.0f );

   void setMode(navMode new_mode)
   { mMode = new_mode; }

   //: yes! you can add any number of colliders!
   void addCollider( collider* val )
   { 
      mCollider.push_back( val );
   }
  
   // query the current speed.
   float speed() const;

   // query the current forward speed.
   const float& zspeed() const;

   // query the current forward speed.
   const float& xspeed() const;

   // query the current forward speed.
   const float& yspeed() const;
   
   //: set the matrix 
   virtual void setOrigin( const vjMatrix& matrix )
   {
      mOrigin = matrix;
   }   
   
   // resets the navigator's matrix to the origin (set by setOrigin)
   virtual void reset()
   {
      mVelocity.set( 0,0,0 );
      mVelocityFromGravityAccumulator.set( 0,0,0 );
      mRotVelocity = 0;
      mRotationalAcceleration.makeIdent();
      this->setCurPos( mOrigin );
   }
   
protected:
   // check if we are hitting anything current, and correct for it
   bool correctPosition(vjVec3 prev_delta);

   // Rotate the environment
   void rotate(vjMatrix rot_mat);

private:
   vjVec3 mVelocity;
   vjVec3 mVelocityFromGravityAccumulator;
   float mMaxVelocity;
   float mRotVelocity, mMaxRotVelocity;
   vjMatrix mRotationalAcceleration;
   float mDamping;
   
   collider* mCorrectingCollider;
   collider* mGravCollider;
   StopWatch stopWatch;
   navMode  mMode;
   int mTimeHack;
   
   vjMatrix mOrigin;
   
   std::vector<collider*> mCollider;
};

velocityNav::velocityNav() : mOrigin(), mTimeHack(0), mDamping( 1.0f ), mRotVelocity( 0.0f ),
   mMaxRotVelocity( 1.0f ),
   mVelocity( 0.0f, 0.0f , 0.0f ),
   mMode( velocityNav::GROUND ),
   mMaxVelocity( 55.0f )
{
   stop();
   stopWatch.start();
   stopWatch.stop();
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
   const vjVec3   gravity( 0.0f, -9.8f, 0.0f );
   
   // get the rotation that takes the scene from localspace (user) to modelspace
   vjMatrix currentRotation = mCurPos;
   currentRotation( 0, 3 ) = 0.0f;
   currentRotation( 1, 3 ) = 0.0f;
   currentRotation( 2, 3 ) = 0.0f;  // zero out the translation...
   
   // get the inverse, we'll need it.
   vjMatrix IcurrentRotation;
   IcurrentRotation.invert( currentRotation );
   
   
   if (mAllowRot)
   {
      this->rotate( mRotationalAcceleration );
   }
   
   if (mMode == GROUND)
   {
      // get the axes of the tracking/pointing device
      // NOTE: constrain to the Y axis in GROUND mode (no flying/hopping or diving faster than gravity allows)
      vjMatrix constrainedToY;
      mRotationalAcceleration.constrainRotAxis( false, true, false, constrainedToY );
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
      //cerr << "Added trans: trans: " << forward << "*" << mTransVelocity * stopWatch.timeInstant<< " ==> move: " << move << endl;
   }

   // add in damping (if any)
   mVelocity[2] += (-mVelocity[2] * (1.0f - mDamping));
   mVelocity[1] += (-mVelocity[1] * (1.0f - mDamping));
   mVelocity[0] += (-mVelocity[0] * (1.0f - mDamping));

   //cout<<"Damping: "<<mDamping<<", Velocity: "<<mVelocity<<"\n"<<flush;
   
   // navigation just calculated navigator's next velocity
   // now convert accumulated velocity to distance traveled this frame (by cancelling out time)
   // NOTE: this is not the final distance, since we still have to do collision correction.
   vjVec3 distanceToMove = velocityAccumulator * stopWatch.timeInstant;
   
      
   //////////////////////////////////
   // do collision corrections...
   //////////////////////////////////
   // convert the info to model coordinates, since that's what the nav routines take.
   
   // distanceToMove is in local (user) coordinates, 
   // move it to model coordinates (since we're testing against the model)
   vjVec3 mcDist2Move;
   mcDist2Move.xformFull( currentRotation, distanceToMove );
   
   // mCurPos is already in model coordinates, since it is used to move the geometry from localspace to modelspace
   vjVec3 whereYouAre, whereYouWantToBe;
   vjCoord cur_pos(mCurPos);
   whereYouAre = cur_pos.pos;

   // Find out where you want to move to (modelspace = modelspace + modelspace::: no conversion nessesary)
   //whereYouWantToBe = whereYouAre + mcDist2Move;
   
   // the collider will return a correction vector in modelspace coordinates
   // add this to correct your distance requested to move.
   vjVec3 correction;
   //vjVec3 totalCorrections;
   
   
   ////////////////////////////////////////////////////////////
   // Test for collisions with all registered colliders
   ////////////////////////////////////////////////////////////
   for (int x = 0; x < mCollider.size(); ++x)
   {
      // If collision, then ...
      if (mCollider[x]->testMove( whereYouAre, mcDist2Move, correction ))
      {
         // ... apply the correction.
         mcDist2Move += correction;
         //totalCorrections += correction;

         // HACK(!) - use the correction vectors to decide how much velocity from gravity to remove
         //        here i'm just zeroing it out... sometimes many times per frame!!! (real bad)
         //    this should really only be affected by the Y component of the correction vector.
         mVelocityFromGravityAccumulator.set( 0.0f, 0.0f, 0.0f );
      } 
   }

   
   // HACK(!) - yes, this was my (small) attempt to fix the hack above (depicted with a (!))
   //
   // velocity from gravity is affected by all collisions in the direction of the gravity vector.
   // reduce the velocity from gravity with these collisions.
   // TODO: this is hackish - do real elastic colisions
   //vjVec3 localSpaceCorrections;
   //localSpaceCorrections.xformFull( IcurrentRotation, totalCorrections );
   //vjVec3 subtractfromGravVel;
   //subtractfromGravVel[2] = (localSpaceCorrections[2] / stopWatch.timeInstant);
   //mVelocityFromGravityAccumulator += subtractfromGravVel;
   
   
   // the navTranslate() function requires a translation in local coordinates
   // move the vector back from modelspace to localspace
   distanceToMove.xformFull( IcurrentRotation, mcDist2Move );
   this->navTranslate( distanceToMove );
}

void velocityNav::rotate(vjMatrix rot_mat)
{
   //: Confused by quaternions???
   //  All this does is scale the angle of rotation back by about %4
   //  Normally this is done to the euler angles, but since we're just using rotations, 
   //  this was the "easiest" way to do it.   Quaternions allow you to interpolate, 
   //  thus getting a scale factor of the original (with respect to identity)
   /////////////////
   
   // I should scale the rotation matrix here
   // I should also compute a relative rotation from the head here as well
   vjMatrix    transform, transformIdent;
   vjQuat      source_rot, goal_rot, slerp_rot;

   // Create an identity quaternion to rotate from
   transformIdent.makeIdent();         
   
   // Create the goal rotation quaternion   
   source_rot.makeQuat(transformIdent);
   goal_rot.makeQuat(rot_mat);                

   // If we don't have two identity matrices
   if(transformIdent != rot_mat)              
   {
      slerp_rot.slerp( 0.04, source_rot, goal_rot );    // Transform part way there
      transform.makeQuaternion( slerp_rot );            // Create the transform matrix to use
   }
   else
   {
      transform.makeIdent();
   }
   navigator::navRotate(transform);
}

// check if we are hitting anything current, and correct for it
bool velocityNav::correctPosition( vjVec3 prev_delta )
{
   //vjVec3 zero_trans(0.0f,0.0f,0.0f);
   vjVec3 correction;
   vjCoord cur_pos(mCurPos);
   
   // TODO: Kevin....
   vjMatrix rotMat = mCurPos;
   rotMat( 0, 3 ) = 0.0f;
   rotMat( 1, 3 ) = 0.0f;
   rotMat( 2, 3 ) = 0.0f;
   prev_delta.xformVec( rotMat, prev_delta );
   

   if ((mCorrectingCollider != NULL) &&
      mCorrectingCollider->testMove( cur_pos.pos, prev_delta, correction, true ))
   {
      navigator::navTranslate( correction );
      return true;
   }
   return false;
}


void velocityNav::accelerateForward( const float& accel )
{
   // add on the velocity
   if (mVelocity[2] < mMaxVelocity)
      mVelocity[2] += (-accel) * stopWatch.timeInstant;
}

void velocityNav::accelerateBackward( const float& accel )
{
   // add on the velocity
   if (mVelocity[2] > (-mMaxVelocity))
      mVelocity[2] += accel * stopWatch.timeInstant;
}

void velocityNav::accelerateRight( const float& accel )
{
   // add on the velocity
   if (mVelocity[2] < mMaxVelocity)
      mVelocity[0] += accel * stopWatch.timeInstant;
}

void velocityNav::accelerateLeft( const float& accel )
{
   // add on the velocity
   if (mVelocity[2] > (-mMaxVelocity))
      mVelocity[0] += (-accel) * stopWatch.timeInstant;
}

void velocityNav::accelerateUp( const float& accel )
{
   // add on the velocity
   if (mVelocity[1] < mMaxVelocity)
      mVelocity[1] += accel * stopWatch.timeInstant;
}

void velocityNav::accelerateDown( const float& accel )
{
   // add on the velocity
   if (mVelocity[1] > (-mMaxVelocity))
      mVelocity[1] += (-accel) * stopWatch.timeInstant;
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

// query the current speed.
float velocityNav::speed() const
{
   vjVec3 speed( mVelocity[2], mVelocity[0], mVelocity[1] );
   float velocity = speed.length();
   return velocity;
}

// query the current forward speed.
const float& velocityNav::zspeed() const
{
   return mVelocity[2];
}

// query the current forward speed.
const float& velocityNav::xspeed() const
{
   return mVelocity[0];
}

// query the current forward speed.
const float& velocityNav::yspeed() const
{
   return mVelocity[1];
}
