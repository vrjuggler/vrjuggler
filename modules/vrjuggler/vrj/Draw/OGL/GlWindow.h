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

#ifndef _VJ_GL_WINDOW_H_
#define _VJ_GL_WINDOW_H_

#include <vrj/vrjConfig.h>

#include <stdio.h>
#include <vpr/vpr.h>

#ifdef VPR_OS_Darwin
#   include <OpenGL/gl.h>
#else
#   include <GL/gl.h>
#endif

#include <vrj/Display/Display.h>
#include <vrj/Display/Viewport.h>


namespace vrj
{

class Projection;
class CameraProjection;

/**
 * Represent cross-platform interface to OpenGL windows.
 * This interface is used by the OpenGL draw manager
 * in order to keep all platform specific code in this
 * one class.
 */
class VJ_CLASS_API GlWindow
{
public:
   GlWindow()
   {
      // XXX: Sync problem on window id value assignment
      mWindowId = getNextWindowId();
      in_stereo = false;
      border = false;
      window_is_open = false;
      mDirtyContext = true;      // Always dirty when window first created
      mDirtyViewport = true;

      mDisplay = NULL;

      window_width = window_height = 0;
      origin_x = origin_y = 0;
   }

   // Cirtual destructor
   virtual ~GlWindow()
   {;}

public:

   /**
    * Opens the OpenGL window.
    * @pre this has been configured.
    */
   virtual int open(){ return 1;}

   /** Closes the OpenGL window. */
   virtual int close(){return 1;}

   /**
    * Sets the current OpenGL context to this window.
    * @post this.context is active context.
    */
   virtual bool makeCurrent(){return false;}

   /**
    * Configures the window.
    * @post this is configured based on the data in display.
    */
   virtual void config(vrj::Display* displayWindow);

   /**
    * Performs an OpenGL swap buffers command.
    * @post A glFlush must be called explicitly by the implementation
    *       or explicitly by the functions used in the implementation.
    */
   virtual void swapBuffers(){;}

   /**
    * Handles any window events that have occured.
    */
   virtual void checkEvents() {;}

public:
   /** Sets the projection matrix for this window to proj. */
   void setProjection(vrj::Projection* proj);

   /**
    * Sets the projection matrix for this window to draw the left eye frame.
    * If the window is in stereo, it changes to the left buffer.
    */
   void setLeftEyeProjection();

   /**
    * Sets the projection matrix for this window to draw the right eye frame.
    * If the window is in stereo, it changes to the right buffer.
    */
   void setRightEyeProjection();

   /** Sets the view buffer for the window (issues glDrawBuffer command). */
   void setViewBuffer(vrj::Viewport::View view);

   /**
    * Sets the viewport in the GL window based on float values.
    *
    * @param xo X-coordinate for the viewport origin.
    * @param yo Y-coordinate for the viewport origin.
    * @param xSize Width of the viewport.
    * @param ySize Height of the viewport.
    *
    * @note The xo,yo values must be in the range [0.0,1.0]
    *       where lowerleft is 0,0 and upper right is 1,1.
    */
   void setViewport(float xo, float yo, float xSize, float ySize);

   /** Updates the viewport and does any viewport cleaning. */
   void updateViewport();

   /** Is the context dirty? */
   bool hasDirtyContext() const
   { return mDirtyContext; }

   /** Sets the dirty bit for the context. */
   void setDirtyContext(bool val=true)
   { mDirtyContext = val; }

   /** Is the viewport dirty? */
   bool hasDirtyViewport() const
   { return mDirtyViewport; }

   /** Sets the dirty bit for the viewport. */
   void setDirtyViewport(bool val=true)
   { mDirtyViewport = val; }

   /**
    * Queries wether the window is open.
    * @return true if window is open.
    */
   bool isOpen() const
   { return window_is_open; }

   /**
    * Queries wether the window is in stereo.
    * @return true if window is in stereo.
    */
   bool isStereo()
   { return in_stereo;}

   vrj::Display* getDisplay()
   { return mDisplay;}

   /**
    * @return A unique window id.
    */
   int getId()
   { return mWindowId; }

   // Called by event function to update size info
   // XXX: Should update Display chunk in some way
   void updateOriginSize(int o_x, int o_y, int width, int height)
   {
      origin_x = o_x; origin_y = o_y;
      window_width = width; window_height = height;
   }

   /** Return the origin and size of the current window */
   void getOriginSize(unsigned& o_x, unsigned& o_y, unsigned& width, unsigned& height) const
   {
      o_x = origin_x;
      o_y = origin_y;
      width = window_width;
      height = window_height;
   }

   friend std::ostream& operator<<(std::ostream& out, GlWindow* win);

public:  /**** Static Helpers *****/
   /* static */ virtual bool createHardwareSwapGroup(std::vector<GlWindow*> wins)
   {
      vprDEBUG(vprDBG_ALL,vprDBG_WARNING_LVL) << "WARNING: hardware swap not supported.\n" << vprDEBUG_FLUSH;
      return false;
   }

protected:
     // we store a pointer to the display that we're
     // created from, to config & to get the viewing
     // transforms from.
   vrj::Display* mDisplay;

   bool mDirtyContext;  /**<  The context is dirty.  We need to (re)initialize it next draw */
   bool mDirtyViewport; /**  The GL window setup (viewport, etc) is dirty and needs to be reinited. */

   /**
    * Wether the display is actually in stereo if we wanted a stereo display
    * but couldn't open it we fall back to mono, and this will be false.
    */
   bool in_stereo;

   bool  border;        /**<  Do we have a border? */
   bool window_is_open; /**< Is the window open? */
   int  window_width, window_height;
   int  origin_x, origin_y;         /**< lower-left corner of window */
   int  mWindowId;                  /**< A unique window id to identify us */

private:
   static vpr::Mutex mWinIdMutex;
   static int        mCurMaxWinId;  /**< The current maximum window id */

   static int getNextWindowId();
};

// ostream& operator<<(ostream& out, GlWindow& win);


} // end namespace
#endif
