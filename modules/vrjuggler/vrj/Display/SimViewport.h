/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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

#ifndef _VRJ_SIM_DISPLAY_H_
#define _VRJ_SIM_DISPLAY_H_
//#pragma once

#include <vrj/vrjConfig.h>
//#include <vrj/Math/Coord.h>
#include <vrj/Util/Debug.h>
#include <vrj/Display/Viewport.h>
#include <vrj/Display/CameraProjection.h>

#include <gadget/Type/PositionInterface.h>

#include <gmtl/Matrix.h>
#include <gmtl/Generate.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>
#include <gmtl/Xforms.h>
#include <gmtl/Output.h>

#include <jccl/Config/ConfigChunkPtr.h>

namespace vrj
{

class SimViewport : public Viewport
{
public:
   SimViewport()
   {;}

public:
   /**  Configure the simulator */
   virtual void config(jccl::ConfigChunkPtr chunk)
   {
      vprASSERT(chunk.get() != NULL);
      vprASSERT(chunk->getDescToken() == std::string("simViewport"));

      Viewport::config(chunk);

      mType = SIM;

      mLeftProj = new CameraProjection;
      mLeftProj->config(chunk);
      mLeftProj->setEye(Projection::LEFT);
      mLeftProj->setViewport(this);

      mRightProj = new CameraProjection;
      mRightProj->config(chunk);
      mRightProj->setEye(Projection::RIGHT);
      mRightProj->setViewport(this);

      std::string camera_proxy_str = chunk->getProperty<std::string>("cameraPos");
      std::string wand_proxy_str = chunk->getProperty<std::string>("wandPos");

      mCamera.init(camera_proxy_str);
      mWand.init(wand_proxy_str);      // Configure the wand to use

      if(!mCamera.isConnected())
      {
         vprDEBUG(vprDBG_ERROR,0)
            << clrOutNORM(clrRED,"ERROR:")
            << "vjSimViewport:: Fatal Error: Camera not found named: "
            << camera_proxy_str.c_str() << vprDEBUG_FLUSH;
         vprASSERT(false);
      }

      // Get drawing parameters
      mDrawProjections = chunk->getProperty<bool>("drawProjections");
      mSurfaceColor[0] = chunk->getProperty<float>("surfaceColor", 0);
      mSurfaceColor[1] = chunk->getProperty<float>("surfaceColor", 1);
      mSurfaceColor[2] = chunk->getProperty<float>("surfaceColor", 2);
   }

   virtual void updateProjections()
   {
      updateInternalData();
      gmtl::Matrix44f camera_pos = getCameraPos();
      gmtl::Vec3f camera_trans = gmtl::makeTrans<gmtl::Vec3f>(camera_pos);

      //mCameraProj->calcViewMatrix(camera_pos);

      gmtl::Matrix44f left_eye_pos, right_eye_pos;     // NOTE: Eye coord system is -z forward, x-right, y-up

      // -- Calculate camera (eye) Positions -- //
      vprDEBUG(vprDBG_ALL,7)
         << "vjDisplay::updateProjections: Getting cam position" << std::endl
         << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_ALL,7) << "CamPos:" << camera_trans << std::endl << vprDEBUG_FLUSH;

      // Compute location of left and right eyes
      float interocularDist = mUser->getInterocularDistance();
      float eye_offset = interocularDist / 2.0f;      // Distance to move eye

      left_eye_pos = camera_pos * gmtl::makeTrans<gmtl::Matrix44f>( gmtl::Vec3f(-eye_offset, 0.0f, 0.0f) );
      right_eye_pos = camera_pos * gmtl::makeTrans<gmtl::Matrix44f>( gmtl::Vec3f(eye_offset, 0.0f, 0.0f) );

      mLeftProj->calcViewMatrix(left_eye_pos);
      mRightProj->calcViewMatrix(right_eye_pos);
   }

   gmtl::Matrix44f getCameraPos()
   { return mCameraPos; }

   gmtl::Matrix44f getHeadPos()
   { return mHeadPos; }

   gmtl::Matrix44f getWandPos()
   { return mWandPos; }


public:  // Sim Drawing parameters
   bool shouldDrawProjections()
   { return mDrawProjections; }

   gmtl::Vec3f getSurfaceColor()
   { return mSurfaceColor; }

protected:
    /**  Update internal simulator data */
   void updateInternalData()
   {
      mHeadPos = *(mUser->getHeadPos());
      mWandPos = *(mWand->getData());

      mCameraPos = *(mCamera->getData());
      gmtl::invert(mCameraPos);
   }

private:
   // Drawing attributes
   bool   mDrawProjections;       /**< Should we draw projections */
   gmtl::Vec3f   mSurfaceColor;   /**< Color to draw surfaces */

   gadget::PositionInterface mCamera; /**< Proxy interfaces to devices needed */
   gadget::PositionInterface mWand;

   gmtl::Matrix44f    mCameraPos;    /**< The data about the position of all this stuff */
   gmtl::Matrix44f    mHeadPos;
   gmtl::Matrix44f    mWandPos;
};

};
#endif
