#ifndef _TWEEK_SUBJECT_MANAGER_IMPL_H_
#define _TWEEK_SUBJECT_MANAGER_IMPL_H_

#include <tweek/tweekConfig.h>

#include <string>
#include <map>
#include <vpr/Sync/Mutex.h>
#include <vpr/Util/Singleton.h>

#include <tweek/CORBA/SubjectManager.h>


namespace tweek
{

class SubjectManagerImpl : public POA_tweek::SubjectManager,
                           public PortableServer::RefCountServantBase,
                           public vpr::Singleton<SubjectManagerImpl>
{
public:
   virtual void registerSubject(Subject_ptr subject, const char* name);

   virtual Subject_ptr getSubject(const char* name);

protected:
   friend class vpr::Singleton<SubjectManagerImpl>;

   /// Default constructor.
   SubjectManagerImpl (void)
   {
      /* Do nothing. */ ;
   }

   // These two have to be here because Visual C++ will try to make them
   // exported public symbols.  This causes problems because copying
   // vpr::Mutex objects is not allowed.
   SubjectManagerImpl (const SubjectManagerImpl& sm) {;}
   void operator= (const SubjectManagerImpl& sm) {;}

private:
   std::map<std::string, Subject_var> m_subjects;
   vpr::Mutex m_subjects_mutex;
};

} // End of tweek namespace

#endif /* _TWEEK_SUBJECT_MANAGER_IMPL_H_ */
