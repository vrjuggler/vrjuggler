#ifndef _GADGET_DIGITAL_MULTI_CLICK_EVENT_H_
#define _GADGET_DIGITAL_MULTI_CLICK_EVENT_H_

#include <gadget/gadgetConfig.h>

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <gadget/Event/ClickAnalyzer.h>
#include <gadget/Event/EventInterface.h>
#include <gadget/Event/BasicEventGenerator.h>
#include <gadget/Type/DigitalProxy.h>


namespace gadget
{

/** \class DigitalMultiClickEventGenerator DigitalMultiClickEventInterface.h gadget/Event/DigitalMultiClickEventInterface.h
 *
 * @tparam ClickCount    The number of clicks required to create a single
 *                       multi-click event. For example, a value of 2 means
 *                       that double-click events will be generated. A value
 *                       of 3 means that triple-click events will be
 *                       generated.
 * @tparam CollectionTag A tag specifyiing which event(s) will be collected by
 *                       the event generator created by this object. This must
 *                       be a valid collection tag in order for the code to
 *                       compile.
 * @tparam GenerationTag A tag specifying how events will be emitted by the
 *                       event generator created by this object. This must be
 *                       a valid generation tag in order for the code to
 *                       compile.
 *
 * @see DigitalMultiClickEventInterface
 *
 * @since 2.1.6
 */
template<unsigned int ClickCount
       , typename CollectionTag
       , typename GenerationTag>
class DigitalMultiClickEventGenerator
   : public BasicEventGenerator<DigitalProxy, CollectionTag, GenerationTag>
   , private event::ClickAnalyzer<ClickCount>
{
public:
   typedef BasicEventGenerator<
      DigitalProxy, CollectionTag, GenerationTag
   > base_type;

   //static const unsigned int click_count = ClickCount;

protected:
   DigitalMultiClickEventGenerator()
      : mLastState(DigitalState::OFF)
   {
      /* Do nothing. */ ;
   }

public:
   virtual ~DigitalMultiClickEventGenerator()
   {
      /* Do nothing. */ ;
   }

   static boost::shared_ptr<DigitalMultiClickEventGenerator> create()
   {
      return boost::shared_ptr<DigitalMultiClickEventGenerator>(
                new DigitalMultiClickEventGenerator()
             );
   }

   using event::ClickAnalyzer<ClickCount>::setClickTime;

protected:
   BOOST_STATIC_ASSERT(
      (boost::is_same<typename base_type::raw_data_type, Digital::State>::value)
   );

   /** @name BasicEventGenerator Interface Overrides */
   //@{
   virtual void onDataAdded(const typename base_type::raw_data_type& data)
   {
      // If the device unit state toggled from ON to OFF, then we treat that
      // as a click.
      if (DigitalState::OFF == data && DigitalState::ON == mLastState)
      {
         const event::ClickEvent click_event(vpr::Interval::now().secf());
         this->addClickEvent(click_event);
      }

      mLastState = data;
   }
   //@}

   /** @name ClickAnalyzer Interface Overrides */
   //@{
   virtual void clickGenerated(const event::ClickEvent& clickEvent)
   {
      // Store or emit the result.
      this->base_type::onDataAdded(DigitalState::ON);
   }
   //@}

private:
   DigitalState::State mLastState;
};

/** \class DigitalMultiClickEventInterface DigitalMultiClickEventInterface.h gadget/Event/DigitalMultiClickEventInterface.h
 *
 * A specialized keyboard/mouse event interface type for detecting
 * double-click events from a digital device. The callback signature is the
 * same as for gadget::DigitalEventInterface, although the data value passed
 * to the callback will always be gadget::DigitalState::ON.
 *
 * @tparam ClickCount    The number of clicks required to create a single
 *                       multi-click event. For example, a value of 2 means
 *                       that double-click events will be generated. A value
 *                       of 3 means that triple-click events will be
 *                       generated.
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
 * @since 2.1.6
 */
template<unsigned int ClickCount
       , typename CollectionTag = event::all_events_tag
       , typename GenerationTag = event::immediate_tag>
class DigitalMultiClickEventInterface
   : public EventInterface<DigitalProxy
                         , DigitalMultiClickEventGenerator<ClickCount
                                                         , CollectionTag
                                                         , GenerationTag>
                         >
{
public:
   typedef DigitalMultiClickEventGenerator<
      ClickCount, CollectionTag, GenerationTag
   > generator_type;

   typedef EventInterface<DigitalProxy, generator_type> base_type;

   typedef typename base_type::proxy_ptr_type proxy_ptr_type;

   /**
    * Constructor.
    *
    * @param clickTime The maximum time (measured in seconds) allowed between
    *                  sequential clicks for the click events to be treated as
    *                  a single multi-click event. This parameter is optional,
    *                  and it defaults to 10 milliseconds.
    *
    * @see setClickTime()
    */
   DigitalMultiClickEventInterface(const float clickTime = 0.01f)
      : mClickTime(clickTime)
   {
      /* Do nothing. */ ;
   }

   virtual ~DigitalMultiClickEventInterface()
   {
      /* Do nothing. */ ;
   }

   /**
    * Changes the maximum click time to be the given value.
    *
    * @pre An event generator for this event interface has not been created
    *      yet. Setting this value after the event generator has been
    *      created will not influence the event creation.
    *
    * @param clickTime The maximum time (measured in seconds) allowed between
    *                  sequential clicks for the click events to be treated as
    *                  a single multi-click event.
    */
   void setClickTime(const float clickTime)
   {
      mClickTime = clickTime;
   }

protected:
   EventGeneratorPtr createEventGenerator(const proxy_ptr_type& proxy)
   {
      EventGeneratorPtr base_generator(
         this->base_type::createEventGenerator(proxy)
      );

      // Downcast to our specific generator type so that we can set the
      // maximum click time that the newly created event generator will use.
      boost::shared_ptr<generator_type> generator(
         boost::dynamic_pointer_cast<generator_type>(base_generator)
      );
      generator->setClickTime(mClickTime);

      return base_generator;
   }

private:
   float mClickTime;    /**< Maximum click time */
};

}


#endif /* _GADGET_DIGITAL_MULTI_CLICK_EVENT_H_ */
