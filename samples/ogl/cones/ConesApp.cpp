/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <iostream>
#include <stdlib.h>
#include <math.h>

#include <Math/vjMatrix.h>
#include <Kernel/vjDebug.h>
#include <Kernel/vjAssert.h>

#include <ConesApp.h>


// ============================================================================
// Public methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Constructor.  This creates the GLU quadric used to render the cones.
// ----------------------------------------------------------------------------
ConesApp::ConesApp () {
    mConeQuad = gluNewQuadric();
    mBaseQuad = gluNewQuadric();
}

// ----------------------------------------------------------------------------
// Destructor.  This deletes the GLU quadric allocated in the constructor.
// ----------------------------------------------------------------------------
ConesApp::~ConesApp () {
    gluDeleteQuadric(mConeQuad);
    gluDeleteQuadric(mBaseQuad);
}

// ----------------------------------------------------------------------------
// Execute any initialization needed before the API is started.  Put device
// initialization here.
// ----------------------------------------------------------------------------
void
ConesApp::init () {
    vjDEBUG(vjDBG_ALL, 0) << "---------- Cones:App:init() ---------------"
                          << std::endl << vjDEBUG_FLUSH;

    std::vector<vjUser*> users = kernel->getUsers();
    int num_users = users.size();

    // Make sure that we actually have users defined.
    vjASSERT(num_users > 0);

    UserData* new_user=NULL;
    mUserData = std::vector<UserData*>(num_users);

    switch (num_users) {
      case (2):
        new_user = new UserData(users[1],"VJWand1", "VJButton0_1",
                                "VJButton1_1", "VJButton2_1");
        mUserData[1] = new_user;
        vjASSERT(users[1]->getId() == 1);
      case (1):
        new_user = new UserData(users[0],"VJWand", "VJButton0", "VJButton1",
                                "VJButton2");
        mUserData[0] = new_user;
        vjASSERT(users[0]->getId() == 0);
        break;
      default:
        vjDEBUG(vjDBG_ALL, 0) << clrOutNORM(clrRED, "ERROR:") << " Bad number of users."
                              << vjDEBUG_FLUSH;
        exit();
      break;
    }
}

// ----------------------------------------------------------------------------
// Execute any initialization needed after API is started but before the Draw
// Manager starts the drawing loops.
// ----------------------------------------------------------------------------
void
ConesApp::apiInit () {
    vjDEBUG(vjDBG_ALL, 0) << "---- ConesApp::apiInit() ----\n"
                          << vjDEBUG_FLUSH;
}

// ----------------------------------------------------------------------------
// Called immediately upon opening a new OpenGL context.  This is called for
// every display window that is opened.  Put OpenGL resource allocation here.
// ----------------------------------------------------------------------------
void
ConesApp::contextInit () {
    // Generate some random lists.  NOTE: Needed for testing only.
    mDlDebugData->maxIndex = rand() % 50;
    mDlDebugData->dlIndex  = glGenLists(mDlDebugData->maxIndex);

    // Create display list.
    mDlData->dlIndex = glGenLists(1);
    glNewList(mDlData->dlIndex, GL_COMPILE);
        drawCone(1.5, 2.0, 20, 10);
    glEndList();

    vjDEBUG(vjDBG_ALL, 0) << "Creating DL:" << mDlData->dlIndex
                          << std::endl << vjDEBUG_FLUSH;
    std::cerr << "created displays lists:" << mDlDebugData->dlIndex + 1
              << std::endl;

    initGLState();
}

// ----------------------------------------------------------------------------
// Called immediately upon closing an OpenGL context.  This is called for
// every display window that is closed.  Put OpenGL deallocation here.
// ----------------------------------------------------------------------------
void
ConesApp::contextClose() {
    // Deallocate the random display lists used for debugging.
    if ( glIsList(mDlDebugData->dlIndex) == GL_TRUE ) {
        vjDEBUG(vjDBG_ALL, 0) << "Deallocating " << mDlDebugData->maxIndex
                              << " debugging display lists\n" << vjDEBUG_FLUSH;
        glDeleteLists(mDlDebugData->dlIndex, mDlDebugData->maxIndex);
    }

    // Deallocate the cube face geometry data from the video hardware.
    if ( glIsList(mDlData->dlIndex) == GL_TRUE ) {
        vjDEBUG(vjDBG_ALL, 0) << "Deallocating cone display list\n"
                              << vjDEBUG_FLUSH;
        glDeleteLists(mDlData->dlIndex, 1);
    }
}

