/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#ifndef _GADGET_KEY_EVENT_H_
#define _GADGET_KEY_EVENT_H_

#include <gadget/gadgetConfig.h>

#include <gadget/Type/EventWindow/Event.h>
#include <gadget/Type/EventWindow/KeyEventPtr.h>
#include <gadget/Type/EventWindow/Keys.h>


namespace gadget
{

class GADGET_CLASS_API KeyEvent : public gadget::Event
{
public:
   KeyEvent(const gadget::Keys& key, const bool& isKeyPress, const int& mask,
            const unsigned long& time)
      : gadget::Event(Event::KeyEvent, time), mKey(key), mModifierMask(mask),
        mIsKeyPress(isKeyPress)
   {
   }

   const gadget::Keys& key() const
   {
      return mKey;
   }

   const bool& isKeyPress() const
   {
      return mIsKeyPress;
   }

   bool isKeyRelease() const
   {
      return ! mIsKeyPress;
   }

   const int& getModifierMask() const
   {
      return mModifierMask;
   }

   char getKeyChar() const;

protected:
   gadget::Keys mKey;
   int          mModifierMask;
   bool         mIsKeyPress;
};

} // End of gadget namespace


#endif /* _GADGET_KEY_EVENT_H_ */
