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

#ifndef _GADGET_EVENT_CLICK_ANALYZER_H_
#define _GADGET_EVENT_CLICK_ANALYZER_H_

#include <gadget/gadgetConfig.h>

#include <algorithm>
#include <vector>
#include <boost/static_assert.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>

#include <gadget/Type/KeyboardMouse/Keys.h>


namespace gadget
{

namespace event
{

/** \struct ClickEvent ClickAnalyzer.h gadget/Event/ClickAnalyzer.h
 *
 * @since 2.1.6
 */
struct ClickEvent
{
   ClickEvent()
      : button(KEY_NONE)
      , x(0)
      , y(0)
      , time(0)
   {
      /* Do nothing. */ ;
   }

   explicit ClickEvent(const unsigned long time_)
      : button(KEY_NONE)
      , x(0)
      , y(0)
      , time(time_)
   {
      /* Do nothing. */ ;
   }

   ClickEvent(const gadget::Keys button_, const int x_, const int y_,
              const unsigned long time_)
      : button(button_)
      , x(x_)
      , y(y_)
      , time(time_)
   {
      /* Do nothing. */ ;
   }

   gadget::Keys button;
   int x;
   int y;
   unsigned long time;  /**< Event time, measured in milliseconds */
};

namespace detail
{

/** \struct EventComparator ClickAnalyzer.h gadget/Event/ClickAnalyzer.h
 *
 * @since 2.1.6
 */
struct EventComparator
{
   EventComparator(const ClickEvent& baseEvent)
      : button(baseEvent.button)
      , x(baseEvent.x)
      , y(baseEvent.y)
      , allEquivalent(true)
      , lastClickTime(baseEvent.time)
      , maxClickDiff(0)
   {
      /* Do nothing. */ ;
   }

   void operator()(const ClickEvent& event)
   {
      allEquivalent = allEquivalent && button == event.button &&
                         x == event.x && y == event.y;
      maxClickDiff  =
         std::max<unsigned long>(maxClickDiff, event.time - lastClickTime);
      lastClickTime = event.time;
   }

   const gadget::Keys button;
   const int x;
   const int y;

   /** @name Comparison Results */
   //@{
   bool          allEquivalent;
   unsigned long lastClickTime; /**< Time of last click event (milliseconds) */
   unsigned long maxClickDiff;  /**< Maximum click delta (milliseconds) */
   //@}
};

}

/** \class ClickAnalyzer ClickAnalyzer.h gadget/Event/ClickAnalyzer.h
 *
 * @since 2.1.6
 */
template<unsigned int ClickCount>
class ClickAnalyzer
{
   BOOST_STATIC_ASSERT(ClickCount > 0);

protected:
   ClickAnalyzer()
      : mClickTime(0.01f)
   {
      mEvents.resize(sClickCount);
   }

public:
   virtual ~ClickAnalyzer()
   {
      /* Do nothing. */ ;
   }

   /**
    * Adds the given click event to this analyzer. If the number of events
    * reaches the required number, then analysis is performed on all recorded
    * click events.
    */
   void addClickEvent(const event::ClickEvent& clickEvent)
   {
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
            clickGenerated(clickEvent);
         }

         reset();
      }
   }

   void setClickTime(const unsigned long clickTime)
   {
      mClickTime = clickTime;
   }

   void reset()
   {
      mEvents.clear();
   }

protected:
   virtual void clickGenerated(const event::ClickEvent& clickEvent) = 0;

   const std::vector<event::ClickEvent>& getEvents() const
   {
      return mEvents;
   }

private:
   static const unsigned int sClickCount = ClickCount;

   unsigned long mClickTime;
   std::vector<event::ClickEvent> mEvents;
};

}

}


#endif /* _GADGET_EVENT_CLICK_ANALYZER_H_ */
