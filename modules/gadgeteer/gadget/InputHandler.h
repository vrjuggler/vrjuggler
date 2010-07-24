#ifndef _GADGET_INPUT_HANDLER_H_
#define _GADGET_INPUT_HANDLER_H_

#include <gadget/gadgetConfig.h>

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/version.hpp>

#if BOOST_VERSION >= 103600
#  include <boost/unordered_map.hpp>
#else
#  include <map>
#endif

#include <gadget/Event/EventGeneratorPtr.h>
#include <gadget/InputHandlerPtr.h>


namespace gadget
{

class AbstractEventInterface;

/** \class InputHandler InputHandler.h gadget/InputHandler.h
 *
 * @since 2.1.2
 */
class GADGET_CLASS_API InputHandler
   : public boost::enable_shared_from_this<InputHandler>
   , private boost::noncopyable
{
private:
   InputHandler();

public:
   static InputHandlerPtr create();

   ~InputHandler();

   /** @name Interface Registration */
   //@{
   /**
    * Registers the given event interface so that its even tgenerator will
    * emit events while the thread managed by this object executes.
    *
    * @pre \p iface will return a valid event generator when asked to do so.
    *
    * @param iface The abstract event interface to register.
    *
    * @see AbstractEventInterface::getEventGenerator()
    */
   void registerPeriodicInterface(AbstractEventInterface* iface);

   void unregisterPeriodicInterface(AbstractEventInterface* iface);

   /**
    * Registers the given event interface so that its even tgenerator will
    * emit events once per frame when the Input Manager invokes the sync()
    * method.
    *
    * @pre \p iface will return a valid event generator when asked to do so.
    *
    * @param iface The abstract event interface to register.
    *
    * @see AbstractEventInterface::getEventGenerator()
    * @see InputManager::updateAllDevices()
    * @see sync()
    */
   void registerSynchronizedInterface(AbstractEventInterface* iface);

   void unregisterSynchronizedInterface(AbstractEventInterface* iface);
   //@}

   /**
    * Causes all registered synchronized event generators to emit their
    * pending events.
    *
    * @since 2.1.3
    */
   void sync();

private:
#if BOOST_VERSION >= 103600
   typedef boost::unordered_map<AbstractEventInterface*, EventGeneratorPtr>
      iface_map_type;
#else
   typedef std::map<AbstractEventInterface*, EventGeneratorPtr> iface_map_type;
#endif

   iface_map_type mPeriodicIfaces;
   iface_map_type mSyncIfaces;
};

}


#endif /* _GADGET_INPUT_HANDLER_H_ */
