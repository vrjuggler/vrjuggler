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

#include <vpr/Sync/Guard.h>
#include <vpr/Util/Debug.h>

#include <tweek/CORBA/CorbaManager.h>
#include <tweek/CORBA/SubjectImpl.h>
#include <tweek/CORBA/SubjectManagerImpl.h>


namespace tweek
{

void SubjectManagerImpl::registerSubject (SubjectImpl* subject_servant,
                                          const char* name)
{
   // We have to register servant with POA first.
   m_corba_mgr.getChildPOA()->activate_object(subject_servant);

   registerSubject(subject_servant->_this(), name);
}

void SubjectManagerImpl::registerSubject (Subject_ptr subject, const char* name)
{
   std::string name_str(name);
   vpr::Guard<vpr::Mutex> guard(m_subjects_mutex);

   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
      << "Registering subject named '" << name << "'\n" << vprDEBUG_FLUSH;

   m_subjects[name_str] = Subject::_duplicate(subject);
}

Subject_ptr SubjectManagerImpl::getSubject (const char* name)
{
   Subject_ptr subject;
   std::string name_str(name);
   vpr::Guard<vpr::Mutex> guard(m_subjects_mutex);

   subject_map_t::iterator i;

   i = m_subjects.find(name_str);

   if ( i != m_subjects.end() )
   {
      subject = Subject::_duplicate((*i).second);
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

   return subject;
}

tweek::SubjectManager::SubjectList* SubjectManagerImpl::getAllSubjects ()
{
   subject_map_t::iterator i;
   CORBA::ULong j;

   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
      << "Constructing sequence of subjects to return to caller ...\n"
      << vprDEBUG_FLUSH;

   // Create the sequence and size it.
   tweek::SubjectManager::SubjectList* subjects =
      new tweek::SubjectManager::SubjectList();
   subjects->length(m_subjects.size());

   vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
      << "Sequence size: " << subjects->length() << std::endl
      << vprDEBUG_FLUSH;

   for ( i = m_subjects.begin(), j = 0; i != m_subjects.end(); i++, j++ )
   {
      tweek::SubjectManager::RegisteredSubject rs;
      rs.subject_name = CORBA::string_dup((*i).first.c_str());
      rs.subject_ref  = Subject::_duplicate((*i).second);

      vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
         << "Adding subject[" << j << "]: " << rs.subject_name << std::endl
         << vprDEBUG_FLUSH;

      (*subjects)[j] = rs;
   }

   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
      << "Returning all subjects to caller\n" << vprDEBUG_FLUSH;

   return subjects;
}

} // End of tweek namespace
