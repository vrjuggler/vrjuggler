#ifndef _CUBES_APP_
#define _CUBES_APP_

#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream.h>
#include <vector>

#include <Kernel/GL/vjGlApp.h>
#include <Kernel/GL/vjGlContextData.h>
#include <Kernel/vjDebug.h>

#include <Math/vjMatrix.h>
#include <Math/vjVec3.h>
#include <Math/vjQuat.h>
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
   void updateNavigation()
   {
      vjVec3 xyzAngles;
      vjVec3 xyzTrans;

      // Cur*Transform = New Location
      vjMatrix    transform, transformIdent;
      vjQuat      source_rot, goal_rot, slerp_rot;

      transformIdent.makeIdent();            // Create an identity matrix to rotate from
      source_rot.makeQuat(transformIdent);

      vjMatrix* wand_matrix;
      wand_matrix = mWand->GetData();
      wand_matrix->getXYZEuler(xyzAngles[0], xyzAngles[1], xyzAngles[2]);


      vjDEBUG(2) << "===================================\n" << vjDEBUG_FLUSH;
      vjDEBUG(2) << "Wand:\n" << *wand_matrix << endl << vjDEBUG_FLUSH;
      vjDEBUG(2) << "Wand XYZ: " << xyzAngles << endl << vjDEBUG_FLUSH;

      goal_rot.makeQuat(*wand_matrix);    // Create the goal rotation quaternion

      if(transformIdent != *wand_matrix)  // If we don't have two identity matrices
      {
         slerp_rot.slerp(0.05, source_rot, goal_rot);    // Transform part way there
         transform.makeQuaternion(slerp_rot);            // Create the transform matrix to use
      }
      else
         transform.makeIdent();

      vjDEBUG(2) << "Transform:\n" << transform << endl << vjDEBUG_FLUSH;
      transform.getXYZEuler(xyzAngles[0], xyzAngles[1], xyzAngles[2]);
      vjDEBUG(2) << "Transform XYZ: " << xyzAngles << endl << vjDEBUG_FLUSH;

      vjDEBUG(2) << "Nav:\n" << mNavMatrix << endl << endl << vjDEBUG_FLUSH;

      // ----- Translation ------- //
      const float velocity_inc = 0.001f;

      // Update velocity
      if(mIncVelocityButton->GetData())
         mCurVelocity += velocity_inc;
      else if(mDecVelocityButton->GetData())
         mCurVelocity -= velocity_inc;
      else if(mStopButton->GetData())
         mCurVelocity = 0.0f;

      if(mIncVelocityButton->GetData() || mDecVelocityButton->GetData())
         vjDEBUG(0) << "Velocity: " << mCurVelocity << endl << vjDEBUG_FLUSH;

      if(mIncVelocityButton->GetData() == vjDigital::TOGGLE_ON)
         vjDEBUG(0) << "-- Toggle ON --" << endl << vjDEBUG_FLUSH;
      if(mIncVelocityButton->GetData() == vjDigital::TOGGLE_OFF)
         vjDEBUG(0) << "-- Toggle OFF --" << endl << vjDEBUG_FLUSH;
      if(mIncVelocityButton->GetData() == vjDigital::ON)
         vjDEBUG(0) << "-- ON --" << endl << vjDEBUG_FLUSH;
      if(mIncVelocityButton->GetData() == vjDigital::OFF)
         vjDEBUG(1) << "-- OFF --" << endl << vjDEBUG_FLUSH;

      // Find direction vector
      vjVec3   forward(0.0f, 0.0f, -1.0f);
      forward *= mCurVelocity;

      vjMatrix rot_mat, local_xform;
      rot_mat.invert(transform);

      local_xform.makeTrans(0, 0, mCurVelocity);
      local_xform.postMult(rot_mat);

      mNavMatrix.preMult(local_xform);

      local_xform.getXYZEuler(xyzAngles[0], xyzAngles[1], xyzAngles[2]);
      local_xform.getTrans(xyzTrans[0], xyzTrans[1], xyzTrans[2]);
      vjDEBUG(2) << "Transform   Rot: " << xyzAngles << endl << vjDEBUG_FLUSH;
      vjDEBUG(2) << "Transform Trans: " << xyzTrans << endl << vjDEBUG_FLUSH;
      vjDEBUG(2) << "-------------------------------------------" << endl << vjDEBUG_FLUSH;
   }


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
   virtual void init()
   {
      cout << "---------- App:init() ---------------" << endl;
      std::vector<vjUser*> users = kernel->getUsers();
      int num_users = users.size();

      UserData* new_user=NULL;
      mUserData = std::vector<UserData*>(num_users,NULL);

      switch (num_users)
      {
      case (2):
         new_user = new UserData(users[1],"VJWand1", "VJButton0_1", "VJButton1_1", "VJButton2_1");
         mUserData[1] = new_user;
         vjASSERT(users[1]->getId() == 1);
      case (1):
         new_user = new UserData(users[0],"VJWand", "VJButton0", "VJButton1", "VJButton2");
         mUserData[0] = new_user;
         vjASSERT(users[0]->getId() == 0);
         break;
      default:
         vjDEBUG(0) << "ERROR: Bad number of users." << vjDEBUG_FLUSH;
         exit();
         break;
      }
   }

   // Execute any initialization needed <b>after</b> API is started
   //  but before the drawManager starts the drawing loops.
   virtual void apiInit()
   {
      ;
   }

   // Called immediately upon opening a new OpenGL context
   virtual void contextInit()
   {
      // Create display list
      vjASSERT(mDlData->firstTime == true);   // We should not have been here yet
      mDlData->firstTime = false;

      glGenLists(rand()%25);        // Generate some random lists.  NOTE: Needed for testing only
      mDlData->cubeDLIndex = glGenLists(1);

      vjDEBUG(0) << "Creating DL:" << mDlData->cubeDLIndex << endl << vjDEBUG_FLUSH;

      glNewList(mDlData->cubeDLIndex, GL_COMPILE);
         drawbox(-0.5, 0.5, -0.5, 0.5, -0.5, 0.5, GL_QUADS);
      glEndList();
   }

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
    *        preDraw();
    *	      draw();
    *	       postDraw();      // Drawing is happening while here
    *       sync();
    *        postSync();      // Drawing is now done
    *
    *	      UpdateTrackers();
    *  }
    *
    */

   /// Function called after tracker update but before start of drawing
   virtual void preDraw()
   {
       vjDEBUG(2) << "cubesApp::preDraw()" << endl << vjDEBUG_FLUSH;

       for(int i=0;i<mUserData.size();i++)
          mUserData[i]->updateNavigation();       // Update the navigation matrix
   }

   /// Function called after drawing has been triggered but BEFORE it completes
   virtual void postDraw()
   {
       vjDEBUG(2) << "cubesApp::postDraw()" << endl << vjDEBUG_FLUSH;
   }

   /// Function called before updating trackers but after the frame is drawn
   virtual void postSync()
   {
      vjDEBUG(2) << "cubesApp::postSync" << endl << vjDEBUG_FLUSH;
   }


