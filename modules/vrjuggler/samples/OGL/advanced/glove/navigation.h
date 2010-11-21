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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/


#ifndef NAVIGATION_H
#define NAVIGATION_H

// this is a really BAD example of navigation.
// it's a hack, and well, it works for this silly little app.
// be warned, it won't be fun to modify.

class TrackedInfo
{
public:
   //: call this once per frame with your tracker's matrix.
   void updateWithMatrix(const gmtl::Matrix44f& matrix);

   const gmtl::Vec3f& vector() const
   {
      return mVec;
   }

   const gmtl::Vec3f& rotDelta() const
   {
      return mRotDelta;
   }

   const gmtl::Vec3f& rotation() const
   {
      return mRot;
   }

private:
   gmtl::Vec3f mVec, mRot, mRotOld;
   gmtl::Vec3f mRotDelta;

   //: a vector pointing forward in our space,
   //  useful for getting what direction a device is pointing.
   static const gmtl::Vec3f mForwardVec;

   //: the origin
   static const gmtl::Vec3f mOrigin;
};

class UserInfo
{
public:
   //: default constructor
   UserInfo();

   //: set the "velocity per frame" once each frame.
   //  required - call this before you use any 'update' functions.
   void setVelocity(const float velocity);

   //: set the "angular velocity per frame" once each frame.
   //  required - call this before you use any 'update' functions.
   //  give - aVelocity, a value from [0,1]
   void setAngularVelocity(const float aVelocity);

   //: call this once per frame with the tracker's TrackerInfo
   //  this will update user data such as position, velocity
   //  NOTE: if in "weightless" mode,
   //        then pass (0,0,0) in for gravity
   void update(const TrackedInfo& tracker, const gmtl::Vec3f& gravity);

   //: get the transform to put the scene from the user's point of view
   //  from the user's info, calculate, then return, the
   //  transform to put the scene into the user's point of view
   void getSceneTransform(gmtl::Matrix44f& sceneMatrtix) const;

   void move(const gmtl::Vec3f& dist)
   {
      move(dist[0], dist[1], dist[2]);
   }

   void move(const float a, const float b, const float c)
   {
      mPos[0] += a;
      mPos[1] += b;
      mPos[2] += c;
   }

   void reset()
   {
      mPos.set(0.0f, 0.0f, 0.0f);
      mPosOld.set(0.0f, 0.0f, 0.0f);
      mRot.set(0.0f, 0.0f, 0.0f);
      mRotOld.set(0.0f, 0.0f, 0.0f);
      mVelocity = 0.0f;
      mVelocityVec.set(0.0f, 0.0f, 0.0f);
      mAngularVelocity = 0.0f;
   }

   bool& walk()
   {
      return mWalkingMode;
   }

private:

   void updateWithTracker(const TrackedInfo& tracker);
   void updateWithGravity(const gmtl::Vec3f& gravity);

   // current and old position
   gmtl::Vec3f mPos, mPosOld;

   // current and old rotations
   gmtl::Vec3f mRot, mRotOld;

   // velocity vector
   gmtl::Vec3f mVelocityVec;

   // velocity per frame
   float mVelocity;

   // angular velocity
   float mAngularVelocity;

   // are we in walking or weightless mode?
   bool mWalkingMode;
};

#endif
