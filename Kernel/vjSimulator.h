#ifndef _VJ_SIMULATOR_H_
#define _VJ_SIMULATOR_H_

#include <vjConfig.h>
#include <Kernel/vjKernel.h>
#include <Input/InputManager/vjInputManager.h>
#include <Kernel/vjDebug.h>
#include <Input/InputManager/vjPosInterface.h>
#include <Kernel/vjUser.h>


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

      std::string wand_proxy_str = chunk->getProperty("simWand");
      std::string user_name = (std::string)(char*)chunk->getProperty("user");

      mUser = vjKernel::instance()->getUser(user_name);

      mCamera.init(proxy_str);
      mWand.init(wand_proxy_str);      // Configure the wand to use

      if(mCamera.getProxyIndex() == -1)
      {
         cerr << "vjSimulator:: Fatal Error: Camera not found named: " << proxy_str << endl;
         exit(1);
      }

      if(mUser == NULL)
      {
         cerr << "vjSimulator:: Fatal Error: User not found named: " << user_name << endl;
         exit(1);
      }
   }

   //: Update internal simulator data
   void update()
   {
      mHeadPos = *(mUser->getHeadPos());
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
   vjPosInterface mWand;
   vjUser*        mUser;

   vjMatrix    mCameraPos;    // The data about the position of all this stuff
   vjMatrix    mHeadPos;
   vjMatrix    mWandPos;
};

#endif
