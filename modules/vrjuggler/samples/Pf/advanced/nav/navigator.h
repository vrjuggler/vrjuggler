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

#ifndef _NAVIGATOR_H_
#define _NAVIGATOR_H_

#include <vrj/Math/Vec3.h>
#include <vrj/Math/Matrix.h>
#include <vrj/Math/Coord.h>
#include <collider.h>
#include <vector>
#include <gadget/Type/DigitalInterface.h>


//: Base class for navigation
class navigator
{
public:
   enum ActionState { OFF = 0, ON = 1, EITHER  = 2 };

   navigator() : mAllowRot( true ), mAllowTrans( true ), mIsActive(true)
   {
      allowAxis[0] = allowAxis[1] = allowAxis[2] = true;    // Initialize to rot in all axes
   }

   // Empty virtual destructor to make GCC happy.
   virtual ~navigator (void) {
      /* Do nothing. */ ;
   }

   // Updates the state of the navigator that influences the nav update
   virtual void updateInteraction() = 0;

   // Updates the navigation
   // PRE: updateInteraction must have been called
   virtual void update() = 0;

  void setRotAxis(bool allowX, bool allowY, bool allowZ);

   void allowTrans( const bool& state = true ){mAllowTrans = state;}
   void allowRot( const bool& state = true ){mAllowRot = state;}

   virtual void heading( float& pitch, float& yaw, float& roll )
   {
      mCurPos.getXYZEuler( pitch, yaw, roll );
      pitch = -pitch;
      yaw = -yaw;
      roll = -roll;
   }

   vrj::Matrix getCurPos() const
   { return mCurPos; }

   void setCurPos(vrj::Matrix pos)
   { mCurPos = pos; }

   void setHomePosition(vrj::Matrix pos)
   {
      mHomePos = pos;
      vrj::Vec3 hpos = mHomePos.getTrans();
      vprDEBUG(vprDBG_ALL,0) << clrOutNORM(clrCYAN,"navigator: HomePosition = ")
                           << hpos << std::endl << vprDEBUG_FLUSH;
    }

   // Reset to home position and initial navigation state
   virtual void reset()
   { mCurPos = mHomePos; }

   //: Add a collider to the system
   void addCollider( collider* val )
   { mColliders.push_back( val ); }

   // Should allow testing to see if the navigator is currently active
   // If the navigator is not active, then we should not be updating
   bool isActive() const { return mIsActive; }
   void setActive(bool state) { mIsActive = state; }

   void setName(std::string name) { mName = name; }
   std::string getName() { return mName; }

protected:

   // give a translation vector in local user coordinates (relative to the user's current orientation)
   // i.e. if you want to move forward (direction facing) in the scene, just give (0,0,-1).
   // NOTE: the direction "forward" would be relative to current user position
   // ARGS: didCollide - true - The navigator collided with the model
   //       totalCorrection - The actual amount moved given the correction (in modelspace)
   void navTranslate( const vrj::Vec3 trans, bool& didCollide, vrj::Vec3& totalCorrection);

   void navRotate( vrj::Matrix rot_mat );

   // Correct the given attempted translation
   // Checks the given translation (in model cordinate space) against
   // the collidors given
   // Returns the corrected trans in trans, also returns the totalCorrection used
   void navCollideTransCorrect(vrj::Vec3& trans, bool& didCollide, vrj::Vec3& totalCorrection);

   // HELPER
   // returns true if the action state is true
   bool checkForAction(std::vector<gadget::DigitalInterface*> btns, std::vector<ActionState> state_combo);

protected:
   std::string       mName;         // Name of the collidor
   bool mAllowRot,   mAllowTrans;
   vrj::Matrix       mCurPos;       // (modelspace_M_user) The current position or the user- In Juggler coords
                                    // Moves the "user" from the models origin to the user's navigated origin (coord system)
   vrj::Matrix       mHomePos;      // Resets to this location

   bool              allowAxis[3];  // Which axes are we allowed to rotate on
   bool              mIsActive;     // Is the navigator currently active in the environment

   std::vector<collider*>  mColliders;    // The collidors in the system
};


