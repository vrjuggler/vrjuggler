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

#ifndef _TWEEK_SUBJECT_MANAGER_IMPL_H_
#define _TWEEK_SUBJECT_MANAGER_IMPL_H_

#include <tweek/tweekConfig.h>

#include <string>
#include <map>
#include <vpr/vpr.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Util/GUID.h>

#include <tweek/CORBA/Subject.h>
#include <tweek/CORBA/SubjectManager.h>


namespace tweek
{

class CorbaManager;
class SubjectImpl;

class TWEEK_CLASS_API SubjectManagerImpl
   : public POA_tweek::SubjectManager,
     public PortableServer::RefCountServantBase
{
public:
   void registerSubject(SubjectImpl* subject, const char* name);

   vpr::ReturnStatus unregisterSubject(const char* name);

   virtual Subject_ptr getSubject(const char* name);

   virtual tweek::SubjectManager::SubjectList* getAllSubjects(void);

   /**
    * Returns the name of the host on which the Subject Manager servant
    * resides.
    */
   virtual char* getHostName();

   /**
    * Returns the name of the application within which the Subject Manager
    * servant exists.  If the application name is not known, the string
    * "unknown" is returned.
    */
   virtual char* getApplicationName();

   void setApplicationName(const std::string& appName)
   {
      mAppName = appName;
   }

   /**
    * Returns the name/ID of the user running the application.  If the user's
    * name cannot be determined, the string "unknown" is returned.
    */
   virtual char* getUserName();

   const vpr::GUID& getGUID() const
   {
      return mGUID;
   }

protected:
   // Only this class can instantiate me.
   friend class tweek::CorbaManager;

   /**
    * Default constructor.  It is protected because only instances of
    * tweek::CorbaManager may create objects of this type.
    */
   SubjectManagerImpl(const CorbaManager& corba_mgr)
      : m_corba_mgr(corba_mgr), mAppName("unknown"), mGUID()
   {
      mGUID.generate();
   }

   // These two have to be here because Visual C++ will try to make them
   // exported public symbols.  This causes problems because copying
   // vpr::Mutex objects is not allowed.
   SubjectManagerImpl(const SubjectManagerImpl& sm)
      : m_corba_mgr(sm.m_corba_mgr)
   {
      /* Do nothing. */ ;
   }

   void operator=(const SubjectManagerImpl& sm)
   {
      /* Do nothing. */ ;
   }

   void registerSubject(Subject_ptr subject, const std::string& name);

private:
   const CorbaManager& m_corba_mgr;
   std::string         mAppName;
   vpr::GUID           mGUID;

   typedef std::map<std::string, Subject_ptr> subject_map_t;
   subject_map_t m_subjects;
   vpr::Mutex m_subjects_mutex;

   typedef std::map<std::string, PortableServer::ObjectId_var> subject_id_map_t;
   subject_id_map_t m_subject_ids;
   vpr::Mutex m_subject_ids_mutex;
};

} // End of tweek namespace

#endif /* _TWEEK_SUBJECT_MANAGER_IMPL_H_ */
