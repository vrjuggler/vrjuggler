#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gl/device.h>
#include <Performer/pf/pfChannel.h>
#include <Performer/pf/pfEarthSky.h>
#include <Performer/pf/pfLightSource.h>
#include <Performer/pf/pfNode.h>
#include <Performer/pf/pfTraverser.h>
#include <Performer/pf/pfDCS.h>
#include <Performer/pfdu.h>
#include <Performer/pfutil.h>

    // --- VR Juggler Stuff --- //
#include <Kernel/vjKernel.h>
#include <Kernel/Pf/vjPfApp.h>
#include <Kernel/vjDebug.h>
#include <Kernel/vjProjection.h>

/*
#include <pfNaver.h>
#include <collidor.h>
#include <planeCollidor.h>
#include <pfCollidor.h>
*/

int lightPreDraw(pfTraverser *trav, void *userData);
int lightPostDraw(pfTraverser* trav, void* userData);
int lightPreCull(pfTraverser *trav, void *userData);
int lightPostCull(pfTraverser* trav, void* userData);
int lightPreApp(pfTraverser *trav, void *userData);
int lightPostApp(pfTraverser* trav, void* userData);
int light_dcsPreCull(pfTraverser* trav, void* userData);
int light_dcsPostCull(pfTraverser* trav, void* userData);

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
      vjProjection::setNearFar(0.5, 1000);
   }

   virtual void apiInit()
   {
      vjDEBUG(vjDBG_ALL,1) << "app::apiInit\n" << vjDEBUG_FLUSH;
   }

   /// Initialize the scene graph
   virtual void initScene()
   {
      // Setup the state like we need

      int cur_light_state = pfGetEnable(PFEN_LIGHTING);
      vjDEBUG(vjDBG_ALL,0) << "initScene: Current lighting state: " << cur_light_state << endl << vjDEBUG_FLUSH;

      // Load the scene
      vjDEBUG(vjDBG_ALL, 0) << "app::initScene\n" << vjDEBUG_FLUSH;
      rootNode = new pfGroup;

      //naver = new pfNaver();

      sun1 = new pfLightSource;
      pfDCS* fake_dcs = new pfDCS;
      sun1->setPos(0.0f, -1.0f, 0.0f, 0.0f);
      sun1->setColor(PFLT_DIFFUSE,1.0f,0.0f,0.0f);
      sun1->setColor(PFLT_AMBIENT,0.0f,1.0f,0.0f);
      sun1->setColor(PFLT_SPECULAR, 1.0f, 1.0f, 1.0f);
      //naver->addChild(sun1);

      fake_dcs->addChild(sun1);
      //sun1->on();     // By default

      sun1->setTravFuncs(PFTRAV_DRAW, lightPreDraw, lightPostDraw);
      sun1->setTravFuncs(PFTRAV_CULL, lightPreCull, lightPostCull);
      sun1->setTravFuncs(PFTRAV_APP, lightPreApp, lightPostApp);
      fake_dcs->setTravFuncs(PFTRAV_CULL, light_dcsPreCull, light_dcsPostCull);

      //rootNode->addChild(new pfLightSource);

      /// Load SIMPLE geometry
      ///*
      //pfNode* obj = pfdLoadFile("/usr/share/Performer/data/chamber.0.lsa");
      pfNode* obj = pfdLoadFile("/usr/share/Performer/data/klingon.flt");
      pfDCS* world_model = new pfDCS;    // The node with the world under it
      //rootNode->addChild(naver);
      rootNode->addChild(world_model);
      rootNode->addChild(fake_dcs);

      world_model->addChild(obj);
      world_model->setScale(0.25f);
      world_model->setTrans(0.0,5.0,-5.0);
      vjMatrix initial_pos;
      initial_pos.setTrans(0,0,0);
      //naver->getNavigator()->setCurPos(initial_pos);
      //naver->addChild(world_model);
      //*/

      // Load the TOWN
      /*
      pfFilePath("/usr/share/Performer/data:/usr/share/Performer/data/town");
      pfNode* obj = pfdLoadFile("/usr/share/Performer/data/town/town_ogl_pfi.pfb");
      pfDCS* world_model = new pfDCS;    // The node with the world under it
      rootNode->addChild(naver);

      world_model->addChild(obj);
      world_model->setScale(3.0f);
      vjMatrix initial_pos;
      initial_pos.setTrans(7500,50,-7500);
      naver->getNavigator()->setCurPos(initial_pos);
      naver->addChild(world_model);

      //planeCollidor* collide = new planeCollidor;
      pfCollidor* collide = new pfCollidor(world_model);

      naver->getNavigator()->setCollidor(collide);
      */

      pfuTravPrintNodes(rootNode, "nodes.out");
      pfdStoreFile(rootNode, "nodes.pfb");
   }

   /// Return the current scene graph
   virtual pfGroup* getScene()
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
      static float x=10;
      static float y=-5;
      static float z=3;
      static float amb=0.0f;

      x+=0.05;
      y+=0.04;
      z+=0.02;
      amb += 0.025f;

      if(x>20.0f)
         x = -20.0f;
      if(y>20.0f)
         y = -20.0f;
      if(z>20.0f)
         z = -20.0f;
      if(amb>1.0f)
         amb = 0.0f;

      //sun1->setPos(x,y,z, 1.0f);
      cerr << "set ambient: " << amb << endl;
      sun1->setColor(PFLT_AMBIENT, amb, amb, amb);

      pfSphere* bound = new pfSphere;
      sun1->getBound(bound);
      cerr << "rad: " << bound->radius << endl;
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
   pfLightSource* sun1;

   //pfNaver*    naver;
   //pfDCS*      baseDCS;
   pfGroup*   rootNode;
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




int lightPreDraw(pfTraverser *trav, void *userData)
{ cerr << "trav: light preDraw\n"; return PFTRAV_CONT;}

int lightPostDraw(pfTraverser* trav, void* userData)
{ cerr << "trav: light postDraw\n"; return PFTRAV_CONT;}

int lightPreCull(pfTraverser *trav, void *userData)
{
   cerr << "trav: light preCull\n";
   pfSphere* bound = new pfSphere;
   trav->getNode()->getBound(bound);
   cerr << "rad: " << bound->radius << endl;
   return PFTRAV_CONT;
}

int lightPostCull(pfTraverser* trav, void* userData)
{
   cerr << "trav: light postCull\n";
   pfSphere* bound = new pfSphere;
   trav->getNode()->getBound(bound);
   cerr << "rad: " << bound->radius << endl;
   return PFTRAV_CONT;
}

int lightPreApp(pfTraverser *trav, void *userData)
{
   cerr << "trav: light preApp\n";
   pfSphere* bound = new pfSphere;
   trav->getNode()->getBound(bound);
   cerr << "rad: " << bound->radius << endl;
   return PFTRAV_CONT;
}

int lightPostApp(pfTraverser* trav, void* userData)
{
   cerr << "trav: light postApp\n";
   pfSphere* bound = new pfSphere;
   trav->getNode()->getBound(bound);
   cerr << "rad: " << bound->radius << endl;
   return PFTRAV_CONT;
}

int light_dcsPreCull(pfTraverser* trav, void* userData)
{
   cerr << "trav: light_dcs: preCull\n";
   pfCullResult(PFIS_MAYBE|PFIS_TRUE|PFIS_ALL_IN);
   return PFTRAV_CONT;
}

int light_dcsPostCull(pfTraverser* trav, void* userData)
{
   cerr << "trav: light_dcs: postCull\n";
   return PFTRAV_CONT;
}

