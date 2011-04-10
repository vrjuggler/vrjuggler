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


namespace gadget
{

namespace event
{

/** @name Click Event Tag */
//@{
/**
 * The generic event tag for mouse/digital clicks.
 *
 * @since 2.1.16
 */
template<unsigned ClickCount> struct click_tag {};

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

/**
 * A specialization of gadget::event::DataExaminer for all instantiations of
 * gadget::event::click_tag.
 *
 * @tparam ClickCount     The number of clicks the designate an event.
 * @tparam DataType       The type of a sample received from a device proxy.
 * @tparam CollectionType The base class that handles event collection.
 *
 * @since 2.1.16
 */
template<unsigned ClickCount, typename DataType, typename CollectionType>
class DataExaminer<click_tag<ClickCount>, DataType, CollectionType>
   : public CollectionType
{
public:
   DataExaminer()
      : mClickTime(10)
   {
      /* Do nothing. */ ;
   }

   void examine(const DataType& d)
   {
      // TEST: Only record double-click events.
      if (ClickCount == 2)
      {
         addEvent(d);
      }
   }

   void setClickTime(const unsigned long clickTime)
   {
      mClickTime = clickTime;
   }

private:
   unsigned long mClickTime;
};

/**
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
 * User-level code will probably end up using something similar to the
 * following:
 *
 * \code
 * typedef gadget::DigitalClickEventInterface<1> single_click_iface_type;
 * typedef gadget::DigitalClickEventInterface<2> double_click_iface_type;
 * typedef gadget::DigitalClickEventInterface<3> triple_click_iface_type;
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

   typedef typename
      boost::mpl::transform<
           boost::mpl::range_c<unsigned, 1, MaxClicks + 1>
         , F<boost::mpl::_1>
         , boost::mpl::back_inserter< boost::mpl::vector<> >
      >::type
   type;
};

}

}


#endif /* _GADGET_EVENT_CLICK_EVENT_H_ */
