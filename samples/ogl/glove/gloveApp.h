#ifndef _GLOVE_APP_
#define _GLOVE_APP_

#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream.h>
#include <iomanip.h>

#include <Kernel/GL/vjGlApp.h>
#include <Math/vjMatrix.h>
#include <Math/vjVec3.h>
#include <math.h>

#include <Kernel/GL/vjGlContextData.h>
#include <Input/InputManager/vjGloveInterface.h>
#include <Input/InputManager/vjGestureInterface.h>
#include <Input/InputManager/vjPosInterface.h>
#include <Input/InputManager/vjAnalogInterface.h>
#include <Input/InputManager/vjDigitalInterface.h>

#include "defines.h"
#include "Scene.h"

//: GloveApp - A Demonstration OpenGL application class
// This application has a table with objects to grab
//
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
class gloveApp : public vjGlApp
{
// utility functions
protected:
    friend vjVec3 operator*( const vjMatrix& m, const vjVec3& v );

// Application Functions:
public:
   //: Constructor
   gloveApp(vjKernel* kern);

   //: Initialize
   // Executes any initialization needed before the API is started
   virtual void init();

   //: API Init
   //  Executes any initialization needed after API is started
   //  but before the drawManager starts the drawing loops.
   virtual void apiInit();

   //: Function to draw the scene
   //! PRE: OpenGL state has correct transformation and buffer selected
   //! POST: The current scene has been drawn
   virtual void draw();

   /// Function called before updating trackers but after the frame is drawn
   virtual void postSync();

   //: Function called after tracker update but before start of drawing
   //  In the glove application, this function does the logic for picking the objects.
   virtual void preDraw();

   /// Function called after drawing has been triggered but BEFORE it completes
   virtual void postDraw();

    virtual void contextInit();
private:
    void initGlState();
    void renderLightsAndMaterials();
    void myDraw();

protected:
   //: VR Juggler devices
   vjGloveInterface    mGlove;
   vjGestureInterface  mGesture;
   vjPosInterface      mGloveTracker;
   
   
   //: Object selection
   bool                mCubeSelected;
   bool                mSphereSelected;
   bool                mConeSelected;

   //: Object positions
   vjVec3               mCubePos;
   vjVec3               mConePos;
   vjVec3               mSpherePos;

   vjMatrix		mNavigation;

   vjGlContextData<Scene> mScene;
};

inline vjVec3 operator*( const vjMatrix& m, const vjVec3& v )
{
    float a[4];
    a[0] = m(0,0) * v[0] + m(0,1) * v[1] + m(0,2)  * v[2] + m(0,3) * 1.0f;
    a[1] = m(1,0) * v[0] + m(1,1) * v[1] + m(1,2)  * v[2] + m(1,3) * 1.0f;
    a[2] = m(2,0) * v[0] + m(2,1) * v[1] + m(2,2) * v[2] + m(2,3) * 1.0f;
    a[3] = m(3,0) * v[0] + m(3,1) * v[1] + m(3,2) * v[2] + m(3,3) * 1.0f;

    float invA3 = 1.0f / a[3];

    return vjVec3( a[0] * invA3, a[1] * invA3, a[2] * invA3 );
}

inline void gloveApp::contextInit()
{
    // Init the scene's displaylists for this context.
    mScene->init();
}

//: Constructor
inline gloveApp::gloveApp(vjKernel* kern) : vjGlApp(kern),
                                mCubeSelected(false),
                                mSphereSelected(false),
                                mConeSelected(false),
				mCubePos(0.0f, 3.5f, -20.0f),
				mConePos(-2.5f, 3.5f, -20.0f),
				mSpherePos(2.5f, 3.5f, -20.0f)
{
}

//: Initialize
// Executes any initialization needed before the API is started
inline void gloveApp::init()
{
    // Initialize devices
    mGlove.init("VJGlove");
    mGesture.init("VJGesture");
    mGloveTracker.init("GlovePos Proxy");
}

//: API Init
//  Executes any initialization needed after API is started
//  but before the drawManager starts the drawing loops.
inline void gloveApp::apiInit()
{
}

inline void gloveApp::renderLightsAndMaterials()
{
    GLfloat light0_ambient[] = { 0.1f,  0.1f,  0.1f,  1.0f};
    GLfloat light0_diffuse[] = { 0.8f,  0.8f,  0.8f,  1.0f};
    GLfloat light0_specular[] = { 1.0f,  1.0f,  1.0f,  1.0f};
    GLfloat light0_position[] = {0.0f, 0.75f, 0.75f, 0.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR,  light0_specular);
    glLightfv(GL_LIGHT0, GL_POSITION,  light0_position);

    GLfloat mat_ambient[] = { 0.0f, 0.0f,  0.0f,  1.0f };
    GLfloat mat_diffuse[] = { 0.7f,  0.7f,  0.7f,  1.0f };
    GLfloat mat_specular[] = { 1.0f,  1.0f,  1.0f,  1.0f };
    GLfloat mat_shininess[] = { 30.0f };
    GLfloat no_emm[] = { 0.0f,  0.0f,  0.0f,  1.0f };

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient );
    glMaterialfv( GL_FRONT,  GL_DIFFUSE, mat_diffuse );
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
    glMaterialfv( GL_FRONT,  GL_SHININESS, mat_shininess );
    glMaterialfv( GL_FRONT,  GL_EMISSION, no_emm);
    glEnable(GL_COLOR_MATERIAL);
}

inline void gloveApp::initGlState()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_CULL_FACE);
}

