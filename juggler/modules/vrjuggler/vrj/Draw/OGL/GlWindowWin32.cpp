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

#include <vrj/vjConfig.h>
#include <vrj/Draw/OGL/GlWinWin32.h>
#include <vrj/Util/Debug.h>
#include <vpr/Util/Assert.h>

#define glWinWin32Classname "vjOGLWin32"

namespace vrj
{
   
// Open the window
// - Creates a window
// - Creates a rendering context
// - Registers new window with the window list
int GlWinWin32::open()
{
	if(false == GlWinWin32::registerWindowClass())
	{
		return 0;
	}

   if(window_is_open)
      return 1;

	HMODULE hMod = GetModuleHandle(NULL);
        DWORD style;
        int root_height;

        // OpenGL requires WS_CLIPCHILDREN and WS_CLIPSIBLINGS.
        style = WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

        // If we want a border, create an overlapped window.  This will have
        // a titlebar and a border.
        if ( border ) {
            vjDEBUG(vjDBG_DRAW_MGR, 5) << "attempting to give window a border"
                                       << std::endl << vjDEBUG_FLUSH;
            style |= WS_OVERLAPPEDWINDOW;
        }
        // Otherwise, come as close as possible to having no border by using
        // the thin-line border.
        else {
            vjDEBUG(vjDBG_DRAW_MGR, 5) << "attempting to make window borderless"
                                       << std::endl << vjDEBUG_FLUSH;
            style |= WS_OVERLAPPED | WS_POPUP | WS_VISIBLE;
        }

        root_height = GetSystemMetrics(SM_CYSCREEN);

	// Create the main application window
        hWnd = CreateWindow(glWinWin32Classname, glWinWin32Classname, style,
                            origin_x, root_height - origin_y - window_height,
                            window_width, window_height, NULL, NULL, hMod,
                            NULL);

	// If window was not created, quit
	if(NULL == hWnd)
		return 0;

	// We have a valid window, so... Create the context
	//case WM_CREATE:			
	hDC = GetDC(hWnd);			         // Store the device context		
	if(false == setPixelFormat(hDC))		// Select the pixel format	
	{
		return 0;
	}

	// Create the rendering context and make it current
	hRC = wglCreateContext(hDC);
	vprASSERT(hRC != NULL);
	wglMakeCurrent(hDC, hRC);
		
	// Register the window with the window list
	GlWinWin32::addWindow(hWnd,this);

	// Display the window
	ShowWindow(hWnd,SW_SHOW);
	UpdateWindow(hWnd);				// Tell the window to paint
   window_is_open = true;

	return 1;
}


//: Close the OpenGL window
//! NOTE: Must be called by the same thread that called open
int GlWinWin32::close()
{
	if(!window_is_open)
      return false;

   // Remove window from window list
	GlWinWin32::removeWindow(hWnd);

	window_is_open = false;

   // destroy the win32 window
	return (1 == DestroyWindow(hWnd));
}

//: Sets the current OpenGL context to this window
//! POST: this.context is active context
bool GlWinWin32::makeCurrent()
{
	vprASSERT((hDC != NULL) && (hRC != NULL));
	wglMakeCurrent(hDC, hRC);		// Make our context current
	return true;
}

// Swap the front and back buffers
// Process events here
void GlWinWin32::swapBuffers()
{
	vprASSERT(hDC != NULL);
	SwapBuffers(hDC);
}

void GlWinWin32::checkEvents()
{
   MSG win_message;   
   
   while(PeekMessage(&win_message,NULL,0,0,PM_REMOVE))
	{
		// Test if quit
		if(win_message.message == WM_QUIT)
			break;

		TranslateMessage(&win_message);		// Translate the accelerator keys
		DispatchMessage(&win_message);		// Send to the WinProc
	}		
}


// WindowProcedure to deal with the events generated.
// Called only for the window that we are controlling
LRESULT GlWinWin32::handleEvent(HWND 	hWnd,
										  UINT	message,
										  WPARAM	wParam,
										  LPARAM	lParam)
{
	switch (message)
	{
		// ---- Window creation, setup for OpenGL ---- //
	case WM_CREATE:
		vprASSERT(false);								// Should never get called because 
														//we are not registered when this gets called

		hDC = GetDC(hWnd);			         // Store the device context		
		if(false == setPixelFormat(hDC))		// Select the pixel format	
		{
			return 0;
		}

		// Create the rendering context and make it current
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);
		break;
		
		// ---- Window is being destroyed, cleanup ---- //
	case WM_DESTROY:
		
		// Deselect the current rendering context and delete it
		wglMakeCurrent(hDC,NULL);
		wglDeleteContext(hRC);
		
		// Tell the application to terminate after the window
		// is gone.
		PostQuitMessage(0);
		break;
		
		// --- Window is resized. --- //
	case WM_SIZE:
		// Call our function which modifies the clipping
		// volume and viewport
		sizeChanged(LOWORD(lParam), HIWORD(lParam));
		break;
		
		
		// The painting function.  This message sent by Windows 
		// whenever the screen needs updating.
	case WM_PAINT:
		{
			PAINTSTRUCT   ps;			// Paint structure
			BeginPaint(hWnd, &ps);	// Validate the drawing of the window
			EndPaint(hWnd, &ps);
		}
		break;
		
	default:   // Passes it on if unproccessed
		return (DefWindowProc(hWnd, message, wParam, lParam));
		
	}
	
