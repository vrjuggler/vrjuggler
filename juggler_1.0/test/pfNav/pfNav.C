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
      cerr << "app::init\n";
      
      wandIndex = kernel->getInputManager()->GetProxyIndex("C2Wand");
      button0Index = kernel->getInputManager()->GetProxyIndex("C2Button0");
      button1Index = kernel->getInputManager()->GetProxyIndex("C2Button1");
      if(wandIndex == -1)
         cerr << "Wand not found\n";
   }

   virtual void apiInit()
   {
      cerr << "app::apiInit\n";
   }

   /// Initialize the scene graph
   virtual void initScene()
   {
      cerr << "app::initScene\n";
      rootNode = new pfScene;
      naver = new pfNaver(wandIndex, button0Index, button1Index);
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
      cerr << "app::getScene\n";
      return rootNode;
   }

   /// Function called before pfSync
   virtual void preSync()
   {
      cerr << "app::preSync\n";
   }

   /// Function called after pfSync and before pfDraw
   virtual void preDraw()
   {
      //cerr << "app::preDraw\n";
   }

   /// Function called after pfDraw
   virtual void postDraw()
   {
      //cerr << "app::postDraw\n";
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

int main(void)
{
    vjKernel* kernel = vjKernel::instance();	// Declare a new Kernel
    myApp* application = new myApp(kernel);	// Delcare an instance of my application
        
    kernel->setApplication(application);    // Set up the kernel
    kernel->start();
    
    //while(!kernel->done())
    while(1)
     {;}
}





