#include <tweek/tweekConfig.h>

#include <vpr/Sync/Guard.h>
#include <vpr/Util/Debug.h>

#include <tweek/CORBA/SubjectManagerImpl.h>


namespace tweek
{

void SubjectManagerImpl::registerSubject (Subject_ptr subject, const char* name)
{
   std::string name_str(name);
   vpr::Guard<vpr::Mutex> guard(m_subjects_mutex);

   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
      << "Registering subject named '" << name << "'\n" << vprDEBUG_FLUSH;

   m_subjects[name_str] = subject;
}

Subject_ptr SubjectManagerImpl::getSubject (const char* name)
{
   Subject_ptr subject;
   std::string name_str(name);
   vpr::Guard<vpr::Mutex> guard(m_subjects_mutex);

   std::map<std::string, Subject_var>::iterator i;

   i = m_subjects.find(name_str);
   
   if ( i != m_subjects.end() )
   {
      subject = (*i).second;
      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
         << "Returning subject named '" << name << "'\n" << vprDEBUG_FLUSH;
   }
   else
   {
      subject = Subject::_nil();
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "No registered subject named '" << name << "'!\n"
         << vprDEBUG_FLUSH;
   }

   return m_subjects[name_str];
}

} // End of tweek namespace
