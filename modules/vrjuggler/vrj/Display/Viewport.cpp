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

#include <vrj/vrjConfig.h>

#include <iomanip>

#include <jccl/Config/ConfigElement.h>

#include <vrj/Kernel/Kernel.h>
#include <vrj/Kernel/User.h>
#include <vrj/Util/Debug.h>
#include <vrj/Display/Display.h>
#include <vrj/Display/Projection.h>
#include <vrj/Display/Viewport.h>


namespace vrj
{

Viewport::Viewport()
   : mType(Viewport::UNDEFINED)
   , mActive(false)
   , mXorigin(-1.0f)
   , mYorigin(-1.0f)
   , mXsize(-1.0f)
   , mYsize(-1.0f)
{
   ;
}

bool Viewport::config(jccl::ConfigElementPtr element)
{
   vprASSERT(element.get() != NULL);

   bool result(true);

   // -- Get config info from element -- //
    float originX = element->getProperty<float>("origin", 0);
    float originY = element->getProperty<float>("origin", 1);
    float sizeX   = element->getProperty<float>("size", 0);
    float sizeY   = element->getProperty<float>("size", 1);
    std::string name  = element->getName();
    mView    = (Viewport::View) element->getProperty<int>("view");
    mActive  = element->getProperty<bool>("active");

   // -- Check for error in configuration -- //
   // NOTE: If there are errors, set them to some default value
   if(sizeX <= 0)
   {
      vprDEBUG(vrjDBG_DISP_MGR, vprDBG_WARNING_LVL)
         << "WARNING: viewport sizeX set to: " << sizeX
         << ".  Setting to 1." << std::endl << vprDEBUG_FLUSH;
      sizeX = 1.0f;
   }

   if(sizeY <= 0)
   {
      vprDEBUG(vrjDBG_DISP_MGR, vprDBG_WARNING_LVL)
         << "WARNING: viewport sizeY set to: " << sizeY
         << ".  Setting to 1." << std::endl << vprDEBUG_FLUSH;
      sizeY = 1.0f;
   }

   // -- Set local window attributes --- //
   setOriginAndSize(originX, originY, sizeX, sizeY);

   // Get the user for this display
   std::string user_name = element->getProperty<std::string>("user");
   mUser = Kernel::instance()->getUser(user_name);

   if(NULL == mUser.get())
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED, "ERROR:") << " User not found named: '"
         << user_name << "'" << std::endl << vprDEBUG_FLUSH;
      vprASSERT(false && "User not found in Viewport::config");
      result = false;
   }
   else
   {
      setName(name);
      mViewportElement = element;        // Save the element for later use

      std::string bufname = "Head Latency " + name;
   }

   return result;
}

std::ostream& operator<<(std::ostream& out, Viewport& viewport)
{
   return viewport.outStream(out);
}

std::ostream& Viewport::outStream(std::ostream& out,
                                  const unsigned int indentLevel)
{
   const int pad_width_dot(20 - indentLevel);
   const std::string indent_text(indentLevel, ' ');

   out << indent_text << std::setw(pad_width_dot)
       << "Name " << " " << getName() << std::endl;
   out << indent_text << std::setw(pad_width_dot)
       << "Active " << " " << (mActive ? "Yes" : "No") << std::endl;
   out << indent_text << std::setw(pad_width_dot)
       << "User " << " " << getUser()->getName() << std::endl;
   out << indent_text << std::setw(pad_width_dot)
       << "Origin " << " " << mXorigin << ", " << mYorigin << std::endl;
   out << indent_text << std::setw(pad_width_dot)
       << "Size " << " " << mXsize << "x" << mYsize << std::endl;
   out << indent_text << std::setw(pad_width_dot)
       << "View " << " "
       << ((mView == Viewport::LEFT_EYE) ? "Left" : ((mView==Viewport::RIGHT_EYE)?"Right" : "Stereo") );

   out << std::flush;

   return out;
}

}
