#ifndef _CUBES_APP_
#define _CUBES_APP_

#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>

#include <Kernel/GL/vjGlApp.h>
#include <Kernel/GL/vjGlContextData.h>
#include <Kernel/vjDebug.h>

#include <Math/vjMatrix.h>
#include <Math/vjVec3.h>
#include <Kernel/vjDebug.h>

#include <Input/InputManager/vjPosInterface.h>
#include <Input/InputManager/vjAnalogInterface.h>
#include <Input/InputManager/vjDigitalInterface.h>

#include <Kernel/vjUser.h>



// Class to hold all context specific data
class ContextData
{
public:
   ContextData()
   {
      firstTime = true;
      cubeDLIndex = -1;
   }

public:
   bool  firstTime;
   int   cubeDLIndex;
};

// Class to hold all data for a specific user
class UserData
{
public:
   // Constructor
   // Takes the string names of the devices to use
   // NOTE: This means that we cannot construct a user until the input manager is loaded
   //       Ex. The Init function
   UserData(vjUser* user, std::string wandName, std::string incButton,
            std::string decButton, std::string stopButton)
   {
      mNavMatrix.makeIdent();

      mUser = user;
      // Initialize devices
      mWand.init(wandName);
      mIncVelocityButton.init(incButton);
      mDecVelocityButton.init(decButton);
      mStopButton.init(stopButton);
   }

   // Update the navigation matrix
   //
   // Uses a quaternion to do rotation in the environment
   void updateNavigation();

public:
      // Devices to use for the given user
   vjPosInterface       mWand;                  // the Wand
   vjDigitalInterface   mIncVelocityButton;     // Button for velocity
   vjDigitalInterface   mDecVelocityButton;
   vjDigitalInterface   mStopButton;            // Button to stop

      // Navigation info for the user
   float                mCurVelocity;  // The current velocity
   vjMatrix             mNavMatrix;    // Matrix for navigation in the application

   vjUser*              mUser;         // The user we hold data for
};

//--------------------------------------------------
// Demonstration OpenGL application class
//
// This application simply renders a field of cubes.
//---------------------------------------------------
class cubesApp : public vjGlApp
{
public:
   cubesApp(vjKernel* kern) : vjGlApp(kern)
   {
      ;
   }

   // Execute any initialization needed before the API is started
   virtual void init();

   // Execute any initialization needed <b>after</b> API is started
   //  but before the drawManager starts the drawing loops.
   virtual void apiInit()
   {
      vjDEBUG(vjDBG_ALL,0) << "---- cubesApp::apiInit() ----\n" << vjDEBUG_FLUSH;
   }

   // Called immediately upon opening a new OpenGL context
   virtual void contextInit();

   /** Function to draw the scene
    * PRE: OpenGL state has correct transformation and buffer selected
    * POST: The current scene has been drawn
    */
   virtual void draw()
   {
      initGLState();    // This should really be in another function

      myDraw(vjGlDrawManager::instance()->currentUserData()->getUser());
   }

   /**   name Drawing Loop Functions
    *
    *  The drawing loop will look similar to this:
    *
    *  while (drawing)
    *  {
    *        preFrame();
    *       draw();
    *        intraFrame();      // Drawing is happening while here
    *       sync();
    *        postFrame();      // Drawing is now done
    *
    *       UpdateTrackers();
    *  }
    *
    */

   /// Function called after tracker update but before start of drawing
   virtual void preFrame()
   {
       vjDEBUG(vjDBG_ALL,2) << "cubesApp::preFrame()" << endl << vjDEBUG_FLUSH;

       for(int i=0;i<mUserData.size();i++)
          mUserData[i]->updateNavigation();       // Update the navigation matrix
   }

   /// Function called after drawing has been triggered but BEFORE it completes
   virtual void intraFrame()
   {
      vjDEBUG(vjDBG_ALL,2) << "cubesApp::intraFrame()" << endl << vjDEBUG_FLUSH;
   }

   /// Function called before updating trackers but after the frame is drawn
   virtual void postFrame()
   {
      vjDEBUG(vjDBG_ALL,2) << "cubesApp::postFrame" << endl << vjDEBUG_FLUSH;
   }

private:

   //----------------------------------------------
   //  Draw the scene.  A bunch of boxes of differing color and stuff
   //----------------------------------------------
   void myDraw(vjUser* user);
   void initGLState();

   void drawCube()
   {
       glCallList(mDlData->cubeDLIndex);
       //drawbox(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, GL_QUADS);
   }

   void drawbox(GLdouble x0, GLdouble x1, GLdouble y0, GLdouble y1,
                GLdouble z0, GLdouble z1, GLenum type);

public:
   vjGlContextData<ContextData>  mDlData;       // Data for display lists
   std::vector<UserData*>        mUserData;     // All the users in the program
};


#endif
