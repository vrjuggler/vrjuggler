#ifndef _SLIDER_SUBJECT_IMPL_H_
#define _SLIDER_SUBJECT_IMPL_H_

#include <tweek/tweekConfig.h>

#include <vector>

#include <tweek/CORBA/SubjectImpl.h>
#include <SliderSubject.h>


namespace networktest
{

/**
 * This class is an extension to the base Tweek SubjectImpl class.  It uses
 * multiple inheritance with that class and with the generated CORBA class
 * corresponding to the IDL for SliderSubject.
 */
class SliderSubjectImpl : public POA_networktest::SliderSubject,
                          public tweek::SubjectImpl
{
public:
   SliderSubjectImpl (void)
      : tweek::SubjectImpl(), mValue(0)
   {
      /* Do nothing. */ ;
   }

   virtual ~SliderSubjectImpl (void)
   {
      /* Do nothing. */ ;
   }

   /**
    * Sets this subject's internal value.
    */
   virtual void setValue(long value);

   /**
    * Returns this subject's internal value.
    */
   virtual long getValue(void);

   /**
    * This overriding method is needed so that the correct type is returned
    * when the _this() method is invoked.  Without this method, an object of
    * type tweek::Subject_ptr would be returned.
    *
    * XXX: It may be possible to remove this requirement in the future.
    */
   networktest::SliderSubject_ptr _this (void)
   {
      return POA_networktest::SliderSubject::_this();
   }

private:
   long mValue;   /**< Our value */
};

} // End of networktest namespace


#endif /* _SLIDER_SUBJECT_IMPL_H_ */
