#ifndef _GADGET_MOUSE_MULTI_CLICK_EVENT_H_
#define _GADGET_MOUSE_MULTI_CLICK_EVENT_H_

#include <gadget/gadgetConfig.h>

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <gadget/Event/ClickAnalyzer.h>
#include <gadget/Event/EventInterface.h>
#include <gadget/Event/BasicEventGenerator.h>
#include <gadget/Event/KeyboardMouse/MouseEvent.h>
#include <gadget/Event/KeyboardMouse/SampleHandler.h>
#include <gadget/Type/KeyboardMouseProxy.h>


namespace gadget
{

/** \class MouseMultiClickEventGenerator MouseMultiClickEventInterface.h gadget/Event/MouseMultiClickEventInterface.h
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
 * @see MouseMultiClickEventInterface
 *
 * @since 2.1.6
 */
template<unsigned int ClickCount
       , typename CollectionTag
       , typename GenerationTag>
class MouseMultiClickEventGenerator
   : public BasicEventGenerator<KeyboardMouseProxy
                              , CollectionTag
                              , GenerationTag
                              , event::kbd::SampleHandler>
   , private event::ClickAnalyzer<ClickCount>
{
public:
   typedef BasicEventGenerator<
      KeyboardMouseProxy, CollectionTag, GenerationTag, event::kbd::SampleHandler
   > base_type;

   //static const unsigned int click_count = ClickCount;

protected:
   MouseMultiClickEventGenerator()
   {
      /* Do nothing. */ ;
   }

public:
   virtual ~MouseMultiClickEventGenerator()
   {
      /* Do nothing. */ ;
   }

   static boost::shared_ptr<MouseMultiClickEventGenerator> create()
   {
      return boost::shared_ptr<MouseMultiClickEventGenerator>(
                new MouseMultiClickEventGenerator()
             );
   }

   using event::ClickAnalyzer<ClickCount>::setClickTime;

protected:
   BOOST_STATIC_ASSERT(
      (boost::is_same<typename base_type::raw_data_type, EventPtr>::value)
   );

   /** @name BasicEventGenerator Interface Overrides */
   //@{
   virtual void onDataAdded(const typename base_type::raw_data_type& data)
   {
      if (data->type() == MouseButtonReleaseEvent)
      {
         mLastEvent = boost::dynamic_pointer_cast<MouseEvent>(data);
         const event::ClickEvent click_event(mLastEvent->getButton(),
                                             mLastEvent->getX(),
                                             mLastEvent->getY(),
                                             mLastEvent->time());
         this->addClickEvent(click_event);
      }
   }
   //@}

   /** @name ClickAnalyzer Interface Overrides */
   //@{
   virtual void clickGenerated(const event::ClickEvent&)
   {
      // Store or emit the result.
      this->base_type::onDataAdded(mLastEvent);
   }
   //@}

private:
   MouseEventPtr mLastEvent;
};

/** \class MouseMultiClickEventInterface MouseMultiClickEventInterface.h gadget/Event/MouseMultiClickEventInterface.h
 *
 * A specialized keyboard/mouse event interface type for detecting
 * double-click events from a mouse. The callback signature is the same as
 * for gadget::KeyboardMouseEventInterface, but the event object passed to the
 * callback is guaranteed to be gadget::MouseEvent. This means that the
 * received gadget::EventPtr can be downcast to gadget::MouseEventPtr without
 * performing a NULL check afterward.
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
class MouseMultiClickEventInterface
   : public EventInterface<KeyboardMouseProxy
                         , MouseMultiClickEventGenerator<ClickCount
                                                       , CollectionTag
                                                       , GenerationTag>
                         >
{
public:
   typedef MouseMultiClickEventGenerator<
      ClickCount, CollectionTag, GenerationTag
   > generator_type;

   typedef EventInterface<KeyboardMouseProxy, generator_type> base_type;

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
   MouseMultiClickEventInterface(const unsigned long clickTime = 10)
      : mClickTime(clickTime)
   {
      /* Do nothing. */ ;
   }

   virtual ~MouseMultiClickEventInterface()
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
    * @param clickTime The maximum time (measured in milliseconds) allowed
    *                  between sequential clicks for the click events to be
    *                  treated as a single multi-click event.
    */
   void setClickTime(const unsigned long clickTime)
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
   unsigned long mClickTime;    /**< Maximum click time (milliseconds) */
};

}


#endif /* _GADGET_MOUSE_MULTI_CLICK_EVENT_H_ */
