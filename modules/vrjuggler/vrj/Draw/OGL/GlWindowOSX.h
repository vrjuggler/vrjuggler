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


#ifndef _VJ_GLOSX_WIN_H
#define _VJ_GLOSX_WIN_H
//#pragma once

#include <vjConfig.h>

#include <Kernel/GL/vjGlWindow.h>
#include <Kernel/vjKernel.h>
#include <Utils/vjDebug.h>
#include <Kernel/vjDisplay.h>

#include <Carbon/Carbon.h>
#include <SetupGL/Carbon SetupGL.h>
#include <SetupGL/Carbon Include.h>
#include <AGL/gl.h>

//------------------------------------
//: A GLOSX specific glWindow
//------------------------------------
// Has all information specific
// to dealing with a GLOSX window
// in OpenGL
//------------------------------------
class vjGlOSXWindow: public vjGlWindow
{
public:
    vjGlOSXWindow();
    ~vjGlOSXWindow();

    void swapBuffers();
    int open();
    int close();
    bool makeCurrent();

   void config(vjDisplay* _display);

public:  /**** Static Helpers *****/
   /* static */ virtual bool createHardwareSwapGroup(std::vector<vjGlWindow*> wins);

protected:

private:
    int          		mPipe;
    structGLWindowInfo		glInfo;
    AGLContext			aglContext;
    Rect 			rectWin;
    
    WindowPtr 			gpWindow;
    long			gFrameWindow;
    AbsoluteTime		gTimeWindow;
    float			gRotation;
    CFStringRef			window_title;
};

#endif
