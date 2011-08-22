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

#ifndef _GADGET_KEYBOARD_MOUSE_H_
#define _GADGET_KEYBOARD_MOUSE_H_

#include <gadget/gadgetConfig.h>

#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/signal.hpp>

#include <vpr/IO/SerializableObject.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Util/Interval.h>
#include <vpr/Util/SignalProxy.h>
#include <jccl/Config/ConfigElementPtr.h>

#include <gadget/Event/EventPtr.h>
#include <gadget/Type/KeyboardMouse/Keys.h>
#include <gadget/Type/KeyboardMouseData.h>
#include <gadget/Type/KeyboardMousePtr.h>


namespace gadget
{

const unsigned short MSG_DATA_EVENT_WINDOW = 420;

/** \class KeyboardMouse KeyboardMouse.h gadget/Type/KeyboardMouse.h
 *
 * gadget::KeyboardMouse is an abstract class for interfacing with keyboard
 * (and other key-based) devices.  Informally, a keyboard/mouse device can be
 * thought of as a map of keyboard and mouse events (presses, releases, and
 * movements) to integers.  The integers indicate the number of times the event
 * occurred since the last update.  That is to say, gadget::KeyboardMouse
 * counts the number of keyboard and mouse events between updates.  Updates in
 * Juggler occur once per frame.
 */
class GADGET_CLASS_API KeyboardMouse
   : public vpr::SerializableObject
   , private boost::noncopyable
{
public:
   typedef KeyboardMouseData::data_type EventQueue;
   typedef boost::signal<void (const EventPtr&)> add_signal_t;

   static const vpr::Uint16 type_id;

protected:
   KeyboardMouse();

public:
   /**
    * Creates a KeyboardMouse instance and returns it wrapped in a
    * KeyboardMousePtr object.
    *
    * @since 1.3.7
    */
   static KeyboardMousePtr create();

   virtual ~KeyboardMouse();

   /**
    * Writes both \c mCurKeys and \c mCurEventQueueLock to a stream using the
    * given object writer.
    *
    * @param writer The object writer to which this object will be serialized.
    *
    * @throw vpr::IOException is thrown if serialization fails.
    */
   virtual void writeObject(vpr::ObjectWriter* writer);

   /**
    * Reads \c mCurKeys and \c mCurEventQueueLock from a stream using the given
    * object reader.
    *
    * @param reader The object reader from which this object will be
    *               de-serialized.
    *
    * @throw vpr::IOException is thrown if de-serialization fails.
    */
   virtual void readObject(vpr::ObjectReader* reader);

   /**
    * @since 2.1.19
    */
   virtual vpr::Uint16 getTypeId() const;

   virtual bool config(jccl::ConfigElementPtr)
   {
      return true;
   }

   /**
    * Gets the interval that will be used for syncronization while only sharing
    * keyboard data across the cluster.
    */
   const vpr::Interval& getSyncTime() const
   {
      return mSyncTime;
   }

   /**
    * Returns the number of times the given key was pressed during the last
    * frame.  The value returned can be used in a conditional expression to
    * determine if the key was pressed at all.
    *
    * @param keyId The identifier for the key whose state will be queried.
    *
    * @return The number of times the key was pressed since last update.
    */
   int keyPressed(const Keys keyId) const
   {
      return mCurKeys[keyId];
   }

   /**
    * Determines if the given modifier key is the only modifier pressed.
    *
    * @pre The given key identifier is one of gadget::NONE, gadget::KEY_ALT,
    *      gadget::KEY_CTRL, or gadget::KEY_SHIFT.
    *
    * @param modKey A gadget::Keys value that must be one of gadget::NONE,
    *               gadget::KEY_ALT, gadget::KEY_CTRL, or gadget::KEY_SHIFT.
    *
    * @return true if the given modifier key is the only modifier key pressed.
    */
   bool modifierOnly(const Keys modKey) const;

   /**
    * Returns the symbolic Gadgeteer name associated with keyId.  For example,
    * getKeyName(gadget::KEY_UP) retuns "KEY_UP".
    *
    * @param keyId The identifier for the key whose name will be returned.
    *
    * @return A string that is the symbolic name of the given key.
    */
   const std::string getKeyName(const Keys keyId) const;

   /**
    * Returns a \em copy of the current queue of events for this device.
    */
   const EventQueue getEventQueue();

   /**
    * Adds the given event object to the in-progress queue.
    */
   void addEvent(EventPtr e);

   /**
    * @since 2.1.4
    */
   vpr::SignalProxy<add_signal_t> dataAdded()
   {
      return mDataAdded;
   }

protected:
   /**
    * (0,*): Copy of keys for this frame that the user reads from between
    * updates.
    */
   int mCurKeys[LAST_KEY];

   /**
    * Copies the in-progress event queue into the current (i.e., user) queue
    * and wipes out the in-progress queue.  This should only be called when
    * it is time to synchronize the data for the current frame.
    *
    * @post mCurEventQueue gets a copy of mWorkingEventQueue at the time of
    *       invocation.  mWorkingEventQueue is emptied.
    */
   void updateEventQueue();

   add_signal_t mDataAdded;

   /** Queue of events returned to users. */
   KeyboardMouseData mCurEventQueue;
   vpr::Mutex mCurEventQueueLock;

   KeyboardMouseData mWorkingEventQueue; /**< In-progress queue of events. */
   vpr::Mutex mWorkingEventQueueLock;

   // We have to create a Interval that the user can use across the cluster to
   // syncronize their applications navigation. We would like to use the
   // timestamps from the event queue but we only get events during frames
   // that have key presses and mouse motion.
   vpr::Interval mSyncTime;      /**< Holds an Interval that is syncrnized across the cluster */

private:
   static const std::string sTypeName;
};

} // end namespace


#endif
