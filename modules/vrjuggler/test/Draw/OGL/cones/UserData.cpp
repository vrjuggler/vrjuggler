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

#include <iostream>

#include <vrj/Math/Vec3.h>
#include <vrj/Math/Matrix.h>
#include <vrj/Math/Quat.h>
#include <vrj/Util/Debug.h>

#include <UserData.h>


// ----------------------------------------------------------------------------
// Constructor.  Takes the string names of the devices to use.
// ----------------------------------------------------------------------------
UserData::UserData(vrj::User* user, std::string wandName, std::string incButton,
                   std::string decButton, std::string stopButton)
{
    mCurVelocity = 0.0;
    mNavMatrix.makeIdent();

    mUser = user;

    // Initialize devices.
    mWand.init(wandName);
    mIncVelocityButton.init(incButton);
    mDecVelocityButton.init(decButton);
    mStopButton.init(stopButton);
}

// ----------------------------------------------------------------------------
// Update the navigation matrix.  Uses a quaternion to do rotation in the
// environment.
// ----------------------------------------------------------------------------
void
UserData::updateNavigation (void) {
    vrj::Vec3 xyzAngles;
    vrj::Vec3 xyzTrans;
    vrj::Matrix* wand_matrix;

    // Cur*Transform = New Location
    vrj::Matrix transform, transformIdent;
    vrj::Quat   source_rot, goal_rot, slerp_rot;

    transformIdent.makeIdent();   // Create an identity matrix to rotate from
    source_rot.makeRot(transformIdent);

    wand_matrix = mWand->getData();
    wand_matrix->getXYZEuler(xyzAngles[0], xyzAngles[1], xyzAngles[2]);

    vprDEBUG(vprDBG_ALL, 2) << "===================================\n"
                          << vprDEBUG_FLUSH;
    vprDEBUG(vprDBG_ALL, 2) << "Wand:\n" << *wand_matrix << std::endl
                          << vprDEBUG_FLUSH;
    vprDEBUG(vprDBG_ALL, 2) << "Wand XYZ: " << xyzAngles << std::endl
                          << vprDEBUG_FLUSH;

    goal_rot.makeRot(*wand_matrix);    // Create the goal rotation quaternion

    // If we don't have two identity matrices
    if ( transformIdent != *wand_matrix ) {
        // Transform part way there...
        slerp_rot.slerp(0.05, source_rot, goal_rot);

        // Create the transform matrix to use.
        transform.makeQuaternion(slerp_rot);
    }
    else {
       transform.makeIdent();
    }

    vprDEBUG(vprDBG_ALL, 2) << "Transform:\n" << transform << std::endl
                          << vprDEBUG_FLUSH;

    transform.getXYZEuler(xyzAngles[0], xyzAngles[1], xyzAngles[2]);

    vprDEBUG(vprDBG_ALL, 2) << "Transform XYZ: " << xyzAngles << std::endl
                          << vprDEBUG_FLUSH;
    vprDEBUG(vprDBG_ALL, 2) << "Nav:\n" << mNavMatrix << std::endl << std::endl
                          << vprDEBUG_FLUSH;

    // Translation.
    const float velocity_inc = 0.005f;

    // Update velocity.
    if ( mIncVelocityButton->getData() ) {
        mCurVelocity += velocity_inc;
    }
    else if ( mDecVelocityButton->getData() ) {
        mCurVelocity -= velocity_inc;
    }
    else if ( mStopButton->getData() ) {
        mCurVelocity = 0.0f;
    }

    if ( mIncVelocityButton->getData() || mDecVelocityButton->getData() ) {
        vprDEBUG(vprDBG_ALL, 2) << "Velocity: " << mCurVelocity << std::endl
                              << vprDEBUG_FLUSH;
    }

    if ( mIncVelocityButton->getData() == gadget::Digital::TOGGLE_ON ) {
        vprDEBUG(vprDBG_ALL, 2) << "-- Toggle ON --" << std::endl
                              << vprDEBUG_FLUSH;
    }

    if(mIncVelocityButton->getData() == gadget::Digital::TOGGLE_OFF ) {
        vprDEBUG(vprDBG_ALL, 2) << "-- Toggle OFF --" << std::endl
                              << vprDEBUG_FLUSH;
    }

    if ( mIncVelocityButton->getData() == gadget::Digital::ON ) {
        vprDEBUG(vprDBG_ALL, 2) << "-- ON --" << std::endl << vprDEBUG_FLUSH;
    }

    if ( mIncVelocityButton->getData() == gadget::Digital::OFF ) {
        vprDEBUG(vprDBG_ALL, 2) << "-- OFF --" << std::endl << vprDEBUG_FLUSH;
    }

    // Find direction vector.
    vrj::Vec3 forward(0.0f, 0.0f, -1.0f);
    forward *= mCurVelocity;

    vrj::Matrix rot_mat, local_xform;
    rot_mat.invert(transform);

    local_xform.makeTrans(0, 0, mCurVelocity);
    local_xform.postMult(rot_mat);

    mNavMatrix.preMult(local_xform);

    local_xform.getXYZEuler(xyzAngles[0], xyzAngles[1], xyzAngles[2]);
    local_xform.getTrans(xyzTrans[0], xyzTrans[1], xyzTrans[2]);

    vprDEBUG(vprDBG_ALL, 2) << "Transform   Rot: " << xyzAngles << std::endl
                          << vprDEBUG_FLUSH;
    vprDEBUG(vprDBG_ALL, 2) << "Transform Trans: " << xyzTrans << std::endl
                          << vprDEBUG_FLUSH;
    vprDEBUG(vprDBG_ALL, 2) << "-------------------------------------------"
                          << std::endl << vprDEBUG_FLUSH;
}
