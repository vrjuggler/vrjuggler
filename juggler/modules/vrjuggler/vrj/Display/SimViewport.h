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

#ifndef _VRJ_SIM_DISPLAY_H_
#define _VRJ_SIM_DISPLAY_H_
//#pragma once

#include <vrj/vrjConfig.h>
#include <vrj/Math/Matrix.h>
#include <vrj/Math/Coord.h>
#include <vrj/Util/Debug.h>
#include <vrj/Display/Viewport.h>
#include <vrj/Display/CameraProjection.h>

#include <gadget/Type/PositionInterface.h>


#include <jccl/Config/ConfigChunkPtr.h>

namespace vrj
{

class SimViewport : public Viewport
{
public:
   SimViewport()
   {;}

public:
   //: Configure the simulator
   virtual void config(jccl::ConfigChunkPtr chunk)
   {
      vprASSERT(chunk.get() != NULL);
      vprASSERT((std::string)chunk->getType() == std::string("simViewport"));

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

      std::string camera_proxy_str = chunk->getProperty("cameraPos");
      std::string wand_proxy_str = chunk->getProperty("wandPos");

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
      mDrawProjections = chunk->getProperty("drawProjections");
      mSurfaceColor[0] = chunk->getProperty("surfaceColor", 0);
      mSurfaceColor[1] = chunk->getProperty("surfaceColor", 1);
      mSurfaceColor[2] = chunk->getProperty("surfaceColor", 2);
   }

   virtual void updateProjections()
   {
      updateInternalData();
      Matrix camera_pos = getCameraPos();
      //mCameraProj->calcViewMatrix(camera_pos);

      Matrix left_eye_pos, right_eye_pos;     // NOTE: Eye coord system is -z forward, x-right, y-up

      // -- Calculate camera (eye) Positions -- //
      Coord  cam_coord(camera_pos);       // Create a user readable version

      vprDEBUG(vprDBG_ALL,5)
         << "vjDisplay::updateProjections: Getting cam position" << std::endl
         << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_ALL,5) << "CamPos:" << cam_coord.pos << "\tHeadOr:"
                           << cam_coord.orient << std::endl << vprDEBUG_FLUSH;

      // Compute location of left and right eyes
      float interocularDist = mUser->getInterocularDistance();
      float eye_offset = interocularDist/2.0f;      // Distance to move eye

      left_eye_pos.postTrans(camera_pos, -eye_offset, 0, 0);
      right_eye_pos.postTrans(camera_pos, eye_offset, 0, 0);

      mLeftProj->calcViewMatrix(left_eye_pos);
      mRightProj->calcViewMatrix(right_eye_pos);
   }

   Matrix getCameraPos()
   { return mCameraPos; }

   Matrix getHeadPos()
   { return mHeadPos; }

   Matrix getWandPos()
   { return mWandPos; }


public:  // Sim Drawing parameters
   bool shouldDrawProjections()
   { return mDrawProjections; }

   Vec3 getSurfaceColor()
   { return mSurfaceColor; }

protected:
    //: Update internal simulator data
   void updateInternalData()
   {
      mHeadPos = *(mUser->getHeadPos());
      mWandPos = *(mWand->getData());

      mCameraPos = *(mCamera->getData());
      mCameraPos.invert(mCameraPos);         // Invert to get viewing version
   }

private:
   // Drawing attributes
   bool   mDrawProjections;    //: Should we draw projections
   Vec3   mSurfaceColor;       //: Color to draw surfaces

   gadget::PositionInterface mCamera;     // Prosy interfaces to devices needed
   gadget::PositionInterface mWand;

   Matrix    mCameraPos;    // The data about the position of all this stuff
   Matrix    mHeadPos;
   Matrix    mWandPos;
};

};
#endif
