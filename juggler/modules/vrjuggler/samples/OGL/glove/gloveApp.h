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
// Macros:
protected:
    //: MAX macro
    //  returns - the maximum of a, b, and c.
    template<class T> 
    inline const T& nMax(const T& a, const T& b, const T& c)
	     { return (a>=b) ? ((a>=c)?a:c) : ((b>=c)?b:c); }
    
    //: MIN macro
    //  returns - the minimum of a, b, and c.
    template<class T> 
    inline const T& nMin(const T& a, const T& b, const T& c)
	     { return (a<=b) ? ((a<=c)?a:c) : ((b<=c)?b:c); }

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
    
public:
   vjGloveInterface    mGlove;      // the glove
   vjGestureInterface  mGesture;    // the gesture
   bool cubeSelected;
   bool sphereSelected;
   bool coneSelected;
   
   float cubePos[3];
   float conePos[3];
   float spherePos[3];
   
   vjGlContextData<Scene> scene;
};

inline void gloveApp::contextInit()
{
    // Init the scene's displaylists for this context.
    scene->init();
}

//: Constructor
inline gloveApp::gloveApp(vjKernel* kern) : vjGlApp(kern), 
                                cubeSelected(false), 
                                sphereSelected(false), 
                                coneSelected(false)
{
   cubePos[0] = 0.0f;cubePos[1] = 3.5f;cubePos[2] = -20.0f;
   conePos[0] = -2.5f;conePos[1] = 3.5f;conePos[2] = -20.0f;
   spherePos[0] = 2.5f;spherePos[1] = 3.5f;spherePos[2] = -20.0f;
}

//: Initialize
// Executes any initialization needed before the API is started
inline void gloveApp::init()
{
    // Initialize devices
    mGlove.init("VJGlove");
    mGesture.init("VJGesture");
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
    glMatrixMode(GL_MODELVIEW);
    
    this->initGlState();
    this->renderLightsAndMaterials();
    
    // draw the floor
    glPushMatrix();
	glScalef( 3.0f, 1.0f, 3.0f );
	scene->drawFloor();
    glPopMatrix();
    
    // draw cube.
    glPushMatrix();
	glColor3f( 0.4f, 0.1f, 0.2f );
	glTranslatef( cubePos[0], cubePos[1], cubePos[2] );
	glEnable(GL_TEXTURE_2D);
	scene->renderRainbowTexture();
	scene->drawCube( 1.0f, 1.0f, 1.0f, cubeSelected );
	glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    // draw cone.
    glPushMatrix();
	glColor3f( 0.6f, 0.2f, 0.6f );
	glTranslatef( conePos[0], conePos[1], conePos[2] );
	scene->drawCone( 1.0f, 1.0f, 1.0f, coneSelected );
    glPopMatrix();
    
    // draw Sphere.
    glPushMatrix();
	glColor3f( 0.8f, 0.8f, 0.2f );
	glTranslatef( spherePos[0], spherePos[1], spherePos[2] );
	scene->drawSphere( 1.0f, 1.0f, 1.0f, sphereSelected );
    glPopMatrix();
    
    // draw table.
    glPushMatrix();
	glTranslatef( 0.0f, 0.0f, -20.0f );
	glEnable(GL_TEXTURE_2D);
	scene->renderWoodTexture();
	scene->drawTable();
	glDisable(GL_TEXTURE_2D);
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

//: Function called after tracker update but before start of drawing
//  In the glove application, this function does the logic for picking the objects.
inline void gloveApp::preDraw()
{
    float x, y, z;
    vjMatrix finger_matrix;
    
    //: Get the position of the index finger:
    finger_matrix = mGlove->getPos(vjGloveData::INDEX);
    finger_matrix.getTrans( x, y, z );
    
    //: pick up the object if you're pointing.
    //  set the object position equal to the glove position.
    if ( mGesture->getGesture() == mGesture->getGestureIndex("Pointing"))
    {
	if (coneSelected)
	{ 
	    conePos[0] = x; 
	    conePos[1] = y; 
	    conePos[2] = z; 
	} else if (sphereSelected)
	{ 
	    spherePos[0] = x; 
	    spherePos[1] = y; 
	    spherePos[2] = z; 
	} else if (cubeSelected)
	{ 
	    cubePos[0] = x; 
	    cubePos[1] = y; 
	    cubePos[2] = z; 
	}
    }
    
    float cubeDistance   = sqrtf( pow(x - cubePos[0], 2) + pow(y - cubePos[1], 2) + pow(z - cubePos[2], 2) );
    float sphereDistance = sqrtf( pow(x - spherePos[0], 2) + pow(y - spherePos[1], 2) + pow(z - spherePos[2], 2) );
    float coneDistance   = sqrtf( pow(x - conePos[0], 2) + pow(y - conePos[1], 2) + pow(z - conePos[2], 2) );
    float min = this->nMin( cubeDistance, sphereDistance, coneDistance);
    
     //: If the distance between hand and object is too far
     //  don't highlight any of them.
     if (min > 1.0f)
     {
	 cubeSelected = false; 
	 sphereSelected = false; 
	 coneSelected = false; 
     }
    
    // ...otherwise, 
    //   If glove is not pointing, or 
    //   we don't already have a selected one, then...
    else if ( mGesture->getGesture() != mGesture->getGestureIndex("Pointing") ||
	      (cubeSelected   == false &&
	       sphereSelected == false &&
	       coneSelected   == false)   )
    {
	// ... highlight the closest one to the glove.
    	if (min == coneDistance)
	{ 
	    cubeSelected = false; 
	    sphereSelected = false; 
	    coneSelected = true; 
	} else if (min == sphereDistance)
	{ 
	    cubeSelected = false; 
	    sphereSelected = true; 
	    coneSelected = false; 
	} else if (min == cubeDistance)
	{
	    cubeSelected = true; 
	    sphereSelected = false; 
	    coneSelected = false; 
	} 
    }
}

/// Function called after drawing has been triggered but BEFORE it completes
inline void gloveApp::postDraw()
{
}




#endif
