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

#ifndef _GADGET_EVENT_CLICK_EVENT_H_
#define _GADGET_EVENT_CLICK_EVENT_H_

#include <gadget/gadgetConfig.h>

#include <vector>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/range_c.hpp>

#include <gadget/Event/DataExaminer.h>
#include <gadget/Event/ClickAnalyzer.h>


namespace gadget
{

namespace event
{

namespace detail
{

/** \struct ClickTimeSetter ClickEvent.h gadget/Event/ClickEvent.h
 *
 * This type is for use with \c boost::mpl::for_each<T>() when an event
 * interface implementation needs to set the maximum time period between
 * mouse click events.
 *
 * @since 2.1.23
 */
template<typename GeneratorPtr>
struct ClickTimeSetter
{
   ClickTimeSetter(const GeneratorPtr& generator,
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

   GeneratorPtr        generator;
   const unsigned long clickTime;
};

}

/** @name Click Event Tag */
//@{
/**
 * The generic event tag for mouse/digital clicks.
 *
 * @since 2.1.16
 */
template<unsigned ClickCount> struct click_tag : base_event_tag {};

/**
 * The event tag for a mouse/digital single click event.
 *
 * @since 2.1.16
 */
typedef click_tag<1> single_click_tag;

/**
 * The event tag for a mouse/digital double-click event.
 *
 * @since 2.1.16
 */
typedef click_tag<2> double_click_tag;
//@}

/** \class NewClickAnalyzer ClickEvent.h gadget/Event/ClickEvent.h
 *
 * @since 2.1.20
 */
template<unsigned int ClickCount>
class NewClickAnalyzer
{
   BOOST_MPL_ASSERT_RELATION(ClickCount, >, 0);

public:
   static const unsigned int sClickCount = ClickCount;

   NewClickAnalyzer()
      : mClickTime(300)
   {
      mEvents.resize(sClickCount);
   }

   /**
    * Adds the given click event to this analyzer. If the number of events
    * reaches the required number, then analysis is performed on all recorded
    * click events.
    */
   bool addClickEvent(const ClickEvent& clickEvent)
   {
      bool event_generated(false);

      // Check for expired events.
      if (! mEvents.empty())
      {
         if (clickEvent.time - mEvents.back().time > mClickTime)
         {
            reset();
         }
      }

      mEvents.push_back(clickEvent);

      if (mEvents.size() == sClickCount)
      {
         // Compare the recorded click events.
         detail::EventComparator compare(mEvents[0]);
         std::for_each(mEvents.begin(), mEvents.end(),
                       boost::bind<void>(boost::ref(compare), _1));

         // If all the events are equivalent (meaning that they satisfy the
         // criteria for proper comparison), then we ensure that the maximum
         // time difference between click events is within the configured time
         // range.
         if (compare.allEquivalent && mClickTime >= compare.maxClickDiff)
         {
            event_generated = true;
         }

         reset();
      }

      return event_generated;
   }

   void setClickTime(const unsigned long clickTime)
   {
      mClickTime = clickTime;
   }

   void reset()
   {
      mEvents.clear();
   }

private:
   /**
    * The maximum time, measured in milliseconds, that can elapse between
    * events in order to generate a multi-click event.
    */
   unsigned long mClickTime;

