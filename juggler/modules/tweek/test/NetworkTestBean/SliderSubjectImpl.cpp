#include <vpr/Util/Debug.h>
#include <SliderSubjectImpl.h>


namespace networktest
{

void SliderSubjectImpl::setValue (long value)
{
   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
      << "Setting m_value to " << value << std::endl << vprDEBUG_FLUSH;
   m_value = value;

   // Notify any observers that our value has changed.  This is very
   // important.
   SubjectImpl::notify();
}

long SliderSubjectImpl::getValue ()
{
   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
      << "Returning " << m_value << " to caller\n" << vprDEBUG_FLUSH;
   return m_value;
}

} // End networktest namespace
