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

#ifndef _CONES_APP_H_
#define _CONES_APP_H_

#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>

#include <vrj/Draw/OGL/GlApp.h>
#include <vrj/Draw/OGL/GlContextData.h>
#include <vrj/Kernel/User.h>

#include <UserData.h>


// ----------------------------------------------------------------------------
// Class to hold all context-specific data.
// ----------------------------------------------------------------------------
class ContextData {
public:
    ContextData (void) {
        dlIndex  = -1;
        maxIndex = -1;
    }

    int   dlIndex;
    int   maxIndex;     // For debugging purposes only!
};

// ----------------------------------------------------------------------------
// Demonstration OpenGL application class.
// This application simply renders a field of cones.
// ----------------------------------------------------------------------------
class ConesApp : public vrj::GlApp {
public:
    // ------------------------------------------------------------------------
    // Constructor.  This creates the GLU quadric used to render the cones.
    // ------------------------------------------------------------------------
    ConesApp(void);

    // ------------------------------------------------------------------------
    // Destructor.  This deletes the GLU quadric allocated in the constructor.
    // ------------------------------------------------------------------------
    virtual ~ConesApp(void);

    // ------------------------------------------------------------------------
    // Execute any initialization needed before the API is started.  Put
    // device initialization here.
    // ------------------------------------------------------------------------
    virtual void init(void);

    // ------------------------------------------------------------------------
    // Execute any initialization needed after API is started but before the
    // Draw Manager starts the drawing loops.
    // ------------------------------------------------------------------------
    virtual void apiInit(void);

    // ------------------------------------------------------------------------
    // Called immediately upon opening a new OpenGL context.  This is called
    // for every display window that is opened.  Put OpenGL resource
    // allocation here.
    // ------------------------------------------------------------------------
    virtual void contextInit(void);

    // ------------------------------------------------------------------------
    // Called immediately upon closing an OpenGL context.  This is called for
    // every display window that is closed.  Put OpenGL deallocation here.
    // ------------------------------------------------------------------------
    virtual void contextClose(void);

    /**   name Drawing Loop Functions
     *
     *  The drawing loop will look similar to this:
     *
     *  while (drawing)
     *  {
     *        preFrame();
     *       draw();
     *        intraFrame();      // Drawing is happening while here
     *       sync();
     *        postFrame();      // Drawing is now done
     *
     *       UpdateTrackers();
     *  }
     *
     */

    // ------------------------------------------------------------------------
    // Function called after tracker update but before start of drawing.
    // Do calculations here.
    // ------------------------------------------------------------------------
    virtual void preFrame(void);

    // ------------------------------------------------------------------------
    // Function to render the scene.  Put OpenGL draw calls here.
    //
    // PRE: OpenGL state has correct transformation and buffer selected.
    // POST: The current scene has been drawn.
    // ------------------------------------------------------------------------
    virtual void draw(void);

    // ------------------------------------------------------------------------
    // Function called after drawing has been triggered but BEFORE it
    // completes.
    // ------------------------------------------------------------------------
    virtual void intraFrame(void);

    // ------------------------------------------------------------------------
    // Function called before updating trackers but after the frame is drawn.
    // Do calculations here.
    // ------------------------------------------------------------------------
    virtual void postFrame(void);

    // ------------------------------------------------------------------------
    // Make sure that all our dependencies are satisfied, and make sure that
    // there are Users registered with the system.
    // ------------------------------------------------------------------------
    virtual bool depSatisfied(void);

    vrj::GlContextData<ContextData> mDlData;      // Data for display lists
    vrj::GlContextData<ContextData> mDlDebugData; // Data for debugging display lists
    std::vector<UserData*> mUserData;    // All the users in the program

private:
    // ------------------------------------------------------------------------
    // Draw the scene.  A bunch of cones of differing color and stuff.
    // ------------------------------------------------------------------------
    void myDraw(vrj::User* user);
    void initGLState(void);

    // ------------------------------------------------------------------------
    // Call the display list created in contextInit() so that the cone will be
    // rendered.
    // ------------------------------------------------------------------------
    inline void
    drawCone (void) {
        glCallList(mDlData->dlIndex);
//        drawCone(1.5, 2.0, 20, 10);
    }

    // ------------------------------------------------------------------------
    // Render a cone using the given parameters.
    // ------------------------------------------------------------------------
    inline void
    drawCone(GLdouble base, GLdouble height, GLint slices, GLint stacks) {
        gluCylinder(mConeQuad, base, 0.0, height, slices, stacks);
        gluDisk(mBaseQuad, 0.0, base, slices, 1);
    }

    GLUquadric* mConeQuad;      // GLU quadric for the cone
    GLUquadric* mBaseQuad;      // GLU quadric for the cone's base
};


#endif	/* _CONES_APP_ */
