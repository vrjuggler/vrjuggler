#ifndef _SG_INPUT_H_
#define _SG_INPUT_H_


#include <Math/vjVec3.h>
//#include <sgSeg.h>
//#include <sgPlane.h>
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
