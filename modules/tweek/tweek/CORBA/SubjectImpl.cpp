/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <tweek/tweekConfig.h>

#include <algorithm>

#include <vpr/Sync/Mutex.h>
#include <vpr/Util/Debug.h>

#include <tweek/Util/Debug.h>
#include <tweek/CORBA/CorbaHelpers.h>
#include <tweek/CORBA/SubjectImpl.h>


namespace tweek
{

void SubjectImpl::attach(Observer_ptr o) throw(CORBA::SystemException)
{
   vpr::Guard<vpr::Mutex> guard(m_observers_mutex);
   vprDEBUG(tweekDBG_CORBA, vprDBG_STATE_LVL) << "Attaching observer\n"
                                              << vprDEBUG_FLUSH;
   m_observers.push_back(Observer::_duplicate(o));
}

template<class T>
struct RemovePred
{
   RemovePred (T o)
   {
      obj = o;
   }

   bool operator() (T thingy)
   {
      return obj->_is_equivalent(thingy);
   }

   T obj;
};

void SubjectImpl::detach(Observer_ptr o) throw(CORBA::SystemException)
{
   vpr::Guard<vpr::Mutex> guard(m_observers_mutex);
   observer_vec_t::iterator i;

   i = std::remove_if(m_observers.begin(), m_observers.end(),
                      RemovePred<Observer_ptr>(o));

   if ( i != m_observers.end() )
   {
      vprDEBUG(tweekDBG_CORBA, vprDBG_STATE_LVL) << "Removing observer\n"
                                                 << vprDEBUG_FLUSH;
   }

   m_observers.erase(i, m_observers.end());
}

void SubjectImpl::notify() throw(CORBA::SystemException)
{
   vpr::Guard<vpr::Mutex> guard(m_observers_mutex);
   observer_vec_t::iterator i;

   vprDEBUG(tweekDBG_CORBA, vprDBG_STATE_LVL) << "Notifying observers\n"
                                              << vprDEBUG_FLUSH;

   for ( i = m_observers.begin(); i != m_observers.end(); i++ )
   {
      try
      {
         (*i)->update();
      }
      catch (CORBA::SystemException& ex)
      {
         vprDEBUG(tweekDBG_CORBA, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING")
            << ": [tweek::SubjectImpl::notify()] "
            << "Caught CORBA system exception while notifying observer:\n"
            << vprDEBUG_FLUSH;
         printSystemException(ex, vprDBG_WARNING_LVL);
      }
   }

   vprDEBUG(tweekDBG_CORBA, vprDBG_STATE_LVL) << "Done notifying observers\n"
                                              << vprDEBUG_FLUSH;
}

// ============================================================================
// Protected methods follow.
// ============================================================================

SubjectImpl::SubjectImpl(const SubjectImpl& s)
   :
#if defined(TWEEK_USE_OMNIORB)
     omniServant(s)
   , tweek::_impl_Subject(s)
   ,
#elif defined(TWEEK_USE_TAO)
     TAO_Abstract_ServantBase(s)
   ,
#endif
     PortableServer::ServantBase(s)
   , POA_tweek::Subject(s)
   , PortableServer::RefCountServantBase(s)
{
   /* Do nothing. */ ;
}

} // End of tweek namespace
