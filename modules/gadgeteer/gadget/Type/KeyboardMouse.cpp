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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/gadgetConfig.h>

#include <algorithm>
#include <boost/bind.hpp>
#include <boost/concept_check.hpp>

#include <vpr/vpr.h>
#include <vpr/Sync/Guard.h>
#include <vpr/IO/ObjectWriter.h>
#include <vpr/IO/ObjectReader.h>
#include <vpr/Util/Assert.h>

#include <gadget/Event/Event.h>
#include <gadget/Event/KeyboardMouse/KeyEvent.h>
#include <gadget/Event/KeyboardMouse/MouseEvent.h>
#include <gadget/Type/KeyboardMouse/EventFactory.h>

#include <gadget/Type/KeyboardMouse.h>


namespace gadget
{

KeyboardMouse::KeyboardMouse()
{
   for (int i = 0; i < LAST_KEY; ++i)
   {
      mCurKeys[i] = 0;
   }

   // XXX: This is copied from the old code, but why is it necessary?
   // -PH 4/1/2004
   mCurKeys[KEY_NONE] = 1;
}

KeyboardMousePtr KeyboardMouse::create()
{
   return KeyboardMousePtr(new KeyboardMouse());
}

KeyboardMouse::~KeyboardMouse()
{;}

/**
 * Write both mCurKeys and mCurEventQueueLock to a stream using the given ObjectWriter.
 */
void KeyboardMouse::writeObject(vpr::ObjectWriter* writer)
{
   writer->writeUint16(MSG_DATA_EVENT_WINDOW); // Write out the data type so that we can assert if reading in wrong place

   writer->writeUint64(mSyncTime.getBaseVal());

   // Write Current Keys to a stream using the given ObjectWriter
   writer->writeUint16(LAST_KEY);
   for (unsigned int i = 0; i < LAST_KEY; ++i)
   {
      writer->writeUint32(mCurKeys[i]);
   }

   const EventQueue& cur_queue(mCurEventQueue.getValue());

   // Write Events to a stream using the given ObjectWriter
   writer->writeUint16(cur_queue.size());

   // Lock the Queue of current events to serialize
   vpr::Guard<vpr::Mutex> cur_guard(mCurEventQueueLock);

   // Serialize all events.
   std::for_each(cur_queue.begin(), cur_queue.end(),
                 boost::bind(&Event::writeObject, _1, writer));
}

/**
 * Read mCurKeys and mCurEventQueueLock from a stream using the given ObjectReader.
 */
void KeyboardMouse::readObject(vpr::ObjectReader* reader)
{
   // ASSERT if the given datastream does not start with the correct datatype
   // flag.
   // XXX: Should there be error checking for the case when vprASSERT() is
   // compiled out?  -PH 8/21/2003
   vpr::Uint16 data_type = reader->readUint16();
   vprASSERT(data_type==MSG_DATA_EVENT_WINDOW && "[KeyboardMouse::readObject()] Not KeyboardMouse Data");
   boost::ignore_unused_variable_warning(data_type);

   // We must save this value to set the sync time after we updateEventQueue.
   // This is because we can not read the timestamp from an event in the
   // queue since we do not have events every frame.
   vpr::Uint64 temp_sync = reader->readUint64();

   // Read Current Keys using the given ObjectReader
   const unsigned short num_keys(reader->readUint16());

   vprASSERT(LAST_KEY == num_keys && "[KeyboardMouse::readObject()] Different number of keys.");

   for (unsigned short i = 0; i < num_keys; ++i)
   {
      mCurKeys[i] = reader->readUint32();
   }

   // Read all events using the given ObjectReader
   const unsigned short num_events(reader->readUint16());

   // -For each event
   //   -Read the event type
   //   -Create the correct Event subclass using the EventFactory
   //   -Set the event type since we could not set it during construction
   //   -Load all necissary data into event using the given ObjectReader
   //   -Add the new event to the working event queue
   //  -Update the event queue, which swaps the working and current queues
   for (unsigned short i = 0; i < num_events; ++i)
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
}

bool KeyboardMouse::modifierOnly(const Keys modKey) const
{
   switch (modKey)
   {
      case KEY_NONE:
         return (!mCurKeys[KEY_SHIFT] && !mCurKeys[KEY_CTRL] && !mCurKeys[KEY_ALT] && !mCurKeys[KEY_COMMAND]);
      case KEY_SHIFT:
         return (mCurKeys[KEY_SHIFT] && !mCurKeys[KEY_CTRL] && !mCurKeys[KEY_ALT] && !mCurKeys[KEY_COMMAND]);
      case KEY_CTRL:
         return (!mCurKeys[KEY_SHIFT] && mCurKeys[KEY_CTRL] && !mCurKeys[KEY_ALT] && !mCurKeys[KEY_COMMAND]);
      case KEY_ALT:
         return (!mCurKeys[KEY_SHIFT] && !mCurKeys[KEY_CTRL] && mCurKeys[KEY_ALT] && !mCurKeys[KEY_COMMAND]);
       case KEY_COMMAND:
           return (!mCurKeys[KEY_SHIFT] && !mCurKeys[KEY_CTRL] && !mCurKeys[KEY_ALT] && mCurKeys[KEY_COMMAND]);
      default:
         vprASSERT(false);
         return 0;
   }
}

const std::string KeyboardMouse::getKeyName(const Keys keyId) const
{
   switch(keyId)
   {
      case KEY_NONE: return std::string("KEY_NONE");
      case KEY_UP: return std::string("KEY_UP");
      case KEY_DOWN: return std::string("KEY_DOWN");
      case KEY_LEFT: return std::string("KEY_LEFT");
      case KEY_RIGHT: return std::string("KEY_RIGHT");
      case KEY_SHIFT: return std::string("KEY_SHIFT");
      case KEY_CTRL: return std::string("KEY_CTRL");
      case KEY_ALT: return std::string("KEY_ALT");
      case KEY_COMMAND: return std::string("KEY_COMMAND");
      case KEY_1: return std::string("KEY_1");
      case KEY_2: return std::string("KEY_2");
      case KEY_3: return std::string("KEY_3");
      case KEY_4: return std::string("KEY_4");
      case KEY_5: return std::string("KEY_5");
      case KEY_6: return std::string("KEY_6");
      case KEY_7: return std::string("KEY_7");
      case KEY_8: return std::string("KEY_8");
      case KEY_9: return std::string("KEY_9");
      case KEY_0: return std::string("KEY_0");
      case KEY_A: return std::string("KEY_A");
      case KEY_B: return std::string("KEY_B");
      case KEY_C: return std::string("KEY_C");
      case KEY_D: return std::string("KEY_D");
      case KEY_E: return std::string("KEY_E");
      case KEY_F: return std::string("KEY_F");
      case KEY_G: return std::string("KEY_G");
      case KEY_H: return std::string("KEY_H");
      case KEY_I: return std::string("KEY_I");
      case KEY_J: return std::string("KEY_J");
      case KEY_K: return std::string("KEY_K");
      case KEY_L: return std::string("KEY_L");
      case KEY_M: return std::string("KEY_M");
      case KEY_N: return std::string("KEY_N");
      case KEY_O: return std::string("KEY_O");
      case KEY_P: return std::string("KEY_P");
      case KEY_Q: return std::string("KEY_Q");
      case KEY_R: return std::string("KEY_R");
      case KEY_S: return std::string("KEY_S");
      case KEY_T: return std::string("KEY_T");
      case KEY_U: return std::string("KEY_U");
      case KEY_V: return std::string("KEY_V");
      case KEY_W: return std::string("KEY_W");
      case KEY_X: return std::string("KEY_X");
      case KEY_Y: return std::string("KEY_Y");
      case KEY_Z: return std::string("KEY_Z");
      case KEY_ESC: return std::string("KEY_ESC");

      case MOUSE_POSX: return std::string("MOUSE_POSX");
      case MOUSE_NEGX: return std::string("MOUSE_NEGX");
      case MOUSE_POSY: return std::string("MOUSE_POSY");
      case MOUSE_NEGY: return std::string("MOUSE_NEGY");
      case MBUTTON1: return std::string("MBUTTON1");
      case MBUTTON2: return std::string("MBUTTON2");
      case MBUTTON3: return std::string("MBUTTON3");
      case MBUTTON4: return std::string("MBUTTON4");
      case MBUTTON5: return std::string("MBUTTON5");
      case MBUTTON6: return std::string("MBUTTON5");
      case MBUTTON7: return std::string("MBUTTON7");
      case MBUTTON8: return std::string("MBUTTON8");
      case MBUTTON9: return std::string("MBUTTON9");
      case NO_MBUTTON: return std::string("NO_MBUTTON");

      case MOUSE_SCROLL_UP: return std::string("MOUSE_SCROLL_UP");
      case MOUSE_SCROLL_DOWN: return std::string("MOUSE_SCROLL_DOWN");
      case MOUSE_SCROLL_LEFT: return std::string("MOUSE_SCROLL_LEFT");
      case MOUSE_SCROLL_RIGHT: return std::string("MOUSE_SCROLL_RIGHT");

      case KEY_TAB          : return std::string("KEY_TAB");
      case KEY_BACKTAB      : return std::string("KEY_BACKTAB");
      case KEY_BACKSPACE    : return std::string("KEY_BACKSPACE");
      case KEY_RETURN       : return std::string("KEY_RETURN");
      case KEY_ENTER        : return std::string("KEY_ENTER");
      case KEY_INSERT       : return std::string("KEY_INSERT");
      case KEY_DELETE       : return std::string("KEY_DELETE");
      case KEY_PAUSE        : return std::string("KEY_PAUSE");
      case KEY_PRINT        : return std::string("KEY_PRINT");
      case KEY_SYSREQ       : return std::string("KEY_SYSREQ");
      case KEY_HOME         : return std::string("KEY_HOME");
      case KEY_END          : return std::string("KEY_END");
      case KEY_PRIOR        : return std::string("KEY_PRIOR");
      case KEY_NEXT         : return std::string("KEY_NEXT");
      case KEY_CAPS_LOCK    : return std::string("KEY_CAPS_LOCK");
      case KEY_NUM_LOCK     : return std::string("KEY_NUM_LOCK");
      case KEY_SCROLL_LOCK  : return std::string("KEY_SCROLL_LOCK");
      case KEY_F1           : return std::string("KEY_F1");
      case KEY_F2           : return std::string("KEY_F2");
      case KEY_F3           : return std::string("KEY_F3");
      case KEY_F4           : return std::string("KEY_F4");
      case KEY_F5           : return std::string("KEY_F5");
      case KEY_F6           : return std::string("KEY_F6");
      case KEY_F7           : return std::string("KEY_F7");
      case KEY_F8           : return std::string("KEY_F8");
      case KEY_F9           : return std::string("KEY_F9");
      case KEY_F10          : return std::string("KEY_F10");
      case KEY_F11          : return std::string("KEY_F11");
      case KEY_F12          : return std::string("KEY_F12");
      case KEY_F13          : return std::string("KEY_F13");
      case KEY_F14          : return std::string("KEY_F14");
      case KEY_F15          : return std::string("KEY_F15");
      case KEY_F16          : return std::string("KEY_F16");
      case KEY_F17          : return std::string("KEY_F17");
      case KEY_F18          : return std::string("KEY_F18");
      case KEY_F19          : return std::string("KEY_F19");
      case KEY_F20          : return std::string("KEY_F20");
      case KEY_F21          : return std::string("KEY_F21");
      case KEY_F22          : return std::string("KEY_F22");
      case KEY_F23          : return std::string("KEY_F23");
      case KEY_F24          : return std::string("KEY_F24");
      case KEY_F25          : return std::string("KEY_F25");
      case KEY_F26          : return std::string("KEY_F26");
      case KEY_F27          : return std::string("KEY_F27");
      case KEY_F28          : return std::string("KEY_F28");
      case KEY_F29          : return std::string("KEY_F29");
      case KEY_F30          : return std::string("KEY_F30");
      case KEY_F31          : return std::string("KEY_F31");
      case KEY_F32          : return std::string("KEY_F32");
      case KEY_F33          : return std::string("KEY_F33");
      case KEY_F34          : return std::string("KEY_F34");
      case KEY_F35          : return std::string("KEY_F35");

      case KEY_SUPER_L : return std::string("KEY_SUPER_L");
      case KEY_SUPER_R : return std::string("KEY_SUPER_R");
      case KEY_MENU    : return std::string("KEY_MENU");
      case KEY_HYPER_L : return std::string("KEY_HYPER_L");
      case KEY_HYPER_R : return std::string("KEY_HYPER_R");
      case KEY_HELP    : return std::string("KEY_HELP");
      case KEY_SPACE   : return std::string("KEY_SPACE");
      case KEY_ANY     : return std::string("KEY_ANY");

      case KEY_EXCLAM        : return std::string("KEY_EXCLAM");
      case KEY_QUOTE_DBL     : return std::string("KEY_QUOTE_DBL");
      case KEY_NUMBER_SIGN   : return std::string("KEY_NUMBER_SIGN");
      case KEY_DOLLAR        : return std::string("KEY_DOLLAR");
      case KEY_PERCENT       : return std::string("KEY_PERCENT");
      case KEY_AMPERSAND     : return std::string("KEY_AMPERSAND");
      case KEY_APOSTROPHE    : return std::string("KEY_APOSTROPHE");
      case KEY_PAREN_LEFT    : return std::string("KEY_PAREN_LEFT");
      case KEY_PAREN_RIGHT   : return std::string("KEY_PAREN_RIGHT");
      case KEY_ASTERISK      : return std::string("KEY_ASTERISK");
      case KEY_PLUS          : return std::string("KEY_PLUS");
      case KEY_COMMA         : return std::string("KEY_COMMA");
      case KEY_MINUS         : return std::string("KEY_MINUS");
      case KEY_PERIOD        : return std::string("KEY_PERIOD");
      case KEY_SLASH         : return std::string("KEY_SLASH");
      case KEY_COLON         : return std::string("KEY_COLON");
      case KEY_SEMICOLON     : return std::string("KEY_SEMICOLON");
      case KEY_LESS          : return std::string("KEY_LESS");
      case KEY_EQUAL         : return std::string("KEY_EQUAL");
      case KEY_GREATER       : return std::string("KEY_GREATER");
      case KEY_QUESTION      : return std::string("KEY_QUESTION");
      case KEY_AT            : return std::string("KEY_AT");
      case KEY_BRACKET_LEFT  : return std::string("KEY_BRACKET_LEFT");
      case KEY_BACKSLASH     : return std::string("KEY_BACKSLASH");
      case KEY_BRACKET_RIGHT : return std::string("KEY_BRACKET_RIGHT");
      case KEY_ASCII_CIRCUM  : return std::string("KEY_ASCII_CIRCUM");
      case KEY_UNDERSCORE    : return std::string("KEY_UNDERSCORE");
      case KEY_QUOTE_LEFT    : return std::string("KEY_QUOTE_LEFT");
      case KEY_BRACE_LEFT    : return std::string("KEY_BRACE_LEFT");
      case KEY_BAR           : return std::string("KEY_BAR");
      case KEY_BRACE_RIGHT   : return std::string("KEY_BRACE_RIGHT");
      case KEY_ASCII_TILDE   : return std::string("KEY_ASCII_TILDE");

      case KEY_UNKNOWN : return std::string("KEY_UNKNOWN");
      case LAST_KEY    : return std::string("LAST_KEY");
   }

   // If all of the above fell through ...
   return std::string("Unrecognized key");
}

const KeyboardMouse::EventQueue KeyboardMouse::getEventQueue()
{
   vpr::Guard<vpr::Mutex> guard(mCurEventQueueLock);
   return mCurEventQueue.getValue();
}

void KeyboardMouse::addEvent(EventPtr e)
{
   vpr::Guard<vpr::Mutex> guard(mWorkingEventQueueLock);
   mWorkingEventQueue.editValue().push_back(e);
   mDataAdded(e);
}

void KeyboardMouse::updateEventQueue()
{
   mSyncTime.setNow();

   vpr::Guard<vpr::Mutex> work_guard(mWorkingEventQueueLock);
   {
      vpr::Guard<vpr::Mutex> cur_guard(mCurEventQueueLock);
      mCurEventQueue = mWorkingEventQueue;
   }

   mWorkingEventQueue.editValue().clear();      // Clear old queue
}

} // End of gadget namespcae