inline void gloveApp::myDraw()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    	
    this->initGlState();
    this->renderLightsAndMaterials();

    glPushMatrix();
	glMatrixMode(GL_MODELVIEW);	
	glMultMatrixf( mNavigation.getFloatPtr() );
	
	// draw the floor
	glPushMatrix();
	    glScalef( 3.0f, 1.0f, 3.0f );
	    mScene->drawFloor();
	glPopMatrix();
	
	// draw cube.
	glPushMatrix();
	    glColor3f( 0.4f, 0.1f, 0.2f );
	    glTranslatef( mCubePos[0], mCubePos[1], mCubePos[2] );
	    glEnable(GL_TEXTURE_2D);
	    mScene->renderRainbowTexture();
	    mScene->drawCube( 1.0f, 1.0f, 1.0f, mCubeSelected );
	    glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
	// draw cone.
	glPushMatrix();
	    glColor3f( 0.6f, 0.2f, 0.6f );
	    glTranslatef( mConePos[0], mConePos[1], mConePos[2] );
	    mScene->drawCone( 1.0f, 1.0f, 1.0f, mConeSelected );
	glPopMatrix();
	
	// draw Sphere.
	glPushMatrix();
	    glColor3f( 0.8f, 0.8f, 0.2f );
	    glTranslatef( mSpherePos[0], mSpherePos[1], mSpherePos[2] );
	    mScene->drawSphere( 1.0f, 1.0f, 1.0f, mSphereSelected );
	glPopMatrix();
	
	// draw table.
	glPushMatrix();
	    glTranslatef( 0.0f, 0.0f, -20.0f );
	    glEnable(GL_TEXTURE_2D);
	    mScene->renderWoodTexture();
	    mScene->drawTable();
	    glDisable(GL_TEXTURE_2D);
	glPopMatrix();
    glPopMatrix();
}

//: Function to draw the scene
//! PRE: OpenGL state has correct transformation and buffer selected
//! POST: The current scene has been drawn
inline void gloveApp::draw()
{
   myDraw();
}

/// Function called before updating trackers but after the frame is drawn
inline void gloveApp::postSync()
{
}

#include "navigation.h"

//: we need to keep track of the wand, and the user.
UserInfo    userInfo;
TrackedInfo wandInfo;
TrackedInfo headInfo;

//: Function called after tracker update but before start of drawing
//  In the glove application, this function does the logic for picking the objects.
inline void gloveApp::preDraw()
{
    vjVec3 glovePos;
    vjMatrix finger_matrix;
    vjMatrix invNav;
    invNav.invert(mNavigation);

    //: Get the position of the index finger:
    finger_matrix = mGlove->getPos(vjGloveData::INDEX);
    finger_matrix.getTrans( glovePos[0], glovePos[1], glovePos[2] );
    glovePos = invNav * glovePos;

    //vjDEBUG(7) << "Gesture: " << mGesture->getGestureString(mGesture->getGesture())<<"\n"<<flush;
    //vjDEBUG(7) << glovePos[0]<<" "<<glovePos[1]<<" "<<glovePos[2]<<" : "<<mCubePos[0]<<" "<<mCubePos[1]<<" "<<mCubePos[2]<<"\n"<<flush;

    ////////////////////////
    // NAVIGATION         //
    ////////////////////////////////////////////////////////
    static float userVelocity = 0;
    
    if (mGesture->getGesture() == mGesture->getGestureIndex("Pointing"))
    {
	userVelocity += 0.0001f;
    } else
    if (mGesture->getGesture() == mGesture->getGestureIndex("Closed Fist"))
    {
	userVelocity = 0.0f;
    }
    userInfo.setVelocity( userVelocity );
    userInfo.setAngularVelocity( 0.01f );
    wandInfo.updateWithMatrix( *mGloveTracker->GetData() );
    userInfo.update( wandInfo, vjVec3(0.0f, 0.0f, 0.0f) );
    userInfo.getSceneTransform( mNavigation );
    ////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////
    //: pick up the object if you're pointing.
    //  set the object position equal to the glove position.
    if ( mGesture->getGesture() == mGesture->getGestureIndex("Open Hand"))
    {
	if (mConeSelected)
	    mConePos = glovePos;
	else if (mSphereSelected)
	    mSpherePos = glovePos;
	else if (mCubeSelected)
	    mCubePos = glovePos;
    }

    float cubeDistance   = (glovePos - mCubePos).length();
    float sphereDistance = (glovePos - mSpherePos).length();
    float coneDistance   = (glovePos - mConePos).length();
    float min = nMin( cubeDistance, sphereDistance, coneDistance);

     //: If the distance between hand and object is too far
     //  don't highlight any of them.
     if (min > 1.0f)
     {
	 mCubeSelected = false;
	 mSphereSelected = false;
	 mConeSelected = false;
     }

    // ...otherwise,
    //   If glove is not pointing, or
    //   we don't already have a selected one, then...
    else if ( mGesture->getGesture() != mGesture->getGestureIndex("Open Hand") ||
	      (mCubeSelected   == false &&
	       mSphereSelected == false &&
	       mConeSelected   == false)   )
    {
	// ... highlight the closest one to the glove.
    	if (min == coneDistance)
	{
	    mCubeSelected = false;
	    mSphereSelected = false;
	    mConeSelected = true;
	} else if (min == sphereDistance)
	{
	    mCubeSelected = false;
	    mSphereSelected = true;
	    mConeSelected = false;
	} else if (min == cubeDistance)
	{
	    mCubeSelected = true;
	    mSphereSelected = false;
	    mConeSelected = false;
	}
    }
}

/// Function called after drawing has been triggered but BEFORE it completes
inline void gloveApp::postDraw()
{
}




#endif
