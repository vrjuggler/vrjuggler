/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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

#if defined(WIN32) || defined(WIN64)
#  include <windows.h>
#endif

#include <sstream>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <vrj/Draw/OpenGL/DrawManager.h>
#include <vrj/Display/DisplayManager.h>
#include <vrj/Display/SurfaceViewport.h>
#include <vrj/Display/Display.h>

#include <WallTest.h>
#include <WallTestHelpers.h>

#include <DisplayInfoMode.h>


void DisplayInfoMode::draw(WallTest*)
{
   DisplayManager* displayManager =
      vrj::opengl::DrawManager::instance()->getDisplayManager();
   vrj::opengl::UserData* user_data =
      vrj::opengl::DrawManager::instance()->currentUserData();
   vrj::DisplayPtr display          = user_data->getViewport()->getDisplay();
   vrj::ViewportPtr viewport        = user_data->getViewport();
   vrj::ProjectionPtr proj          = user_data->getProjection();
   int cur_eye                      = proj->getEye();

   std::stringstream display_text;

   display_text << "Display Info\n"
                << "   Display: " << display->getName() << std::endl
                << "      pipe: " << display->getPipe() << std::endl;

   if ( vrj::Projection::LEFT == cur_eye )
   {
      display_text << "   Eye: Left" << std::endl;
   }
   else
   {
      display_text << "   Eye: Right" << std::endl;
   }

   display_text << "  Viewport: " << std::endl;
   viewport->outStream(display_text, 6);

   display_text << std::endl;

   std::string text = display_text.str();

   glColor3f(1.0f, 1.0f, 0.0f);
   mText->drawTextBlock(text, 0.4f, 0.4f);
}
