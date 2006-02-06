#ifndef _TEST_PATTERN_MODE_H_
#define _TEST_PATTERN_MODE_H_

#include <TestMode.h>

#ifdef VPR_OS_Darwin
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <gmtl/Math.h>
#include <gmtl/Matrix.h>
#include <gmtl/Generate.h>
#include <gmtl/Vec.h>

#include <gadget/Type/Position/PositionUnitConversion.h>

#include <vrj/Draw/OGL/GlDrawManager.h>
#include <vrj/Display/DisplayManager.h>
#include <vrj/Display/SurfaceViewport.h>
#include <vrj/Display/Display.h>

#include "WallTest.h"
#include "WallTestHelpers.h"

class TestPatternMode : public TestMode
{
public:
   virtual std::string getName()
   { return std::string("TestPatternMode"); }

   virtual void update(WallTest *wallTest)
   {
   }
   
   virtual void draw(WallTest *wallTest)
   {
      DisplayManager* displayManager=vrj::GlDrawManager::instance()->getDisplayManager();
     std::vector<Display*> disps=displayManager->getAllDisplays();    
   
     for(unsigned int i=0;i<disps.size();i++)
     {
        for (unsigned int v=0;v<disps[i]->getNumViewports();v++)
        {
           Viewport* viewport=disps[i]->getViewport(v);
   
           if (viewport->isSurface())
           {
              // Get a pointer to the surface
              SurfaceViewport* surface = dynamic_cast<SurfaceViewport*>(viewport);
              vprASSERT(surface!=NULL);
   
              drawWallPattern(surface);            
           }
        }
     }  

   }

   void drawWallPattern(SurfaceViewport* surf)
   {
      gmtl::Matrix44f rotate;
      gmtl::Vec3f center;
      float wall_width, wall_height;

      calcSurfaceParameters(surf, rotate, center, wall_width, wall_height);

      glPushMatrix();
      {
         // Translate to center of wall and rotate to looking down wall
         glTranslatef(center[0],center[1],center[2]);
         glMultMatrixf(rotate.getData());

         drawGrid(wall_width, wall_height);         
      }
      glPopMatrix();
   }

   void drawGrid(float wallWidth, float wallHeight)
   {
      // Scale to size of wall and then to 10 units wide and tall
      glPushMatrix();
      {
         glScalef(wallWidth, wallHeight, 1.0f);
         glScalef(0.1, 0.1, 0.1);
      
         glPushAttrib(GL_LIGHTING);
         glDisable(GL_LIGHTING);
      
         glLineWidth(2);
         
         glBegin(GL_LINES);
      
         float x;
         for(x=-5;x<=5;x+=0.5)
         {
            if(x==-2.5)      glColor3f(0,1,1);
            else if(x==-2)   glColor3f(1,1,0);
            else if(x==-1.5) glColor3f(1,0,1);
            else if(x==-1)   glColor3f(1,0,0);
            else if(x==-0.5) glColor3f(0,1,0);
            else if(x==0)    glColor3f(0,0,1);
            else if(x==0.5)  glColor3f(0,1,0);
            else if(x==1)    glColor3f(1,0,0);
            else if(x==1.5)  glColor3f(1,0,1);
            else if(x==2)    glColor3f(1,1,0);
            else if(x==2.5)  glColor3f(0,1,1);
            else             glColor3f(1,1,1);
      
            glVertex2f(x,-5);
            glVertex2f(x,5);
      
            glVertex2f(-5,x);
            glVertex2f(5,x);
         }
      
         glEnd();
      
         glColor3f(1,1,0);
         glBegin(GL_LINE_LOOP);
         for ( x = 0; x < gmtl::Math::TWO_PI; x += gmtl::Math::PI / 20.0 )
         {
            glVertex2f(sin(x)*5,cos(x)*5);
         }
         glEnd();
      
         glColor3f(0,1,1);
         glLineWidth(5);
         glBegin(GL_LINES);
            glVertex2f(-1,0);
            glVertex2f(1,0);
            glVertex2f(0,-1);
            glVertex2f(0,1);
         glEnd();
         glLineWidth(1);
      
         glPopAttrib();
      }
      glPopMatrix();
   }

};

#endif