// ----------------------------------------------------------------------------
// Function called after tracker update but before start of drawing.
// Do calculations here.
// ----------------------------------------------------------------------------
void
ConesApp::preFrame () {
   vjDEBUG(vjDBG_ALL, 2) << "ConesApp::preFrame()" << std::endl
                         << vjDEBUG_FLUSH;

    for ( unsigned int i = 0 ; i < mUserData.size() ; i++ ) {
        // Update the navigation matrix
        mUserData[i]->updateNavigation();
    }
}

// ----------------------------------------------------------------------------
// Function to render the scene.  Put OpenGL draw calls here.
//
// PRE: OpenGL state has correct transformation and buffer selected.
// POST: The current scene has been drawn.
// ----------------------------------------------------------------------------
void
ConesApp::draw () {
    initGLState();    // This should really be in another function

    myDraw(vjGlDrawManager::instance()->currentUserData()->getUser());
}

// ----------------------------------------------------------------------------
// Function called after drawing has been triggered but BEFORE it completes.
// ----------------------------------------------------------------------------
void
ConesApp::intraFrame () {
    vjDEBUG(vjDBG_ALL, 2) << "ConesApp::intraFrame()" << std::endl
                          << vjDEBUG_FLUSH;
}

// ----------------------------------------------------------------------------
// Function called before updating trackers but after the frame is drawn.  Do
// calculations here.
// ----------------------------------------------------------------------------
void
ConesApp::postFrame () {
    vjDEBUG(vjDBG_ALL, 2) << "ConesApp::postFrame" << std::endl
                          << vjDEBUG_FLUSH;
}

// ----------------------------------------------------------------------------
// Make sure that all our dependencies are satisfied and make sure that there.
// are vjUsers registered with the system.
// ----------------------------------------------------------------------------
bool
ConesApp::depSatisfied () {
    // We can't start until there are users registered wth the system.
    // We rely upon users to keep track of the multi-user data structure.
    int num_users = vjKernel::instance()->getUsers().size();
    return (num_users > 0);
}

// ============================================================================
// Private methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Draw the scene.  A bunch of cones of differing color and stuff.
// ----------------------------------------------------------------------------
void
ConesApp::myDraw (vjUser* user) {
    vjDEBUG(vjDBG_ALL, 2) << "\n--- myDraw() ---\n" << vjDEBUG_FLUSH;

    static const float SCALE = 100;
//    static const float SCALE = 10;
    static const float INCR = 0.1;

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
        // Push on Navigation matrix for the user.
        vjMatrix nav_matrix = mUserData[user->getId()]->mNavMatrix;
        glMultMatrixf(nav_matrix.getFloatPtr());

        // Main cone loop.
        for ( float x = 0; x < 1; x += INCR ) {
            for ( float y = 0; y < 1; y += INCR ) {
                for ( float  z= 0; z < 1; z += INCR ) {
                    glColor3f(x, y, z);     // Set the Color
                    glPushMatrix();
                        glTranslatef((x - 0.5) * SCALE, (y - 0.5) * SCALE,
                                     (z - 0.5) * SCALE);
                        drawCone();
                    glPopMatrix();
                }
            }
        }
   glPopMatrix();
}

void
ConesApp::initGLState () {
    GLfloat light0_ambient[]   = { 0.2, 0.2, 0.2, 1.0};
    GLfloat light0_diffuse[]   = { 1.0, 1.0, 1.0, 1.0};
    GLfloat light0_specular[]  = { 1.0, 1.0, 1.0, 1.0};
    GLfloat light0_position[]  = { 2000.0, 1000.0, 100.0, 1.0 };
    GLfloat light0_direction[] = { -100, -100.0, -100.0 };

    GLfloat mat_ambient[]   = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat mat_diffuse[]   = { 1.0, 0.5, 0.8, 1.0 };
    GLfloat mat_specular[]  = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
//    GLfloat mat_emission[]  = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat no_mat[]        = { 0.0, 0.0, 0.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_direction);

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient );
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse );
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess );
    glMaterialfv( GL_FRONT, GL_EMISSION, no_mat);

    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);

    glMatrixMode(GL_MODELVIEW);
}
