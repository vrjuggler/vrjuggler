#include <vpr/Util/Debug.h>
#include <SliderSubjectImpl.h>


namespace networktest
{

void SliderSubjectImpl::setValue (long value)
{
   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
      << "Setting mValue to " << value << std::endl << vprDEBUG_FLUSH;
   mValue = value;

   // Notify any observers that our value has changed.  This is very
   // important.
   tweek::SubjectImpl::notify();
}

long SliderSubjectImpl::getValue ()
{
   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
      << "Returning " << mValue << " to caller\n" << vprDEBUG_FLUSH;
   return mValue;
}

} // End networktest namespace
