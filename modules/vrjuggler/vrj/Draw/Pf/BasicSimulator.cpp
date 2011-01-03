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

#include <vrj/Draw/Pf/Config.h>

#include <boost/concept_check.hpp>
#include <gmtl/Matrix.h>
#include <gmtl/Generate.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>
#include <gmtl/Xforms.h>
#include <gmtl/Output.h>

#include <Performer/pf.h>
#include <Performer/pfdu.h>
#include <Performer/pf/pfDCS.h>
#include <Performer/pf/pfScene.h>
#include <Performer/pf/pfChannel.h>
#include <Performer/pf/pfLightSource.h>
#include <Performer/pfdu.h>
#include <Performer/pf/pfTraverser.h>

#include <vpr/Util/FileUtils.h>
#include <jccl/Config/ConfigElement.h>

#include <vrj/Kernel/User.h>

#include <vrj/Draw/Pf/DrawManager.h>

#include <vrj/Display/DisplayManager.h>
#include <vrj/Display/Projection.h>
#include <vrj/Display/Viewport.h>
#include <vrj/Display/SimViewport.h>
#include <vrj/Display/SurfaceViewport.h>

#include <vrj/Draw/Pf/SimInterfaceFactory.h>
#include <vrj/Draw/Pf/Util.h>

#include <vrj/Draw/Pf/BasicSimulator.h>


namespace vrj
{

namespace pf
{

VRJ_REGISTER_PF_SIM_INTERFACE_CREATOR(BasicSimulator);

BasicSimulator::BasicSimulator()
   : mRootWithSim(NULL)
   , mSimTree(NULL)
   , mHeadDCS(NULL)
   , mWandDCS(NULL)
{
   //setDrawWandFunctor(new GlDrawConeWandFunctor());
   //setDrawWandFunctor(new GlDrawRightAngleWandFunctor());
}

// Configure the basic simulator config.
bool BasicSimulator::config(jccl::ConfigElementPtr element)
{
   vprASSERT(element.get() != NULL);
   vprASSERT(element->getID() == std::string("default_simulator"));

   std::string camera_proxy_str = element->getProperty<std::string>("camera_pos");
   std::string wand_proxy_str = element->getProperty<std::string>("wand_pos");

   mCamera.init(camera_proxy_str);
   mWand.init(wand_proxy_str);      // Configure the wand to use

   if ( ! mCamera.isConnected() )
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED, "ERROR")
         << ": [vrj::pf::BasicSimulator::config()] Fatal Error: Camera not "
         << "found named: " << camera_proxy_str << vprDEBUG_FLUSH;
      vprASSERT(false);
   }

   // Get drawing parameters
   mDrawProjections = element->getProperty<bool>("draw_projections");
   mSurfaceColor[0] = element->getProperty<float>("surface_color", 0);
   mSurfaceColor[1] = element->getProperty<float>("surface_color", 1);
   mSurfaceColor[2] = element->getProperty<float>("surface_color", 2);

   configPerformerAPI(element);

   return true;
}

// Sets the keyboard/mouse device the simulator can use to get input from the
// user.
void
BasicSimulator::setKeyboardMouse(gadget::KeyboardMouseInterface kmInterface)
{
   boost::ignore_unused_variable_warning(kmInterface);
}

void BasicSimulator::updateProjectionData(const float positionScale,
                                          ProjectionPtr leftProj,
                                          ProjectionPtr rightProj)
{
   updateInternalData(positionScale);

   gmtl::Matrix44f camera_pos = getCameraPos();
   gmtl::Vec3f camera_trans = gmtl::makeTrans<gmtl::Vec3f>(camera_pos);

   gmtl::Matrix44f left_eye_pos, right_eye_pos;     // NOTE: Eye coord system is -z forward, x-right, y-up

   // -- Calculate camera (eye) Positions -- //
   vprDEBUG(vprDBG_ALL, vprDBG_HEX_LVL)
      << "[vrj::pf::BasicSimulator::updateProjectionData()] Getting cam "
      << "position" << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL, vprDBG_HEX_LVL)
      << "CamPos:" << camera_trans << std::endl << vprDEBUG_FLUSH;

   // Compute location of left and right eyes
   float interocular_dist = mSimViewport->getUser()->getInterocularDistance();
   interocular_dist *= positionScale;               // Scale into correct units
   float eye_offset = interocular_dist / 2.0f;      // Distance to move eye

   left_eye_pos = camera_pos * gmtl::makeTrans<gmtl::Matrix44f>( gmtl::Vec3f(-eye_offset, 0.0f, 0.0f) );
   right_eye_pos = camera_pos * gmtl::makeTrans<gmtl::Matrix44f>( gmtl::Vec3f(eye_offset, 0.0f, 0.0f) );

   leftProj->calcViewMatrix(left_eye_pos, positionScale);
   rightProj->calcViewMatrix(right_eye_pos, positionScale);
}

