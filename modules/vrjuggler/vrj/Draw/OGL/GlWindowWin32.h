#ifndef _GL_WIN_WIN32_
#define _GL_WIN_WIN32_

#pragma warning(disable:4786)  // identifier truncated to 255 characters in the debug information


#include <vjConfig.h>
#include <GL/gl.h>
#include <Kernel/GL/vjGlWindow.h>

class vjGlWinWin32 : public vjGlWindow
{
public:
   vjGlWinWin32()
   {
		match = NULL;		// Initialize all values to null
		hWnd = NULL;
		hRC = NULL;
		hDC = NULL;
   }

public:

   //: Open the OpenGL window
   //! PRE: this has been configured
	// POST: Window created
	//			Create message sets the visual
	//! RETURN: true - window opened correctly
   virtual int open();

   //: Close the OpenGL window
	//! NOTE: Must be called by the same thread that called open
	//! RETURN: true - window closed correctly
   virtual int close();

   //: Sets the current OpenGL context to this window
   //! POST: this.context is active context
   virtual bool makeCurrent();

   //: Performs an OpenGL swap buffers command
   virtual void swapBuffers();

   //: Check for window events
   //! POST: All win32 events have ben dispatched and dealt with
   virtual void checkEvents();

protected:
	// WindowProcedure to deal with the events generated.
	// Called only for the window that we are controlling
	LRESULT handleEvent(HWND 	hWnd,
							  UINT	message,
							  WPARAM	wParam,
							  LPARAM	lParam);
	
	// Set the pixel format for the given display context
	//! RETURN: success
	bool setPixelFormat(HDC hDC);

	void sizeChanged(long width, long height);

protected:
	// Register the window class with windows
	static bool registerWindowClass();
	static LRESULT CALLBACK WndProc(	HWND 	hWnd,
												UINT	message,
												WPARAM	wParam,
												LPARAM	lParam);

	static WNDCLASS winClass;			// The window class to register
	static bool		 mWinRegisteredClass;	// Have we already registered

protected:
	// Keep a map of window handles to ogl windows
	// This can be used by the message handler to call a class specific
	// message handler for each window

	// Add a window to the map
	static void addWindow(HWND handle, vjGlWinWin32* glWin);
	// Remove a window from the map
	static void removeWindow(HWND handle);
	// Get the glWin we are dealing with
	static vjGlWinWin32* getGlWin(HWND handle);

   static std::map<HWND, vjGlWinWin32*>		glWinMap;

public:
	HWND	hWnd;		// Window handle
	HGLRC hRC;		// Permenant Rendering context
	HDC	hDC;		// Private GDI Device context

	PIXELFORMATDESCRIPTOR* match;		// Points the the found pixel format
};




#endif

