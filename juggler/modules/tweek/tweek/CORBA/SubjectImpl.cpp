/***************** <Tweek heading BEGIN do not edit this line> ****************
 * Tweek
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 ***************** <Tweek heading END do not edit this line> *****************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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
