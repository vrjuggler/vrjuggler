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

#ifndef _USER_DATA_H_
#define _USER_DATA_H_

#include <string>

#include <vrj/Math/Matrix.h>
#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/DigitalInterface.h>

#include <vrj/Kernel/User.h>


// Class to hold all data for a specific user
class UserData {
public:
    // ------------------------------------------------------------------------
    // Constructor.  Takes the string names of the devices to use.
    //
    // NOTE: This means that we cannot construct a user until the input manager
    // is loaded.
    //       Ex. The Init function
    // ------------------------------------------------------------------------
    UserData(vrj::User* user, std::string wandName, std::string incButton,
             std::string decButton, std::string stopButton);

    // ------------------------------------------------------------------------
    // Update the navigation matrix.  Uses a quaternion to do rotation in the
    // environment.
    // ------------------------------------------------------------------------
    void updateNavigation(void);

    // Devices to use for the given user.
    gadget::PositionInterface  mWand;                  // the Wand
    gadget::DigitalInterface   mIncVelocityButton;     // Button for velocity
    gadget::DigitalInterface   mDecVelocityButton;
    gadget::DigitalInterface   mStopButton;            // Button to stop

      // Navigation info for the user
    float              mCurVelocity; // The current velocity
    vrj::Matrix        mNavMatrix;   // Matrix for navigation in the app
    vrj::User*         mUser;        // The user we hold data for
};


#endif	/* _USER_DATA_H_ */
