/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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

#ifndef _NAVIGATOR_H_
#define _NAVIGATOR_H_

#include <gmtl/Vec.h>
#include <gmtl/Point.h>
#include <gmtl/Matrix.h>
#include <collider.h>
#include <vector>
#include <gadget/Type/DigitalInterface.h>


//: Base class for navigation
class navigator
{
public:
   enum ActionState { OFF = 0, ON = 1, EITHER  = 2 };

   navigator();

   /// virtual destructor is required since subclasses are polymorphic
   virtual ~navigator();

   // Updates the state of the navigator that influences the nav update
   virtual void updateInteraction() = 0;

   // Updates the navigation
   // PRE: updateInteraction must have been called
   virtual void update() = 0;

   /// how many colliders?
   std::vector<gmtl::Vec3f>::size_type numColliders() const
   {
      return mCorrections.size();
   }
   
   /// get the correction that happened for collider x...
   const gmtl::Vec3f& getCorrection(const unsigned int x) const
   {
      vprASSERT(x < mCorrections.size());
      return mCorrections[x];
   }
   
   void setRotAxis(const bool allowX, const bool allowY, const bool allowZ);

   void allowTrans(const bool state = true)
   {
      mAllowTrans = state;
   }

   void allowRot(const bool state = true)
   {
      mAllowRot = state;
   }

   virtual void heading(float& pitch, float& yaw, float& roll);

   const gmtl::Matrix44f& getCurPos() const
   {
      return mCurPos;
   }

   void setCurPos(const gmtl::Matrix44f& pos)
   {
      mCurPos = pos;
   }

   void setHomePosition(const gmtl::Matrix44f& pos);

   // Reset to home position and initial navigation state
   virtual void reset()
   {
      mCurPos = mHomePos;
   }

   //: Add a collider to the system
   void addCollider(collider* val)
   {
      mColliders.push_back(val);
   }

   // Should allow testing to see if the navigator is currently active
   // If the navigator is not active, then we should not be updating
   bool isActive() const
   {
      return mIsActive;
   }

   void setActive(const bool state)
   {
      mIsActive = state;
   }

   void setName(const std::string& name)
   {
      mName = name;
   }

   const std::string& getName() const
   {
      return mName;
   }
   
   virtual gmtl::Vec3f getVelocity() const
   {
      return gmtl::Vec3f();
   }

   virtual float getSpeed() const
   {
      return 0.0f;
   }

protected:

   // give a translation vector in local user coordinates (relative to the user's current orientation)
   // i.e. if you want to move forward (direction facing) in the scene, just give (0,0,-1).
   // NOTE: the direction "forward" would be relative to current user position
   // ARGS: didCollide - true - The navigator collided with the model
   //       totalCorrection - The actual amount moved given the correction (in modelspace)
   void navTranslate(const gmtl::Vec3f& trans, bool& didCollide,
                     gmtl::Vec3f& totalCorrection);

   void navRotate(const gmtl::Matrix44f& rot_mat);

   // Correct the given attempted translation
   // Checks the given translation (in model cordinate space) against
   // the collidors given
   // Returns the corrected trans in trans, also returns the totalCorrection used
   void navCollideTransCorrect(gmtl::Point3f& trans, bool& didCollide,
                               gmtl::Vec3f& totalCorrection);

   // HELPER
   // returns true if the action state is true
   bool checkForAction(const std::vector<gadget::DigitalInterface*>& btns,
                       const std::vector<ActionState>& state_combo);

protected:
   std::string       mName;         // Name of the collidor
   bool mAllowRot,   mAllowTrans;
   gmtl::Matrix44f   mCurPos;       // (modelspace_M_user) The current position or the user- In Juggler coords
                                    // Moves the "user" from the models origin to the user's navigated origin (coord system)
   gmtl::Matrix44f   mHomePos;      // Resets to this location

   bool              allowAxis[3];  // Which axes are we allowed to rotate on
   bool              mIsActive;     // Is the navigator currently active in the environment

   std::vector<collider*>  mColliders;    // The collidors in the system
   std::vector<gmtl::Vec3f> mCorrections; // a list of corrections for the last frame for use by the application
};


#endif
