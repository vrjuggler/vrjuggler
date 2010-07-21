#ifndef _GADGET_ABSTRACT_EVENT_INTERFACE_H_
#define _GADGET_ABSTRACT_EVENT_INTERFACE_H_

#include <gadget/gadgetConfig.h>

#include <string>
#include <vector>
#include <boost/noncopyable.hpp>

#include <gadget/Event/EventGeneratorPtr.h>


namespace gadget
{

/** \class AbstractEventInterface AbstractEventInterface.h gadget/Event/AbstractEventInterface.h
 *
 * @since 2.1.2
 */
class GADGET_CLASS_API AbstractEventInterface : private boost::noncopyable
{
protected:
   AbstractEventInterface();

public:
   virtual ~AbstractEventInterface();

   const std::string& getProxyName() const
   {
      return mProxyName;
   }

   virtual bool isConnected() const = 0;

   virtual void refresh() = 0;

   virtual const EventGeneratorPtr& getEventGenerator() const = 0;

   static void refreshAllInterfaces();

protected:
   void setProxyName(const std::string& proxyName)
   {
      mProxyName = proxyName;
      refresh();
   }

private:
   std::string mProxyName;

   static std::vector<AbstractEventInterface*> sAllocatedInterfaces;
};

}


#endif /* _GADGET_ABSTRACT_EVENT_INTERFACE_H_ */
