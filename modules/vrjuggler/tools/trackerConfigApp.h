/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _TRK_CONFIG_APP_H_
#define _TRK_CONFIG_APP_H_

#include <sgGlutApp.h>
#include <sgGlutWin.h>
#include <sgDrawFunctor.h>

//#include <sgTbXformer.h>

class trackerConfigApp : public sgGlutApp, public sgDrawFunctor
{
protected:
	trackerConfigApp()
	{
		init();
	}

public:
	
	void init()
	{
			// -- Setup Trackball -- //
		//trackBall.setRootNode(rootNode);
		//trackBall.setDCS(rootDCS);		

      // -- Initialize state //
      mDrawWireFrame = false;
      mTransmitterPos.makeIdent();
	}
	
	
	// function to do setup needed after windows have been allocated.
	// PRE: win array has values in it.
	void initWindows()
	{
		winHandles[0] = createNewGlutWin(50, 50, 500, 500, "Tracker Configuration");
		
		int winHndl = winHandles[0];
		win[winHndl]->getChan()->setDrawFunc(this);        // We are the draw function
      win[winHndl]->chan.setFov(60.0f);
		win[winHndl]->chan.setNearFar(0.1, 1000.0f);
      win[winHndl]->chan.setView(vrj::Vec3(0.0f, -5.0f, -20.0f), vrj::Vec3(0.0f, -45.0f, 0.0f));
		//**//win[winHndl]->chan.setView(sgVec3(0.0, 0.0, 20.0), sgVec3(0.0, 0.0, 0.0));
		
			// Set the trackballs input
		//trackBall.setMouseInput(&(win[winHndl]->input));
	}

	virtual void appFunc()
	{
		// Do my updates
	}

   // Define what to do when drawing
   //
   // Draw function for sgDrawFunctor
   virtual void draw(sgChan* chan)
   {
      // Set base model matrix
      glPushMatrix();
         vrj::Matrix model_dcs; model_dcs = getModelMatrix();
         glMultMatrixf(model_dcs.getFloatPtr());

         // Draw a base system XXX: In future this should come from a config file or something
         glPushMatrix();
            drawBaseSystem();
         glPopMatrix();

         // ----- TRANSMITTER ------ //
         const vrj::Vec3 TmtCubeColor(0.0,1.0,1.0f);
         const float TmtCubeSize(1.0f);
         glPushMatrix();
            glMultMatrixf(mTransmitterPos.getFloatPtr());
            glColor3fv(TmtCubeColor.vec);
            drawWireCube(TmtCubeSize);
            drawAxis(2.0f,0.8f,2.5f);
         glPopMatrix();

         // ----- TRACKER ------ //
         const vrj::Vec3 TrkCubeColor(1.0,1.0,0.0f);
         const float TrkCubeSize(0.20f);
         glPushMatrix();
            glMultMatrixf(mTrackerPos.getFloatPtr());
            glColor3fv(TrkCubeColor.vec);
            drawWireCube(TrkCubeSize);
            drawAxis(2.0f,0.6f,1.5f);

             // ----- PROXY ------ //
            const vrj::Vec3 PxyCubeColor(1.0,0.0,1.0f);
            const float PxyCubeSize(0.20f);
            glPushMatrix();
               glMultMatrixf(mProxyPos.getFloatPtr());
               glColor3fv(PxyCubeColor.vec);
               drawWireCube(PxyCubeSize);
               drawAxis(4.0f,0.95f,0.75f);
            glPopMatrix();

         glPopMatrix();

      glPopMatrix();    // Model matrix
   }

   void drawBaseSystem()
   {
      const float CubeSize(10.0f);     // Size of the system to draw
      const vrj::Vec3 CubeColor(0.8f,0.8f,0.8f);

      drawAxis(1.0f, 0.5, 1.5f);
      glPushMatrix();
         glTranslatef(0.0, CubeSize/2.0f, 0.0f);
         glColor3fv(CubeColor.vec);
         drawWireCube(CubeSize);
      glPopMatrix();
   }

	virtual void preDraw()
	{
		//cout << "----------- PreDraw -----------" << endl << flush;

		//glEnable(GL_CULL_FACE);
		glDisable(GL_CULL_FACE);
		
		if(mDrawWireFrame)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnable(GL_DEPTH_TEST);

			// --- LIGHT --- //
		//sgVec4 specular(1.0, 1.0, 1.0, 1.0);
		//float shininess = 15.0;
		//sgVec4 lightPos(0.0, 1.0, 1.0, 0.0);

		//glMaterialfv(GL_FRONT, GL_SPECULAR, specular.vec);
		//glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
		//glLightfv(GL_LIGHT0, GL_POSITION, lightPos.vec);

		//glEnable(GL_LIGHTING);
		//glEnable(GL_LIGHT0);
	}

	virtual void postDraw()
	{
		//**//trackBall.update();
	}

public:	// State interface //
	void setWireframeMode(bool mode)
	{ mDrawWireFrame = mode; }

   vrj::Matrix getTransmitterPos()
   { return mTransmitterPos; }
   void setTransmitterPos(vrj::Matrix mat)
   { mTransmitterPos = mat; }

