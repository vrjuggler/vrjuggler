/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VJ_D3D_WINDOW_H_
#define _VJ_D3D_WINDOW_H_

#include <vrj/Draw/Direct3D/Config.h>
#include <vector>

#include <stdio.h>
#include <vpr/vpr.h>

#include <vrj/Display/Display.h>
#include <vrj/Display/ProjectionPtr.h>
#include <vrj/Display/Viewport.h>
#include <gadget/Devices/KeyboardMouseDevice/InputAreaWin32.h>

#include <vpr/Util/Debug.h>
#include <d3d9.h>
#include <d3dx9.h>


namespace vrj
{

namespace direct3d
{

// NOTE: Win32 message processing works like the following:
//
// - GlDrawManager calls checkEvents()
// - While there are events
//   - Translate the event
//   - If we have a valid KeyboardMouseDevice
//     - Handle the input from the window.
//   - Dispatch the message to WndPrc
//   - If WndProc can find vrj::direct3d::Window
//     - Send message to vrj::direct3d::Window
//       - Process all context specific stuff
//   - Else DefWindowProc()
//
// Note: We have to have a static registry since Win32 message
//       processing send messages to a static method of a class
//       and to handle certain messages like PAINT we need to
//       have context information contained in each instance
//       of vrj::direct3d::Window.
// TODO: Try to find a way to process PAINT event at the same
//       time as input events.

/** \class vrj::direct3d::Window Window.h vrj/Draw/Direct3D/Window.h
 *
 * An interface to Direct3D windows. This interface is used by the Direct3D
 * Draw Manager in order to keep all platform-specific code in this one class.
 *
 * @since 2.1.16
 *
 * @note This class was renamed from vrj::D3dWindow in VR Juggler 2.3.11.
 */
class VJ_D3D_CLASS_API Window
   : public gadget::InputAreaWin32
{
public:
   Window()
      : mSwapCount(0)
      // The context is always dirty when the window is first created
      , mDirtyContext(true)
      , mDirtyViewport(true)
      , mInStereo(false)
      , mHasBorder(false)
      , mIsFullScreen(false)
      , mAlwaysOnTop(false)
      , mHideMouse(false)
      , mWindowIsOpen(false)
      , mWindowWidth(0)
      , mWindowHeight(0)
      , mOriginX(0)
      , mOriginY(0)
      // XXX: Sync problem on window id value assignment
      , mWindowId(getNextWindowId())
      , mIsEventSource(false)
      , mRenderDevice(NULL)
      , mDirect3D(NULL)
      //, mBlocking(false)
   {
      mBlocking = false;
   }

   /** Virtual destructor. */
   virtual ~Window()
   {
      this->close();
   }

public:

   /**
    * Opens the OpenGL window.
    *
    * @pre this has been configured.
    * @post Window created.  Create message sets the visual.
    *
    * @return true if the window opened correctly.
    */
   virtual bool open();

   /**
    * Closes the OpenGL window.
    *
    * @return true if the window closed correctly.
    * @note Must be called by the same thread that called open.
    */
   virtual bool close();

   /**
    * Sets the current OpenGL context to this window.
    * @post this.context is active context.
    */
   virtual bool makeCurrent();

   /**
    * Checks for window events.
    * @post All win32 events have ben dispatched and dealt with.
    */
   virtual void checkEvents();

   /**
    * Configures the window.
    * @post this is configured based on the data in display.
    */
   virtual void configWindow(vrj::DisplayPtr displayWindow);

   /**
    * Performs an Direct3D swap buffers command.
    * @post A glFlush must be called explicitly by the implementation
    *       or explicitly by the functions used in the implementation.
    */
   virtual void swapBuffers();

   /**
    * Completes any setup that is needed after open.
    * @pre Window is open.
    */
   virtual void finishSetup();

public:
   /** Sets the projection matrix for this window to proj. */
   void setProjection(vrj::ProjectionPtr proj);

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
    * Sets the viewport in the D3D window based on float values.
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

   vrj::DisplayPtr getDisplay()
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

   vpr::Uint64 getSwapCount() const
   {
      return mSwapCount;
   }

   friend std::ostream& operator<<(std::ostream& out,
                                   vrj::direct3d::Window& win);

private:
   // WindowProcedure to deal with the events generated.
   // Called only for the window that we are controlling
   LRESULT handleEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

   /**
    * Sets the pixel format for the given display context.
    * @return success
    */
   bool setPixelFormat(HDC hDC);

   void sizeChanged(long width, long height);

private:
   // Register the window class with windows
   static bool registerWindowClass();
   static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                                   LPARAM lParam);

   static WNDCLASS mWinClass;            /**< The window class to register */
   static bool     mWinRegisteredClass;  /**< Have we already registered */

   // Keep a map of window handles to ogl windows
   // This can be used by the message handler to call a class specific
   // message handler for each window

   /** Adds a window to the map. */
   static void addWindow(HWND handle, vrj::direct3d::Window* d3dWin);

   /** Removes a window from the map. */
   static void removeWindow(HWND handle);

   /** Gets the glWin we are dealing with. */
   static vrj::direct3d::Window* getD3dWin(HWND handle);

   static std::map<HWND, vrj::direct3d::Window*> mD3dWinMap;

   /**
    * We store a pointer to the display that we're created from in order to
    * config and to get the viewing transforms from.
    */
   vrj::DisplayPtr mVrjDisplay;

   vpr::Uint64 mSwapCount; /**< The number of times that swapBuffers() has been called. */

   bool mDirtyContext;  /**< The context is dirty.  We need to (re)initialize it next draw. */
   bool mDirtyViewport; /**< The D3D window setup (viewport, etc) is dirty and needs to be reinited. */

   /**
    * Whether the display is actually in stereo if we wanted a stereo display
    * but couldn't open it we fall back to mono, and this will be false.
    */
   bool mInStereo;

   bool mHasBorder;     /**< Do we have a border? */
   bool mIsFullScreen;  /**< Is the window full screen? */
   bool mAlwaysOnTop;   /**< Should the window always be on top? */
   bool mHideMouse;     /**< Should we hide the mouse pointer? */
   bool mWindowIsOpen;  /**< Is the window open? */
   int  mWindowWidth;
   int  mWindowHeight;
   int  mOriginX;       /**< X component of the lower left-hand corner of the window. */
   int  mOriginY;       /**< Y component of the lower left-hand corner of the window. */
   int  mWindowId;      /**< A unique window id to identify us. */
   bool mIsEventSource; /**< Should we act as an event source too? */

public:
   LPDIRECT3DDEVICE9    mRenderDevice;

private:
   LPDIRECT3D9          mDirect3D;

   std::string  mWindowName;
   int          mPipe;
   std::string  mXDisplayName;       /**<  Name of the X11 display to use */

   static vpr::Mutex mWinIdMutex;
   static int        mCurMaxWinId;  /**< The current maximum window id */

   static int getNextWindowId();
};

// ostream& operator<<(ostream& out, vrj::direct3d::Window& win);

} // end direct3d namespace

} // end vrj namespace


#endif
