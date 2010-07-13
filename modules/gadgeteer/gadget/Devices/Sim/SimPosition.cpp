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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/gadgetConfig.h>

#include <jccl/Config/ConfigElement.h>
#include <gadget/Devices/Sim/SimPosition.h>

#include <gmtl/Plane.h>
#include <gmtl/LineSeg.h>
#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/EulerAngle.h>

#include <boost/concept_check.hpp>

namespace gadget
{

std::string SimPosition::getElementType()
{
   return "simulated_positional_device";
}

bool SimPosition::config(jccl::ConfigElementPtr element)
{
   if (! (Input::config(element) && Position::config(element) &&
          SimInput::config(element)) )
   {
      return false;
   }

   mDTrans = element->getProperty<float>("translation_delta");
   mDRot   = element->getProperty<float>("rotation_delta");

   mTransCoordSystem = element->getProperty<int>("translation_coordinate_system");
   mRotCoordSystem   = element->getProperty<int>("rotation_coordinate_system");

   std::vector<jccl::ConfigElementPtr> key_list;
   int key_count = element->getNum("key_pair");
   for ( int i = 0; i < key_count; ++i )
   {
      key_list.push_back(element->getProperty<jccl::ConfigElementPtr>("key_pair", i));
   }
   std::vector<KeyModPair> key_pairs = readKeyList(key_list);

   // Create keypairs
   vprASSERT(key_pairs.size() == NUM_POS_CONTROLS);
   for ( int i = 0; i < NUM_POS_CONTROLS; ++i )
   {
      mSimKeys[i] = key_pairs[i];
   }

   // Set initial position
   float x_pos = element->getProperty<float>("initial_position",0);
   float y_pos = element->getProperty<float>("initial_position",1);
   float z_pos = element->getProperty<float>("initial_position",2);
   float x_rot = element->getProperty<float>("initial_rotation",0);
   float y_rot = element->getProperty<float>("initial_rotation",1);
   float z_rot = element->getProperty<float>("initial_rotation",2);

   gmtl::Matrix44f& pos_data(mPos.editValue());

   if (x_pos != 0.0f || y_pos != 0.0f || z_pos != 0.0f)
   {
      gmtl::setTrans(pos_data, gmtl::Vec3f(x_pos, y_pos, z_pos));
   }

   if (x_rot != 0.0f || y_rot != 0.0f || z_rot != 0.0f)
   {
      gmtl::EulerAngleXYZf euler(gmtl::Math::deg2Rad(x_rot),
                                 gmtl::Math::deg2Rad(y_rot),
                                 gmtl::Math::deg2Rad(z_rot));
      gmtl::postMult(pos_data, gmtl::makeRot<gmtl::Matrix44f>(euler));
   }

   mPos.setTime();

   return true;
}

void SimPosition::updateData()
{
   float amt(0);      // Number of times key pressed
                     // Used to keep from calling checkKey twice on success
      // NOTE: Could have implemented using side effects of assignment
      //       and used less lines, but this is more explicit

   amt = checkKeyPair(mSimKeys[FORWARD]);
   if(amt > 0.0f)
   {
      moveFor( amt );
   }

   amt = checkKeyPair(mSimKeys[BACK]);
   if(amt > 0.0f)
   {
      moveFor( -amt );
   }

   amt = checkKeyPair(mSimKeys[LEFT]);
   if(amt > 0.0f)
   {
      moveLeft( amt );
   }

   amt = checkKeyPair(mSimKeys[RIGHT]);
   if(amt > 0.0f)
   {
      moveLeft( -amt );
   }

   amt = checkKeyPair(mSimKeys[UP]);
   if(amt > 0.0f)
   {
      moveUp ( amt );
   }

   amt = checkKeyPair(mSimKeys[DOWN]);
   if(amt > 0.0f)
   {
      moveUp ( -amt );
   }

   amt = checkKeyPair(mSimKeys[ROTR]);
   if(amt > 0.0f)
   {
      rotLeft( -amt );
   }

   amt = checkKeyPair(mSimKeys[ROTL]);
   if(amt > 0.0f)
   {
      rotLeft( amt );
   }

   amt = checkKeyPair(mSimKeys[ROTU]);
   if(amt > 0.0f)
   {
      rotUp( amt );
   }

   amt = checkKeyPair(mSimKeys[ROTD]);
   if(amt > 0.0f)
   {
      rotUp( -amt );
   }

   amt = checkKeyPair(mSimKeys[ROT_ROLL_CCW]);
   if(amt > 0.0f)
   {
      rotRollCCW( amt );
   }

   amt = checkKeyPair(mSimKeys[ROT_ROLL_CW]);
   if(amt > 0.0f)
   {
      rotRollCCW( -amt );
   }

   // Set the time for the position data to the KeyboardMouse timestamp
   mPos.setTime(mKeyboardMouse->getTimeStamp());
   addPositionSample(std::vector< gadget::PositionData>(1, mPos) );

   swapPositionBuffers();  // Swap the buffers
}

void SimPosition::moveDir(const float amt, const gmtl::Vec3f dir)
{
   gmtl::Vec3f move_vector(dir);  // Base movement
   move_vector *= (amt*mDTrans);

   if(isTransAllowed(move_vector))
   {
      if(mTransCoordSystem == LOCAL)
      {
         gmtl::postMult(mPos.editValue(),
                        gmtl::makeTrans<gmtl::Matrix44f>(move_vector));
      }
      else
      {
         gmtl::preMult(mPos.editValue(),
                       gmtl::makeTrans<gmtl::Matrix44f>(move_vector));
      }
   }
   else
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL)
         << "SimPos hit a surface.\n" << vprDEBUG_FLUSH;
   }
}


