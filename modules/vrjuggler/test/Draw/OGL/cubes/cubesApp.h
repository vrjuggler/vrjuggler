#ifndef _CUBES_APP_
#define _CUBES_APP_

#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream.h>

#include <Kernel/vjGlApp.h>
#include <Math/vjMatrix.h>
#include <Math/vjVec3.h>
#include <Math/vjQuat.h>
#include <Kernel/vjDebug.h>

class UserData
{
public:
   UserData()
   {
      firstTime = true;
      cubeDLIndex = -1;
   }
   
public:
   bool  firstTime;
   int   cubeDLIndex;
};


/**
 * Demonstration OpenGL application class
 *
 * This application simply renders a field of cubes.
 */
class cubesApp : public vjGlUserApp<UserData>
{
public:
   cubesApp(vjKernel* kern) : vjGlUserApp<UserData>(kern)
   {
      navMatrix.makeIdent();
   }
   
   // Execute any initialization needed before the API is started
   virtual void init()
   {
      wandIndex = kernel->getInputManager()->GetProxyIndex("VJWand");
      wandEndIndex = kernel->getInputManager()->GetProxyIndex("VJWandEnd");

      if(wandIndex == -1)
         cerr << "Wand not found\n";

      if(wandEndIndex == -1)
         cerr << "Wand end not found\n";
   }

   // Execute any initialization needed <b>after</b> API is started
   //  but before the drawManager starts the drawing loops.
   virtual void apiInit()
   {
      ;
   }

   /** Function to draw the scene
    * PRE: OpenGL state has correct transformation and buffer selected
    * POST: The current scene has been drawn
    */
   virtual void draw()
   {
      
      if(contextData().firstTime == true)  // If not inited
      {
         contextData().firstTime = false;
         contextData().cubeDLIndex = glGenLists(1);
         
         cerr << "Creating DL:" << contextData().cubeDLIndex << endl;
      
         glNewList(contextData().cubeDLIndex, GL_COMPILE);
            drawbox(-0.5, 0.5, -0.5, 0.5, -0.5, 0.5, GL_QUADS);
         glEndList();
      }
      
      
      initGLState();    // This should really be in another function
      myDraw();
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
    * </pre>
    */
   
   /// Function called before updating trackers but after the frame is drawn
   virtual void postSync()
   {
      vjDEBUG(2) << "cubesApp::postSync" << endl;
   }
   
   /// Function called after tracker update but before start of drawing
   virtual void preDraw()
   {
       vjDEBUG(2) << "cubesApp::preDraw()" << endl;

       //updateNavigation();       // Update the navigation matrix
   }
   
   /// Function called after drawing has been triggered but BEFORE it completes
   virtual void postDraw()
   {
       vjDEBUG(2) << "cubesApp::postDraw()" << endl;
   }

private:
      // Update the navigation matrix
   void updateNavigation()
   {
      // Cur*Transform = New Location
      vjMatrix    transform, transformIdent;
      vjQuat      source_rot, goal_rot, slerp_rot;
      
      transformIdent.makeIdent();
      source_rot.makeQuat(transformIdent);

      vjMatrix* wand_matrix;
      wand_matrix = kernel->getInputManager()->GetPosData(wandIndex);
      
      cout << "===================================\n";
      cout << "Wand:\n" << *wand_matrix << endl;

      transform = *wand_matrix;
      goal_rot.makeQuat(transform);

      if(transformIdent != transform)
      {
         slerp_rot.slerp(0.0005, source_rot, goal_rot);
         transform.makeQuaternion(slerp_rot);
      }
      else
         transform.makeIdent();

      cout << "Transform:\n" << transform << endl;

      navMatrix.postMult(transform);

      cout << "Nav:\n" << navMatrix << endl << endl << flush;
   }

   //----------------------------------------------
   //  Draw the scene.  A bunch of boxes of differing color and stuff
   //----------------------------------------------
   
   void myDraw()
   {
      vjDEBUG(2) << "\n--- myDraw() ---\n" << flush;
      
      //static const float SCALE = 100;
      static const float SCALE = 10;
      static const float INCR = 0.25;
   
      glClearColor(0.0, 0.0, 0.0, 0.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
      glPushMatrix();
            // --- Push on Navigation matrix --- //
         glMultMatrixf(navMatrix.getFloatPtr());
         
         
      /*
         //---- Main box loop -----///
         for (float x=0;x<1;x += INCR)
            for (float y=0;y<1; y += INCR)
               for (float z=0;z<1;z += INCR)
               {
                  glColor3f(x, y, z);     // Set the Color
                  glPushMatrix();
                     glTranslatef( (x-0.5)*SCALE, (y-0.5)*SCALE, (z-0.5)*SCALE);
                     glScalef(5.0f, 5.0f, 5.0f);
   		            drawCube();
                  glPopMatrix();
               }
       */  

       
            // --- Draw corner boxes --- //
         for(float x=0;x<=1;x++)
            for(float y=0;y<=1;y++)
               for(float z=0;z<=1;z++)
               {
                  glColor3f(x,y,z);    // Set the color
                  glPushMatrix();
                  {
                     glTranslatef( (x-0.5)*SCALE, (y-0.0)*SCALE, (z-0.5)*SCALE);
                     glScalef(2.0f, 2.0f, 2.0f);
   		            drawCube();
                  }
                  glPopMatrix();
               }
       
      glPopMatrix();
   
         // -- Draw box on wand --- //
      
      /*
      vjMatrix* wandMatrix;
      vjMatrix* wandEndMatrix;
      wandMatrix = kernel->getInputManager()->GetPosData(wandIndex);
      wandEndMatrix = kernel->getInputManager()->GetPosData(wandEndIndex);

      glPushMatrix();         
         // cout << "wand:\n" << *wandMatrix << endl;
         glMultMatrixf(wandMatrix->getFloatPtr());
         glColor3f(1.0f, 0.0f, 1.0f);
         glScalef(1.0f, 1.0f, 1.0f);
         drawCube();

         glBegin(GL_LINES);
            glColor3f(0.0,1.0,0.0);
            glVertex3f(0.0,0.0,0.0);
            glVertex3f(8.0,0.0,0.0);
         glEnd();
      glPopMatrix();

      glPushMatrix();
         // cout << "End:\n" << *wandEndMatrix << endl;
         glMultMatrixf(wandEndMatrix->getFloatPtr());
         glColor3f(0.0f, 1.0f, 1.0f);
         glScalef(0.5f, 0.5f, 0.5f);
         drawCube();

      glPopMatrix();
      */
      
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
      
      /*
      if(GL_TRUE == glIsList(contextData().cubeDLIndex))
         cerr << "Valid display List" << endl;
      else
         cerr << "In-Valid display list" << endl;
      */

      glCallList(contextData().cubeDLIndex);
      
      drawbox(-0.5, 0.5, -0.5, 0.5, -0.5, 0.5, GL_QUADS);
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
   int   wandIndex;     // the index of the wand
   int   wandEndIndex;  // The index to the end of the wand

   vjMatrix    navMatrix;     // Matrix for navigation in the application

};
 
#endif