// Update internal simulator data.
void BasicSimulator::updateInternalData(float positionScale)
{
   mHeadPos = mSimViewport->getUser()->getHeadPosProxy()->getData(positionScale);
   mWandPos = mWand->getData(positionScale);

   mCameraPos = mCamera->getData(positionScale);
   gmtl::invert(mCameraPos);
}

bool BasicSimulator::configPerformerAPI(jccl::ConfigElementPtr element)
{
   //vprASSERT(Element->getID() == std::string("apiPerformer"));

   vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL)
   << "[vrj::pf::BasicSimulator::configPerformerAPI()] "
   << " Configuring Performer\n" << vprDEBUG_FLUSH;

   // --- Get simulator model info --- //
   std::string head_file = element->getProperty<std::string>("head_model");
   std::string wand_file = element->getProperty<std::string>("wand_model");
   if(head_file.empty())
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
         << "WARNING: [vrj::pf::BasicSimulator::configPerformerAPI()] "
         << "head_model not set." << std::endl << vprDEBUG_FLUSH;
   }
   if(wand_file.empty())
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
         << "WARNING: [vrj::pf::BasicSimulator::configPerformerAPI()] "
         << "wand_model not set." << std::endl << vprDEBUG_FLUSH;
   }

   mHeadModel = vpr::replaceEnvVars(head_file);
   mWandModel = vpr::replaceEnvVars(wand_file);

   vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL)
      << "Head Model: " << mHeadModel.c_str() << std::endl
      << "Wand Model: " << mWandModel.c_str() << std::endl << vprDEBUG_FLUSH;

   mRootWithSim = vrj::pf::DrawManager::instance()->getRootWithSim();
   if (NULL != mRootWithSim)
   {
      initSimulatorGraph();
      return true;
   }
   return false;
}

void BasicSimulator::initSimulatorGraph()
{
   pfNode* head_node(NULL);
   pfNode* wand_node(NULL);

   if(!mHeadModel.empty())
   {
      head_node = pfdLoadFile(mHeadModel.c_str());     // Load head model
      vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL)
         << "[vrj::pf::BasicSimulator::initSimulatorGraph()] Loaded head "
         << "model: " << mHeadModel << std::endl << vprDEBUG_FLUSH;
   }
   else
   {
      vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL)
         << "[vrj::pf::BasicSimulator::initSimulatorGraph()] "
         << "No wand head specified.\n" << vprDEBUG_FLUSH;
   }

   if(!mWandModel.empty())
   {
      wand_node = pfdLoadFile(mWandModel.c_str());     // Load wand model
      vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL)
         << "[vrj::pf::BasicSImulator::initSimulatorGraph()] "
         << "Loaded wand model: " << mWandModel << std::endl << vprDEBUG_FLUSH;
   }
   else
   {
      vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL)
         << "[vrj::pf::BasicSimulator::initSimulatorGraph()] "
         << "No wand model specified.\n" << vprDEBUG_FLUSH;
   }

   mSimTree = new pfGroup;

   //mRootWithSim = new pfScene;

   mHeadDCS = new pfDCS;
   mWandDCS = new pfDCS;
   mSimTree->addChild(mHeadDCS);
   mSimTree->addChild(mWandDCS);
   if(NULL != head_node)
   {
      mHeadDCS->addChild(head_node);
   }
   if(NULL != wand_node)
   {
      mWandDCS->addChild(wand_node);
   }

   if((head_node != NULL) && (wand_node != NULL))
   {
      mRootWithSim->addChild(mSimTree);      // Put sim stuff in the graph
   }
}

void BasicSimulator::updateSimulatorSceneGraph()
{
   gmtl::Matrix44f vj_head_mat = getHeadPos();          // Get Juggler matrices
   gmtl::Matrix44f vj_wand_mat = getWandPos();
   pfMatrix head_mat = GetPfMatrix(vj_head_mat);    // Convert to Performer
   pfMatrix wand_mat = GetPfMatrix(vj_wand_mat);
   mHeadDCS->setMat(head_mat);                        // Set the DCS nodes
   mWandDCS->setMat(wand_mat);
}

}

}