// Move forward the given amount on position data n
// Forward is in th -Z direction
void SimPosition::moveFor(const float amt)
{
   gmtl::Vec3f move_forward(0.0,0.0,-1.0);  // Base movement
   moveDir(amt, move_forward);
}

// Move left the given amount on position data n
// Left is -X dir
void SimPosition::moveLeft(const float amt)
{
   gmtl::Vec3f move_left(-1.0,0.0,0.0);  // Base movement
   moveDir(amt, move_left);
}

// Move up the given amount on position data n
// Up is in th +Y dir
void SimPosition::moveUp(const float amt)
{
   gmtl::Vec3f move_up(0.0,1.0,0.0);  // Base movement
   moveDir(amt, move_up);
}

/** Rotation amt around axis */
void SimPosition::rotAxis(const float amt, const gmtl::Vec3f& rotAxis)
{
   // convert the input...
   gmtl::AxisAnglef axisAngle( gmtl::Math::deg2Rad(amt*mDRot), gmtl::makeNormal( rotAxis ) );

   gmtl::Matrix44f delta_rot( gmtl::makeRot<gmtl::Matrix44f>( axisAngle ) );   // make delta rot

   gmtl::Matrix44f& pos_data(mPos.editValue());

   if(mRotCoordSystem == LOCAL)
   {
      gmtl::postMult(pos_data, delta_rot);
   }
   else
   {
      // Get the translation and rotation seperated
      // Make new matrix with Trans*DeltaRot*Rot
      gmtl::Vec3f trans_vec(gmtl::makeTrans<gmtl::Vec3f>(pos_data));          // Get translation
      gmtl::Matrix44f trans_mat(gmtl::makeTrans<gmtl::Matrix44f>( trans_vec ));   // Make trans matrix
      gmtl::Matrix44f rot_mat(pos_data);

      gmtl::setTrans(rot_mat, gmtl::Vec3f(0.0f,0.0f,0.0f));  // Clear out trans
      pos_data = trans_mat * delta_rot * rot_mat;
      /*
      gmtl::setTrans(*m, gmtl::Vec3f(0,0,0));      // Get to rotation only
      gmtl::preMult(*m, delta_rot);
      gmtl::preMult(*m, trans);
      */
   }
}

// Pitch up - rot +x axis
void SimPosition::rotUp(const float amt)
{
   gmtl::Vec3f x_axis(1.0,0.0,0.0);
   rotAxis(amt, x_axis);
}

// Yaw left - rot +Y axis
void SimPosition::rotLeft(const float amt)
{
   gmtl::Vec3f y_axis(0.0, 1.0, 0.0);
   rotAxis(amt, y_axis);
}

// Roll Left - rot -z axis
void SimPosition::rotRollCCW(const float amt)
{
   gmtl::Vec3f neg_z_axis(0.0, 0.0, -1.0);
   rotAxis(amt, neg_z_axis);
}

// Checks if movement is allowed.
// NOTE: It is not allowed if it hits a simulated wall, etc.
bool SimPosition::isTransAllowed(gmtl::Vec3f trans)
{
   boost::ignore_unused_variable_warning(trans);
   // check if the movement is goign to intersect with any of the surface displays
   // If it does, then return false
   /*
   gmtl::Vec3f ll, lr, ur, ul;
   Seg trans_seg;
   float t_dist;
   gmtl::Vec3f src_pt;
   mPos.getTrans(src_pt[0],src_pt[1], src_pt[2]);
   trans_seg.makePts(src_pt, (src_pt+trans));

   std::vector<Display*> disps = DisplayManager::instance()->getAllDisplays();

   for ( unsigned int i = 0; i < disps.size(); ++i )
   {
      if(disps[i]->isSurface())
      {
         SurfaceDisplay* surf_disp = dynamic_cast<SurfaceDisplay*>(disps[i]);
         vprASSERT(surf_disp != NULL);

         // Get corners
         surf_disp->getCorners(ll,lr,ur,ul);

         // Check the tris
         if(trans_seg.isectTriangle(ll,lr,ul,&t_dist))   // If ray hits
         {
            if(trans_seg.tValueOnSeg(t_dist))            // If t_dist on seg
            {
               return false;                             // Hit tri
            }
         }

         if(trans_seg.isectTriangle(ul,lr,ur,&t_dist))   // If ray hits
         {
            if(trans_seg.tValueOnSeg(t_dist))            // If t_dist on seg
            {
                return false;                             // Hit tri
            }
         }
      }
   }
   */

   return true;
}

} // End of gadget namespace
