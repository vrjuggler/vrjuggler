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

#ifndef _GADGET_EVENT_TAGS_H_
#define _GADGET_EVENT_TAGS_H_


namespace gadget
{

namespace event
{

/**
 * @name Event Generator Tags
 *
 * Event generator tags are used to dictate when events should be emitted by
 * an event generator.
 *
 * @see gadget::BasicEventGenerator
 * @see gadget::InputHandler
 * @see gadget::InputManager
 */
//@{
/**
 * Base event generator tag.
 *
 * @since 2.1.2
 */
struct generator_tag {};

/**
 * This tag tells an event generator to create an event and emit it as soon
 * as data (a sample) is received from an input device. Events are thus
 * emitted during the input sample loop. Therefore, a callback connected to
 * an event interface may be invoked asynchronously to the application frame
 * loop (and to other callbacks connected to different devices). Devices that
 * generate samples during the frame loop, however, will cause events to be
 * emitted during the frame loop.
 *
 * @since 2.1.2
 *
 * @see gadget::BasicEventGenerator
 */
struct immediate_tag : generator_tag {};

/**
 * This tag tells an event generator to create an event and store it until
 * told to emit it at some point in the future. (Event storage behavior is
 * dictated by a collector tag.)
 *
 * @since 2.1.2
 *
 * @see gadget::BasicEventGenerator
 * @see gadget::InputHandler::registerPeriodicInterface()
 */
struct periodic_tag : generator_tag {};

/**
 * This tag tells an event generator to create an event and store it until
 * the Input Manager synchronizes all devices. Events will thereore be
 * emitted as part of the frame loop. (Event storage behavior is dictated by
 * a collector tag.)
 *
 * @since 2.1.2
 *
 * @see gadget::BasicEventGenerator
 * @see gadget::InputHandler::registerSynchronizedInterface()
 * @see gadget::InputManager::updateAllDevices()
 */
struct synchronized_tag : generator_tag {};
//@}

/**
 * @name Event Collector Tags
 *
 * Event collector tags are used to tell an event generator how many events
 * should be retained in between synchronization points. These only apply
 * when the generator tag is periodic_tag or synchronized_tag.
 *
 * @see gadget::BasicEventGenerator
 * @see gadget::InputManager::updateAllDevices()
 */
//@{
/**
 * Base event collector tag.
 *
 * @since 2.1.2
 */
struct collector_tag {};

/**
 * Collect all events created between synchronization points. When the events
 * are emitted (depending on the generator tag), all collected events will be
 * sent through individual callback invocations.
 *
 * @since 2.1.2
 *
 * @see gadget::BasicEventGenerator::emitEvents()
 */
struct all_events_tag : collector_tag {};

/**
 * Store only the most recently created event between synchronization points.
 * When the events are emitted (depending on the generator tag), only the
 * newest event will be emitted.
 *
 * @since 2.1.2
 *
 * @see gadget::BasicEventGenerator::emitEvents()
 */
struct last_event_tag : collector_tag {};
//@}

}

}

#endif /* _GADGET_EVENT_TAGS_H_ */