   // Used to get a to the correct model view
   vrj::Matrix getModelMatrix()
   { return mModelRotation; }
   void setModelMatrix(vrj::Matrix mat)
   { mModelRotation = mat; }

   vrj::Matrix getTrackerPos()
   { return mTrackerPos; }
   void setTrackerPos(vrj::Matrix mat)
   { mTrackerPos = mat; }

   vrj::Matrix getProxyPos()
   { return mProxyPos; }
   void setProxyPos(vrj::Matrix mat)
   { mProxyPos = mat; }


private: // State vars //
	bool        mDrawWireFrame;
   vrj::Matrix    mModelRotation;      // Updated by menu system
   vrj::Matrix    mTransmitterPos;     // Position of transmitter
   vrj::Matrix    mTrackerPos;         // Position of a tracker
   vrj::Matrix    mProxyPos;           // Tracker proxy relative to tracker

protected:     // --- Geom helpers --- //
   void initQuadObj();
   void drawLine(vrj::Vec3& start, vrj::Vec3& end);
   void drawSphere(float radius, int slices, int stacks);
   void drawCone(float base, float height, int slices, int stacks);
   void drawCube(float size, GLenum type);
   void drawWireCube(float size);
   void drawSolidCube(float size);
   // Draw a set of axis
   void drawAxis(float lineWidth=1.0f, float brightness=1.0f, float scale=1.0f);

   GLUquadricObj* mQuadObj;
	
public:
   //sgTbXformer	trackBall;		// Trackball xformer

	int   winHandles[10];

public:
	static trackerConfigApp*	getInstance()
	{
		if(_instance == NULL)
			_instance = new trackerConfigApp;		// Allocate an object

		return dynamic_cast<trackerConfigApp*>(_instance);
	}
};

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////



// Draw a set of axis
void trackerConfigApp::drawAxis(float lineWidth, float brightness, float scale)
{
   vrj::Vec3 x_axis(scale,0.0f,0.0f);
   vrj::Vec3 y_axis(0.0f, scale, 0.0f);
   vrj::Vec3 z_axis(0.0f, 0.0f, scale);
   vrj::Vec3 origin(0.0f, 0.0f, 0.0f);

   glPushAttrib(GL_LIGHTING_BIT | GL_ENABLE_BIT | GL_LINE_BIT);
   glDisable(GL_LIGHTING);
   glLineWidth(lineWidth);
   glPushMatrix();
      glBegin(GL_LINES);
         glColor3f(brightness, 0.0f, 0.0f);
         glVertex3fv(origin.vec);
         glVertex3fv(x_axis.vec);

         glColor3f(0.0f, brightness, 0.0f);
         glVertex3fv(origin.vec);
         glVertex3fv(y_axis.vec);

         glColor3f(0.0f, 0.0f, brightness);
         glVertex3fv(origin.vec);
         glVertex3fv(z_axis.vec);
      glEnd();
   glPopMatrix();

   glPopAttrib();
}

void trackerConfigApp::initQuadObj()
{
   if (mQuadObj == NULL)
      mQuadObj = gluNewQuadric();
}

void trackerConfigApp::drawLine(vrj::Vec3& start, vrj::Vec3& end)
{
   glBegin(GL_LINES);
      glVertex3fv(start.vec);
      glVertex3fv(end.vec);
   glEnd();
}

void trackerConfigApp::drawSphere(float radius, int slices, int stacks)
{
  initQuadObj();
  gluQuadricDrawStyle(mQuadObj, (GLenum) GLU_FILL);
  gluQuadricNormals(mQuadObj, (GLenum) GLU_SMOOTH);
  gluSphere(mQuadObj, radius, slices, stacks);
}


void trackerConfigApp::drawCone(float base, float height, int slices, int stacks)
{
  initQuadObj();
  gluQuadricDrawStyle(mQuadObj, (GLenum) GLU_FILL);
  gluQuadricNormals(mQuadObj, (GLenum) GLU_SMOOTH);
  gluCylinder(mQuadObj, base, 0.0, height, slices, stacks);
}



void trackerConfigApp::drawCube(float size, GLenum type)
{
  static GLfloat n[6][3] =
  {
    {-1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {1.0, 0.0, 0.0},
    {0.0, -1.0, 0.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, -1.0}
  };

  static GLint faces[6][4] =
  {
    {0, 1, 2, 3},
    {3, 2, 6, 7},
    {7, 6, 5, 4},
    {4, 5, 1, 0},
    {5, 6, 2, 1},
    {7, 4, 0, 3}
  };
  GLfloat v[8][3];
  GLint i;

  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

  for (i = 5; i >= 0; i--) {
    glBegin(type);
    glNormal3fv(&n[i][0]);
    glVertex3fv(&v[faces[i][0]][0]);
    glVertex3fv(&v[faces[i][1]][0]);
    glVertex3fv(&v[faces[i][2]][0]);
    glVertex3fv(&v[faces[i][3]][0]);
    glEnd();
  }
}

void trackerConfigApp::drawWireCube(float size)
{
  drawCube(size, GL_LINE_LOOP);
}

void trackerConfigApp::drawSolidCube(float size)
{
  drawCube(size, GL_QUADS);
}


#endif

