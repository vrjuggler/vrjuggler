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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_KEYBOARD_MOUSE_SAMPLE_HANDLER_H_
#define _GADGET_KEYBOARD_MOUSE_SAMPLE_HANDLER_H_

#include <gadget/gadgetConfig.h>

#include <gadget/Event/EventPtr.h>


namespace gadget
{

namespace event
{

namespace kbd
{

/** \struct KeyboardEventSampleHandler KeyboardEventSampleHandler.h gadget/Event/KeyboardEventSampleHandler.h
 *
 * A handler of samples collected by gadget::KeyboardMouse instances. This
 * device type is quite different than any other Gadgeteer device type in
 * that its proxy type is \em designed to return a collection of samples
 * rather than only the most recent. This influences how the device type
 * itself collect samples, thus necessitating this alternative to
 * gadget::DefaultSampleHandler<T>.
 *
 * @since 2.1.6
 */
struct SampleHandler
{
   typedef EventPtr sample_type;
   typedef EventPtr raw_data_type;

   static const raw_data_type& getData(const raw_data_type& sample,
                                       const unsigned int)
   {
      return sample;
   }
};

}

}

}


#endif /* _GADGET_KEYBOARD_MOUSE_SAMPLE_HANDLER_H_ */
