/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#include <sgGlutWin.h>
#include <sgGlutUtil.h>


sgGlutWin::sgGlutWin()
{
	winHandle = -1;
	chan.setInputObject(&input);		// Set the object for input to update
	showPerformance = true;		      // Default to not showing performance
	previousFrameTimings[0] = previousFrameTimings[1] = previousFrameTimings[2] = 0;			
	curFrameToTime = 0;
	prevFrameStart = 0;
}


	// Creates a new Glut window and assigns it to us.
int sgGlutWin::createWindow(int xo, int yo, int xs, int ys, char* name, unsigned int dispMode)
{
	if(dispMode == 0)	// Not set
		dispMode = (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

	glutInitDisplayMode(dispMode);
	winHandle = glutCreateWindow(name);
	glutPositionWindow(xo, yo);
	glutReshapeWindow(xs, ys);
	width = xs; height = ys;
	updateChan();

	cout << "---- Window information ---" << endl;
	cout << "Zbits:" << glutGet(GLUT_WINDOW_DEPTH_SIZE) << endl
		 << "Double:" << glutGet(GLUT_WINDOW_DOUBLEBUFFER) << endl
		 << "R:" << glutGet(GLUT_WINDOW_RED_SIZE) << endl
		 << "G:" << glutGet(GLUT_WINDOW_GREEN_SIZE) << endl
		 << "B:" << glutGet(GLUT_WINDOW_BLUE_SIZE) << endl
		 << "A:" << glutGet(GLUT_WINDOW_ALPHA_SIZE) << endl;

	cout << "----- GL Info -------" << endl;
	cout << "Vendor: " << glGetString(GL_VENDOR) << endl
		  << "Renderer: " << glGetString(GL_RENDERER) << endl
		  << "Version: " << glGetString(GL_VERSION) << endl
		  << "Ext: " << glGetString(GL_EXTENSIONS) << endl;

#if defiend(WIN32) || defined(WIN64)
	cout << "------ Windows ------\n" << endl;
	PIXELFORMATDESCRIPTOR  pfd;
	HDC  hdc;
	int  iPixelFormat;
	hdc = wglGetCurrentDC();
	iPixelFormat = GetPixelFormat(hdc);

	// obtain a detailed description of that pixel format
	DescribePixelFormat(hdc, iPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	
	if(pfd.dwFlags & PFD_GENERIC_FORMAT)
	{
		if(pfd.dwFlags & PFD_GENERIC_ACCELERATED)
			cout << "Renderer: MCD ---> So So" << endl;
		else
			cout << "Renderer: MS Software ---> Bad" << endl;
	} else {
		cout << "Renderer: ICD  --> Good" << endl;
	}

	if(pfd.dwFlags & PFD_SUPPORT_OPENGL)
		cout << "OpenGL: Supported\n";
	else
		cout << "OpenGL: Unsupported\n";
	
	if(pfd.dwFlags & PFD_DOUBLEBUFFER)
		cout << "Double Buffered\n";
	else
		cout << "Single Buffered\n";
	cout << "-----------------\n\n\n" << flush;

#endif

	return winHandle;
}

void sgGlutWin::updateChan()
{
	chan.setViewport(0, 0, width, height);
}


void sgGlutWin::draw()
{
	// Update statistics
   int curTime = glutGet(GLUT_ELAPSED_TIME);
	previousFrameTimings[curFrameToTime] = (curTime - prevFrameStart);
	prevFrameStart = curTime;
	curFrameToTime += 1;
	if(curFrameToTime >= 3)
		curFrameToTime = 0;

	// Clear the buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// DRAW the channel
	chan.draw();

	if(showPerformance)			// Should we draw performance
	{
		char buffer[25];
		sprintf(buffer, "%.1f fps", getFPS());
		glColor3f(3.0f, 0.0f, 0.0f);
		sgGlutUtil::text(5, 20, 20, buffer);
	}
	//glutSwapBuffers();  --- Moved because it eliminates post draw
}

	// Function called once each frame at end of idle function
void sgGlutWin::app()
{
		// Save Buttons here so that we only detect up/down in one frame
	input.saveButtonState();		// Save button state so we can detect ups	
}

void sgGlutWin::reshape(int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;
	updateChan();
}
	

void sgGlutWin::mouseInput(int button, int state, int x, int y)
{
		//cerr << "sgGLUTApp::mouseInput:x:" << x << "\ty:" << y << endl;
	input.updateMouse(x, y, 0);
	
	int inputButton = ((button == GLUT_LEFT_BUTTON) ? 1 : ((button == GLUT_MIDDLE_BUTTON) ? 2 : 3));
	if(state == GLUT_DOWN)
		input.addButton(inputButton);
	else
		input.clearButton(inputButton);
}

void sgGlutWin::mouseMotion(int x, int y)
{
	//cerr << "sgGLUTApp::mouseMotion:x:" << x << "\ty:" << y << endl;
	input.updateMouse(x, y, 1);
}

void sgGlutWin::mousePassiveMotion(int x, int y)
{
	//cerr << "sgGLUTApp::mousePassiveMotion:x:" << x << "\ty:" << y << endl;
	input.updateMouse(x, y, 1);
}

float sgGlutWin::getFPS()
{
   long sum = 0;
   for(int i=0;i<3; i++)
   {
      sum += previousFrameTimings[i];
   }

   float retVal;
   retVal = float(sum)/3.0f;
   retVal = retVal/1000.0f;
   retVal = 1.0f/retVal;			// Get frame rate
   return retVal;
}
