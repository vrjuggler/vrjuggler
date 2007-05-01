/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#include <vrj/Display/DisplayManager.h>
#include <vrj/Display/Display.h>
#include <vrj/Display/Projection.h>
#include <vrj/Draw/OGL/GlDrawManager.h>
#include <vpr/System.h>

#include <WallTestHelpers.h>
#include <SwapSyncTestMode.h>
#include <sstream>

SwapSyncTestMode::SwapSyncTestMode()
{
   mLeftBufferColors.push_back(gmtl::Vec4f(0.0, 0.0, 0.0, 0.0));
   mLeftBufferColors.push_back(gmtl::Vec4f(1.0, 0.0, 0.0, 0.0));
   mLeftBufferColors.push_back(gmtl::Vec4f(0.5, 0.5, 0.0, 0.0));
   mLeftBufferColors.push_back(gmtl::Vec4f(0.25, 0.25, 0.25, 0.0));

   mRightBufferColors.push_back(gmtl::Vec4f(1.0, 1.0, 1.0, 1.0));
   mRightBufferColors.push_back(gmtl::Vec4f(0.0, 0.0, 1.0, 0.0));
   mRightBufferColors.push_back(gmtl::Vec4f(0.0, 0.5, 0.5, 0.0));
   mRightBufferColors.push_back(gmtl::Vec4f(0.0, 1.0, 0.0, 0.0));  
   
   mFrameNum = 0;       
}


void SwapSyncTestMode::update(WallTest*)
{
   mFrameNum += 1;
}

void SwapSyncTestMode::bufferPreDraw(WallTest*)
{
   vrj::Projection* currentProjection =
      vrj::GlDrawManager::instance()->currentUserData()->getProjection();
   int currentEye = currentProjection->getEye();

   unsigned choice = (mFrameNum % 4);
   std::cout << "frame num: " << mFrameNum << std::endl;
   std::cout << "choice: " << choice << std::endl;
   std::cout << "eye: " << currentEye << std::endl;

   gmtl::Vec4f clear_color;
   if ( currentEye == vrj::Projection::LEFT )
   {
      clear_color = mLeftBufferColors[choice];      
   }
   else
   {
      clear_color = mRightBufferColors[choice];         
   }
   glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);

   glClear(GL_COLOR_BUFFER_BIT);
}


void SwapSyncTestMode::draw(WallTest*)
{
   vrj::Projection* currentProjection =
      vrj::GlDrawManager::instance()->currentUserData()->getProjection();
   int cur_eye = currentProjection->getEye();


   std::stringstream display_text;
   display_text << "SwapSyncTest:\n"
                << "   frame number: " << mFrameNum << std::endl
		<< "         choice: " << (mFrameNum%4) << std::endl
		<< "            eye: ";
 
   if ( vrj::Projection::LEFT == cur_eye )
   {
      display_text << "   Eye: Left" << std::endl;
   }
   else
   {
      display_text << "   Eye: Right" << std::endl;
   }

   std::string text = display_text.str();
   glColor3f(1.0, 0.0, 1.0);
   mText->drawTextBlock(text, 0.4f, 0.4f);
   
   // Sleep for one of a predetermined times.
   // - Use this instead of random because we want to generate worst case
   //   on some of these sleeps (ie. one node 0, one node 2 seconds)
   unsigned sleep_choice = rand() % 3;
   vpr::Uint32 sleep_ms;
   
   if(0 == sleep_choice) { sleep_ms = 0; }
   else if(1 == sleep_choice) { sleep_ms = 1000; }   
   else if(2 == sleep_choice) { sleep_ms = 4000; }      
   
   /*
   float rand_num = drand48();
   float base_ms = 1500.0f;         // Sleep this much each time
   float var_ms = 2000.0f;          // Variability each frame
   
   vpr::Uint32 sleep_ms = vpr::Uint32(float(base_ms + (var_ms * rand_num)));
   */
   
   //vprDEBUG(vprDBG_ALL, 0) << "Frame: " << mFrameNum << std::endl << vprDEBUG_FLUSH;;
   vpr::System::msleep(sleep_ms);
   
}

