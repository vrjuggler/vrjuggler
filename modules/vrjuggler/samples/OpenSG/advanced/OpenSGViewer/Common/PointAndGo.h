/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#ifndef POINTANDGO_H
#define POINTANDGO_H

#include <Common/NavStrategy.h>
#include <vpr/Util/Interval.h>

//#include <snx/SoundHandle.h>

class User;

/**
* Simple navigation strategy that just goes where the user points
*/
class PointAndGo : public NavStrategy
{
public:
   enum SoundNames { NavTranslateSound = 0, NavRotateSound = 1, NumSounds = 2 };

public:
   PointAndGo()
      : mVelocity(0), mAllowRotate(true)
   {
      mLastUpdate = vpr::Interval::NoTimeout;
   //   initSounds();
   }

   virtual ~PointAndGo()
   {;}

   //void initSounds();

   /**
     * Template method for updating the view platform (navigation, etc).
     * Called once per frame
     * @param user - The user that is currently controlling the view platform
     */
    virtual void update(User* user);

protected:
   float          mVelocity;     /**< Velocity of current travel */
   bool           mAllowRotate;  /**< Wether or not to allow rotations */
   vpr::Interval  mLastUpdate;   /**< Time of the last update, used to compute delta */
    
protected:
   //std::vector<snx::SoundHandle> mSounds;
};
#endif //POINTANDGO_H

