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

#ifndef _VJ_DISPLAY_WINDOW_H_
#define _VJ_DISPLAY_WINDOW_H_

#include <vrj/vrjConfig.h>
#include <vector>
#include <jccl/Config/ConfigElementPtr.h>
#include <vpr/Util/Assert.h>


namespace vrj
{
   class Viewport;

/**
 * Container class for viewports and window information.
 * Stores location of window and viewports within the window.
 *
 * @date 3-5-2001
 */
class VJ_CLASS_API Display
{
public:
   Display()
      : mOriginX(-1)
      , mOriginY(-1)
      , mWidth(-1)
      , mHeight(-1)
      , mBorder(true)
      , mHideMouse(false)
      , mPipe(-1)
      , mActive(true)
      , mStereoRequested(false)
   {
      ;
   }

   virtual ~Display()
   {;}

public:
   /**
    * Takes a display config element and configures the display based one it.
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

   /** Updates the projection data for each contained viewport.
   * @param positionScale - Scale value for converting from Juggler units (meters) to the display units
   */
   void updateProjections(const float positionScale);

public:
   bool isActive() const
   {
      return mActive;
   }

   void setName(std::string name)
   {
      mName = name;
   }

   /** Gets the name of this display. */
   std::string getName() const
   {
      return mName;
   }

   bool shouldDrawBorder() const
   {
      return mBorder;
   }

   bool shouldHideMouse() const
   {
      return mHideMouse;
   }

   /** Explicitly set the origin and size 
   * @param updateConfig - If true, then the changes will be reflected in the config element for this display.
   */
   void setOriginAndSize(const int originX, const int originY, const int width,
                         const int height, const bool updateConfig = false);
   
   /** Return the current origin and size */
   void getOriginAndSize(int& originX, int& originY, int& width, int& height)
   {
      vprASSERT(mOriginX != -1);     // Make sure we have been configured
      originX = mOriginX;
      originY = mOriginY;
      width   = mWidth;
      height  = mHeight;
   }

   void setPipe(const int pipe)
   {
      mPipe = pipe;
   }

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

   jccl::ConfigElementPtr getGlFrameBufferConfig() const;

   friend VJ_API(std::ostream&) operator<<(std::ostream& out, vrj::Display& disp);

   // --- Viewport handling --- //
   std::vector<vrj::Viewport*>::size_type getNumViewports() const
   {
      return mViewports.size();
   }

   vrj::Viewport* getViewport(const int vpNum)
   {
      return mViewports[vpNum];
   }

protected:
   std::string mName;         /**< Name of the window */
   int  mOriginX;             /**< X coordinate of the window origin. */
   int  mOriginY;             /**< Y coordinate of the window origin. */
   int  mWidth;               /**< Window width. */
   int  mHeight;              /**< Window height. */
   bool mBorder;              /**< Should we have a border? */
   bool mHideMouse;           /**< Should we hide the mouse pointer? */
   int  mPipe;                /**< Hardware pipe. Index of the rendering hardware. */
   bool mActive;              /**< Is the display active or not? */
   bool mStereoRequested;     /**< Has stereo been requested? */

   jccl::ConfigElementPtr mDisplayElement;  /**< The config data for this display */

   std::vector<vrj::Viewport*> mViewports;  /**<  Contained viewports */
};

VJ_API(std::ostream&) operator<<(std::ostream& out, vrj::Display& disp);

} // end namespace


#endif
