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
      char* head_str = NULL;
      head_str  = chunk->getProperty("headPos");

      mCamera.init(proxy_str);
      mHead.init(head_str);
      mWand.init("VJWand");

      if(mCamera.getProxyIndex() == -1)
      {
         cerr << "vjSimulator:: Fatal Error: Camera not found named: " << proxy_str << endl;
         exit(1);
      }

      if(mHead.getProxyIndex() == -1)
      {
         cerr << "vjSimulator:: Fatal Error: Head not found named: " << head_str << endl;
         exit(1);
      }
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
