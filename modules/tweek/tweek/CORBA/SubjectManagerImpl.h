#ifndef _TWEEK_SUBJECT_MANAGER_IMPL_H_
#define _TWEEK_SUBJECT_MANAGER_IMPL_H_

#include <tweek/tweekConfig.h>

#include <tweek/CORBA/SubjectManager.h>


namespace tweek
{

class SubjectManagerImpl : public POA_tweek::SubjectManager
{
public:
   virtual Subject_ptr getSubject (const char* name);
};

} // End of tweek namespace

#endif /* _TWEEK_SUBJECT_MANAGER_IMPL_H_ */
