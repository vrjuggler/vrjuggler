#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <gloveApp.h>


/*
vjVec3 operator*( const vjMatrix& m, const vjVec3& v )
{
    float a[4];
    a[0] = m(0,0) * v[0] + m(0,1) * v[1] + m(0,2)  * v[2] + m(0,3) * 1.0f;
    a[1] = m(1,0) * v[0] + m(1,1) * v[1] + m(1,2)  * v[2] + m(1,3) * 1.0f;
    a[2] = m(2,0) * v[0] + m(2,1) * v[1] + m(2,2) * v[2] + m(2,3) * 1.0f;
    a[3] = m(3,0) * v[0] + m(3,1) * v[1] + m(3,2) * v[2] + m(3,3) * 1.0f;

    float invA3 = 1.0f / a[3];

    return vjVec3( a[0] * invA3, a[1] * invA3, a[2] * invA3 );
}
*/

void gloveApp::renderLightsAndMaterials()
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

void gloveApp::initGlState()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_CULL_FACE);
}

void gloveApp::myDraw()
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

//: Function called after tracker update but before start of drawing
//  In the glove application, this function does the logic for picking the
//  objects.
void gloveApp::preFrame()
{
    //: we need to keep track of the wand, and the user.
    UserInfo    userInfo;
    TrackedInfo wandInfo;
    TrackedInfo headInfo;

    vjVec3 glovePos;
    vjMatrix finger_matrix;
    vjMatrix invNav;
    invNav.invert(mNavigation);

    //: Get the position of the index finger:
    finger_matrix = mGlove->getPos(vjGloveData::INDEX);
    finger_matrix.getTrans( glovePos[0], glovePos[1], glovePos[2] );
    glovePos.xformVec( invNav, glovePos );

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
    wandInfo.updateWithMatrix( mGlove->getPos() );
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
