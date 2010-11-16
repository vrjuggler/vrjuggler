/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VJ_DISPLAY_WINDOW_H_
#define _VJ_DISPLAY_WINDOW_H_

#include <vrj/vrjConfig.h>

#include <boost/enable_shared_from_this.hpp>
#include <vector>

#include <jccl/Config/ConfigElementPtr.h>
#include <vpr/Util/Assert.h>
#include <vrj/Display/DisplayPtr.h>
#include <vrj/Display/ViewportPtr.h>


namespace vrj
{

/** \class Display Display.h vrj/Display/Display.h
 *
 * Container class for viewports and window information.  This stores the
 * size and location of the window and the viewports contained within.
 *
 * @date 3-5-2001
 */
class VJ_CLASS_API Display : public boost::enable_shared_from_this<Display>
{
protected:
   Display();

public:
   /**
    * Creates a new vrj::Display object and wraps in a vrj::DisplayPtr.
    *
    * @since 2.1.20
    */
   static DisplayPtr create()
   {
      return DisplayPtr(new Display());
   }

   virtual ~Display();

   /**
    * Takes a display config element and configures the display based one it.
    *
    * @pre element is a valid configuration element.
    * @post display is configured.
    *        If there is an error is the specified config, we output error
    *        and "fix" the error.
    *
    * @note All derived display classes MUST call this function
    *        after doing local configuration.
    */
   virtual void config(jccl::ConfigElementPtr element);

   void configDisplayWindow(jccl::ConfigElementPtr element);
   void configViewports(jccl::ConfigElementPtr element);

   /**
    * Updates the projection data for each contained viewport.
    *
    * @param positionScale Scale value for converting from Juggler units
    *                      (meters) to the display units.
    */
   void updateProjections(const float positionScale);

public:
   /** Determines whether this display window is active. */
   bool isActive() const
   {
      return mActive;
   }

   /** Sets the name of this display window. */
   void setName(const std::string& name)
   {
      mName = name;
   }

   /** Gets the name of this display window. */
   const std::string& getName() const
   {
      return mName;
   }

   /**
    * Determines whether the window manager should decorate this display
    * window with a title and border.
    */
   bool shouldDrawBorder() const
   {
      return mBorder;
   }

   /**
    * Determines whether the mouse pointer should be hidden while within the
    * borders of this display window.
    */
   bool shouldHideMouse() const
   {
      return mHideMouse;
   }

   /**
    * Determines whether the window should be full screen.
    */
   bool isFullScreen() const
   {
      return mFullScreen;
   }

   /**
    * Determines whether the window should always be on top.
    */
   bool isAlwaysOnTop() const
   {
      return mAlwaysOnTop;
   }


   /**
    * Explicitly sets the origin and size of this display window.
    *
    * @param originX      The X coordinate for the origin of this display
    *                     window.
    * @param originY      The Y coordinate for the origin of this display
    *                     window.
    * @param width        The new width for this display window.
    * @param height       The new height for this display window.
    * @param updateConfig If true, then the changes will be reflected in the
    *                     config element for this display window.
    */
   void setOriginAndSize(const int originX, const int originY, const int width,
                         const int height, const bool updateConfig = false);
   
   /** Returns the current origin and size of this display window. */
   void getOriginAndSize(int& originX, int& originY, int& width, int& height)
      const
   {
      vprASSERT(mWidth != -1);     // Make sure we have been configured
      originX = mOriginX;
      originY = mOriginY;
      width   = mWidth;
      height  = mHeight;
   }

   /**
    * Sets the graphics pipe for this display window.
    */
   void setPipe(const int pipe)
   {
      mPipe = pipe;
   }

   /**
    * Gets the graphics pipe for this display window.
    */
   int getPipe() const
   {
      return mPipe;
   }

   /**
    * Indicates whether stereo rendering has been requested for this display.
    *
    * @note If we are in simulator, we can not be in stereo.
    */
   bool isStereoRequested() const
   {
      return mStereoRequested;
   }

   /** Gets the config element that configured this display window. */
   jccl::ConfigElementPtr getConfigElement() const
   {
      return mDisplayElement;
   }

   /** Gets the frame buffer configuration element for this display window. */
   jccl::ConfigElementPtr getGlFrameBufferConfig() const;

   friend VJ_API(std::ostream&) operator<<(std::ostream& out, vrj::Display& disp);

   /** @name Viewport handling */
   //@{
   /** Gets the number of viewports contained within this window. */
   std::vector<vrj::ViewportPtr>::size_type getNumViewports() const
   {
      return mViewports.size();
   }

   /**
    * Returns the requested viewport from this display window.
    *
    * @pre vpNum < getNumViewports().
    *
    * @param vpNum The integer identifier of the requested viewport.
    */
   vrj::ViewportPtr getViewport(const std::vector<vrj::ViewportPtr>::size_type vpNum)
      const
   {
      return mViewports[vpNum];
   }
   //@}

protected:
   std::string mName;         /**< Name of the window */
   int  mOriginX;             /**< X coordinate of the window origin. */
   int  mOriginY;             /**< Y coordinate of the window origin. */
   int  mWidth;               /**< Window width. */
   int  mHeight;              /**< Window height. */
   bool mBorder;              /**< Should we have a border? */
   bool mHideMouse;           /**< Should we hide the mouse pointer? */
   bool mFullScreen;          /**< Should the window be full screen? */
   bool mAlwaysOnTop;         /**< Should the window always be on top? */
   int  mPipe;                /**< Hardware pipe. Index of the rendering hardware. */
   bool mActive;              /**< Is the display active or not? */
   bool mStereoRequested;     /**< Has stereo been requested? */

   jccl::ConfigElementPtr mDisplayElement;  /**< The config data for this display */

   std::vector<vrj::ViewportPtr> mViewports;  /**<  Contained viewports */
};

VJ_API(std::ostream&) operator<<(std::ostream& out, vrj::Display& disp);

} // end namespace


#endif
