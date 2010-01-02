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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _SWAP_SYNC_TEST_MODE_H_
#define _SWAP_SYNC_TEST_MODE_H_

#include <TestMode.h>
#include <gmtl/Vec.h>
#include <vector>
#include <vrj/Draw/OpenGL/ContextData.h>
#include <Text.h>

class WallTest;

class SwapSyncTestMode : public TestMode
{
public:
   SwapSyncTestMode();
   
   virtual std::string getName()
   {
      return std::string("SwapSyncTestMode");
   }

   virtual void update(WallTest*);

   virtual void bufferPreDraw(WallTest* wallTest);

   virtual void draw(WallTest* wallTest);
   
protected:
   std::vector<gmtl::Vec4f>  mLeftBufferColors;
   std::vector<gmtl::Vec4f>  mRightBufferColors;
   unsigned                  mFrameNum;
   vrj::opengl::ContextData<Text> mText; /**< Context specific copy of text. */
};


#endif
