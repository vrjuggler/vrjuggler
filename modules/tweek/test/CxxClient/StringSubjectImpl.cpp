#include <vpr/Sync/Guard.h>
#include <vpr/Util/Debug.h>
#include <StringSubjectImpl.h>


namespace CxxClientTest
{

void StringSubjectImpl::setValue(const char* value)
{
   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
      << "Setting mValue to '" << value << "'\n" << vprDEBUG_FLUSH;

   {
      vpr::Guard<vpr::Mutex> val_guard(mValueLock);
      mValue = std::string(value);
   }

   // Notify any observers that our value has changed.  This is very
   // important.
   tweek::SubjectImpl::notify();
}

char* StringSubjectImpl::getValue()
{
   vpr::Guard<vpr::Mutex> val_guard(mValueLock);
   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
      << "Returning '" << mValue << "' to caller\n" << vprDEBUG_FLUSH;
   return CORBA::string_dup(mValue.c_str());
}

} // End CxxClientTest namespace
