#ifndef _STRING_SUBJECT_IMPL_H_
#define _STRING_SUBJECT_IMPL_H_

#include <tweek/tweekConfig.h>

#include <string>

#include <vpr/vpr.h>
#include <vpr/Sync/Mutex.h>
#include <tweek/CORBA/SubjectImpl.h>
#include <StringSubject.h>


namespace CxxClientTest
{

/**
 * This class is an extension to the base Tweek SubjectImpl class.  It uses
 * multiple inheritance with that class and with the generated CORBA class
 * corresponding to the IDL for StringSubject.
 */
class StringSubjectImpl : public POA_CxxClientTest::StringSubject,
                          public tweek::SubjectImpl
{
public:
   StringSubjectImpl()
      : tweek::SubjectImpl(), mValue("")
   {
      /* Do nothing. */ ;
   }

   virtual ~StringSubjectImpl()
   {
      /* Do nothing. */ ;
   }

   /**
    * Sets this subject's internal value.
    */
   virtual void setValue(const char* value);

   /**
    * Returns this subject's internal value.
    */
   virtual char* getValue();

   /**
    * This overriding method is needed so that the correct type is returned
    * when the _this() method is invoked.  Without this method, an object of
    * type tweek::Subject_ptr would be returned.
    *
    * XXX: It may be possible to remove this requirement in the future.
    */
   CxxClientTest::StringSubject_ptr _this()
   {
      return POA_CxxClientTest::StringSubject::_this();
   }

private:
   std::string mValue;      /**< Our value */
   vpr::Mutex  mValueLock;  /**< A mutex to protect mValue accesses */
};

} // End of CxxClientTest namespace


#endif /* _STRING_SUBJECT_IMPL_H_ */
