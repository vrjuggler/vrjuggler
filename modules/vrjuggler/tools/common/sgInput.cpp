/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#include <sgInput.h>
#include <sgChan.h>
#include <GL/gl.h>

sgInput::sgInput()
{
    //**//zZeroPlane.makePts(sgVec3(0.0, 0.0, 0.0), sgVec3(1.0, 0.0, 0.0), sgVec3(0.0, 1.0, 0.0));
    clearButtons();

    oldX = oldY = curX = curY = -1;
    oldGLmouseX = GLmouseX = oldGLmouseY = GLmouseY = -1;

    //**//zZeroPt.set(-1, -1, -1);
    //**//oldZZeroPt.set(-1, -1, -1);
}

   // motion = -1  <--- Default
void sgInput::updateMouse(int xLoc, int yLoc, int motion)
{
    if(motion >= 0)     // Has been set
      setMotion(motion);

    oldX = curX;
    oldY = curY;
    oldGLmouseX = GLmouseX;
    oldGLmouseY = GLmouseY;

    curX = xLoc;
    curY = yLoc;
}

    /**
     * This routine updates all the cursor world location data for
     * OpenGL.  It should be called in the drawing "thread" by
     * the channel that the cursor is currently over.
     */
void sgInput::update()
{
    updateViewportAndMats();      // Get the current vp and mats
    updateGLmouse();
    //updateProjSeg();
    //updateZeroPlaneIsect();
}

void sgInput::updateViewportAndMats()
{
    glGetIntegerv(GL_VIEWPORT, viewport);   // Get the viewport to work with
    glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);
}

void sgInput::updateGLmouse()
{
    GLmouseX = curX;
    GLmouseY = viewport[3] - curY - 1;
}

/*
void sgInput::updateProjSeg()
{
    GLdouble wx, wy, wz;    // Returned world coords
    GLdouble wxFar, wyFar, wzFar;

    gluUnProject((GLdouble)GLmouseX, (GLdouble)GLmouseY, 0.001,
      mvmatrix, projmatrix, viewport, &wx, &wy, &wz);
//    cerr << "UnProject: Near: " << wx << " " << wy << " " << wz << endl;

    gluUnProject((GLdouble)GLmouseX, (GLdouble)GLmouseY, 0.99,
      mvmatrix, projmatrix, viewport, &wxFar, &wyFar, &wzFar);
//    cerr << "UnProject: Far: " << wxFar << " " << wyFar << " " << wzFar << endl;

   // Make the projecting segment
    projSeg.makePts(sgVec3(wx, wy, wz), sgVec3(wxFar, wyFar, wzFar));
}

void sgInput::updateZeroPlaneIsect()
{
    float tDist = 0.0f;
    int hit = zZeroPlane.isectLine(projSeg, &tDist);

    if (hit)
    {
         sgDEBUG(2) << "Hit Z Zero Plane: tDist:" << tDist << endl;
      oldZZeroPt = zZeroPt;
      zZeroPt = projSeg.pos + (projSeg.dir * tDist);
         sgDEBUG(2) << "        pt:" << zZeroPt[0] << "  " << zZeroPt[1] << "  " << zZeroPt[2] << endl;
    }

#if defined(DEBUG_SG)
   glBegin(GL_POINTS);
      glColor3f(0.0, 1.0, 0.0);
      glVertex3fv(oldZZeroPt.vec);
      glColor3f(1.0, 0.0, 0.0);
      glVertex3fv(zZeroPt.vec);
   glEnd();
#endif
}
*/
