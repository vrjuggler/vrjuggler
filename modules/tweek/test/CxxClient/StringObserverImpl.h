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

   virtual ~StringObserverImpl()
   {
      mSubject->detach(_this());
   }

   virtual void update();

private:
   CxxClientTest::StringSubject_var mSubject;
};


#endif /* _STRING_OBSERVER_IMPL_H_ */
