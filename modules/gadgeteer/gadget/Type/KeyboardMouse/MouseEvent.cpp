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
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/gadgetConfig.h>
#include <vpr/IO/ObjectWriter.h>
#include <vpr/IO/ObjectReader.h>
#include <gadget/Type/KeyboardMouse/MouseEvent.h>

namespace gadget
{

MouseEvent::MouseEvent(const gadget::EventType type, const gadget::Keys button,
                       int x, int y, int globalX, int globalY, int state,
                       unsigned long time)
   : gadget::Event(type, time)
   , mButton(button)
   , mRelativeX(x)
   , mRelativeY(y)
   , mGlobalX(globalX)
   , mGlobalY(globalY)
   , mState(state)
{
}

MouseEvent::MouseEvent()
   : gadget::Event(NoEvent, 0)
   , mButton(gadget::NO_MBUTTON)
   , mRelativeX(0)
   , mRelativeY(0)
   , mGlobalX(0)
   , mGlobalY(0)
   , mState(0)
{
}

// Serializes this event using the given ObjectWriter.
void MouseEvent::writeObject(vpr::ObjectWriter* writer)
   throw (vpr::IOException)
{
   writer->writeUint16(mType);

   // Serialize all member variables
   writer->writeUint32(mButton);
   writer->writeUint32(mRelativeX);
   writer->writeUint32(mRelativeY);
   writer->writeUint32(mGlobalX);
   writer->writeUint32(mGlobalY);
   writer->writeUint32(mState);
}

// De-serializes this event using the given ObjectReader.
void MouseEvent::readObject(vpr::ObjectReader* reader) throw (vpr::IOException)
{
   // We have already read the type in EventWindoe to decide
   // if we should construct a KeyEvent or a MouseEvent
   //mType = reader->readUint16();

   // De-Serialize all member variables
   mButton = (gadget::Keys)reader->readUint32();
   mRelativeX = reader->readUint32();
   mRelativeY = reader->readUint32();
   mGlobalX = reader->readUint32();
   mGlobalY = reader->readUint32();
   mState = reader->readUint32();
}

} // End of gadget namespace
