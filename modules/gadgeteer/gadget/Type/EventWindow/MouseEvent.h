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

#ifndef _GADGET_MOUSE_EVENT_H_
#define _GADGET_MOUSE_EVENT_H_

#include <gadget/gadgetConfig.h>

#include <gadget/Type/EventWindow/Event.h>
#include <gadget/Type/EventWindow/MouseEventPtr.h>
#include <gadget/Type/EventWindow/Keys.h>


namespace gadget
{

class MouseEvent : public gadget::Event
{
public:
   MouseEvent(const int& state, const bool& isButtonPress, const int& x,
              const int& y, const int& globalX, const int& globalY,
              const unsigned long& time)
      : gadget::Event(Event::MouseEvent, time), mState(state), mRelativeX(x),
        mRelativeY(y),mGlobalX(globalX), mGlobalY(globalY)
   {
   }

   const bool& isButtonPress() const
   {
      return mIsButtonPress;
   }

   bool isButtonRelease() const
   {
      return ! mIsButtonPress;
   }

   const int& getX() const
   {
      return mRelativeX;
   }

   const int& getY() const
   {
      return mRelativeY;
   }

   const int& getGlobalX() const
   {
      return mGlobalX;
   }

   const int& getGlobalY() const
   {
      return mGlobalY;
   }

   const int& getState() const
   {
      return mState;
   }

private:
   int  mState;
   bool mIsButtonPress;
   int  mRelativeX;
   int  mRelativeY;
   int  mGlobalX;
   int  mGlobalY;
};

} // End of gadget namespace


#endif /* _GADGET_MOUSE_EVENT_H_ */
