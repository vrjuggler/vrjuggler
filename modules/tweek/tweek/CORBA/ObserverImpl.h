#ifndef _TWEEK_CORBA_OBSERVER_IMPL_H_
#define _TWEEK_CORBA_OBSERVER_IMPL_H_

#include <tweek/tweekConfig.h>

#include <tweek/CORBA/Observer.h>


namespace tweek
{

/**
 * CORBA servant for the tweek::Observer interface (the implementation of the
 * interface).  Methods here should be invoked only via a reference to an
 * Observer object (Observer_ptr, Observer_var).
 */
class ObserverImpl : public POA_tweek::Observer,
                     public PortableServer::RefCountServantBase
{
public:
   ObserverImpl ()
   {
   }

   virtual ~ObserverImpl ()
   {
   }

   virtual void update();

private:
};

} // End of tweek namespace


#endif /* _TWEEK_CORBA_OBSERVER_IMPL_H_ */
