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

#ifndef _GADGET_MOUSE_CLICK_EVENT_INTERFACE_H_
#define _GADGET_MOUSE_CLICK_EVENT_INTERFACE_H_

#include <gadget/gadgetConfig.h>

#include <gadget/Event/ClickEvent.h>
#include <gadget/Event/MultiEventInterface.h>
#include <gadget/Event/MultiEventGenerator.h>
#include <gadget/Event/KeyboardMouse/SampleHandler.h>
#include <gadget/Type/KeyboardMouseProxy.h>


namespace gadget
{

/** \class MouseClickEventInterface MouseClickEventInterface.h gadget/Event/MouseClickEventInterface.h
 *
 * A specialized keyboard/mouse event interface type for detecting
 * click events from a mouse. For a given instantiation, events for 1 to
 * \c ClickCount clicks are generated. The callback signature is the same as
 * for gadget::KeyboardMouseEventInterface, but the event object passed to the
 * callback is guaranteed to point to an instance of gadget::MouseEvent. This
 * means that the received gadget::EventPtr can be downcast to
 * gadget::MouseEventPtr without performing a NULL check afterward.
 *
 * This is a multi-event interface, and callbacks are classified based on
 * event tags. The tags for this type are generated automatically based on the
 * \c ClickCount template parameter. The tag type is gadget::event::click_tag,
 * a type instantiated based on the number of clicks in the event. Thus, if
 * \c ClickCount is 2, then there are two event tags that can be used:
 * gadget::event::click_tag<1> and gadget::event::click_tag<2>. There are
 * typedefs for these two types because they are so common:
 * gadget::event::single_click_tag and gadget::event::double_click_tag.
 *
 * Event tags are used to correlate event tags to callbacks. For example, if
 * an application class the following member:
 *
 * \code
 * gadget::MouseClickEventInterface<3> mClickIface;
 * \endcode
 *
 * it effectively declares interest in single, double-, and/or triple-click
 * events from a gadget::KeyboardMouseProxy. The callbacks are added as
 * follows:
 *
 * \code
 * void singleClickCallback(const gadget::EventPtr& event)
 * {
 *    gadget::MouseEventPtr mouse_event(
 *       boost::dynamic_pointer_cast<gadget::MouseEvent>(event)
 *    );
 *    // Handle a single click event...
 * }
 *
 * void doubleClickCallback(const gadget::EventPtr& event)
 * {
 *    gadget::MouseEventPtr mouse_event(
 *       boost::dynamic_pointer_cast<gadget::MouseEvent>(event)
 *    );
 *    // Handle a double-click event...
 * }
 *
 * void tripleClickCallback(const gadget::EventPtr& event)
 * {
 *    gadget::MouseEventPtr mouse_event(
 *       boost::dynamic_pointer_cast<gadget::MouseEvent>(event)
 *    );
 *    // Handle a triple-click event...
 * }
 *
 * void MyApp::init()
 * {
 *    mClickIface.init("VJKeyboard");
 *
 *    using namespace gadget::event;
 *    mClickIface.addCallback<single_click_tag>(singleClickCallback);
 *    mClickIface.addCallback<double_click_tag>(doubleClickCallback);
 *    mClickIface.addCallback<click_tag<3> >(tripleClickCallback);
 * }
 * \endcode
 *
 * Receipt of a double-click event is preceded by delivery of two single click
 * events. Whether the single click events are of interest is up to the
 * user-level code.
 *
 * @tparam ClickCount    The maximum number of clicks that can be present in a
 *                       click event. For example, a value of 2 means that
 *                       single and double-click events will be generated. A
 *                       value of 3 means that single, double-, and
 *                       triple-click events will be generated.
 * @tparam CollectionTag A tag specifyiing which event(s) will be collected by
 *                       the event generator created by this object. This must
 *                       be a valid collection tag in order for the code to
 *                       compile. This template paramter is optional, and it
 *                       defaults to gadget::event::all_events_tag.
 * @tparam GenerationTag A tag specifying how events will be emitted by the
 *                       event generator created by this object. This must be
 *                       a valid generation tag in order for the code to
 *                       compile. This template paramter is optional, and it
 *                       defaults to gadget::event::immediate_tag.
 *
 * @since 2.1.20
 */
template<unsigned int ClickCount
       , typename CollectionTag = event::all_events_tag
       , typename GenerationTag = event::immediate_tag>
class MouseClickEventInterface
   : public MultiEventInterface<
                 KeyboardMouseProxy
               , MultiEventGenerator<
                      KeyboardMouseProxy
                    , typename event::ClickMaker<ClickCount>::type
                    , CollectionTag
                    , GenerationTag
                    , event::kbd::SampleHandler
                 >
            >
{
public:
   typedef typename MouseClickEventInterface::event_interface_ base_type;
   typedef typename base_type::proxy_ptr_type proxy_ptr_type;

   /**
    * Constructor.
    *
    * @param clickTime The maximum time (measured in milliseconds) allowed
    *                  between sequential clicks for the click events to be
    *                  treated as a single multi-click event. This parameter
    *                  is optional, and it defaults to 10 milliseconds.
    *
    * @see setClickTime()
    */
   MouseClickEventInterface(const unsigned long clickTime = 10)
      : mClickTime(clickTime)
   {
      /* Do nothing. */ ;
   }

   virtual ~MouseClickEventInterface()
   {
      /* Do nothing. */ ;
   }

   /**
    * Changes the maximum click time to be the given value.
    *
    * @pre An event generator for this event interface has not been created
    *      yet. Setting this value after the event generator has been created
    *      will not influence the event creation.
    *
    * @param clickTime The maximum time (measured in milliseconds) allowed
    *                  between sequential clicks for the click events to be
    *                  treated as a single multi-click event.
    */
   void setClickTime(const unsigned long clickTime)
   {
      mClickTime = clickTime;
   }

protected:
   typedef typename base_type::generator_type     generator_type;
   typedef typename base_type::generator_ptr_type generator_ptr_type;

   EventGeneratorPtr createEventGenerator(const proxy_ptr_type& proxy)
   {
      EventGeneratorPtr base_generator(
         this->base_type::createEventGenerator(proxy)
      );

      // Downcast to our specific generator type so that we can set the
      // maximum click time that the newly created event generator will use.
      generator_ptr_type generator(
         boost::dynamic_pointer_cast<generator_type>(base_generator)
      );
      ClickTimeSetter setter(generator, mClickTime);
      boost::mpl::for_each<typename base_type::event_tags>(setter);

      return base_generator;
   }

   struct ClickTimeSetter
   {
      ClickTimeSetter(const generator_ptr_type& generator,
                      const unsigned long clickTime)
         : generator(generator)
         , clickTime(clickTime)
      {
         /* Do nothing. */ ;
      }

      template<typename EventTag>
      void operator()(const EventTag&)
      {
         generator->template getExaminer<EventTag>().setClickTime(clickTime);
      }

      generator_ptr_type  generator;
      const unsigned long clickTime;
   };

private:
   unsigned long mClickTime;    /**< Maximum click time (milliseconds) */
};

}


#endif /* _GADGET_MOUSE_CLICK_EVENT_INTERFACE_H_ */
