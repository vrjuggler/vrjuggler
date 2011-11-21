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

#include <vrj/Draw/Direct3D/Config.h>

#include <gmtl/Matrix.h>
#include <gmtl/Generate.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>
#include <gmtl/Xforms.h>
#include <gmtl/Output.h>

#include <jccl/Config/ConfigElement.h>

#include <vrj/Kernel/User.h>

#include <vrj/Draw/Direct3D/DrawManager.h>

#include <vrj/Display/DisplayManager.h>
#include <vrj/Display/Display.h>
#include <vrj/Display/Projection.h>
#include <vrj/Display/Viewport.h>
#include <vrj/Display/SimViewport.h>
#include <vrj/Display/SurfaceViewport.h>

#include <vrj/Draw/Direct3D/SimInterfaceFactory.h>
#include <vrj/Draw/Direct3D/BasicSimulator.h>


namespace vrj
{

namespace direct3d
{

VRJ_REGISTER_D3D_SIM_INTERFACE_CREATOR(BasicSimulator);

BasicSimulator::BasicSimulator()
{;}

BasicSimulator::~BasicSimulator()
{;}

/*
* Configure the basic simulator config
* @pre element is a valid configuration element.
* @post It should be configured
*/
bool BasicSimulator::config(jccl::ConfigElementPtr element)
{
   vprASSERT(element.get() != NULL);
   vprASSERT(element->getID() == std::string("default_simulator"));

   std::string camera_proxy_str = element->getProperty<std::string>("camera_pos");
   std::string wand_proxy_str = element->getProperty<std::string>("wand_pos");

   mCamera.init(camera_proxy_str);
   mWand.init(wand_proxy_str);      // Configure the wand to use

   if(!mCamera.isConnected())
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED, "ERROR:")
         << "[vrj::direct3d::BasicSimulator::config()] Fatal Error: Camera "
         << "not found named: " << camera_proxy_str << vprDEBUG_FLUSH;
      vprASSERT(false);
   }

   // Get drawing parameters
   mDrawProjections = element->getProperty<bool>("draw_projections");
   mSurfaceColor[0] = element->getProperty<float>("surface_color", 0);
   mSurfaceColor[1] = element->getProperty<float>("surface_color", 1);
   mSurfaceColor[2] = element->getProperty<float>("surface_color", 2);

   return true;
}

/**
 * Draws this sim device using the given information about the Window it
 * will be drawing into.
 */
void BasicSimulator::draw(const float scaleFactor)
{
   drawSimulator(scaleFactor);
}

/**
 * Sets the keyboard/mouse device the simulator can use to get input from the
 * user.
 */
void BasicSimulator::setKeyboardMouse(gadget::KeyboardMouseInterface)
{
}

void BasicSimulator::updateProjectionData(const float positionScale,
                                          ProjectionPtr leftProj,
                                          ProjectionPtr rightProj)
{
   updateInternalData(positionScale);

   gmtl::Matrix44f camera_pos = getCameraPos();
   gmtl::Vec3f camera_trans = gmtl::makeTrans<gmtl::Vec3f>(camera_pos);

   // -- Calculate camera (eye) Positions -- //
   vprDEBUG(vprDBG_ALL, vprDBG_HEX_LVL)
      << "[vrj::direct3d::BasicSimulator::updateProjectionData()] Getting "
      << "camera position" << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL, vprDBG_HEX_LVL) << "CamPos:" << camera_trans
                                        << std::endl << vprDEBUG_FLUSH;

   // Compute location of left and right eyes
   float interocular_dist = mSimViewport->getUser()->getInterocularDistance();
   interocular_dist *= positionScale;               // Scale into correct units
   float eye_offset = interocular_dist / 2.0f;      // Distance to move eye

   gmtl::Matrix44f left_eye_pos, right_eye_pos;     // NOTE: Eye coord system is -z forward, x-right, y-up
   left_eye_pos = camera_pos * gmtl::makeTrans<gmtl::Matrix44f>( gmtl::Vec3f(-eye_offset, 0.0f, 0.0f) );
   right_eye_pos = camera_pos * gmtl::makeTrans<gmtl::Matrix44f>( gmtl::Vec3f(eye_offset, 0.0f, 0.0f) );

   leftProj->calcViewMatrix(left_eye_pos, positionScale);
   rightProj->calcViewMatrix(right_eye_pos, positionScale);
}

/**  Update internal simulator data */
void BasicSimulator::updateInternalData(float positionScale)
{
   mHeadPos = mSimViewport->getUser()->getHeadPosProxy()->getData(positionScale);
   mWandPos = mWand->getData(positionScale);

   mCameraPos = mCamera->getData(positionScale);
   gmtl::invert(mCameraPos);
}

// Draw any objects that we need to display in the scene
// from the system.  (i.e. Gloves, etc)
void BasicSimulator::drawObjects()
{
   // XXX: Not implemented
}

/**
 * Draw the projections.
 *
 * @post Draws the projections.
 *       If withApex, then it draws the frustums with different colors.
 *       If !withApex, then just draws the surfaces in all white.
 */
void BasicSimulator::drawProjections(const bool drawFrustum,
                                     const gmtl::Vec3f& surfColor,
                                     const float scaleFactor)
{
   const float ALPHA_VALUE(0.25f);

   DisplayManager* display_man =
      vrj::direct3d::DrawManager::instance()->getDisplayManager();
   display_man->updateProjections(scaleFactor);                     // Update all projections for drawing

   // XXX: Not implemented
}

/**
 * Draws a simulator using OpenGL commands.
 * @note This is called internally by the library.
 */
void BasicSimulator::drawSimulator(const float scaleFactor)
{
   // XXX: Not implemented
}

}

}
