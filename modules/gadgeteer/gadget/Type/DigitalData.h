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

#ifndef _GADGET_DIGITAL_DATA_H_
#define _GADGET_DIGITAL_DATA_H_

#include <gadget/Type/DeviceData.h>

namespace gadget 
{

/**
 * Enum for the state of the digital buttons.
 *
 * @since 2.1.5
 */
struct DigitalState
{
   enum State
   {
      OFF        = 0,   /**< Device is in the "off" state. */
      ON         = 1,   /**< Device is in the "on" state. */
      TOGGLE_ON  = 2,   /**< Device was in the "off" state and has changed to
                             "on" since the last frame. */
      TOGGLE_OFF = 3    /**< Device was in the "on" state and has changed to
                             "off" since the last frame. */
   };
};

typedef DeviceData<DigitalState::State> DigitalData;

} // namespace gadget


#endif /* _GADGET_DIGITAL_DATA_H_ */
