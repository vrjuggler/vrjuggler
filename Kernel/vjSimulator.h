#ifndef _VJ_SIMULATOR_H_
#define _VJ_SIMULATOR_H_

#include <Kernel/vjKernel.h>
#include <Input/InputManager/vjInputManager.h>
#include <Kernel/vjDebug.h>
#include <Input/InputManager/vjPosInterface.h>

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

      mCamera.init(proxy_str); 
      mHead.init("VJHead");
      mWand.init("VJWand");
   }

   //: Update internal simulator data
   void update()
   {
      mHeadPos = *(mHead->GetData());
      mWandPos = *(mWand->GetData());

      mCameraPos = *(mCamera->GetData());
      mCameraPos.invert(mCameraPos);         // Invert to get viewing version
   }

   vjMatrix getCameraPos()
   { return mCameraPos; }

   vjMatrix getHeadPos()
   { return mHeadPos; }

   vjMatrix getWandPos()
   { return mWandPos; }

private:
   vjPosInterface mCamera;     // Prosy interfaces to devices needed
   vjPosInterface mHead;
   vjPosInterface mWand;

   vjMatrix    mCameraPos;    // The data about the position of all this stuff
   vjMatrix    mHeadPos;
   vjMatrix    mWandPos;
};

#endif
