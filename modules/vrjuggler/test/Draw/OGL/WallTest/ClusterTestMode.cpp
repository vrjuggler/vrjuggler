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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date: 2006-02-08 13:44:09 -0600 (Wed, 08 Feb 2006) $
 * Version:       $Revision: 18378 $
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <cluster/ClusterManager.h>

#include <vrj/Display/DisplayManager.h>
#include <vrj/Display/Display.h>
#include <vrj/Draw/OGL/GlDrawManager.h>

#include <WallTestHelpers.h>
#include <ClusterTestMode.h>

#include <gmtl/Output.h>

#include <sstream>


void ClusterTestMode::draw(WallTest* wallTest)
{
   vrj::GlUserData* user_data = vrj::GlDrawManager::instance()->currentUserData();
   vrj::GlWindow* window = user_data->getGlWindow();
   gmtl::Matrix wand_matrix = wallTest->mWand->getData();
   gmtl::Vec3f wand_pos = gmtl::makeTrans<gmtl::Vec3f>(wand_matrix);

   std::stringstream display_text;
   display_text << "Wand Position: " << wand_pos << std::endl;
   display_text << "Wand Timestamp: " << wallTest->mWand->getTimeStamp().getBaseVal() << std::endl;
   display_text << "Sleep Time: " << mSleepTime << std::endl;
   display_text << "Swap Count: " << window->getSwapCount() << std::endl;
   display_test << cluster::ClusterManager::instance();

 
   glColor(1.0f, 1.0f, 1.0f);
   mText->(display_text.str(), 0.4f, 0.4f);
}

void ClusterTestMode::update(WallTest* wallTest)
{
   if (wallTest->mButton1->getData() == gadget::Digital::TOGGLE_ON)
   {
      mSleepTime += 0.25;
   }
   if (wallTest->mButton1->getData() == gadget::Digital::TOGGLE_ON &&
       mSleepTime >= 0.25)
   {
      mSleepTime -= 0.25;
   }

   vpr::System::sleep(mSleepTime);
}
