#include <vpr/Util/Debug.h>
#include <StringObserverImpl.h>


void StringObserverImpl::update()
{
   char* cur_value = mSubject->getValue();

   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
      << "Current string value is now '" << cur_value << "'\n"
      << vprDEBUG_FLUSH;
   delete cur_value;
}
