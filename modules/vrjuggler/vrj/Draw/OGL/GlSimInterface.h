/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#ifndef _VRJ_GL_SIM_INTERFACE_H_
#define _VRJ_GL_SIM_INTERFACE_H_

#include <vrj/vrjConfig.h>

#include <gadget/Type/KeyboardMouseProxy.h>
#include <gadget/Type/DeviceInterface.h>

#include <vrj/Draw/DrawSimInterface.h>

namespace vrj
{
   /** \class GlSimInterface GlSimInterface.h vrj/Draw/OGL/GlSimInterface.h
    *
    * Interface for objects that wish to perform simulator function with an
    * OpenGL application.
    */
   class GlSimInterface : public DrawSimInterface
   {
   public:
      virtual ~GlSimInterface()
      {
         /* Do nothing. */ ;
      }

      /**
       * Draws this sim device using the given information about the Window it
       * will be drawing into.
       */
      virtual void draw(const float scaleFactor) = 0;

      /**
       * Sets the keyboard/mouse device the simulator can use to get input from
       * the user.
       */
      virtual void setKeyboardMouse(gadget::KeyboardMouseInterface kmInterface) = 0;
   };
}

#endif
