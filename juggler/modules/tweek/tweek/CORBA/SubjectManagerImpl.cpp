#include <tweek/tweekConfig.h>

#include <tweek/CORBA/SubjectManagerImpl.h>


namespace tweek
{

Subject_ptr SubjectManagerImpl::getSubject (const char* name)
{
   return Subject::_nil();
}

} // End of tweek namespace
