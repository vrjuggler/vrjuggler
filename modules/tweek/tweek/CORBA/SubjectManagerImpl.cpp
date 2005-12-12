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
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#include <vpr/vpr.h>
#include <vpr/System.h>
#include <vpr/Sync/Guard.h>
#include <vpr/Util/Debug.h>
#include <vpr/IO/Socket/InetAddr.h>
#include <vpr/IO/Socket/UnknownHostException.h>

#include <tweek/Util/Debug.h>
#include <tweek/CORBA/CorbaManager.h>
#include <tweek/CORBA/SubjectImpl.h>
#include <tweek/CORBA/SubjectManagerImpl.h>


namespace tweek
{

const std::string SubjectManagerImpl::USERNAME_KEY("Username");
const std::string SubjectManagerImpl::APPNAME_KEY("Application");

SubjectManagerImpl::~SubjectManagerImpl()
{
   subject_map_t::iterator i;
   std::string name_str;

   for ( i = mSubjects.begin(); i != mSubjects.end(); ++i )
   {
      vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
         << "Deactivating subject named '" << (*i).first
         << "' in tweek::SubjectManagerImpl destructor\n" << vprDEBUG_FLUSH;

      // Deactivate the object in the POA.
      name_str = (*i).first;
      mCorbaMgr.getChildPOA()->deactivate_object(mSubjectIds[name_str]);
      Subject_ptr subj = mSubjects[name_str];
      CORBA::release(subj);
   }
}

void SubjectManagerImpl::registerSubject(SubjectImpl* subjectServant,
                                         const char* name)
   throw(CORBA::SystemException)
{
   const std::string name_str(name);

   const std::string begin_str =
      std::string("[tweek::SubjectManagerImpl::registerSubject()] Registering subject named '") +
      name_str + std::string("'\n");
   vpr::DebugOutputGuard g1(tweekDBG_CORBA, vprDBG_STATE_LVL,
                            begin_str,
                            "[tweek::SubjectManagerImpl::registerSubject()] Exiting.\n");

   {
      vprDEBUG(tweekDBG_CORBA, vprDBG_VERB_LVL)
         << "Activating servant with the POA.\n" << vprDEBUG_FLUSH;
      vprDEBUG(tweekDBG_CORBA, vprDBG_HEX_LVL)
         << "Servant: " << std::hex << subjectServant << std::dec
         << std::endl << vprDEBUG_FLUSH;

      vpr::Guard<vpr::Mutex> subj_ids_guard(mSubjectIdsMutex);
      // We have to register servant with POA first.
      mSubjectIds[name_str] =
         mCorbaMgr.getChildPOA()->activate_object(subjectServant);

      vprDEBUG(tweekDBG_CORBA, vprDBG_VERB_LVL)
         << "Servant activated.\n" << vprDEBUG_FLUSH;
   }

   storeSubject(subjectServant->_this(), name_str);
}

vpr::ReturnStatus SubjectManagerImpl::unregisterSubject(const char* name)
   throw(CORBA::SystemException)
{
   vpr::ReturnStatus status;
   std::string name_str(name);
   vpr::Guard<vpr::Mutex> guard(mSubjectsMutex);

   if ( mSubjects.count(name_str) > 0 )
   {
      // Deactivate the object in the POA.
      {
         vpr::Guard<vpr::Mutex> subj_ids_guard(mSubjectIdsMutex);
         mCorbaMgr.getChildPOA()->deactivate_object(mSubjectIds[name_str]);
      }

      Subject_ptr subj = mSubjects[name_str];
      CORBA::release(subj);
      mSubjects.erase(name_str);
   }
   else
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "ERROR:")
         << " No subject registered under the name '" << name_str
         << "'\n" << vprDEBUG_FLUSH;
      status.setCode(vpr::ReturnStatus::Fail);
   }

   return status;
}

void SubjectManagerImpl::storeSubject(Subject_ptr subject,
                                      const std::string& name)
{
   vpr::Guard<vpr::Mutex> guard(mSubjectsMutex);

   vprDEBUG(tweekDBG_CORBA, vprDBG_STATE_LVL)
      << "Adding subject named '" << name << "' to collection.\n"
      << vprDEBUG_FLUSH;

   mSubjects[name] = Subject::_duplicate(subject);
}

bool SubjectManagerImpl::hasSubject(const char* name)
   throw (CORBA::SystemException)
{
   return mSubjects.count(std::string(name)) > 0;
}

Subject_ptr SubjectManagerImpl::getSubject(const char* name)
   throw(CORBA::SystemException)
{
   vprDEBUG_OutputGuard(tweekDBG_CORBA, vprDBG_STATE_LVL,
                        "tweek::SubjectManagerImpl::getSubject() entered\n",
                        "tweek::SubjectManagerImpl::getSubject() done\n");

   Subject_ptr subject;
   std::string name_str(name);
   vpr::Guard<vpr::Mutex> guard(mSubjectsMutex);

   subject_map_t::iterator i;

   i = mSubjects.find(name_str);

   if ( i != mSubjects.end() )
   {
      subject = Subject::_duplicate((*i).second);
      vprDEBUG(tweekDBG_CORBA, vprDBG_STATE_LVL)
         << "Returning subject named '" << name << "'\n" << vprDEBUG_FLUSH;
   }
   else
   {
      subject = Subject::_nil();
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << clrOutBOLD(clrYELLOW, "WARNING:") << " No subject named '" << name
         << "' registered with Subject Manager!\n" << vprDEBUG_FLUSH;
   }

   return subject;
}

