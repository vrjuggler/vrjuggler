#ifndef _SG_GLUT_WIN_H_
#define _SG_GLUT_WIN_H_

#include <iostream.h>
#include <stdio.h>
#include <GL/glut.h>
#include <sgChan.h>
#include <sgInput.h>

/**
 * sgGlutWin: This class defines a window for glut.
 *		this should be used by the glut application.
 */
class sgGlutWin
{
public:
	sgGlutWin();

	void setWindow(int handle)
		{ winHandle = handle; }

	// Creates a new Glut window and assigns it to us.
	// Create the Glut window
	// Returns: id of the glut window
	int createWindow(int xo = 0, int yo = 0, int xs = 400, int ys = 400, char* name = NULL, unsigned int dispMode = 0);
		
	// Updates the channels viewport to current window size
	void updateChan();

   //: Get the channel that we are displaying
   sgChan* getChan()
   { return &chan; }

public:
	// Draw the window.
	// usually just drawing the channel
	virtual void draw();

	// Function called once each frame at end of idle function
	// Used now to save button state
	virtual void app();

	// Called when the window resizes.
	// Then updates channel
	virtual void reshape(int newWidth, int newHeight);
		
public:
	// Mouse callback functions
	virtual void mouseInput(int button, int state, int x, int y);
	virtual void mouseMotion(int x, int y);
	virtual void mousePassiveMotion(int x, int y);

public:		// --- Performance Functions --- //
	// Returns the Frames Per Second (FPS)
	float getFPS();

public:
	sgChan	chan;
	sgInput  input;
	int		winHandle;		// The glut window.  If -1, then has not been set.
	int		width, height;
	bool		showPerformance;	// Should we show performance

		// Performance parameters
	int   prevFrameStart;					// The starting time of the previous frame
	int   previousFrameTimings[3];			// The timing of the previous 3 frames
	int   curFrameToTime;					// The index of the current frame to time
};

#endif
