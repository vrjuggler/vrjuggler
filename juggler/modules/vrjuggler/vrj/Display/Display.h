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
#include <jccl/Config/ConfigChunkPtr.h>
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
   Display() : mBorder(true), mPipe(-1), mActive(true), mStereoRequested(false)
   {
      _xo = _yo = _xs = _ys = -1;
   }

   virtual ~Display()
   {;}

public:
   /**
    * Takes a display chunk and configures the display based one it.
    * @pre chunk is a valid chunk.
    * @post display is configured.
    *        If there is an error is the specified config, we output error
    *        and "fix" the error.
    *
    * @note All derived display classes MUST call this function
    *        after doing local configuration.
    */
   virtual void config(jccl::ConfigChunkPtr chunk);

   void configDisplayWindow(jccl::ConfigChunkPtr chunk);
   void configViewports(jccl::ConfigChunkPtr chunk);

   /** Updates the projection data for each contained viewport.
   * @param positionScale - Scale value for converting from Juggler units (meters) to the display units
   */
   void updateProjections(const float positionScale);

public:
   bool isActive()
   { return mActive; }

   void setName(std::string name)
   { mName = name; }

   /** Gets the name of this display. */
   std::string getName()
   { return mName;}

   bool  shouldDrawBorder()
   { return mBorder;}

   void setOriginAndSize(int xo, int yo, int xs, int ys)
   { _xo = xo; _yo = yo; _xs = xs; _ys = ys;}
   void getOriginAndSize(int& xo, int& yo, int& xs, int& ys)
   {
      vprASSERT(_xo != -1);     // Make sure we have been configured
      xo = _xo; yo = _yo; xs = _xs; ys = _ys;
   }

   void setPipe(int pipe)
   { mPipe = pipe; }
   int getPipe()
   { return mPipe; }

   /**
    * Indicates whether stereo rendering has been requested for this display.
    *
    * @note If we are in simulator, we can not be in stereo.
    */
   bool isStereoRequested()
   {
      return mStereoRequested;
   }

   /** Gets the config chunk that configured this display window. */
   jccl::ConfigChunkPtr getConfigChunk()
   { return mDisplayChunk; }

   jccl::ConfigChunkPtr getGlFrameBufferConfig();

   friend VJ_API(std::ostream&) operator<<(std::ostream& out, vrj::Display& disp);

   // --- Viewport handling --- //
   unsigned getNumViewports()
   { return mViewports.size(); }

   vrj::Viewport* getViewport(int vpNum)
   { return mViewports[vpNum]; }

protected:
   std::string    mName;                /**< Name of the window */
   int            _xo, _yo, _xs, _ys;   /**< X and Y origin and size of the view */
   bool           mBorder;              /**< Should we have a border */
   int            mPipe;                /**< Hardware pipe. Index of the rendering hardware */
   bool           mActive;              /**< Is the display active or not? */
   bool           mStereoRequested;     /**< Has stereo been requested? */
   jccl::ConfigChunkPtr mDisplayChunk;  /**< The chunk data for this display */

   std::vector<vrj::Viewport*>   mViewports;    /**<  Contained viewports */
};

VJ_API(std::ostream&) operator<<(std::ostream& out, vrj::Display& disp);

} // end namespace

#endif
