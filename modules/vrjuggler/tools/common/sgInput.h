/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

#ifndef _SG_INPUT_H_
#define _SG_INPUT_H_


#include <vrj/Math/Vec3.h>
//#include <sgSeg.h>
//#include <sgPlane.h>
#include <GL/gl.h>
class sgChan;

/**
 * sgInput: Class to track the mouse input and location.
 *
 * All xformers and interaction (picking, etc) should use
 * this class for input.
 */
class sgInput
{
public:
    sgInput();

	/// Update object with new mouse location, motion = 1 if in motion
    void updateMouse(int xLoc, int yLoc, int motion = -1);

	/**
	 * This routine updates all the cursor world location data for
	 * OpenGL.  It should be called in the drawing "thread" by
	 * the channel that the cursor is currently over.
	 */
    void update();

    void updateViewportAndMats();
    void updateGLmouse();

    /*
    void updateProjSeg();
    void updateZeroPlaneIsect();
    */

	// Set if we are in motion
    void setMotion(int val)
		{ _inMotion = val; }
	
    int inMotion()
		{ return _inMotion; }
	
	// ---- BUTTON ROUTINES ---- //
	/// Must be called each frame BEFORE updating buttons. Allows for release & press detection
    void saveButtonState()
		{ oldButtonState = buttonState; }

    void clearButtons()
		{ buttonState = 0; }

    void addButton(int button)
		{ buttonState |= (1<<(button-1)); }
	
    void clearButton(int button)
		{ buttonState &= ~(1<<(button-1)); }
	

public:
		// -- User Button Routines -- //
	int btnDown(int button)
		{ return (buttonState & (1<<(button-1))); }

    int oldBtnDown(int button)
		{ return (oldButtonState & (1<<(button-1))); }
	
	/// Bool = btn was just released this frame
    int btnReleased(int button)
		{ return (oldBtnDown(button) && (!btnDown(button))); }
		
	/// Returns whethere button was just pressed this frame.
    int btnPressed(int button)
		{ return ((!oldBtnDown(button)) && btnDown(button)); }

public:
	/// Cursor location
    int curX, curY;
    int oldX, oldY;

    int GLmouseX, GLmouseY;
    int oldGLmouseX, oldGLmouseY;

    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];

    //sgSeg   projSeg;		// Segment to calculate z=0 intersection
    //sgPlane zZeroPlane;		// Z=0 place to isect with
    //sgVec3  zZeroPt, oldZZeroPt;

    int	    _inMotion;		// Is the mouse in motion??

	/// Button State variables
    int	buttonState;
    int oldButtonState;

};

#endif
