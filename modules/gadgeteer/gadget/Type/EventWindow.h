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

#ifndef _GADGET_EVENT_WINDOW_H_
#define _GADGET_EVENT_WINDOW_H_

#include <gadget/gadgetConfig.h>

#include <string>
//#include <queue>
#include <vector>
#include <boost/concept_check.hpp>

#include <vpr/IO/SerializableObject.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Util/Interval.h>
#include <jccl/Config/ConfigChunkPtr.h>

#include <gadget/Type/EventWindow/Keys.h>
#include <gadget/Type/EventWindow/EventPtr.h>


namespace gadget
{

const unsigned short MSG_DATA_EVENT_WINDOW = 420;

/**
 * gadget::EventWindow is an abstract class for interfacing with keyboard (and
 * other key-based) devices.
 *
 * Informally, an event window can be thought of as map of keys to number of
 * times prseed since last update.
 *
 * That is to say, that gadget::EventWindow counts the number of key presses
 * between updates.  Updates in Juggler occur once per frame.
 */
class GADGET_CLASS_API EventWindow : public vpr::SerializableObject
{
public:
   //typedef std::queue<gadget::EventPtr> EventQueue;
   typedef std::vector<gadget::EventPtr> EventQueue;

   EventWindow();

   virtual ~EventWindow()
   {
      /* Do nothing. */ ;
   }

   virtual std::string getBaseType()
   {
      return std::string("EventWindow");
   }
   
   /**
    * Write both mCurKeys and mCurEventQueueLock to a stream using the given ObjectWriter.
    */
   virtual vpr::ReturnStatus writeObject(vpr::ObjectWriter* writer);

   /**
    * Read mCurKeys and mCurEventQueueLock from a stream using the given ObjectReader.
    */
   virtual vpr::ReturnStatus readObject(vpr::ObjectReader* reader);

   virtual bool config(jccl::ConfigChunkPtr chunk)
   {
      boost::ignore_unused_variable_warning(chunk);
      return true;
   }

   /**
    * Get the interval that will be used for syncronization while only sharing 
    * keyboard data across the cluster.
    */
   vpr::Interval getSyncTime()
   {
      return mSyncTime;
   }  

   /**
    * Is the given key pressed?
    * @return The number of times the key was pressed since last update.
    */
   int keyPressed(gadget::Keys keyId)
   {
      return mCurKeys[keyId];
   }

   /**
    * Checks for the given modifier key pressed only.
    * @return true if key pressed exclusively.
    */
   bool modifierOnly(gadget::Keys modKey);

   std::string getKeyName(gadget::Keys keyId);

   /**
    * Returns a copy of the current queue of events for this window.
    */
   EventQueue getEventQueue();

   /**
    * (0,*): Copy of keys for this frame that the user reads from between
    * updates.
    */
   int mCurKeys[gadget::LAST_KEY];

protected:
   /**
    * Adds the given event object to the in-progress queue.
    */
   void addEvent(gadget::EventPtr e);

   /**
    * Copies the in-progress event queue into the current (i.e., user) queue
    * and wipes out the in-progress queue.  This should only be called when
    * it is time to synchronize the data for the current frame.
    *
    * @post mCurEventQueue gets a copy of mWorkingEventQueue at the time of
    *       invocation.  mWorkingEventQueue is emptied.
    */
   void updateEventQueue();

   EventQueue mCurEventQueue;     /**< Queue of events returned to users. */
   vpr::Mutex mCurEventQueueLock;

   EventQueue mWorkingEventQueue; /**< In-progress queue of events. */
   vpr::Mutex mWorkingEventQueueLock;

   // We have to create a Interval that the user can use across the cluster to syncronize
   // their applications navigation. We would like to use the timestamps from the event queue
   // but we only get events during frames that have key presses and mouse motion. 
   vpr::Interval mSyncTime;      /**< Holds an Interval that is syncrnized across the cluster */
};

} // end namespace


#endif
