#include <tweek/tweekConfig.h>

#include <vpr/Util/Debug.h>

#include <tweek/CORBA/SubjectImpl.h>


namespace tweek
{

void SubjectImpl::attach (Observer_ptr o)
{
   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "Attaching observer\n"
                                          << vprDEBUG_FLUSH;
   m_observers.push_back(o);
}

void SubjectImpl::detach (Observer_ptr o)
{
   std::vector<Observer_var>::iterator i;

   for ( i = m_observers.begin(); i != m_observers.end(); i++ )
   {
      if ( o->_is_equivalent(*i) )
      {
         std::vector<Observer_var>::iterator j = i;
         i++;

         vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "Detaching observer\n"
                                                << vprDEBUG_FLUSH;
         m_observers.erase(j);
      }
   }
}

void SubjectImpl::notify ()
{
   std::vector<Observer_var>::iterator i;

   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "Notifying observers\n"
                                          << vprDEBUG_FLUSH;

   for ( i = m_observers.begin(); i != m_observers.end(); i++ )
   {
      (*i)->update();
   }
}

} // End of tweek namespace
