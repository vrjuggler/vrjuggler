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

#ifndef _VJ_GL_WINDOW_H_
#define _VJ_GL_WINDOW_H_

#include <vrj/vrjConfig.h>
#include <vector>

#include <stdio.h>
#include <vpr/vpr.h>

#ifdef VPR_OS_Darwin
#   include <OpenGL/gl.h>
#else
#   include <GL/gl.h>
#endif

#include <vrj/Display/Display.h>
#include <vrj/Display/Viewport.h>

#include <vpr/Util/Debug.h>

#include <boost/concept_check.hpp>


namespace vrj
{

class Projection;
class CameraProjection;

/** \class GlWindow GlWindow.h vrj/Draw/OGL/GlWindow.h
 *
 * Represent cross-platform interface to OpenGL windows.  This interface is
 * used by the OpenGL Draw Manager in order to keep all platform-specific code
 * in this one class.
 */
class VJ_CLASS_API GlWindow
{
public:
   GlWindow()
      : mVrjDisplay(NULL)
      // The context is always dirty when the window is first created
      , mDirtyContext(true)
      , mDirtyViewport(true)
      , mInStereo(false)
      , mHasBorder(false)
      , mHideMouse(false)
      , mWindowIsOpen(false)
      , mWindowWidth(0)
      , mWindowHeight(0)
      , mOriginX(0)
      , mOriginY(0)
      // XXX: Sync problem on window id value assignment
      , mWindowId(getNextWindowId())
      , mIsEventSource(false)
   {
      /* Do nothing. */ ;
   }

   /** Virtual destructor. */
   virtual ~GlWindow()
   {
      /* Do nothing. */ ;
   }

public:

   /**
    * Opens the OpenGL window.
    * @pre this has been configured.
    */
   virtual bool open()
   {
      return 1;
   }

   /** Closes the OpenGL window. */
   virtual bool close()
   {
      return 1;
   }

   /**
    * Sets the current OpenGL context to this window.
    * @post this.context is active context.
    */
   virtual bool makeCurrent()
   {
      return false;
   }

   /**
    * Configures the window.
    * @post this is configured based on the data in display.
    */
   virtual void configWindow(vrj::Display* displayWindow);

   /**
    * Performs an OpenGL swap buffers command.
    * @post A glFlush must be called explicitly by the implementation
    *       or explicitly by the functions used in the implementation.
    */
   virtual void swapBuffers()
   {;}

   /**
    * Handles any window events that have occured.
    */
   virtual void checkEvents()
   {;}

   /**
    * Completes any setup that is needed after open.
    * @pre Window is open.
    */
   virtual void finishSetup();

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
   {
      return mDirtyContext;
   }

   /** Sets the dirty bit for the context. */
   void setDirtyContext(bool val=true)
   {
      mDirtyContext = val;
   }

   /** Is the viewport dirty? */
   bool hasDirtyViewport() const
   {
      return mDirtyViewport;
   }

   /** Sets the dirty bit for the viewport. */
   void setDirtyViewport(bool val=true)
   {
      mDirtyViewport = val;
   }

   /**
    * Queries wether the window is open.
    * @return true if window is open.
    */
   bool isOpen() const
   {
      return mWindowIsOpen;
   }

   /**
    * Queries wether the window is in stereo.
    * @return true if window is in stereo.
    */
   bool isStereo() const
   {
      return mInStereo;
   }

   bool isEventSource() const
   {
      return mIsEventSource;
   }

   vrj::Display* getDisplay()
   {
      return mVrjDisplay;
   }

   /**
    * @return A unique window id.
    */
   int getId() const
   {
      return mWindowId;
   }

   /** Called by event function to update size info. */
   // XXX: Should update Display configuration element in some way.
   void updateOriginSize(const int originX, const int originY, const int width,
                         const int height)
   {
      mOriginX      = originX;
      mOriginY      = originY;
      mWindowWidth  = width;
      mWindowHeight = height;

      // Update the display configuration
      mVrjDisplay->setOriginAndSize(originX, originY, width, height);
   }

   /** Returns the origin and size of the current window. */
   void getOriginSize(int& originX, int& originY, int& width,
                      int& height) const
   {
      originX = mOriginX;
      originY = mOriginY;
      width   = mWindowWidth;
      height  = mWindowHeight;
   }

   friend std::ostream& operator<<(std::ostream& out, GlWindow& win);

public:
   virtual bool createHardwareSwapGroup(const std::vector<GlWindow*>& wins)
   {
      boost::ignore_unused_variable_warning(wins);
      vprDEBUG(vprDBG_ALL,vprDBG_WARNING_LVL)
         << "WARNING: hardware swap not supported.\n" << vprDEBUG_FLUSH;
      return false;
   }

protected:
   /**
    * We store a pointer to the display that we're created from in order to
    * config and to get the viewing transforms from.
    */
   vrj::Display* mVrjDisplay;

   bool mDirtyContext;  /**< The context is dirty.  We need to (re)initialize it next draw. */
   bool mDirtyViewport; /**< The GL window setup (viewport, etc) is dirty and needs to be reinited. */

   /**
    * Whether the display is actually in stereo if we wanted a stereo display
    * but couldn't open it we fall back to mono, and this will be false.
    */
   bool mInStereo;

   bool mHasBorder;     /**< Do we have a border? */
   bool mHideMouse;     /**< Should we hide the mouse pointer? */
   bool mWindowIsOpen;  /**< Is the window open? */
   int  mWindowWidth;
   int  mWindowHeight;
   int  mOriginX;       /**< X component of the lower left-hand corner of the window. */
   int  mOriginY;       /**< Y component of the lower left-hand corner of the window. */
   int  mWindowId;      /**< A unique window id to identify us. */
   bool mIsEventSource; /**< Should we act as an event source too? */

private:
   static vpr::Mutex mWinIdMutex;
   static int        mCurMaxWinId;  /**< The current maximum window id */

   static int getNextWindowId();
};

// ostream& operator<<(ostream& out, GlWindow& win);

} // end namespace

#endif