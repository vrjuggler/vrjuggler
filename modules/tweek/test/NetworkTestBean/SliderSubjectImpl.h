#ifndef _SLIDER_SUBJECT_IMPL_H_
#define _SLIDER_SUBJECT_IMPL_H_

#include <tweek/tweekConfig.h>

#include <vector>

#include <tweek/CORBA/SubjectImpl.h>
#include <tweek/CORBA/Observer.h>
#include <SliderSubject.h>


namespace networktest
{

/**
 */
class SliderSubjectImpl : public POA_networktest::SliderSubject,
                          public tweek::SubjectImpl
{
public:
   SliderSubjectImpl (void)
      : tweek::SubjectImpl()
   {
      m_value = 0;
   }

   virtual ~SliderSubjectImpl (void)
   {
      /* Do nothing. */ ;
   }

   virtual void setValue(long value);

   virtual long getValue(void);

   networktest::SliderSubject_ptr _this (void)
   {
      return SliderSubject::_this();
   }

private:
   long m_value;
};

} // End of networktest namespace


#endif /* _SLIDER_SUBJECT_IMPL_H_ */
