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

//#include <Common.h>
#include <gmtl/Vec.h>
#include <gmtl/Matrix.h>
#include <gmtl/Generate.h>
#include <gmtl/Xforms.h>

#include "navigation.h"

//: a vector pointing forward in our space,
//  useful for getting what direction a device is pointing.
const gmtl::Vec3f TrackedInfo::forwardVec( 0.0f, 0.0f, -1.0f );

//: the origin
const gmtl::Vec3f TrackedInfo::origin( 0.0f, 0.0f, 0.0f );

//: call this once per frame with your tracker's matrix.
void TrackedInfo::updateWithMatrix( const gmtl::Matrix44f& matrix )
{
    // save the old values.
    _rotOld = _rot;

    // get the forward direction that the tracker is pointing.
    // (_vec = matrix * forwardVec)
    gmtl::Vec3f wandPos, wandForward;
    gmtl::xform( wandForward, matrix, forwardVec );
    gmtl::xform( wandPos, matrix, origin );
    _vec = wandForward - wandPos;

    // get the x,y,z rotations of the tracker.
    gmtl::EulerAngleXYZf euler;
    gmtl::setRot( euler, matrix );
    _rot.mData[0] = euler[0];
    _rot.mData[1] = euler[1];
    _rot.mData[2] = euler[2];

    // calculate the new rotational delta
    _rotDelta = _rot - _rotOld;
};

//: default constructor
UserInfo::UserInfo() : _walkingMode(true)
{

}

//: call this once per frame with the tracker's TrackerInfo
//  this will update user data such as position, velocity
//  NOTE: if in "weightless" mode,
//        then pass (0,0,0) in for gravity
void  UserInfo::update( const TrackedInfo& tracker, const gmtl::Vec3f& gravity )
{
    // save the old values.
    _posOld = _pos;
    _rotOld = _rot;

    _updateWithTracker( tracker );
    _updateWithGravity( gravity );
}

void UserInfo::_updateWithTracker( const TrackedInfo& tracker )
{
    //: get the scene's rotation for use in computing tracker vector
    gmtl::Matrix44f sceneRotation;
    gmtl::identity(sceneRotation);
    gmtl::EulerAngleXYZf euler( _rot[0], _rot[1], _rot[2] );
    sceneRotation = gmtl::makeRot<gmtl::Matrix44f>( euler );

    //: transform the tracker vector from cave space to model space.
    gmtl::Vec3f trackerVec;
    gmtl::xform( trackerVec, sceneRotation, tracker.vector() );

    // constrain this vector in XZ plane if in walking mode.
    if (_walkingMode == true)
    trackerVec[1] = 0.0f;

    //: calculate the user's velocity vector (vel = pos/frame)
    //  To get this, you need a unit vector (length 1),
    //    pointing in the direction of the tracker...
    gmtl::normalize(trackerVec);

    // ...then multiply it by the velocity scalar
    //    to get the velocity vector.
    _velocityVec = trackerVec * _velocity;

    //: add the velocity to the user's position.
    //  This will move _pos by _velocity's length, which is the same as _velocityVec length.
    //  NOTE: _velocity is "velocity per frame"
    _pos += _velocityVec;

    //: move user's orientation by the tracker's rotation delta.
    // TODO: you may want to scale this down by some value to prevent really large deltas
    _rot += tracker.rotation() * _angularVelocity;

    // constrain orientation to only yaw.
    // this keeps people from being sick,
    //  but is a poor choice for flight simulators etc..
    _rot[0] = 0;
    _rot[2] = 0;
}

void UserInfo::_updateWithGravity( const gmtl::Vec3f& gravity )
{
    // apply gravity to the position
    // NOTE gravity is in (vel = pos/frame)
    _pos += gravity;
}




//: get the transform to put the scene from the user's point of view
//  from the user's info, calculate, then return, the
//  transform to put the scene into the user's point of view
void  UserInfo::getSceneTransform( gmtl::Matrix44f& sceneMatrix ) const
{
    gmtl::Matrix44f sceneTranslation;
    gmtl::Matrix44f sceneRotation;

    //: set the translation of the scene
    //  if we want to move forward in the scene, then we need to move the scene backwards.
    sceneTranslation = gmtl::makeTrans<gmtl::Matrix44f>( -_pos );

    //: set the rotation of the scene
    //  if we want to move clockwise in the scene,
    //   then we need to move the scene counter-clockwise.
    gmtl::EulerAngleXYZf euler( -_rot[0], -_rot[1], -_rot[2] );
    sceneRotation = gmtl::makeRot<gmtl::Matrix44f>( euler );

    //: translate to your position, then rotate the scene.
    sceneMatrix = sceneRotation * sceneTranslation;
}

//: set the "velocity per frame" once each frame.
//  required - call this before you use any 'update' functions.
void  UserInfo::setVelocity( const float& velocity )
{
    _velocity = velocity;
}

//: set the "angular velocity per frame" once each frame.
//  required - call this before you use any 'update' functions.
// set this value from [0,1]
void  UserInfo::setAngularVelocity( const float& aVelocity )
{
    _angularVelocity = aVelocity;
}