   /**
    * Events stored for multi-click analysis.
    */
   std::vector<ClickEvent> mEvents;
};

/**
 * A specialization of gadget::event::DataExaminer for all instantiations of
 * gadget::event::click_tag when gadget::EventPtr is the raw data type. In
 * other words, this is the data examiner for mouse events. An instantiation
 * of this type is tied to a compile-time constant specifying the click count.
 * As such, a given instance can be used for N-click events for any value N
 * (as set by the \c ClickCount template parameter).
 *
 * @tparam ClickCount The number of clicks that designate an event.
 *
 * @since 2.1.16
 */
template<unsigned ClickCount>
class DataExaminer<click_tag<ClickCount>, EventPtr>
   : public BaseExaminer<EventPtr>
{
public:
   DataExaminer()
   {
      /* Do nothing. */ ;
   }

   void examine(const EventPtr& event, bool& consumed)
   {
      if (! consumed && event->type() == MouseButtonReleaseEvent)
      {
         MouseEventPtr mouse_event(
            boost::dynamic_pointer_cast<MouseEvent>(event)
         );

         const ClickEvent click_event(mouse_event->getButton(),
                                      mouse_event->getX(),
                                      mouse_event->getY(),
                                      mouse_event->time());

         const bool have_event(mAnalyzer.addClickEvent(click_event));

         if (have_event)
         {
            this->addEvent(event, consumed);
         }
      }
   }

   void setClickTime(const unsigned long clickTime)
   {
      mAnalyzer.setClickTime(clickTime);
   }

private:
   NewClickAnalyzer<ClickCount> mAnalyzer;
};

/**
 * A specialization of gadget::event::DataExaminer for all instantiations of
 * gadget::event::click_tag when gadget::DigitaiState::State is the raw data
 * type. In other words, this is the data examiner for digital device events.
 * An instantiation of this type is tied to a compile-time constant
 * specifying the click count. As such, a given instance can be used for
 * N-click events for any value N (as set by the \c ClickCount template
 * parameter).
 *
 * @tparam ClickCount The number of clicks that designate an event.
 *
 * @since 2.1.16
 */
template<unsigned ClickCount>
class DataExaminer<click_tag<ClickCount>, DigitalState::State>
   : public BaseExaminer<DigitalState::State>
{
public:
   DataExaminer()
      : mLastState(DigitalState::OFF)
   {
      /* Do nothing. */ ;
   }

   void examine(const DigitalState::State data, bool& consumed)
   {
      // If the device unit state toggled from ON to OFF, then we treat that
      // as a click.
      if (DigitalState::OFF == data && DigitalState::ON == mLastState)
      {
         if (! consumed)
         {
            const ClickEvent click_event(vpr::Interval::now().msec());
            const bool have_event(mAnalyzer.addClickEvent(click_event));

            if (have_event)
            {
               this->addEvent(data, consumed);
            }
         }
      }

      mLastState = data;
   }

   void setClickTime(const unsigned long clickTime)
   {
      mAnalyzer.setClickTime(clickTime);
   }

private:
   DigitalState::State          mLastState;
   NewClickAnalyzer<ClickCount> mAnalyzer;
};

/** \struct ClickMaker ClickEvent.h gadget/Event/ClickEvent.h
 *
 * A metafunction that produces an instantiation of the variadic type
 * boost::mpl::vector containing instantiations of click_tag for click counts
 * in the range [1,MaxClicks]. This metafunction is intended for use with
 * gadget::MultiEventInterface as a means to define the event tag sequence
 * easily.
 *
 * The following demonstrates a rather direct use of this metafunction:
 *
 * \code
 * typedef gadget::event::ClickMaker<2>::type event_tags;
 * typedef
 *    gadget::MultiEventGenerator<
 *         gadget::DigitalProxy
 *       , event_tags
 *       , gadget::event::all_events_tag
 *       , gadget::event::immediate_tag
 *    >
 * generator_type;
 * typedef
 *    gadget::MultiEventInterface<
 *         gadget::DigitalProxy
 *       , event_tags
 *       , generator_type
 *    >::type
 * iface_type;
 * \endcode
 *
 * @tparam MaxClicks The maximum number of clicks. This must be greater than 0.
 *
 * @since 2.1.16
 */
template<unsigned MaxClicks>
struct ClickMaker
{
   BOOST_MPL_ASSERT_RELATION(MaxClicks, >, 0);

   template<typename T>
   struct F
   {
      typedef click_tag<T::value> type;
   };

   // boost::mpl::reverse_transform is used here to sort the event tags such
   // that events with higher click counts are given precendence over those
   // with lower click counts. For example, if the data examiner determines
   // that a triple-click event has been generated, then no double-click or
   // single click events should be generated for that occurrence.
   typedef typename
      boost::mpl::reverse_transform<
           boost::mpl::range_c<unsigned, 1, MaxClicks + 1>
         , F<boost::mpl::_1>
         , boost::mpl::back_inserter< boost::mpl::vector<> >
      >::type
   type;
};

}

}


#endif /* _GADGET_EVENT_CLICK_EVENT_H_ */
