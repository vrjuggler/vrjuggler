#ifndef _VJ_SIM_DISPLAY_H_
#define _VJ_SIM_DISPLAY_H_
#pragma once

#include <vjConfig.h>
#include <Math/vjMatrix.h>
#include <Kernel/vjDebug.h>
#include <Input/InputManager/vjPosInterface.h>
#include <Kernel/vjDisplay.h>
#include <Kernel/vjCameraProjection.h>

class vjSimDisplay : public vjDisplay
{
public:
   vjSimDisplay()
   {;}

public:
   //: Configure the simulator
   virtual void config(vjConfigChunk* chunk)
   {
      vjDisplay::config(chunk);

      mType = SIM;
      mCameraProj = new vjCameraProjection;
      mCameraProj->config(chunk);

      std::string camera_proxy_str = chunk->getProperty("cameraPos");
      std::string wand_proxy_str = chunk->getProperty("wandPos");

      mCamera.init(camera_proxy_str);
      mWand.init(wand_proxy_str);      // Configure the wand to use

      if(mCamera.getProxyIndex() == -1)
      {
         vjDEBUG(0) << "ERROR: vjSimDisplay:: Fatal Error: Camera not found named: " << camera_proxy_str << endl;
         exit(1);
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
      vjMatrix camera_pos = getCameraPos();
      mCameraProj->calcViewMatrix(camera_pos);
   }

   vjMatrix getCameraPos()
   { return mCameraPos; }

   vjMatrix getHeadPos()
   { return mHeadPos; }

   vjMatrix getWandPos()
   { return mWandPos; }

   vjProjection* getCameraProj()
   { return mCameraProj; }

public:  // Sim Drawing parameters
   bool shouldDrawProjections()
   { return mDrawProjections; }

   vjVec3 getSurfaceColor()
   { return mSurfaceColor; }

protected:
    //: Update internal simulator data
   void updateInternalData()
   {
      mHeadPos = *(mUser->getHeadPos());
      mWandPos = *(mWand->GetData());

      mCameraPos = *(mCamera->GetData());
      mCameraPos.invert(mCameraPos);         // Invert to get viewing version
   }

private:
   // Drawing attributes
   bool     mDrawProjections;    //: Should we draw projections
   vjVec3   mSurfaceColor;       //: Color to draw surfaces

   /// Defines the projection for this window. Ex. RIGHT, LEFT, FRONT
   vjProjection*   mCameraProj;            // Camera projection. (For sim, etc.)

   vjPosInterface mCamera;     // Prosy interfaces to devices needed
   vjPosInterface mWand;

   vjMatrix    mCameraPos;    // The data about the position of all this stuff
   vjMatrix    mHeadPos;
   vjMatrix    mWandPos;
};

#endif
