#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gl/device.h>
#include <Performer/pf/pfChannel.h>
#include <Performer/pf/pfEarthSky.h>
#include <Performer/pf/pfLightSource.h>
#include <Performer/pf/pfDCS.h>
#include <Performer/pfdu.h>

    // --- New Lib Stuff --- //
#include <Kernel/vjKernel.h>
#include <Kernel/Pf/vjPfApp.h>
#include <Kernel/vjDebug.h>

#include <pfNaver.h>

// Declare my application class
class myApp : public vjPfApp
{
public:
   myApp(vjKernel* kern) : vjPfApp(kern)
   {
      //rotation = 0.0f;
      //zPos = 0.0f;
   }

   virtual void init()
   {
      vjDEBUG(vjDBG_ALL, 1) << "app::init\n" << vjDEBUG_FLUSH;
   }

   virtual void apiInit()
   {
      vjDEBUG(vjDBG_ALL,1) << "app::apiInit\n" << vjDEBUG_FLUSH;
   }

   /// Initialize the scene graph
   virtual void initScene()
   {
      vjDEBUG(vjDBG_ALL, 0) << "app::initScene\n" << vjDEBUG_FLUSH;
      rootNode = new pfScene;
      naver = new pfNaver();
      pfLightSource* sun = new pfLightSource;
      pfLightSource* sun1 = new pfLightSource;
      pfGeoState* gstate = new pfGeoState;

      gstate->setMode(PFSTATE_ENLIGHTING, PF_ON);
      gstate->setMode(PFSTATE_CULLFACE, PFCF_OFF);
      rootNode->setGState(gstate);

      sun->setPos(3, -15, 20, 1);
      rootNode->addChild(sun);
      sun1->setPos(-3, -15, 20, 1);
      rootNode->addChild(sun1);
      sun1->on();

      rootNode->addChild(naver);

      pfNode* obj = pfdLoadFile("/usr/share/Performer/data/chamber.0.lsa");

      naver->addChild(obj);
      naver->setScale(10.0f);
      naver->setTrans(0.0, 0.0, -10.0f);
   }

   /// Return the current scene graph
   virtual pfScene* getScene()
   {
      vjDEBUG(vjDBG_ALL, 0) << "app::getScene\n" << vjDEBUG_FLUSH;
      return rootNode;
   }

   /// Function called before pfSync
   virtual void preSync()
   {
      //vjDEBUG(vjDBG_ALL, 1) << "app::preSync\n" << vjDEBUG_FLUSH;
   }

   /// Function called after pfSync and before pfDraw
   virtual void preDraw()
   {
      //vjDEBUG(vjDBG_ALL, 1) << "app::preDraw\n" << vjDEBUG_FLUSH;
   }

   /// Function called after pfDraw
   virtual void postDraw()
   {
      //vjDEBUG(vjDBG_ALL,1) << "app::postDraw\n" << vjDEBUG_FLUSH;
   }

public:
   int   wandIndex;     // the index of the wand
   int   button0Index;   // The index of the wand button
   int   button1Index;  // The index of button1

   pfNaver*    naver;
   //pfDCS*      baseDCS;
   pfScene*   rootNode;
};


float transSpeed = 0.1;

int main(int argc, char* argv[])
{
    vjKernel* kernel = vjKernel::instance();	// Declare a new Kernel
    myApp* application = new myApp(kernel);	// Delcare an instance of my application

    for ( int i = 1; i < argc; i++ ) {
        kernel->loadConfigFile(argv[i]);
    }

    kernel->start();

    kernel->setApplication(application);    // Set up the kernel

    //while(!kernel->done())
    while(1)
     {;}
}





