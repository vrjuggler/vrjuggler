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


#include <Kernel/GL/vjGlOSXWindow.h>
#include <vjConfig.h>


#include <Kernel/GL/vjGlWindow.h>
#include <Kernel/vjKernel.h>
#include <Utils/vjDebug.h>
#include <Kernel/vjDisplay.h>
#include <Kernel/vjDisplayManager.h>
#include <GL/gl.h>
#include <Config/vjConfigChunk.h>


vjGlOSXWindow::vjGlOSXWindow():vjGlWindow() {

}

vjGlOSXWindow::~vjGlOSXWindow() {
    close();
}

void vjGlOSXWindow::swapBuffers() {
    vjDEBUG(vjDBG_INPUT_MGR,7) << "vjGlOSXWindow::swapBuffers()" << std::endl << vjDEBUG_FLUSH;
    if(aglContext)
    {
        aglSwapBuffers (aglContext);
        
        Rect rectPort;
        if(gpWindow)
        {
            GetWindowPortBounds (gpWindow, &rectPort);
            int newWidth = rectPort.right - rectPort.left;
            int newHeight = rectPort.bottom - rectPort.top;
            if( newWidth != window_width || newHeight != window_height)
            {
                // Refresh the window with black...
                aglUpdateContext (aglContext);
                glViewport (0, 0, rectPort.right - rectPort.left, rectPort.bottom - rectPort.top);
                window_width = newWidth; window_height = newHeight;
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glClear (GL_COLOR_BUFFER_BIT);
                aglSwapBuffers (aglContext);
                
                glViewport (0, 0, rectPort.right - rectPort.left, rectPort.bottom - rectPort.top);
            }
        }
    }
}

int vjGlOSXWindow::open() {
    vjDEBUG(vjDBG_INPUT_MGR,2) << "vjGlOSXWindow::open()" << std::endl << vjDEBUG_FLUSH;
    
    GDHandle hGDWindow;

    rectWin.top = origin_y;
    rectWin.left = origin_x;
    rectWin.bottom = origin_y + window_height;
    rectWin.right = origin_x + window_width;
    
    if (noErr != CreateNewWindow (kDocumentWindowClass, kWindowStandardDocumentAttributes | kWindowNoShadowAttribute | kWindowLiveResizeAttribute, &rectWin, &gpWindow))
    {
        vjDEBUG(vjDBG_INPUT_MGR,0) << "vjGlOSXWindow::open()	Window failed to open!" << std::endl << vjDEBUG_FLUSH;
        return false;
    } 
    SetWindowTitleWithCFString(gpWindow,window_title);
    InstallStandardEventHandler(GetWindowEventTarget(gpWindow));
    ChangeWindowAttributes(gpWindow, NULL, kWindowCloseBoxAttribute );
    
    ShowWindow (gpWindow);
    SetPort ( (GrafPtr) GetWindowPort(gpWindow) );
    glInfo.fAcceleratedMust = false; 	// must renderer be accelerated?
    glInfo.VRAM = 0 * 1048576;			// minimum VRAM (if not zero this is always required)
    glInfo.textureRAM = 0 * 1048576;	// minimum texture RAM (if not zero this is always required)
    glInfo.fDraggable = false; 		// desired vertical refresh frquency in Hz (0 = any)
    glInfo.fmt = 0;					// output pixel format
    
    int i = 0;
    glInfo.aglAttributes [i++] = AGL_RGBA;
    glInfo.aglAttributes [i++] = AGL_DOUBLEBUFFER;
    glInfo.aglAttributes [i++] = AGL_DEPTH_SIZE;
    glInfo.aglAttributes [i++] = 32;
    glInfo.aglAttributes [i++] = AGL_NONE;
    
    BuildGLFromWindow (gpWindow, &aglContext, &glInfo);
    if (!aglContext)
    {
        DestroyGLFromWindow (&aglContext, &glInfo);
        vjDEBUG(vjDBG_INPUT_MGR,0) << "vjGlOSXWindow::open()	Window could not create GL Context!" << std::endl << vjDEBUG_FLUSH;
        return false;
    }
    Rect rectPort;
    GetWindowPortBounds (gpWindow, &rectPort);
    aglUpdateContext (aglContext);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT);
    aglSwapBuffers (aglContext);
    glViewport (0, 0, rectPort.right - rectPort.left, rectPort.bottom - rectPort.top);
    
    window_is_open = true;
    return true;
}

int vjGlOSXWindow::close() {
    vjDEBUG(vjDBG_INPUT_MGR,2) << "vjGlOSXWindow::close()" << std::endl << vjDEBUG_FLUSH;
    if(!gpWindow) return false;
    
    DestroyGLFromWindow (&aglContext, &glInfo);
    DisposeWindow (gpWindow);
    
    gpWindow = NULL;
    gFrameWindow = 0;
    gTimeWindow.hi = 0;
    gTimeWindow.lo = 0;
    gRotation = 0.0;
    return true;
}

bool vjGlOSXWindow::makeCurrent() {
    vjDEBUG(vjDBG_INPUT_MGR,7) << "vjGlOSXWindow::makeCurrent()" << std::endl << vjDEBUG_FLUSH;
    if(!aglContext) return false;
    aglSetCurrentContext (aglContext);
    return true;
}

void vjGlOSXWindow::config(vjDisplay* _display)
{
   vjDEBUG(vjDBG_INPUT_MGR,0) << "vjGlOSXWindow::config(vjDisplay* _display)" << std::endl << vjDEBUG_FLUSH;
   
   vjGlWindow::config(_display);

    // Get the vector of display chunks
   vjConfigChunk* dispSysChunk = vjDisplayManager::instance()->getDisplaySystemChunk();
   vjConfigChunk* displayChunk = _display->getConfigChunk();
   
   mPipe = _display->getPipe();
   vjASSERT(mPipe >= 0);
   
   window_title = CFStringCreateWithCString(NULL, _display->getName().c_str(), kCFStringEncodingMacRoman);
}


/**** Static Helpers *****/
/* static */ bool vjGlOSXWindow::createHardwareSwapGroup(std::vector<vjGlWindow*> wins)
{
   return true; // This is not supported, just stubbed out.
}