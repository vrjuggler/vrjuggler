/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#include <cluster/ClusterManager.h>

#include <gadget/Type/Digital.h>
#include <vrj/Display/DisplayManager.h>
#include <vrj/Display/Display.h>
#include <vrj/Draw/OGL/GlDrawManager.h>
#include <vrj/Draw/OGL/GlWindow.h>

#include <WallTest.h>
#include <WallTestHelpers.h>
#include <ClusterTestMode.h>

#include <gmtl/Output.h>

#include <sstream>


void ClusterTestMode::draw(WallTest* wallTest)
{
   vrj::GlUserData* user_data = vrj::GlDrawManager::instance()->currentUserData();
   vrj::GlWindow* window = user_data->getGlWindow();
   gmtl::Matrix44f wand_matrix = wallTest->mWand->getData();
   gmtl::Vec3f wand_pos = gmtl::makeTrans<gmtl::Vec3f>(wand_matrix);

   std::stringstream display_text;
   display_text << "Wand Position: " << wand_pos << std::endl;
   display_text << "Wand Timestamp: " << wallTest->mWand->getTimeStamp().getBaseVal() << std::endl;
   display_text << "Sleep Time: " << mSleepTime << " ms" << std::endl;
   display_text << "Swap Count: " << window->getSwapCount() << std::endl;
   display_text << *cluster::ClusterManager::instance();

   int origin_x, origin_y, width, height;
   window->getOriginSize(origin_x, origin_y, width, height);

   glColor3f(1.0f, 1.0f, 1.0f);
   height *= 1.5;
   mText->drawTextBlock(display_text.str(), 0.4f, 0.4f, 10.0f, height, height*1.4);
}

void ClusterTestMode::update(WallTest* wallTest)
{
   if (wallTest->mButton1->getData() == gadget::Digital::TOGGLE_ON)
   {
      mSleepTime += 25;
   }
   if (wallTest->mButton1->getData() == gadget::Digital::TOGGLE_ON &&
       mSleepTime >=25)
   {
      mSleepTime -= 25;
   }

   vpr::System::msleep(mSleepTime);
}