inline void navigator::navTranslate(vrj::Vec3 trans, bool& didCollide, vrj::Vec3& totalCorrection)
{
   /*
   vrj::Coord cur_pos(mCurPos);
   std::cerr << "Cur P: " << cur_pos.pos << std::endl << "Trans: " << trans
             << std::endl
             << "    =: " << cur_pos.pos+trans << std::endl;
   */

   // convert the info to model coordinates, since that's what the nav routines take.
   // trans is in local (user) coordinates,  xform it to model coordinates (since we are testing against virtual world (model))
   // Mtrans_model = model_M_user * Mtrans_user
   vrj::Matrix cur_rotation = mCurPos;             // Get rotation only part of model_M_user
   cur_rotation.setTrans( 0.0f, 0.0f, 0.0f );   // zero out the translation...
   vrj::Vec3 trans_in_modelspace;
   trans_in_modelspace.xformFull( cur_rotation, trans );

   // Get correction for collision detection
   // This func returns the totalCorrection
   // NOTE: this func returns 3 values.
   navCollideTransCorrect( trans_in_modelspace, didCollide, totalCorrection );

   // Convert back to trans_in_modelspace back into local space
   vrj::Matrix inv_cur_rotation;
   inv_cur_rotation.invert(cur_rotation);
   trans.xformFull(inv_cur_rotation, trans_in_modelspace);

   // Post mult cur_mat by the trans we need to do
   // model_M_new-user = model_M_user*user_M_new-user
   vrj::Matrix user_M_newUser;
   user_M_newUser.makeTrans(trans);
   mCurPos.postMult(user_M_newUser);
}


inline void navigator::navRotate( vrj::Matrix rot_mat )
{
   //rot_mat.constrainRotAxis( allowAxis[0], allowAxis[1], allowAxis[2], rot_mat );

   mCurPos.postMult( rot_mat );
   
   //mCurPos.constrainRotAxis( allowAxis[0], allowAxis[1], allowAxis[2], mCurPos );

   vrj::Matrix old_pos = mCurPos;
   float x_pos, y_pos, z_pos;
   old_pos.getTrans(x_pos,y_pos,z_pos);
   mCurPos.makeXYZEuler(0,old_pos.getYRot(),0);     // Only allow Yaw (rot y)
   mCurPos.setTrans(x_pos,y_pos,z_pos);
   
   /*
   float x_rot, y_rot, z_rot;
   mCurPos.getXYZEuler(x_rot,y_rot,z_rot);
   mCurPos.makeXYZEuler((allowAxis[0]?x_rot:0),(allowAxis[1]?y_rot:0),(allowAxis[2]?z_rot:0));
   */
}


// Correct the given attempted translation
// Checks the given translation (in model cordinate space) against
// the collidors given
// returns the modified trans, and the total correction that was applied
inline void navigator::navCollideTransCorrect(vrj::Vec3& trans, bool& didCollide, vrj::Vec3& totalCorrection)
{
   didCollide = false;
   totalCorrection.set(0,0,0);

   // mCurPos (model_M_user) is already in model coordinates,
   // since it is used to move the geometry from modelspace to userSpace
   vrj::Vec3 whereYouAre = vrj::Coord(mCurPos).pos;
   //vrj::Coord cur_pos(mCurPos);
   //whereYouAre = cur_pos.pos;

   // Find out where you want to move to (modelspace = modelspace + modelspace::: no conversion nessesary)
   //whereYouWantToBe = whereYouAre + trans;

   // the collider will return a correction vector in modelspace coordinates
   // add this to correct your distance requested to move.
   vrj::Vec3 local_correction;
   totalCorrection.set(0.0,0.0,0.0);

   ////////////////////////////////////////////////////////////
   // Test for collisions with all registered colliders
   ////////////////////////////////////////////////////////////
   for (unsigned x = 0; x < mColliders.size(); x++)
   {
      // If collision, then ...
      if (mColliders[x]->testMove( whereYouAre, trans, local_correction ))
      {
                  // ... apply the correction.
         trans += local_correction;
         totalCorrection += local_correction;

         // HACK(!) - use the correction vectors to decide how much velocity from gravity to remove
         //        here i'm just zeroing it out... sometimes many times per frame!!! (real bad)
         //    this should really only be affected by the Y component of the correction vector.
         //mVelocityFromGravityAccumulator.set( 0.0f, 0.0f, 0.0f );
         didCollide = true;      // Tell callee that we collided
      }
   }
}


inline void navigator::setRotAxis(bool allowX, bool allowY, bool allowZ)
{
   allowAxis[0] = allowX;
   allowAxis[1] = allowY;
   allowAxis[2] = allowZ;
}

// HELPER
// returns true if the action state is true
inline bool navigator::checkForAction( std::vector<gadget::DigitalInterface*> btns, std::vector<navigator::ActionState> state_combo)
{
   bool ret_val( true );

   // Check state of all the required combos
   for (unsigned int i=0;i<state_combo.size();i++)
   {
      if(state_combo[i] == navigator::ON)
      {
         if (!(*btns[i])->getData())
         { ret_val = false; }          // We need it on, and it is off
      }
      else if (state_combo[i] == navigator::OFF)
      {
         if ((*btns[i])->getData())
         { ret_val = false; }          // We need it off, and it is on
      }
   }

   return ret_val;
}



#endif
