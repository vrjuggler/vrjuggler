#ifndef _GADGET_EVENT_PROXY_SETTER_H_
#define _GADGET_EVENT_PROXY_SETTER_H_


namespace gadget
{

namespace event
{

namespace detail
{

/** \struct ProxySetter ProxySetter.h gadget/Event/ProxySetter.h
 *
 * This type is for use with \c boost::mpl::for_each<T>() when an event
 * interface implementation needs to associate its proxy with all event data
 * examiners.
 *
 * @see gadget::event::MultiEventGenerator
 * @see gadget::event::DataExaminer
 *
 * @since 2.1.21
 */
template<typename GeneratorPtr, typename ProxyPtr>
struct ProxySetter
{
   ProxySetter(const GeneratorPtr& generator, const ProxyPtr& proxy)
      : generator(generator)
      , proxy(proxy)
   {
      /* Do nothing. */ ;
   }

   template<typename EventTag>
   void operator()(const EventTag&)
   {
      generator->template getExaminer<EventTag>().setProxy(proxy);
   }

   GeneratorPtr generator;
   ProxyPtr     proxy;
};

}

}

}


#endif /* _GADGET_EVENT_PROXY_SETTER_H_ */
