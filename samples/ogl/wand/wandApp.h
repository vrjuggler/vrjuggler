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
#include <Input/InputManager/vjAnalogInterface.h>
#include <Input/InputManager/vjDigitalInterface.h>

// Utility function to draw a cube
void drawbox(GLdouble x0, GLdouble x1, GLdouble y0, GLdouble y1,
                GLdouble z0, GLdouble z1, GLenum type);

//----------------------------------------------------
//: Demonstration OpenGL application class
//
// This application simply renders a box on the wand
//----------------------------------------------------
class wandApp : public vjGlApp
{
public:
   wandApp(vjKernel* kern)
      : vjGlApp(kern)            // Initialize base class
   {;}
   
   // Execute any initialization needed before the API is started
   virtual void init()
   {
      cout << "---------- App:init() ---------------" << endl;
         // Initialize devices
      mWand.init("VJWand");
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
   {
      cout << "cubesApp::postSync\n";
   }
   
   /// Function called after tracker update but before start of drawing
   virtual void preDraw()
   {
       cout << "cubesApp::preDraw()\n";
   }
   
   /// Function called after drawing has been triggered but BEFORE it completes
   virtual void postDraw()
   {
       cout << "cubesApp::postDraw()\n";
   }

private:
   //----------------------------------------------
   //  Draw the scene.  A box on the end of the wand
   //----------------------------------------------
   
   void myDraw()
   {
      cout << "\n--- myDraw() ---\n";
      
   
      glClearColor(0.0, 0.0, 0.0, 0.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glMatrixMode(GL_MODELVIEW);

         // -- Draw box on wand --- //
      vjMatrix* wandMatrix;
      wandMatrix = mWand->GetData();

      glPushMatrix();         
         // cout << "wand:\n" << *wandMatrix << endl;
         glMultMatrixf(wandMatrix->getFloatPtr());
         glColor3f(1.0f, 0.0f, 1.0f);
         glScalef(0.25f, 0.25f, 0.25f);
         drawCube();

            // A little laser pointer
         glLineWidth(2.0f);
         glBegin(GL_LINES);
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, -10.0f);
         glEnd();
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

   void drawCube()
   {
      drawbox(-0.5, 0.5, -0.5, 0.5, -0.5, 0.5, GL_QUADS);
   }
   
public:
   vjPosInterface    mWand;      // the Wand
};


//: Utility function for drawing a cube
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

#endif
