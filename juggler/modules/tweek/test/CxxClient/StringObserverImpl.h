#ifndef _STRING_OBSERVER_IMPL_H_
#define _STRING_OBSERVER_IMPL_H_

#include <tweek/CORBA/Observer.h>

#include <StringSubject.h>


class StringObserverImpl : public POA_tweek::Observer
{
public:
   StringObserverImpl(CxxClientTest::StringSubject_var subject)
      : mSubject(subject)
   {
   }

   // XXX: It would be nice if the work done by detach() could be done in
   // the destructor, but that does not seem to satisfy the POA.  Whatever...
   virtual ~StringObserverImpl()
   {
   }

   virtual void update();

   void detach()
   {
      mSubject->detach(_this());
   }

private:
   CxxClientTest::StringSubject_var mSubject;
};


#endif /* _STRING_OBSERVER_IMPL_H_ */
