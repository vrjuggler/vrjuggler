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

#ifndef _VRJ_GL_WIN_OSX_H_
#define _VRJ_GL_WIN_OSX_H_

#include <vrj/vrjConfig.h>

#include <vrj/Draw/OGL/GlWindow.h>
#include <vrj/Kernel/Kernel.h>
#include <vrj/Util/Debug.h>
#include <vrj/Display/Display.h>
#include <gadget/Devices/Keyboard/KeyboardOSX.h>

#ifdef __APPLE_CC__
#   include <Carbon/Carbon.h>
#   include <DrawSprocket/DrawSprocket.h>
#   include <AGL/agl.h>
#else
#   include <Carbon.h>
#   include <DrawSprocket.h>
#   include <agl.h>
#endif

/**
 * Structure for creating a context from a window.
 * This structure comes from Carbon SetupGL 1.5 distributed by Apple
 * Corporation.  Its use is here is permitted by the license.
 */
struct structGLWindowInfo {
   Boolean fAcceleratedMust;    // input: must renderer be accelerated?
   GLint aglAttributes[64];     // input: pixel format attributes always
                                //        required (reset to what was actually
                //        allocated)
   long VRAM;           // input: minimum VRAM; output: actual (if
                                //        successful otherwise input)
   long textureRAM;     // input: amount of texture RAM required on
                                //        card; output: same (used in
                //        allcoation to ensure enough texture
   AGLPixelFormat   fmt;    // input: none; output pixel format...
   Boolean fDraggable;      // input: is window going to be dragable,
                //        * if so renderer check (accel, VRAM,
                //          textureRAM) will look at all
                //          renderers vice just the current one
                //        * if window is not dragable renderer
                //          check will either check the single
                //          device or short circuit to software
                //        * if window spans multiple devices
                //          software renderer is consider to
                //          have unlimited VRAM, unlimited
                //          textureRAM and to not be
                //          accelerated
};

namespace vrj
{

/**
 * A GL specific glWindow for OS X.
 * Has all information specific to dealing with a GL window in OpenGL.
 */
class GlWindowOSX: public GlWindow, public gadget::KeyboardOSX
{
public:
    GlWindowOSX();
    ~GlWindowOSX();

    void swapBuffers();
    int open();
    int close();
    bool makeCurrent();
    
    int startSampling();
    //int stopSampling();

   void config(Display* _display);

public:  /**** Static Helpers *****/
   /* static */ virtual bool createHardwareSwapGroup(std::vector<GlWindow*> wins);

protected:
    OSStatus BuildGLFromWindow(WindowPtr pWindow, AGLContext* paglContext,
                               structGLWindowInfo* pcontextInfo);

    OSStatus BuildGLonWindow(WindowPtr pWindow, AGLContext* paglContext,
                             structGLWindowInfo* pcontextInfo);

    OSStatus DestroyGLFromWindow(AGLContext* paglContext,
                                 structGLWindowInfo* pcontextInfo);

    short FindGDHandleFromWindow(WindowPtr pWindow, GDHandle* phgdOnThisDevice);

    Boolean CheckRenderer(GDHandle hGD, long *VRAM, long *textureRAM, GLint*,
                          Boolean fAccelMust);

    Boolean CheckAllDeviceRenderers(long* pVRAM, long* pTextureRAM,
                                    GLint* pDepthSizeSupport,
                                    Boolean fAccelMust);

    void ReportError(const char* strError);
    GLenum aglReportError(void);

private:
    int                 mPipe;
    structGLWindowInfo      glInfo;
    AGLContext          aglContext;
    Rect            rectWin;

    WindowPtr           gpWindow;
    long            gFrameWindow;
    AbsoluteTime        gTimeWindow;
    float           gRotation;
    CFStringRef         window_title;
    bool           mAreKeyboardDevice;  /**< Should we act as a keyboard device too? */

    static AGLContext       aglShareContext;
};

};

#endif
