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

    // --- VR Juggler Stuff --- //
#include <Kernel/vjKernel.h>
#include <Kernel/Pf/vjPfApp.h>
#include <Kernel/vjDebug.h>
#include <Kernel/vjProjection.h>

#include <pfNaver.h>
#include <collidor.h>
#include <planeCollidor.h>
#include <pfCollidor.h>

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
      vjProjection::setNearFar(1.0, 10000);
   }

   virtual void apiInit()
   {
      vjDEBUG(vjDBG_ALL,1) << "app::apiInit\n" << vjDEBUG_FLUSH;
      pfEnable(PFEN_LIGHTING);

   }

   /// Initialize the scene graph
   virtual void initScene()
   {

      // Setup the state like we need
      //pfEnable(PFEN_LIGHTING);
      pfEnable(PFEN_LIGHTING);

      int cur_light_state = pfGetEnable(PFEN_LIGHTING);
      cout << "initScene: Current lighting state: " << cur_light_state << endl;

      // Load the scene
      vjDEBUG(vjDBG_ALL, 0) << "app::initScene\n" << vjDEBUG_FLUSH;
      rootNode = new pfScene;

      naver = new pfNaver();
      pfLightSource* sun = new pfLightSource;
      pfLightSource* sun1 = new pfLightSource;

      //pfGeoState* gstate = new pfGeoState;
      //gstate->setMode(PFSTATE_ENLIGHTING, PF_ON);
      //gstate->setMode(PFSTATE_CULLFACE, PFCF_OFF);
      //rootNode->setGState(gstate);


      /*
      sun->setPos(0.0f, 1.0f, 0.0f, 0.0f);
      sun->setColor(PFLT_DIFFUSE,1.0f,1.0f,0.8f);
      sun->setColor(PFLT_AMBIENT,0.4f,0.4f,0.3f);
      rootNode->addChild(sun);
      sun->on();

      sun1->setPos(0.0f, 0.0f, 1.0f, 0.0f);
      sun1->setColor(PFLT_DIFFUSE,1.0f,1.0f,0.8f);
      sun1->setColor(PFLT_AMBIENT,0.4f,0.4f,0.3f);
      rootNode->addChild(sun1);
      sun1->on();
      */

      rootNode->addChild(new pfLightSource);

      pfDCS* world_model = new pfDCS;    // The node with the world under it
      //rootNode->addChild(naver);

      // Load the data files
      //pfFilePath("/usr/share/Performer/data:/usr/share/Performer/data/town");
      pfNode* obj = pfdLoadFile("/usr/share/Performer/data/chamber.0.lsa");
      //pfNode* obj = pfdLoadFile("/usr/share/Performer/data/cow.obj");
      //pfNode* obj = pfdLoadFile("/usr/share/Performer/data/town/town_ogl_pfi.pfb");

      /*
      world_model->addChild(obj);
      //world_model->setScale(3.0f);
      //vjMatrix initial_pos;  initial_pos.setTrans(-2500,-400,2500);
      vjMatrix initial_pos;
      //initial_pos.setTrans(7500,50,-7500);
      //initial_pos.setTrans(100,200,350);
      naver->getNavigator()->setCurPos(initial_pos);
      naver->addChild(world_model);

      //planeCollidor* collide = new planeCollidor;
      pfCollidor* collide = new pfCollidor(world_model);

      naver->getNavigator()->setCollidor(collide);
      */

      rootNode->addChild(obj);
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