private:


   //----------------------------------------------
   //  Draw the scene.  A bunch of boxes of differing color and stuff
   //----------------------------------------------

   void myDraw(vjUser* user)
   {
      vjDEBUG(2) << "\n--- myDraw() ---\n" << vjDEBUG_FLUSH;

      static const float SCALE = 100;
      //static const float SCALE = 10;
      static const float INCR = 0.1;

      glClearColor(0.0, 0.0, 0.0, 0.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      glPushMatrix();
            // --- Push on Navigation matrix for the user --- //

         vjMatrix nav_matrix = mUserData[user->getId()]->mNavMatrix;
         glMultMatrixf(nav_matrix.getFloatPtr());


         //---- Main box loop -----///
         for (float x=0;x<1;x += INCR)
            for (float y=0;y<1; y += INCR)
               for (float z=0;z<1;z += INCR)
               {
                  glColor3f(x, y, z);     // Set the Color
                  glPushMatrix();
                     glTranslatef( (x-0.5)*SCALE, (y-0.5)*SCALE, (z-0.5)*SCALE);
                     glScalef(1.5f, 1.5f, 1.5f);
   		            drawCube();
                  glPopMatrix();
               }


        /*
            // --- Draw corner boxes --- //
         for(float x=0;x<=1.0;x++)
            for(float y=0.0;y<=1.0;y++)
               for(float z=0.0;z<=1.0;z++)
               {
                  //glColor3f(x,y,z);    // Set the color
                  glColor3f(1.0, 0.0,  0.0f);
		            glPushMatrix();
                  {
                     glTranslatef( (x-0.5)*SCALE, (y-0.0)*SCALE, (z-0.5)*SCALE);
                     glScalef(2.0f, 2.0f, 2.0f);
   		            drawCube();
                  }
                  glPopMatrix();
               }
        */

      glPopMatrix();


   }

   void initGLState()
   {
      GLfloat light0_ambient[] = { .2,  .2,  .2,  1.0};
      GLfloat light0_diffuse[] = { 1.0,  1.0,  1.0,  1.0};
      GLfloat light0_specular[] = { 1.0,  1.0,  1.0,  1.0};
      GLfloat light0_position[] = {2000.0, 1000.0, 100.0, 1.0};
      GLfloat light0_direction[] = {-100, -100.0, -100.0};

      GLfloat mat_ambient[] = { 0.7, 0.7,  0.7,  1.0};
      GLfloat mat_diffuse[] = { 1.0,  0.5,  0.8,  1.0};
      GLfloat mat_specular[] = { 1.0,  1.0,  1.0,  1.0};
      GLfloat mat_shininess[] = { 50.0};
      GLfloat mat_emission[] = { 1.0,  1.0,  1.0,  1.0};
      GLfloat no_mat[] = { 0.0,  0.0,  0.0,  1.0};

      glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
      glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
      glLightfv(GL_LIGHT0, GL_SPECULAR,  light0_specular);
      glLightfv(GL_LIGHT0, GL_POSITION,  light0_position);
      glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_direction);

      glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient );
      glMaterialfv( GL_FRONT,  GL_DIFFUSE, mat_diffuse );
      glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
      glMaterialfv( GL_FRONT,  GL_SHININESS, mat_shininess );
      glMaterialfv( GL_FRONT,  GL_EMISSION, no_mat);

      glEnable(GL_DEPTH_TEST);
      glEnable(GL_NORMALIZE);
      glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT0);
      glEnable(GL_COLOR_MATERIAL);
      glShadeModel(GL_SMOOTH);

      glMatrixMode(GL_MODELVIEW);
   }

   void drawCube()
   {
      glCallList(mDlData->cubeDLIndex);
      //drawbox(-0.5, 0.5, -0.5, 0.5, -0.5, 0.5, GL_QUADS);
   }



   void drawbox(GLdouble x0, GLdouble x1, GLdouble y0, GLdouble y1,
                GLdouble z0, GLdouble z1, GLenum type)
   {
      static GLdouble n[6][3] = {
         {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
         {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0}
      };
      static GLint faces[6][4] = {
         { 0, 1, 2, 3}, { 3, 2, 6, 7}, { 7, 6, 5, 4},
         { 4, 5, 1, 0}, { 5, 6, 2, 1}, { 7, 4, 0, 3}
      };
      GLdouble v[8][3], tmp;
      GLint i;

      if (x0 > x1)
      {
         tmp = x0; x0 = x1; x1 = tmp;
      }
      if (y0 > y1)
      {
         tmp = y0; y0 = y1; y1 = tmp;
      }
      if (z0 > z1)
      {
         tmp = z0; z0 = z1; z1 = tmp;
      }
      v[0][0] = v[1][0] = v[2][0] = v[3][0] = x0;
      v[4][0] = v[5][0] = v[6][0] = v[7][0] = x1;
      v[0][1] = v[1][1] = v[4][1] = v[5][1] = y0;
      v[2][1] = v[3][1] = v[6][1] = v[7][1] = y1;
      v[0][2] = v[3][2] = v[4][2] = v[7][2] = z0;
      v[1][2] = v[2][2] = v[5][2] = v[6][2] = z1;

      for (i = 0; i < 6; i++)
      {
         glBegin(type);
            glNormal3dv(&n[i][0]);
            glVertex3dv(&v[faces[i][0]][0]);
            glNormal3dv(&n[i][0]);
            glVertex3dv(&v[faces[i][1]][0]);
            glNormal3dv(&n[i][0]);
            glVertex3dv(&v[faces[i][2]][0]);
            glNormal3dv(&n[i][0]);
            glVertex3dv(&v[faces[i][3]][0]);
         glEnd();
      }
   }

public:
   vjGlContextData<ContextData>  mDlData;       // Data for display lists
   std::vector<UserData*>        mUserData;     // All the users in the program
};

#endif
