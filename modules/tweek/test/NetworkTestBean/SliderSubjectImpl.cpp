#include <SliderSubjectImpl.h>


namespace networktest
{

void SliderSubjectImpl::setValue (long value)
{
   m_value = value;
   notify();
}

long SliderSubjectImpl::getValue ()
{
   return m_value;
}

} // End networktest namespace
