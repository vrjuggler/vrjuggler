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

#ifndef _VJ_DISPLAY_WINDOW_H_
#define _VJ_DISPLAY_WINDOW_H_

#include <vrj/vrjConfig.h>
#include <vector>
#include <vrj/Display/Viewport.h>
#include <jccl/Config/ConfigChunkPtr.h>


namespace vrj
{


//---------------------------------------------------------------------
//: Container class for viewports and window information.
//
// Stores location of window and viewports within the window.
//
// @author Allen Bierbaum
//  Date: 3-5-2001
//-----------------------------------------------------------------------
class Display
{
public:
   Display() : mDisplayChunk(NULL)
   {
      _xo = _yo = _xs = _ys = -1;
      mPipe = -1;
   }

   virtual ~Display()
   {;}

public:
      //: Takes a display chunk and configures the display based one it.
      //! PRE: chunk is a valid chunk
      //! POST: display is configured
      //+       If there is an error is the specified config, we output error
      //+       and "fix" the error.
      //! NOTE: All derived display classes MUST call this function
      //+       after doing local configuration.
   virtual void config(jccl::ConfigChunkPtr chunk);

   void configDisplayWindow(jccl::ConfigChunkPtr chunk);
   void configViewports(jccl::ConfigChunkPtr chunk);

   //: Updates the projection data for each contained viewport
   void updateProjections();

public:
   bool isActive()
   { return mActive; }

   void setName(std::string name)
   { mName = name; }

   //: Get the name of the display
   std::string getName()
   { return mName;}

   bool  shouldDrawBorder()
   { return mBorder;}

   void setOriginAndSize(int xo, int yo, int xs, int ys)
   { _xo = xo; _yo = yo; _xs = xs; _ys = ys;}
   void getOriginAndSize(int& xo, int& yo, int& xs, int& ys)
   {
      vprASSERT(xo != -1);     // Make sure we have been configured
      xo = _xo; yo = _yo; xs = _xs; ys = _ys;
   }

   void setPipe(int pipe)
   { mPipe = pipe; }
   int getPipe()
   { return mPipe; }

   //!NOTE: If we are in simulator, we can not be in stereo
   bool inStereo()
   { return mInStereo; }

   //: Get the config chunk that configured this display window
   jccl::ConfigChunkPtr getConfigChunk()
   { return mDisplayChunk; }

   friend VJ_API(std::ostream&) operator<<(std::ostream& out, vrj::Display& disp);

   // --- Viewport handling --- //
   unsigned getNumViewports()
   { return mViewports.size(); }

   vrj::Viewport* getViewport(int vpNum)
   { return mViewports[vpNum]; }

protected:
   std::string    mName;                  //: Name of the window
   int            _xo, _yo, _xs, _ys;     //: X and Y origin and size of the view
   bool           mBorder;                //: Should we have a border
   int            mPipe;                  //: Hardware pipe. Index of the rendering hardware
   bool           mActive;                //: Is the display active or not
   bool           mInStereo;              //: Is the window in stereo mode?
   jccl::ConfigChunkPtr mDisplayChunk;       //: The chunk data for this display

   std::vector<vrj::Viewport*>   mViewports;    //: Contained viewports
};

VJ_API(std::ostream&) operator<<(std::ostream& out, vrj::Display& disp);

} // end namespace
#endif
