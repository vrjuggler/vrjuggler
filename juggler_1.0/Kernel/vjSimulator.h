#ifndef _VJ_SIMULATOR_H_
#define _VJ_SIMULATOR_H_

#include <Kernel/vjKernel.h>
#include <Input/InputManager/vjInputManager.h>
#include <Kernel/vjDebug.h>
#include <mstring.h>

class vjSimulator
{
public:
   vjSimulator()
   {;}

public:
   //: Configure the simulator
   void config(vjConfigChunk* chunk)
   {
      char* proxy_str = NULL;
      proxy_str  = chunk->getProperty("simCamera");

      vjInputManager* input_group = vjKernel::instance()->getInputManager();
      mCameraIndex = input_group->GetProxyIndex(proxy_str); 
      mHeadIndex = input_group->GetProxyIndex("VJHead");
      mWandIndex = input_group->GetProxyIndex("VJWand");

      if((mCameraIndex == -1) || (mHeadIndex == -1) || (mWandIndex == -1))
      {
         vjDEBUG(0) << "vjSimulator::config: ERROR: Can't find Proxy for camera or head or wand" << endl << vjDEBUG_FLUSH;
      }
   }

   //: Update internal simulator data
   void update()
   {
      vjMatrix *camera_matrix, *head_matrix, *wand_matrix;
      vjInputManager* input_group = vjKernel::instance()->getInputManager();
      camera_matrix = input_group->GetPosData(mCameraIndex);
      head_matrix = input_group->GetPosData(mHeadIndex);
      wand_matrix = input_group->GetPosData(mWandIndex);

         // --- Copy Over wand and head --- //
      mHeadPos = *head_matrix;
      mWandPos = *wand_matrix;

         // --- Compute camera view --- //
         // Rotation is rotation of the world
         // Translation is translation of the eye
      /*
      float x_trans, y_trans, z_trans;
      mCameraPos = *camera_matrix;
      mCameraPos.getTrans(x_trans, y_trans, z_trans);
      mCameraPos.preTrans(-x_trans, -y_trans, -z_trans, mCameraPos);
      mCameraPos.postTrans(mCameraPos, x_trans, y_trans, z_trans);
      */
      mCameraPos = *camera_matrix;
      mCameraPos.invert(mCameraPos);
   }

   vjMatrix getCameraPos()
   { return mCameraPos; }

   vjMatrix getHeadPos()
   { return mHeadPos; }

   vjMatrix getWandPos()
   { return mWandPos; }

private:
   int   mCameraIndex;     // The index of the posProxy for the camera
   int   mHeadIndex;
   int   mWandIndex;

   vjMatrix    mCameraPos;    // The data about the position of all this stuff
   vjMatrix    mHeadPos;
   vjMatrix    mWandPos;
};

#endif
