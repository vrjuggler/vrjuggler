#ifndef _WAND_APP_
#define _WAND_APP_

#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream.h>
#include <iomanip.h>

#include <Kernel/GL/vjGlApp.h>
#include <Math/vjMatrix.h>
#include <Math/vjVec3.h>

#include <Input/InputManager/vjPosInterface.h>
#include <Input/InputManager/vjGloveInterface.h>

//----------------------------------------------------
//: Demonstration OpenGL application class
//
// This application draws a glove and some rays and spheres
// on the glove
//----------------------------------------------------
class simpleGloveApp : public vjGlApp
{
public:
   simpleGloveApp(vjKernel* kern)
      : vjGlApp(kern)            // Initialize base class
   {
      mQuadObj = NULL;
   }

   // Execute any initialization needed before the API is started
   // Initialize interfaces
   virtual void init()
   {
      mGlove.init("VJGlove");
   }

   // Execute any initialization needed after API is started
   //  but before the drawManager starts the drawing loops.
   virtual void apiInit()
   {;}

   //: Function to draw the scene
   //! PRE: OpenGL state has correct transformation and buffer selected
   //! POST: The current scene has been drawn
   virtual void draw()
   {
      initGLState();    // This should really be in another function
      myDraw();
   }

   // ----- Drawing Loop Functions ------
   //
   //  The drawing loop will look similar to this:
   //
   //  while (drawing)
   //  {
   //        preDraw();
   //	      draw();
   //	       postDraw();      // Drawing is happening while here
   //       sync();
   //        postSync();      // Drawing is now done
   //
   //	      UpdateTrackers();
   //  }
   //------------------------------------

   /// Function called before updating trackers but after the frame is drawn
   virtual void postSync()
   {;}

   /// Function called after tracker update but before start of drawing
   virtual void preDraw()
   {;}

   /// Function called after drawing has been triggered but BEFORE it completes
   virtual void postDraw()
   {;}

private:
   //-----------------------------------------------------
   //  Draw the scene.  Rays and spheres on glove fingers
   //------------------------------------------------------

   void myDraw()
   {
	// Clear the scene
      glClearColor(0.0, 0.0, 0.0, 0.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glMatrixMode(GL_MODELVIEW);

      int finger;

         // -- Draw box on wand --- //
      vjMatrix finger_matrix;

      // Draw green balls on finger tips
      glColor3f(0.0f, 1.0f, 0.0f);
      glPushMatrix();
      {
         for(finger=vjGloveData::INDEX;finger<=vjGloveData::PINKY;finger++)
         {
         glPushMatrix();
            finger_matrix =
               mGlove->getPos((vjGloveData::vjGloveComponent)finger);
            glMultMatrixf(finger_matrix.getFloatPtr());
            drawSphere((0.1f*(1.0f/12.0f)), 4, 4);
         glPopMatrix();
         }
      }
      glPopMatrix();

      // Draw red rays out of fingers
      // I am not doing this in the most efficient way.
      // I am just trying to test the vector functions
      glColor3f(1.0f, 0.0f, 0.0f);
      glPushMatrix();
      {
         for(finger=vjGloveData::INDEX;finger<=vjGloveData::PINKY;finger++)
         {
         glPushMatrix();
            vjVec3   origin(0,0,0);    // Base of the vector
            finger_matrix =
               mGlove->getPos((vjGloveData::vjGloveComponent)finger);
            origin.xformFull(finger_matrix, origin);     // Go to new coord system
            drawLine(origin,
                     origin + (0.25 * mGlove->getVector((vjGloveData::vjGloveComponent)finger)));
         glPopMatrix();
         }
      }
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
   }

protected:
   void drawLine(vjVec3& start, vjVec3& end)
   {
      glBegin(GL_LINES);
         glVertex3fv(start.vec);
         glVertex3fv(end.vec);
      glEnd();
   }

   void drawSphere(float radius, int slices, int stacks)
   {
      if (mQuadObj == NULL)
         mQuadObj = gluNewQuadric();

      gluQuadricDrawStyle(mQuadObj, GLU_FILL);
      gluQuadricNormals(mQuadObj, GLU_SMOOTH);
      gluSphere(mQuadObj, radius, slices, stacks);
   }

   GLUquadricObj* mQuadObj;

public:
   vjGloveInterface    mGlove;      // the glove
};


#endif
