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

#include <vrj/vjConfig.h>

#include <vrj/Math/Coord.h>
#include <vrj/Math/Plane.h>
#include <vrj/Math/Seg.h>
#include <vrj/Display/DisplayManager.h>
#include <vrj/Display/Display.h>
#include <vrj/Config/ConfigChunk.h>
#include <vrj/Input/Devices/Sim/SimPosition.h>

namespace vrj
{
   
bool SimPosition::config(ConfigChunk* chunk)
{
   if(! (Input::config(chunk) && Position::config(chunk) && SimInput::config(chunk) ))
      return false;

   mDTrans = chunk->getProperty("dtrans");
   mDRot   = chunk->getProperty("drot");

   mTransCoordSystem = chunk->getProperty("transCoordSystem");
   mRotCoordSystem = chunk->getProperty("rotCoordSystem");

   std::vector<VarValue*> key_list = chunk->getAllProperties("keyPairs");
   std::vector<KeyModPair> key_pairs = readKeyList(key_list);


   // Create keypairs
   vprASSERT(key_pairs.size() == NUM_POS_CONTROLS);
   for(int i=0;i<NUM_POS_CONTROLS;i++)
   {
      mSimKeys[i] = key_pairs[i];
   }


      // Set initial position
   float x_pos = chunk->getProperty("initialPos",0);
   float y_pos = chunk->getProperty("initialPos",1);
   float z_pos = chunk->getProperty("initialPos",2);
   float x_rot = chunk->getProperty("initialRot",0);
   float y_rot = chunk->getProperty("initialRot",1);
   float z_rot = chunk->getProperty("initialRot",2);

   if((x_pos != 0.0f) || (y_pos != 0.0f) || (z_pos != 0.0f))
      mPos.makeTrans(x_pos, y_pos, z_pos);
   if((x_rot != 0.0f) || (y_rot != 0.0f) || (z_rot != 0.0f))
      mPos.postXYZEuler(mPos, x_rot, y_rot, z_rot);

   return true;
}


void SimPosition::updateData()
{
   int amt = 0;      // Number of times key pressed
                     // Used to keep from calling checkKey twice on success
      // NOTE: Could have implemented using side effects of assignment
      //       and used less lines, but this is more explicit
   mUpdateTime.set();

   amt = checkKeyPair(mSimKeys[FORWARD]);
   if(amt)
      moveFor( 1 * amt);
   amt = checkKeyPair(mSimKeys[BACK]);
   if(amt)
      moveFor( -1 * amt);
   amt = checkKeyPair(mSimKeys[LEFT]);
   if(amt)
      moveLeft( 1 * amt);
   amt = checkKeyPair(mSimKeys[RIGHT]);
   if(amt)
      moveLeft( -1 * amt);
   amt = checkKeyPair(mSimKeys[UP]);
   if(amt)
      moveUp ( 1 * amt);
   amt = checkKeyPair(mSimKeys[DOWN]);
   if(amt)
      moveUp (-1 * amt);

   amt = checkKeyPair(mSimKeys[ROTR]);
   if(amt)
      rotLeft( -1 *amt);
   amt = checkKeyPair(mSimKeys[ROTL]);
   if(amt)
      rotLeft( 1  * amt);
   amt = checkKeyPair(mSimKeys[ROTU]);
   if(amt)
      rotUp( 1 * amt);
   amt = checkKeyPair(mSimKeys[ROTD]);
   if(amt)
      rotUp( -1 * amt);
   amt = checkKeyPair(mSimKeys[ROT_ROLL_CCW]);
   if(amt)
      rotRollCCW( 1 * amt);
   amt = checkKeyPair(mSimKeys[ROT_ROLL_CW]);
   if(amt)
      rotRollCCW( -1 * amt);

   // Debug output
   //vjCoord pos_data(mPos);
   //vjDEBUG(vjDBG_ALL,1) << "simPos: pos:" << pos_data.pos << "  or:" << pos_data.orient << endl << vjDEBUG_FLUSH;
}


// Move forward the given amount on position data n
// Forward is in th -Z direction
void SimPosition::moveFor(const float amt)
{
   Vec3 move_forward(0.0,0.0,-1.0);  // Base movement
   move_forward *= (amt*mDTrans);

   if(isTransAllowed(move_forward))
   {
      if(mTransCoordSystem == LOCAL)
         mPos.postTrans(mPos, move_forward);
      else
         mPos.preTrans(move_forward, mPos);
   }
   else
      vjDEBUG(vjDBG_INPUT_MGR,4) << "SimPos hit a surface.\n" << vjDEBUG_FLUSH;
}

// Move left the given amount on position data n
// Left is -X dir
void SimPosition::moveLeft(const float amt)
{
   Vec3 move_left(-1.0,0.0,0.0);  // Base movement
   move_left *= (amt*mDTrans);

   if(isTransAllowed(move_left))
   {
      if(mTransCoordSystem == LOCAL)
         mPos.postTrans(mPos, move_left);
      else
         mPos.preTrans(move_left, mPos);
   }
   else
      vjDEBUG(vjDBG_INPUT_MGR,4) << "SimPos hit a surface.\n" << vjDEBUG_FLUSH;
}

// Move up the given amount on position data n
// Up is in th +Y dir
void SimPosition::moveUp(const float amt)
{
   Vec3 move_up(0.0,1.0,0.0);  // Base movement
   move_up *= (amt*mDTrans);

   if(isTransAllowed(move_up))
   {
      if(mTransCoordSystem == LOCAL)
         mPos.postTrans(mPos, move_up);
      else
         mPos.preTrans(move_up, mPos);
   }
   else
      vjDEBUG(vjDBG_INPUT_MGR,4) << "SimPos hit a surface.\n" << vjDEBUG_FLUSH;
}

// Pitch up - rot +x axis
void SimPosition::rotUp(const float amt)
{
   static Vec3 x_axis(1.0,0.0,0.0);
   if(mRotCoordSystem == LOCAL)
      mPos.postRot(mPos, amt*mDRot, x_axis);
   else
   {
      // Get the translation and rotation seperated
      // Make new matrix with Trans*DeltaRot*Rot
      float x,y,z;
      mPos.getTrans(x,y,z);      // Get translation
      Matrix trans;
      trans.makeTrans(x,y,z);

      Matrix delta_rot;        // make delta rot
      delta_rot.makeRot(amt*mDRot, x_axis);

      mPos.setTrans(0,0,0);      // Get to rotation only
      mPos.preMult(delta_rot);
      mPos.preMult(trans);
   }
}

// Yaw left - rot +Y axis
void SimPosition::rotLeft(const float amt)
{
   static Vec3 y_axis(0.0, 1.0, 0.0);

   if(mRotCoordSystem == LOCAL)
      mPos.postRot(mPos, amt*mDRot, y_axis);
   else
   {
      // Get the translation and rotation seperated
      // Make new matrix with Trans*DeltaRot*Rot
      float x,y,z;
      mPos.getTrans(x,y,z);      // Get translation
      Matrix trans;
      trans.makeTrans(x,y,z);

      Matrix delta_rot;        // make delta rot
      delta_rot.makeRot(amt*mDRot, y_axis);

      mPos.setTrans(0,0,0);      // Get to rotation only
      mPos.preMult(delta_rot);
      mPos.preMult(trans);
   }
}

// Roll Left - rot -z axis
void SimPosition::rotRollCCW(const float amt)
{
   static Vec3 neg_z_axis(0.0, 0.0, -1.0);

   if(mRotCoordSystem == LOCAL)
      mPos.postRot(mPos, amt*mDRot, neg_z_axis);
   else
   {
      // Get the translation and rotation seperated
      // Make new matrix with Trans*DeltaRot*Rot
      float x,y,z;
      mPos.getTrans(x,y,z);      // Get translation
      Matrix trans;
      trans.makeTrans(x,y,z);

      Matrix delta_rot;        // make delta rot
      delta_rot.makeRot(amt*mDRot, neg_z_axis);

      mPos.setTrans(0,0,0);      // Get to rotation only
      mPos.preMult(delta_rot);
      mPos.preMult(trans);
   }
}

//: Check if movement is allowed
//! NOTE: It is not allowed if it hits a simulated wall, etc.
bool SimPosition::isTransAllowed(Vec3 trans)
{
   // check if the movement is goign to intersect with any of the surface displays
   // If it does, then return false
   /*
   Vec3 ll, lr, ur, ul;
   Seg trans_seg;
   float t_dist;
   Vec3 src_pt;
   mPos.getTrans(src_pt[0],src_pt[1], src_pt[2]);
   trans_seg.makePts(src_pt, (src_pt+trans));

   std::vector<Display*> disps = DisplayManager::instance()->getAllDisplays();

   for(unsigned int i=0;i<disps.size();i++)
   {
      if(disps[i]->isSurface())
      {
         SurfaceDisplay* surf_disp = dynamic_cast<SurfaceDisplay*>(disps[i]);
         vprASSERT(surf_disp != NULL);

         // Get corners
         surf_disp->getCorners(ll,lr,ur,ul);

         // Check the tris
         if(trans_seg.isectTriangle(ll,lr,ul,&t_dist))   // If ray hits
            if(trans_seg.tValueOnSeg(t_dist))            // If t_dist on seg
               return false;                             // Hit tri

         if(trans_seg.isectTriangle(ul,lr,ur,&t_dist))   // If ray hits
           if(trans_seg.tValueOnSeg(t_dist))            // If t_dist on seg
              return false;                             // Hit tri
      }
   }
   */

   return true;

}

};