	return (0L);
}

// Set the pixel format for the given window
bool GlWinWin32::setPixelFormat(HDC hDC)
{
	int pixel_format;
	PIXELFORMATDESCRIPTOR pfd;
	match = NULL;

	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = (sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nVersion = 1;

	/* Defaults. */
	pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;

        if ( mDisplay->inStereo() ) {
            pfd.dwFlags |= PFD_STEREO;
        }

	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8; /* Try to get the maximum. */
   pfd.cGreenBits = 8;
   pfd.cBlueBits = 8;
   pfd.cAlphaBits = 8;
   pfd.cDepthBits = 32;
   pfd.cStencilBits = 0;
	pfd.cAccumBits = 0;
	pfd.cAuxBuffers = 0;
   
	// Let Win32 choose one for us 
	pixel_format = ChoosePixelFormat(hDC, &pfd);
	if (pixel_format > 0)
	{
		match = (PIXELFORMATDESCRIPTOR *) malloc(sizeof(PIXELFORMATDESCRIPTOR));
		DescribePixelFormat(hDC, pixel_format, sizeof(PIXELFORMATDESCRIPTOR), match);

    /* ChoosePixelFormat is dumb in that it will return a pixel
       format that doesn't have stereo even if it was requested
       so we need to make sure that if stereo was selected, we
       got it. */
		if (mDisplay->inStereo())
		{
			if (!(match->dwFlags & PFD_STEREO))
			{
				free(match);
				return NULL;
			}
		}
	}

	// Set the pixel format for the device context
	SetPixelFormat(hDC, pixel_format, &pfd);
	return true;
}

// the user has changed the size of the window
void GlWinWin32::sizeChanged(long width, long height)
{
	window_width = width;
	window_height = height;

	if(window_width == 0)        // Make sure we don't have window of 1 size (divide by zero would follow)
		window_width = 1;
	if(window_height == 0)
		window_height = 1;

	// XXX: Should reset viewport here
}



//-------------------------------
//: Global Window event handler
//-------------------------------
LRESULT CALLBACK GlWinWin32::WndProc(	HWND 	hWnd,
												UINT	message,
												WPARAM	wParam,
												LPARAM	lParam)
{
	GlWinWin32* glWin = getGlWin(hWnd);
	
	if(glWin != NULL)		// Message for one of ours
	{
		return glWin->handleEvent(hWnd, message, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

//-------------------------------
//: Window registration
//-------------------------------
bool GlWinWin32::mWinRegisteredClass = false;
WNDCLASS GlWinWin32::winClass;			// The window class to register
std::map<HWND, GlWinWin32*> GlWinWin32::glWinMap;


bool GlWinWin32::registerWindowClass()
{
	if(mWinRegisteredClass)
		return true;

	char lpszAppName[1024];
	GetModuleFileName(NULL,lpszAppName,sizeof(lpszAppName));

	mWinRegisteredClass = true;		// We have registered now

	HINSTANCE hInstance = GetModuleHandle(NULL);

	// Register Window style
	winClass.style				= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	winClass.lpfnWndProc		= (WNDPROC)GlWinWin32::WndProc;
	winClass.cbClsExtra		= 0;
	winClass.cbWndExtra		= 0;
	winClass.hInstance 		= hInstance;			// Get handle to the module that created current process
	winClass.hIcon				= LoadIcon(NULL, IDI_WINLOGO);;
	winClass.hCursor			= LoadCursor(NULL, IDC_ARROW);
	
	// No need for background brush for OpenGL window
	winClass.hbrBackground	= NULL;		
	
	winClass.lpszMenuName	= NULL;
	winClass.lpszClassName	= glWinWin32Classname;

	// Register the window class
	if(RegisterClass(&winClass) == 0)
		return false;
	else
		return true;
};

//----------------------//
//			WinList			//
//----------------------//
void GlWinWin32::addWindow(HWND handle, GlWinWin32* glWin)
{
	vprASSERT(glWin != NULL);

	if(glWinMap.find(handle) == glWinMap.end())		// Not already there
		glWinMap[handle] = glWin;
	//else
	// vprASSERT(false);
}

void GlWinWin32::removeWindow(HWND handle)
{
	glWinMap.erase(handle);		// Erase the entry in the list
}

GlWinWin32* GlWinWin32::getGlWin(HWND handle)
{
   std::map<HWND, GlWinWin32*>::iterator glWinIter;

	glWinIter = glWinMap.find(handle);
	if(glWinIter == glWinMap.end())		// Not found
		return NULL;
	else
		return (*glWinIter).second;					// Return the found window
}

};
