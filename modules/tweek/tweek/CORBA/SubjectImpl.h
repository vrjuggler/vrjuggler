#ifndef _TWEEK_CORBA_SUBJECT_IMPL_H_
#define _TWEEK_CORBA_SUBJECT_IMPL_H_

#include <tweek/tweekConfig.h>

#include <vector>
#include <vpr/Sync/Mutex.h>

#include <tweek/CORBA/Subject.h>
#include <tweek/CORBA/Observer.h>


namespace tweek
{

/**
 * CORBA servant for the tweek::Subject interface (the implementation of the
 * interface).  Methods here should be invoked only via a reference to an
 * Subject object (Subject_ptr, Subject_var).
 */
class SubjectImpl : public POA_tweek::Subject,
                    public PortableServer::RefCountServantBase
{
public:
   SubjectImpl ()
   {
   }

   virtual ~SubjectImpl ()
   {
   }

   virtual void attach(Observer_ptr o);

   virtual void detach(Observer_ptr o);

   virtual void notify();

private:
   std::vector<Observer_ptr> m_observers;
   vpr::Mutex m_observers_mutex;
};

} // End of tweek namespace


#endif /* _TWEEK_CORBA_SUBJECT_IMPL_H_ */
