#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <vjConfig.h>
#include <Kernel/GL/vjGlWinWin32.h>

#define glWinWin32Classname "vjOGLWin32"

// Open the window
// - Creates a window
// - Creates a rendering context
// - Registers new window with the window list
int vjGlWinWin32::open()
{
	if(false == vjGlWinWin32::registerWindowClass())
	{
		return 0;
	}

   if(window_is_open)
      return 1;

	HMODULE hMod = GetModuleHandle(NULL);

	// Create the main application window
	hWnd = CreateWindow(
				glWinWin32Classname,
				glWinWin32Classname,
				
				// OpenGL requires WS_CLIPCHILDREN and WS_CLIPSIBLINGS
				WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
	
				// Window position and size
				origin_x, origin_y,
				window_width, window_height,
				NULL,
				NULL,
				hMod,
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
	assert(hRC != NULL);
	wglMakeCurrent(hDC, hRC);
		
	// Register the window with the window list
	vjGlWinWin32::addWindow(hWnd,this);

	// Display the window
	ShowWindow(hWnd,SW_SHOW);
	UpdateWindow(hWnd);				// Tell the window to paint
   window_is_open = true;

	return 1;
}


//: Close the OpenGL window
//! NOTE: Must be called by the same thread that called open
int vjGlWinWin32::close()
{
	if(!window_is_open)
      return false;

   // Remove window from window list
	vjGlWinWin32::removeWindow(hWnd);

	window_is_open = false;

   // destroy the win32 window
	return (1 == DestroyWindow(hWnd));
}

//: Sets the current OpenGL context to this window
//! POST: this.context is active context
bool vjGlWinWin32::makeCurrent()
{
	assert((hDC != NULL) && (hRC != NULL));
	wglMakeCurrent(hDC, hRC);		// Make our context current
	return true;
}

// Swap the front and back buffers
void vjGlWinWin32::swapBuffers()
{
	assert(hDC != NULL);
	SwapBuffers(hDC);
}

// WindowProcedure to deal with the events generated.
// Called only for the window that we are controlling
LRESULT vjGlWinWin32::handleEvent(HWND 	hWnd,
										  UINT	message,
										  WPARAM	wParam,
										  LPARAM	lParam)
{
	switch (message)
	{
		// ---- Window creation, setup for OpenGL ---- //
	case WM_CREATE:
		assert(false);								// Should never get called because 
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
bool vjGlWinWin32::setPixelFormat(HDC hDC)
{
	int pixel_format;
	PIXELFORMATDESCRIPTOR pfd;
	match = NULL;

	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = (sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nVersion = 1;

	/* Defaults. */
	pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
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
		/*
		if (stereo)
		{
			if (!(match->dwFlags & PFD_STEREO))
			{
				free(match);
				return NULL;
			}
		}
		*/
	}

	// Set the pixel format for the device context
	SetPixelFormat(hDC, pixel_format, &pfd);
	return true;
}

// the user has changed the size of the window
void vjGlWinWin32::sizeChanged(long width, long height)
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
LRESULT CALLBACK vjGlWinWin32::WndProc(	HWND 	hWnd,
												UINT	message,
												WPARAM	wParam,
												LPARAM	lParam)
{
	vjGlWinWin32* glWin = getGlWin(hWnd);
	
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
bool vjGlWinWin32::mWinRegisteredClass = false;
WNDCLASS vjGlWinWin32::winClass;			// The window class to register
std::map<HWND, vjGlWinWin32*> vjGlWinWin32::glWinMap;


bool vjGlWinWin32::registerWindowClass()
{
	if(mWinRegisteredClass)
		return true;

	char lpszAppName[1024];
	GetModuleFileName(NULL,lpszAppName,sizeof(lpszAppName));

	mWinRegisteredClass = true;		// We have registered now

	HINSTANCE hInstance = GetModuleHandle(NULL);

	// Register Window style
	winClass.style				= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	winClass.lpfnWndProc		= (WNDPROC)vjGlWinWin32::WndProc;
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
void vjGlWinWin32::addWindow(HWND handle, vjGlWinWin32* glWin)
{
	assert(glWin != NULL);

	if(glWinMap.find(handle) == glWinMap.end())		// Not already there
		glWinMap[handle] = glWin;
	//else
	// assert(false);
}

void vjGlWinWin32::removeWindow(HWND handle)
{
	glWinMap.erase(handle);		// Erase the entry in the list
}

vjGlWinWin32* vjGlWinWin32::getGlWin(HWND handle)
{
   std::map<HWND, vjGlWinWin32*>::iterator glWinIter;

	glWinIter = glWinMap.find(handle);
	if(glWinIter == glWinMap.end())		// Not found
		return NULL;
	else
		return (*glWinIter).second;					// Return the found window
}

