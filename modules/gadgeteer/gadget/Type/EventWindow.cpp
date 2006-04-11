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

#include <gadget/gadgetConfig.h>

#include <boost/concept_check.hpp>
#include <vpr/vpr.h>
#include <vpr/Sync/Guard.h>
#include <vpr/Util/Assert.h>

#include <gadget/Type/EventWindow.h>
#include <gadget/Type/EventWindow/Event.h>
#include <gadget/Type/EventWindow/KeyEvent.h>
#include <gadget/Type/EventWindow/MouseEvent.h>
#include <gadget/Type/EventWindow/EventFactory.h>


namespace gadget
{
/**
 * Write both mCurKeys and mCurEventQueueLock to a stream using the given ObjectWriter.
 */
vpr::ReturnStatus EventWindow::writeObject(vpr::ObjectWriter* writer)
{
   writer->writeUint16(MSG_DATA_EVENT_WINDOW); // Write out the data type so that we can assert if reading in wrong place
   
   writer->writeUint64(mSyncTime.getBaseVal());

   // Write Current Keys to a stream using the given ObjectWriter
   writer->writeUint16(gadget::LAST_KEY);
   for ( unsigned int i = 0; i < gadget::LAST_KEY; ++i )
   {
      writer->writeUint32(mCurKeys[i]);
   }
   
   // Write Events to a stream using the given ObjectWriter
   writer->writeUint16(mCurEventQueue.size());
   
   // Lock the Queue of current events to serialize
   vpr::Guard<vpr::Mutex> cur_guard(mCurEventQueueLock);

   /*
   // Copy the queue since the only way to iterate over it requires
   // you to pop events off the front, which destroys the queue
   std::queue<gadget::EventPtr> temp_queue(mCurEventQueue); 

   // While the queue is not empty
   //  -Grab the event on the front of the queue
   //  -Serialize the Event
   //  -Pop the event off the front of the queue
   while (!temp_queue.empty())
   {
      EventPtr temp_event = temp_queue.front();
      temp_event->writeObject(writer);
      temp_queue.pop();
   }   
   */

   for(unsigned i = 0; i<mCurEventQueue.size(); ++i)
      mCurEventQueue[i]->writeObject(writer);

   return vpr::ReturnStatus::Succeed;
}

/**
 * Read mCurKeys and mCurEventQueueLock from a stream using the given ObjectReader.
 */
vpr::ReturnStatus EventWindow::readObject(vpr::ObjectReader* reader)
{
   // ASSERT if the given datastream does not start with the correct datatype
   // flag.
   // XXX: Should there be error checking for the case when vprASSERT() is
   // compiled out?  -PH 8/21/2003
   vpr::Uint16 data_type = reader->readUint16();
   vprASSERT(data_type==MSG_DATA_EVENT_WINDOW && "[EventWindow::readObject()]Not EventWindow Data");
   boost::ignore_unused_variable_warning(data_type);
   
   // We must save this value to set the sync time after we updateEventQueue.
   // This is because we can not ready the timestamp from an event in the
   // queue since we do not have events every frame.
   vpr::Uint64 temp_sync = reader->readUint64();
   
   // Read Current Keys using the given ObjectReader
   unsigned int num_keys = reader->readUint16();
   for ( unsigned int i = 0; i < num_keys; ++i )
   {
      mCurKeys[i] = reader->readUint32();
   }

   // Read all events using the given ObjectReader
   unsigned num_events = reader->readUint16();
      
   // -For each event
   //   -Read the event type
   //   -Create the correct Event subclass using the EventFactory
   //   -Set the event type since we could not set it during construction
   //   -Load all necissary data into event using the given ObjectReader
   //   -Add the new event to the working event queue
   //  -Update the event queue, which swaps the working and current queues
   for (unsigned i = 0; i < num_events; ++i )
   {
      EventType event_type = (EventType)reader->readUint16();      
      EventPtr temp_event(EventFactory::instance()->createObject(event_type));
      
      vprASSERT(NULL != temp_event.get() && "temp_event == NULL, Event Type does not exist.");
      
      temp_event->setType(event_type);
      temp_event->readObject(reader);

      addEvent(temp_event);
   }
   updateEventQueue();

   // We must set sync time correctly here since updateEventQueue() changes it.
   mSyncTime.set(temp_sync, vpr::Interval::Base);

   return vpr::ReturnStatus::Succeed;
}

/**
 * Checks for the given modifier key pressed only.
 * @return true if key pressed exclusively.
 */
//virtual bool modifierOnly(int modKey) =0;
bool EventWindow::modifierOnly(gadget::Keys modKey)
{
   switch (modKey)
   {
      case gadget::KEY_NONE:
         return (!mCurKeys[gadget::KEY_SHIFT] && !mCurKeys[gadget::KEY_CTRL] && !mCurKeys[gadget::KEY_ALT]);
      case gadget::KEY_SHIFT:
         return (mCurKeys[gadget::KEY_SHIFT] && !mCurKeys[gadget::KEY_CTRL] && !mCurKeys[gadget::KEY_ALT]);
      case gadget::KEY_CTRL:
         return (!mCurKeys[gadget::KEY_SHIFT] && mCurKeys[gadget::KEY_CTRL] && !mCurKeys[gadget::KEY_ALT]);
      case gadget::KEY_ALT:
         return (!mCurKeys[gadget::KEY_SHIFT] && !mCurKeys[gadget::KEY_CTRL] && mCurKeys[gadget::KEY_ALT]);
      default:
         vprASSERT(false);
         return 0;
   }
}

std::string EventWindow::getKeyName(gadget::Keys keyId)
{
   switch(keyId)
   {
      case gadget::KEY_NONE: return std::string("KEY_NONE");
      case gadget::KEY_UP: return std::string("KEY_UP");
      case gadget::KEY_DOWN: return std::string("KEY_DOWN");
      case gadget::KEY_LEFT: return std::string("KEY_LEFT");
      case gadget::KEY_RIGHT: return std::string("KEY_RIGHT");
      case gadget::KEY_SHIFT: return std::string("KEY_SHIFT");
      case gadget::KEY_CTRL: return std::string("KEY_CTRL");
      case gadget::KEY_ALT: return std::string("KEY_ALT");
      case gadget::KEY_1: return std::string("KEY_1");
      case gadget::KEY_2: return std::string("KEY_2");
      case gadget::KEY_3: return std::string("KEY_3");
      case gadget::KEY_4: return std::string("KEY_4");
      case gadget::KEY_5: return std::string("KEY_5");
      case gadget::KEY_6: return std::string("KEY_6");
      case gadget::KEY_7: return std::string("KEY_7");
      case gadget::KEY_8: return std::string("KEY_8");
      case gadget::KEY_9: return std::string("KEY_9");
      case gadget::KEY_0: return std::string("KEY_0");
      case gadget::KEY_A: return std::string("KEY_A");
      case gadget::KEY_B: return std::string("KEY_B");
      case gadget::KEY_C: return std::string("KEY_C");
      case gadget::KEY_D: return std::string("KEY_D");
      case gadget::KEY_E: return std::string("KEY_E");
      case gadget::KEY_F: return std::string("KEY_F");
      case gadget::KEY_G: return std::string("KEY_G");
      case gadget::KEY_H: return std::string("KEY_H");
      case gadget::KEY_I: return std::string("KEY_I");
      case gadget::KEY_J: return std::string("KEY_J");
      case gadget::KEY_K: return std::string("KEY_K");
      case gadget::KEY_L: return std::string("KEY_L");
      case gadget::KEY_M: return std::string("KEY_M");
      case gadget::KEY_N: return std::string("KEY_N");
      case gadget::KEY_O: return std::string("KEY_O");
      case gadget::KEY_P: return std::string("KEY_P");
      case gadget::KEY_Q: return std::string("KEY_Q");
      case gadget::KEY_R: return std::string("KEY_R");
      case gadget::KEY_S: return std::string("KEY_S");
      case gadget::KEY_T: return std::string("KEY_T");
      case gadget::KEY_U: return std::string("KEY_U");
      case gadget::KEY_V: return std::string("KEY_V");
      case gadget::KEY_W: return std::string("KEY_W");
      case gadget::KEY_X: return std::string("KEY_X");
      case gadget::KEY_Y: return std::string("KEY_Y");
      case gadget::KEY_Z: return std::string("KEY_Z");
      case gadget::KEY_ESC: return std::string("KEY_ESC");

      case gadget::MOUSE_POSX: return std::string("MOUSE_POSX");
      case gadget::MOUSE_NEGX: return std::string("MOUSE_NEGX");
      case gadget::MOUSE_POSY: return std::string("MOUSE_POSY");
      case gadget::MOUSE_NEGY: return std::string("MOUSE_NEGY");
      case gadget::MBUTTON1: return std::string("MBUTTON1");
      case gadget::MBUTTON2: return std::string("MBUTTON2");
      case gadget::MBUTTON3: return std::string("MBUTTON3");
      case gadget::MBUTTON4: return std::string("MBUTTON4");
      case gadget::MBUTTON5: return std::string("MBUTTON5");
      case gadget::NO_MBUTTON: return std::string("NO_MBUTTON");

      case gadget::KEY_TAB          : return std::string("KEY_TAB");
      case gadget::KEY_BACKTAB      : return std::string("KEY_BACKTAB");
      case gadget::KEY_BACKSPACE    : return std::string("KEY_BACKSPACE");
      case gadget::KEY_RETURN       : return std::string("KEY_RETURN");
      case gadget::KEY_ENTER        : return std::string("KEY_ENTER");
      case gadget::KEY_INSERT       : return std::string("KEY_INSERT");
      case gadget::KEY_DELETE       : return std::string("KEY_DELETE");
      case gadget::KEY_PAUSE        : return std::string("KEY_PAUSE");
      case gadget::KEY_PRINT        : return std::string("KEY_PRINT");
      case gadget::KEY_SYSREQ       : return std::string("KEY_SYSREQ");
      case gadget::KEY_HOME         : return std::string("KEY_HOME");
      case gadget::KEY_END          : return std::string("KEY_END");
      case gadget::KEY_PRIOR        : return std::string("KEY_PRIOR");
      case gadget::KEY_NEXT         : return std::string("KEY_NEXT");
      case gadget::KEY_CAPS_LOCK    : return std::string("KEY_CAPS_LOCK");
      case gadget::KEY_NUM_LOCK     : return std::string("KEY_NUM_LOCK");
      case gadget::KEY_SCROLL_LOCK  : return std::string("KEY_SCROLL_LOCK");
      case gadget::KEY_F1           : return std::string("KEY_F1");
      case gadget::KEY_F2           : return std::string("KEY_F2");
      case gadget::KEY_F3           : return std::string("KEY_F3");
      case gadget::KEY_F4           : return std::string("KEY_F4");
      case gadget::KEY_F5           : return std::string("KEY_F5");
      case gadget::KEY_F6           : return std::string("KEY_F6");
      case gadget::KEY_F7           : return std::string("KEY_F7");
      case gadget::KEY_F8           : return std::string("KEY_F8");
      case gadget::KEY_F9           : return std::string("KEY_F9");
      case gadget::KEY_F10          : return std::string("KEY_F10");
      case gadget::KEY_F11          : return std::string("KEY_F11");
      case gadget::KEY_F12          : return std::string("KEY_F12");
      case gadget::KEY_F13          : return std::string("KEY_F13");
      case gadget::KEY_F14          : return std::string("KEY_F14");
      case gadget::KEY_F15          : return std::string("KEY_F15");
      case gadget::KEY_F16          : return std::string("KEY_F16");
      case gadget::KEY_F17          : return std::string("KEY_F17");
      case gadget::KEY_F18          : return std::string("KEY_F18");
      case gadget::KEY_F19          : return std::string("KEY_F19");
      case gadget::KEY_F20          : return std::string("KEY_F20");
      case gadget::KEY_F21          : return std::string("KEY_F21");
      case gadget::KEY_F22          : return std::string("KEY_F22");
      case gadget::KEY_F23          : return std::string("KEY_F23");
      case gadget::KEY_F24          : return std::string("KEY_F24");
      case gadget::KEY_F25          : return std::string("KEY_F25");
      case gadget::KEY_F26          : return std::string("KEY_F26");
      case gadget::KEY_F27          : return std::string("KEY_F27");
      case gadget::KEY_F28          : return std::string("KEY_F28");
      case gadget::KEY_F29          : return std::string("KEY_F29");
      case gadget::KEY_F30          : return std::string("KEY_F30");
      case gadget::KEY_F31          : return std::string("KEY_F31");
      case gadget::KEY_F32          : return std::string("KEY_F32");
      case gadget::KEY_F33          : return std::string("KEY_F33");
      case gadget::KEY_F34          : return std::string("KEY_F34");
      case gadget::KEY_F35          : return std::string("KEY_F35");

      case gadget::KEY_SUPER_L : return std::string("KEY_SUPER_L");
      case gadget::KEY_SUPER_R : return std::string("KEY_SUPER_R");
      case gadget::KEY_MENU    : return std::string("KEY_MENU");
      case gadget::KEY_HYPER_L : return std::string("KEY_HYPER_L");
      case gadget::KEY_HYPER_R : return std::string("KEY_HYPER_R");
      case gadget::KEY_HELP    : return std::string("KEY_HELP");
      case gadget::KEY_SPACE   : return std::string("KEY_SPACE");
      case gadget::KEY_ANY     : return std::string("KEY_ANY");

      case gadget::KEY_EXCLAM        : return std::string("KEY_EXCLAM");
      case gadget::KEY_QUOTE_DBL     : return std::string("KEY_QUOTE_DBL");
      case gadget::KEY_NUMBER_SIGN   : return std::string("KEY_NUMBER_SIGN");
      case gadget::KEY_DOLLAR        : return std::string("KEY_DOLLAR");
      case gadget::KEY_PERCENT       : return std::string("KEY_PERCENT");
      case gadget::KEY_AMPERSAND     : return std::string("KEY_AMPERSAND");
      case gadget::KEY_APOSTROPHE    : return std::string("KEY_APOSTROPHE");
      case gadget::KEY_PAREN_LEFT    : return std::string("KEY_PAREN_LEFT");
      case gadget::KEY_PAREN_RIGHT   : return std::string("KEY_PAREN_RIGHT");
      case gadget::KEY_ASTERISK      : return std::string("KEY_ASTERISK");
      case gadget::KEY_PLUS          : return std::string("KEY_PLUS");
      case gadget::KEY_COMMA         : return std::string("KEY_COMMA");
      case gadget::KEY_MINUS         : return std::string("KEY_MINUS");
      case gadget::KEY_PERIOD        : return std::string("KEY_PERIOD");
      case gadget::KEY_SLASH         : return std::string("KEY_SLASH");
      case gadget::KEY_COLON         : return std::string("KEY_COLON");
      case gadget::KEY_SEMICOLON     : return std::string("KEY_SEMICOLON");
      case gadget::KEY_LESS          : return std::string("KEY_LESS");
      case gadget::KEY_EQUAL         : return std::string("KEY_EQUAL");
      case gadget::KEY_GREATER       : return std::string("KEY_GREATER");
      case gadget::KEY_QUESTION      : return std::string("KEY_QUESTION");
      case gadget::KEY_AT            : return std::string("KEY_AT");
      case gadget::KEY_BRACKET_LEFT  : return std::string("KEY_BRACKET_LEFT");
      case gadget::KEY_BACKSLASH     : return std::string("KEY_BACKSLASH");
      case gadget::KEY_BRACKET_RIGHT : return std::string("KEY_BRACKET_RIGHT");
      case gadget::KEY_ASCII_CIRCUM  : return std::string("KEY_ASCII_CIRCUM");
      case gadget::KEY_UNDERSCORE    : return std::string("KEY_UNDERSCORE");
      case gadget::KEY_QUOTE_LEFT    : return std::string("KEY_QUOTE_LEFT");
      case gadget::KEY_BRACE_LEFT    : return std::string("KEY_BRACE_LEFT");
      case gadget::KEY_BAR           : return std::string("KEY_BAR");
      case gadget::KEY_BRACE_RIGHT   : return std::string("KEY_BRACE_RIGHT");
      case gadget::KEY_ASCII_TILDE   : return std::string("KEY_ASCII_TILDE");

      case gadget::KEY_UNKNOWN : return std::string("KEY_UNKNOWN");
      case gadget::LAST_KEY    : return std::string("LAST_KEY");
   }

   // If all of the above fell through ...
   return std::string("Unrecognized key");
}

EventWindow::EventQueue EventWindow::getEventQueue()
{
   vpr::Guard<vpr::Mutex> guard(mCurEventQueueLock);
   return mCurEventQueue;
}

void EventWindow::addEvent(gadget::EventPtr e)
{
   vpr::Guard<vpr::Mutex> guard(mWorkingEventQueueLock);
   mWorkingEventQueue.push_back(e);
}

void EventWindow::updateEventQueue()
{
   mSyncTime.setNow();

   vpr::Guard<vpr::Mutex> work_guard(mWorkingEventQueueLock);
   {
      vpr::Guard<vpr::Mutex> cur_guard(mCurEventQueueLock);
      mCurEventQueue = mWorkingEventQueue;
   }
   
   mWorkingEventQueue.clear();      // Clear old queue
}

} // End of gadget namespcae