tweek::SubjectManager::SubjectList* SubjectManagerImpl::getAllSubjects()
   throw(CORBA::SystemException)
{
   vprDEBUG_OutputGuard(tweekDBG_CORBA, vprDBG_STATE_LVL,
                        "tweek::SubjectManagerImpl::getAllSubjects() entered\n",
                        "tweek::SubjectManagerImpl::getAllSubjects() done\n");

   subject_map_t::iterator i;
   CORBA::ULong j;

   vprDEBUG(tweekDBG_CORBA, vprDBG_STATE_LVL)
      << "Constructing sequence of subjects to return to caller ...\n"
      << vprDEBUG_FLUSH;

   // Lock mSubjects while we read from it.
   vpr::Guard<vpr::Mutex> guard(mSubjectsMutex);

   // Create the sequence and size it.
   tweek::SubjectManager::SubjectList* subjects =
      new tweek::SubjectManager::SubjectList();
   subjects->length(mSubjects.size());

   vprDEBUG(tweekDBG_CORBA, vprDBG_VERB_LVL)
      << "Sequence size: " << subjects->length() << std::endl
      << vprDEBUG_FLUSH;

   for ( i = mSubjects.begin(), j = 0; i != mSubjects.end(); ++i, ++j )
   {
      tweek::SubjectManager::RegisteredSubject rs;
      rs.subject_name = CORBA::string_dup((*i).first.c_str());
      rs.subject_ref  = Subject::_duplicate((*i).second);

      vprDEBUG(tweekDBG_CORBA, vprDBG_VERB_LVL)
         << "Adding subject[" << j << "]: " << rs.subject_name << std::endl
         << vprDEBUG_FLUSH;

      (*subjects)[j] = rs;
   }

   return subjects;
}

SubjectManager::SubjectManagerInfoList* SubjectManagerImpl::getInfo()
   throw(CORBA::SystemException)
{
   std::map<std::string, std::string>::iterator i;
   CORBA::ULong j;

   vprDEBUG(tweekDBG_CORBA, vprDBG_STATE_LVL)
      << "Constructing sequence of info items to return to caller ...\n"
      << vprDEBUG_FLUSH;

   // Use a guard here so that the lock is exception safe.
   vpr::Guard<vpr::Mutex> lock(mInfoMapMutex);

   // Create the sequence and size it.
   tweek::SubjectManager::SubjectManagerInfoList_var info_items =
      new tweek::SubjectManager::SubjectManagerInfoList();
   info_items->length(mInfoMap.size());

   vprDEBUG(tweekDBG_CORBA, vprDBG_VERB_LVL)
      << "Sequence size: " << info_items->length() << std::endl
      << vprDEBUG_FLUSH;

   for ( i = mInfoMap.begin(), j = 0; i != mInfoMap.end(); ++i, ++j )
   {
      tweek::SubjectManager::SubjectMgrInfoItem item;
      item.key   = CORBA::string_dup((*i).first.c_str());
      item.value = CORBA::string_dup((*i).second.c_str());

      vprDEBUG(tweekDBG_CORBA, vprDBG_VERB_LVL)
         << "Adding item[" << j << "]: " << item.key << " => " << item.value
         << std::endl << vprDEBUG_FLUSH;

      info_items[j] = item;
   }

   vprDEBUG(tweekDBG_CORBA, vprDBG_STATE_LVL)
      << "Returning all info items to caller\n" << vprDEBUG_FLUSH;

   return info_items._retn();
}

char* SubjectManagerImpl::getName()
   throw(CORBA::SystemException)
{
   return CORBA::string_dup(mGUID.toString().c_str());
}

void SubjectManagerImpl::initInfoMap()
{
   try
   {
      mInfoMap["Hostname"] = vpr::InetAddr::getLocalHost().getHostname();
   }
   catch (vpr::UnknownHostException& ex)
   {
      mInfoMap["Hostname"] = std::string("Unknown");
   }

   std::string user_name;

   if ( vpr::System::getenv("USER", user_name).success() )
   {
      mInfoMap[USERNAME_KEY] = user_name;
   }
   else
   {
      mInfoMap[USERNAME_KEY] = "unknown";
   }

   mInfoMap[APPNAME_KEY] = "unknown";
}

SubjectManagerImpl::SubjectManagerImpl(const SubjectManagerImpl& sm)
   :
#if defined(TWEEK_USE_OMNIORB)
     omniServant(sm)
   , tweek::_impl_SubjectManager(sm)
   ,
#elif defined(TWEEK_USE_TAO)
     TAO_Abstract_ServantBase(sm)
   ,
#endif
     PortableServer::ServantBase(sm)
   , POA_tweek::SubjectManager(sm)
   , PortableServer::RefCountServantBase(sm)
   , mCorbaMgr(sm.mCorbaMgr)
   , mInfoMap(sm.mInfoMap)
{
   /* Do nothing. */ ;
}

} // End of tweek namespace
