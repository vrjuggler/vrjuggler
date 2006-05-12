/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#include <Common/PointAndGo.h>

#include <Common/User.h>

#include <gmtl/Matrix.h>
#include <gmtl/Coord.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Output.h>
#include <gmtl/Generate.h>

#include <iostream>

/*
void PointAndGo::initSounds()
{
   // Make sounds the right size
   mSounds.resize(NumSounds);

   mSounds[NavTranslateSound].init("NavTranslateSound");
   mSounds[NavRotateSound].init("NavRotateSound");
}
*/

/**
* Template method for updating the view platform (navigation, etc).
* Called once per frame
* @param user - The user that is currently controlling the view platform
*/
void PointAndGo::update(User* user)
{
   // Update delta time
   if (mLastUpdate == vpr::Interval::NoTimeout)
   {
      mLastUpdate.setNow();
   }

   vpr::Interval cur_time = user->getWandPos()->getTimeStamp();
   vpr::Interval delta_time = cur_time - mLastUpdate;
   mLastUpdate = cur_time;
   float delta_secs = delta_time.secf();

   // Clip delta time
   if (delta_secs > 1.0f)
   { delta_secs = 1.0f; }

   // --- MOVE AROUND THE MODEL --- //
   const float accelleration(1.25f);
   const float max_vel(7.0f);
   bool do_nav( user->getButton(2)->getData() == gadget::Digital::ON) ;
   //bool do_rotate = (wand_i->getButton(2)->getData() == gadget::Digital::ON);
   //bool do_stop = (wand_i->getButton(1)->getData() == gadget::Digital::ON);
   bool do_stop(false);

   // Update velocity
   if (do_nav)
   {
      mVelocity += (accelleration*delta_secs);
      std::cout << "vel: " << mVelocity << " -- Partial vel: (accel*delta_sec):" 
                << accelleration << "*"<< delta_secs << std::endl << std::flush;
   } 
   else if (mVelocity > 0)
   {
      // Decrease at y= -2x + max/2
      /*
      float dec_accell = (-2.0f * mVelocity) + (max_vel/2.0f);
      mVelocity -= (dec_accell * delta_secs);
      cout << "vel: " << mVelocity << std::endl;
      */
      mVelocity = 0.0f;
   }

   // Restrict range
   if (mVelocity < 0)
   {
      mVelocity = 0;
   }
   if (mVelocity > max_vel)
   {
      mVelocity = max_vel;
   }

   if (do_stop)
   {
      mVelocity = 0;
   }

   // --- TRAVEL IN THE MODEL --- //
   // -- Get wand info -- //
   gmtl::Matrix44f wandMatrix;
   gmtl::Coord3fXYZ wand_coord;
   wandMatrix = user->getWandPos()->getData();      // Get the wand matrix
   gmtl::Vec3f direction;
   gmtl::Vec3f forward_dir = gmtl::Vec3f(0.0f, 0.0f, -(mVelocity*delta_secs));
   gmtl::xform( direction, wandMatrix, forward_dir );
   gmtl::set(wand_coord, wandMatrix);
   float y_rot = wand_coord.getRot()[1];

   // Update the navigation matrix
   float rotation_divisor = 1.5f;
   gmtl::Matrix44f trans_mat = gmtl::makeTrans<gmtl::Matrix44f>( direction );
   gmtl::Matrix44f rot_mat = gmtl::make<gmtl::Matrix44f>( gmtl::Coord3fXYZ(0.0f, 0.0f, 0.0f,
                                                                           0.0, ((y_rot/rotation_divisor) * delta_secs), 0.0f));

   if (!mAllowRotate || !do_nav)
   {
      mCurPos = mCurPos * trans_mat;
   } else  // Do rotate
   {
      mCurPos = mCurPos * rot_mat * trans_mat;
   }

   // Sounds of navigation
   if(mVelocity > 0)
   {
      float sound_scale = (mVelocity/max_vel);
      float sound_vol = (0.5 + (0.5f * sound_scale));  // between 0.5 and 1.0
      //std::cout << "vel: " << mVelocity  << "  scale:" << sound_scale << "  vol:" << sound_vol << std::endl << std::flush;

      //mSounds[NavTranslateSound].setVolume( sound_vol);  // between 0.33 and 1.0
      //mSounds[NavTranslateSound].setPosition(0.1f, 0.0f, 0.0f);     // Do this to get gain to work
      //mSounds[NavTranslateSound].trigger();
   }
   else
   {
      //mSounds[NavTranslateSound].stop();
   }

   if(do_nav && mAllowRotate)
   {
      float rotate_sound_scale = fabs(y_rot/1.57f);
      if(rotate_sound_scale > 1.0f)
         rotate_sound_scale = 1.0f;
      float sound_vol = 0.5f + (0.5f * rotate_sound_scale);    // Scale between 0.5 and 1.

      //std::cout << "rot: " << y_rot  << "  scale:" << rotate_sound_scale << "  vol:" << sound_vol << std::endl << std::flush;
      //mSounds[NavRotateSound].setVolume(sound_vol);    // Scale between 0.5 and 1.0
      //mSounds[NavRotateSound].setPosition(0.1f, 0.0f, 0.0f);     // Do this to get gain to work
      //mSounds[NavRotateSound].trigger();
   }
   else
   {
      //mSounds[NavRotateSound].stop();
   }

   gmtl::invert(mCurPosInv, mCurPos);     // Set the inverse as well
}


