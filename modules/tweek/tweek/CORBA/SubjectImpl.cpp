#include <tweek/tweekConfig.h>

#include <vpr/Sync/Mutex.h>
#include <vpr/Util/Debug.h>

#include <tweek/CORBA/SubjectImpl.h>


namespace tweek
{

void SubjectImpl::attach (Observer_ptr o)
{
   vpr::Guard<vpr::Mutex> guard(m_observers_mutex);
   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "Attaching observer\n"
                                          << vprDEBUG_FLUSH;
   m_observers.push_back(Observer::_duplicate(o));
}

void SubjectImpl::detach (Observer_ptr o)
{
   vpr::Guard<vpr::Mutex> guard(m_observers_mutex);
   observer_vec_t::iterator i;

   for ( i = m_observers.begin(); i != m_observers.end(); i++ )
   {
      if ( o->_is_equivalent(*i) )
      {
         observer_vec_t::iterator j = i;
         i++;

         vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "Detaching observer\n"
                                                << vprDEBUG_FLUSH;
         CORBA::release(*j);
         m_observers.erase(j);
      }
   }
}

void SubjectImpl::notify ()
{
   vpr::Guard<vpr::Mutex> guard(m_observers_mutex);
   observer_vec_t::iterator i;

   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "Notifying observers\n"
                                          << vprDEBUG_FLUSH;

   for ( i = m_observers.begin(); i != m_observers.end(); i++ )
   {
      (*i)->update();
   }

   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "Done notifying observers\n"
                                          << vprDEBUG_FLUSH;
}

} // End of tweek namespace
