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

#include <vjConfig.h>

#include <OpenGL/gl.h>

#include <Kernel/GL/vjGlWindow.h>
#include <Kernel/vjKernel.h>
#include <Utils/vjDebug.h>
#include <Kernel/vjDisplay.h>
#include <Kernel/vjDisplayManager.h>
#include <Config/vjConfigChunk.h>

#include <Kernel/GL/vjGlOSXWindow.h>

namespace vrj
{
   
AGLContext GlOSXWindow::aglShareContext = NULL;

GlOSXWindow::GlOSXWindow():GlWindow() {

}

GlOSXWindow::~GlOSXWindow() {
    close();
}

void GlOSXWindow::swapBuffers() {
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

int GlOSXWindow::open() {
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

int GlOSXWindow::close() {
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

bool GlOSXWindow::makeCurrent() {
    vjDEBUG(vjDBG_INPUT_MGR,7) << "vjGlOSXWindow::makeCurrent()" << std::endl << vjDEBUG_FLUSH;
    if(!aglContext) return false;
    aglSetCurrentContext (aglContext);
    return true;
}

void GlOSXWindow::config(Display* _display)
{
   vjDEBUG(vjDBG_INPUT_MGR,0) << "vjGlOSXWindow::config(Display* _display)" << std::endl << vjDEBUG_FLUSH;
   
   GlWindow::config(_display);

    // Get the vector of display chunks
   ConfigChunk* dispSysChunk = DisplayManager::instance()->getDisplaySystemChunk();
   ConfigChunk* displayChunk = _display->getConfigChunk();
   
   mPipe = _display->getPipe();
   vprASSERT(mPipe >= 0);
   
   window_title = CFStringCreateWithCString(NULL, _display->getName().c_str(), kCFStringEncodingMacRoman);
}


/**** Static Helpers *****/
/* static */ bool GlOSXWindow::createHardwareSwapGroup(std::vector<GlWindow*> wins)
{
   return true; // This is not supported, just stubbed out.
}

// ============================================================================
// The following methods come from Carbon SetupGL 1.5 distributed by Apple
// Corporation.  Their use is here is permitted by the license.
// ============================================================================

// ----------------------------------------------------------------------------
// BuildGLFromWindow
//
// Takes window in the form of an AGLDrawable and geometry request and tries
// to build best context
//
// Inputs: 	aglDraw: a valid AGLDrawable (i.e., a WindowPtr)
//		*pcontextInfo: request and requirements for cotext and drawable
//
// Outputs: *paglContext as allocated
//			*pcontextInfo:  allocated parameters
//
// if fail to allocate: paglContext will be NULL
// if error: will return error and paglContext will be NULL
// ----------------------------------------------------------------------------
OSStatus GlOSXWindow::BuildGLFromWindow (WindowPtr pWindow,
                                           AGLContext* paglContext,
                                           structGLWindowInfo* pcontextInfo)
{
   if (!pWindow)
      return paramErr;
   return BuildGLonWindow(pWindow, paglContext, pcontextInfo);
}

OSStatus GlOSXWindow::BuildGLonWindow (WindowPtr pWindow,
                                         AGLContext* paglContext,
                                         structGLWindowInfo* pcontextInfo)
{
   GDHandle hGD = NULL;
   short numDevices;
   GLint depthSizeSupport;
   OSStatus err = noErr;
   
   if (!pWindow || !pcontextInfo)
   {
      ReportError ("NULL parameter passed to BuildGLonDrawable.");
      return paramErr;
   }

   // check renderere VRAM and acceleration
   numDevices = FindGDHandleFromWindow (pWindow, &hGD);
   // so what do we do here?
   if (!pcontextInfo->fDraggable)    // if numDevices > 1 then we will only be using the software renderer otherwise check only window device
   {
      if ((numDevices > 1) || (numDevices == 0)) // this window spans mulitple devices thus will be software only
      {
         // software renderer
         // infinite VRAM, infinite textureRAM, not accelerated
         if (pcontextInfo->fAcceleratedMust)
         {
            ReportError ("Unable to accelerate window that spans multiple devices");
            return err;
         }
      }
      else // not draggable on single device
      {
         if (!CheckRenderer (hGD, &(pcontextInfo->VRAM), &(pcontextInfo->textureRAM), &depthSizeSupport, pcontextInfo->fAcceleratedMust))
         {
            ReportError ("Renderer check failed");
            return err;
         }
      }
   }
   // else draggable so must check all for support (each device should have at least one renderer that meets the requirements)
   else if (!CheckAllDeviceRenderers (&(pcontextInfo->VRAM), &(pcontextInfo->textureRAM), &depthSizeSupport, pcontextInfo->fAcceleratedMust))
   {
      ReportError ("Renderer check failed");
      return err;
   }
   
   // do agl
   if ((Ptr) kUnresolvedCFragSymbolAddress == (Ptr) aglChoosePixelFormat) // check for existance of OpenGL
   {
      ReportError ("OpenGL not installed");
      return NULL;
   }   
   // we successfully passed the renderer check

   if ((!pcontextInfo->fDraggable && (numDevices == 1)))  // not draggable on a single device
      pcontextInfo->fmt = aglChoosePixelFormat (&hGD, 1, pcontextInfo->aglAttributes); // get an appropriate pixel format
   else
      pcontextInfo->fmt = aglChoosePixelFormat (NULL, 0, pcontextInfo->aglAttributes); // get an appropriate pixel format

   aglReportError();

   if (NULL == pcontextInfo->fmt) 
   {
      ReportError("Could not find valid pixel format");
      return NULL;
   }

   // using a default method of sharing all the contexts enables texture sharing across these contexts by default
   *paglContext = aglCreateContext (pcontextInfo->fmt, aglShareContext);            // Create an AGL context
   aglReportError ();
   if (NULL == *paglContext) 
   {
      ReportError ("Could not create context");
      return NULL;
   }
   if (aglShareContext == NULL)
      aglShareContext = *paglContext;
   
   if (!aglSetDrawable (*paglContext, GetWindowPort (pWindow))) // attach the CGrafPtr to the context
      return aglReportError ();
   
   if(!aglSetCurrentContext (*paglContext)) // make the context the current context
      return aglReportError ();

   return err;
}

// ----------------------------------------------------------------------------
// DestroyGLFromWindow
//
// Destroys context that waas allocated with BuildGLFromWindow
// Ouputs: *paglContext should be NULL on exit
// ----------------------------------------------------------------------------
OSStatus GlOSXWindow::DestroyGLFromWindow (AGLContext* paglContext,
                                             structGLWindowInfo* pcontextInfo)
{
	OSStatus err;
	
	if ((!paglContext) || (!*paglContext))
		return paramErr; // not a valid context
	glFinish ();
	aglSetCurrentContext (NULL);
	err = aglReportError ();
	aglSetDrawable (*paglContext, NULL);
	err = aglReportError ();
	aglDestroyContext (*paglContext);
	err = aglReportError ();
	*paglContext = NULL;
	
	if (pcontextInfo->fmt)
	{
		aglDestroyPixelFormat (pcontextInfo->fmt); // pixel format is no longer valid
		err = aglReportError ();
	}
	pcontextInfo->fmt = 0;
	
	return err;
}

short GlOSXWindow::FindGDHandleFromWindow (WindowPtr pWindow,
                                             GDHandle* phgdOnThisDevice)
{
   GrafPtr pgpSave;
   Rect rectWind, rectSect;
   long greatestArea, sectArea;
   short numDevices = 0;
   GDHandle hgdNthDevice;
   
   if (!pWindow || !phgdOnThisDevice)
      return NULL;
      
   *phgdOnThisDevice = NULL;
   
   GetPort (&pgpSave);
   SetPortWindowPort (pWindow);

#if TARGET_API_MAC_CARBON
   GetWindowPortBounds (pWindow, &rectWind);
#else
   rectWind = pWindow->portRect;
#endif // TARGET_API_MAC_CARBON
   LocalToGlobal ((Point*)& rectWind.top);   // convert to global coordinates
   LocalToGlobal ((Point*)& rectWind.bottom);
   hgdNthDevice = GetDeviceList ();
   greatestArea = 0;
   // check window against all gdRects in gDevice list and remember 
   //  which gdRect contains largest area of window}
   while (hgdNthDevice)
   {
      if (TestDeviceAttribute (hgdNthDevice, screenDevice))
         if (TestDeviceAttribute (hgdNthDevice, screenActive))
         {
            // The SectRect routine calculates the intersection 
            //  of the window rectangle and this gDevice 
            //  rectangle and returns TRUE if the rectangles intersect, 
            //  FALSE if they don't.
            SectRect (&rectWind, &(**hgdNthDevice).gdRect, &rectSect);
            // determine which screen holds greatest window area
            //  first, calculate area of rectangle on current device
            sectArea = (long) (rectSect.right - rectSect.left) * (rectSect.bottom - rectSect.top);
            if (sectArea > 0)
               numDevices++;
            if (sectArea > greatestArea)
            {
               greatestArea = sectArea; // set greatest area so far
               *phgdOnThisDevice = hgdNthDevice; // set zoom device
            }
            hgdNthDevice = GetNextDevice(hgdNthDevice);
         }
   }
   
   SetPort (pgpSave);
   return numDevices;
}

// ----------------------------------------------------------------------------
// CheckRenderer
//
// looks at renderer attributes it has at least the VRAM is accelerated
//
// Inputs: 	hGD: GDHandle to device to look at
//		pVRAM: pointer to VRAM in bytes required; out is actual VRAM
//		       if a renderer was found, otherwise it is the input
//		       parameter
//		pTextureRAM:  pointer to texture RAM in bytes required; out is
//		       same (implementation assume VRAM returned by card is
//		       total so we add texture and VRAM)
//		fAccelMust: do we check for acceleration
//
// Returns: true if renderer for the requested device complies, false otherwise
// ----------------------------------------------------------------------------
Boolean GlOSXWindow::CheckRenderer (GDHandle hGD, long* pVRAM,
                                      long* pTextureRAM,
                                      GLint* pDepthSizeSupport,
                                      Boolean fAccelMust)
{
   AGLRendererInfo info, head_info;
   GLint inum;
   GLint dAccel = 0;
   GLint dVRAM = 0, dMaxVRAM = 0;
   Boolean canAccel = false, found = false;
   head_info = aglQueryRendererInfo(&hGD, 1);
   aglReportError ();
   if(!head_info)
   {
      ReportError ("aglQueryRendererInfo error");
      return false;
   }
   else
   {
      info = head_info;
      inum = 0;
      // see if we have an accelerated renderer, if so ignore non-accelerated ones
      // this prevents returning info on software renderer when actually we'll get the hardware one
      while (info)
      {   
         aglDescribeRenderer(info, AGL_ACCELERATED, &dAccel);
         aglReportError ();
         if (dAccel)
            canAccel = true;
         info = aglNextRendererInfo(info);
         aglReportError ();
         inum++;
      }
         
      info = head_info;
      inum = 0;
      while (info)
      {
         aglDescribeRenderer (info, AGL_ACCELERATED, &dAccel);
         aglReportError ();
         // if we can accel then we will choose the accelerated renderer 
         // how about compliant renderers???
         if ((canAccel && dAccel) || (!canAccel && (!fAccelMust || dAccel)))
         {
            aglDescribeRenderer (info, AGL_VIDEO_MEMORY, &dVRAM);   // we assume that VRAM returned is total thus add texture and VRAM required
            aglReportError ();
            if (dVRAM >= (*pVRAM + *pTextureRAM))
            {
               if (dVRAM >= dMaxVRAM) // find card with max VRAM
               {
                  aglDescribeRenderer (info, AGL_DEPTH_MODES, pDepthSizeSupport);   // which depth buffer modes are supported
                  aglReportError ();
                  dMaxVRAM = dVRAM; // store max
                  found = true;
               }
            }
         }
         info = aglNextRendererInfo(info);
         aglReportError ();
         inum++;
      }
   }
   aglDestroyRendererInfo(head_info);
   if (found) // if we found a card that has enough VRAM and meets the accel criteria
   {
      *pVRAM = dMaxVRAM; // return VRAM
      return true;
   }
   // VRAM will remain to same as it did when sent in
   return false;
}

//-----------------------------------------------------------------------------
// CheckAllDeviceRenderers 
//
// looks at renderer attributes and each device must have at least one
// renderer that fits the profile
//
// Inputs: 	pVRAM: pointer to VRAM in bytes required; out is actual min
//                     VRAM of all renderers found, otherwise it is the input
//                     parameter
//		pTextureRAM:  pointer to texture RAM in bytes required; out is
//		       same (implementation assume VRAM returned by card is
//		       total so we add texture and VRAM)
//		fAccelMust: do we check fro acceleration
//
// Returns: true if any renderer for on each device complies (not necessarily
// the same renderer), false otherwise
//-----------------------------------------------------------------------------
Boolean GlOSXWindow::CheckAllDeviceRenderers (long* pVRAM, long* pTextureRAM,
                                                GLint* pDepthSizeSupport,
                                                Boolean fAccelMust)
{
   AGLRendererInfo info, head_info;
   GLint inum;
   GLint dAccel = 0;
   GLint dVRAM = 0, dMaxVRAM = 0;
   Boolean canAccel = false, found = false, goodCheck = true; // can the renderer accelerate, did we find a valid renderer for the device, are we still successfully on all the devices looked at
   long MinVRAM = 0x8FFFFFFF; // max long
   GDHandle hGD;
   
   hGD = GetDeviceList (); // get the first screen
   while (hGD && goodCheck)
   {
      head_info = aglQueryRendererInfo(&hGD, 1);
      aglReportError ();
      if(!head_info)
      {
         ReportError ("aglQueryRendererInfo error");
         return false;
      }
      else
      {
         info = head_info;
         inum = 0;
         // see if we have an accelerated renderer, if so ignore non-accelerated ones
         // this prevents returning info on software renderer when actually we'll get the hardware one
         while (info)
         {
                aglDescribeRenderer(info, AGL_ACCELERATED, &dAccel);
            aglReportError ();
            if (dAccel)
               canAccel = true;
            info = aglNextRendererInfo(info);
            aglReportError ();
            inum++;
         }
            
         info = head_info;
         inum = 0;
         while (info)
         {   
            aglDescribeRenderer(info, AGL_ACCELERATED, &dAccel);
            aglReportError ();
            // if we can accel then we will choose the accelerated renderer 
            // how about compliant renderers???
            if ((canAccel && dAccel) || (!canAccel && (!fAccelMust || dAccel)))
            {
                    aglDescribeRenderer(info, AGL_VIDEO_MEMORY, &dVRAM);   // we assume that VRAM returned is total thus add texture and VRAM required
               aglReportError ();
               if (dVRAM >= (*pVRAM + *pTextureRAM))
               {
                  if (dVRAM >= dMaxVRAM) // find card with max VRAM
                  {
                     aglDescribeRenderer(info, AGL_DEPTH_MODES, pDepthSizeSupport);   // which depth buffer modes are supported
                     aglReportError ();
                     dMaxVRAM = dVRAM; // store max
                     found = true;
                  }
               }
            }
            info = aglNextRendererInfo(info);
            aglReportError ();
            inum++;
         }
      }
      aglDestroyRendererInfo(head_info);
      if (found) // if we found a card that has enough VRAM and meets the accel criteria
      {
         if (MinVRAM > dMaxVRAM)
            MinVRAM = dMaxVRAM; // return VRAM
         
      }
      else
         goodCheck = false; // one device failed thus entire requirement fails
      hGD = GetNextDevice (hGD); // get next device
   } // while
   if (goodCheck) // we check all devices and each was good
   {
      *pVRAM = MinVRAM; // return VRAM
      return true;
   }
   return false; //at least one device failed to have mins
}

void GlOSXWindow::ReportError (const char * strError)
{
   char errMsgCStr [256];
   Str255 strErr = "\0";

   sprintf (errMsgCStr, "%s", strError);

   // out as debug string
#ifdef kVerboseErrors
#ifdef kQuake3
      ri.Printf( PRINT_ALL, errMsgCStr);
#else
      // ensure we are faded in
      if (gDSpStarted)
         DSpContext_CustomFadeGammaIn (NULL, NULL, 0);
      CStrToPStr (strErr, errMsgCStr);
      DebugStr (strErr);
#endif //  kQuake3
#endif // kVerboseErrors
}

GLenum GlOSXWindow::aglReportError () {
   GLenum err = aglGetError();
   if (AGL_NO_ERROR != err)    
      ReportError ((char *)aglErrorString(err));

   return err;
}

};
