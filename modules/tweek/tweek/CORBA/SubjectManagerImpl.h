/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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
#include <boost/noncopyable.hpp>

#include <vpr/vpr.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/Guard.h>

#include <tweek/idl/Subject.h>
#include <tweek/idl/SubjectManager.h>


namespace tweek
{

class CorbaManager;
class SubjectImpl;

/** \class SubjectManagerImpl SubjectManagerImpl.h tweek/CORBA/SubjectManagerImpl.h
 *
 * This class provides a simplified level of access to CORBA references.
 * C++ implementations of CORBA interfaces (specified with IDL) are registeered
 * with a Subject Manager.  This process activates the object (a CORBA servant)
 * within the local ORB.  Once registered, remote code can request references
 * to the servant through the Subject Manager.  Most details of CORBA reference
 * management are hidden by this class.
 */
class TWEEK_CLASS_API SubjectManagerImpl
   : private boost::noncopyable
   , public POA_tweek::SubjectManager
   , public PortableServer::RefCountServantBase
{
public:
   virtual ~SubjectManagerImpl();

   /**
    * Registers the given SubjectImpl object (a CORBA servant) with this
    * Subject Manager instance.  This also performs the necessary steps to
    * activate the servant with CORBA.
    *
    * @pre The given servant has not already been activated with the local
    *      ORB.
    * @post The given servant is activated within the local ORB and is
    *       registered with this Subject Manager instance.  From this point
    *       forward, remote code can get a reference to the servant through
    *       this Subject Manager.
    *
    * @param subject The implementation object that will be registered with
    *                the local ORB and this Subject Manager.
    * @param name    The symbolic name used to request a reference to the
    *                registered subject.
    */
   void registerSubject(SubjectImpl* subject, const char* name)
      throw(CORBA::SystemException);

   /**
    * Attempts to "unregister" the named subject and deactivate it within the
    * local ORB.
    *
    * @post If the named subject was registered with this Subject Manager, it
    *       is removed from the registry and is deactivated within the local
    *       ORB.
    *
    * @param name The symbolic name used to request a reference to the
    *             registered subject.
    */
   bool unregisterSubject(const char* name) throw (CORBA::SystemException);

   /**
    * Returns whether this Subject Manager contains the named subject.
    *
    * @param name The name of the subject to search for in this Subject
    *             Manager's collection of subjects.
    *
    * @since 1.1.2
    */
   bool hasSubject(const char* name) throw (CORBA::SystemException);

   /**
    * Returns the named Tweek Subject reference to the caller if the Subject
    * is registered.  If not, a reference equal to CORBA::nil() is returned.
    */
   virtual Subject_ptr getSubject(const char* name)
      throw(CORBA::SystemException);

   /**
    * Returns a sequence of all the registered Tweek Subjects packaged in the
    * structure RegisteredSubject.
    */
   virtual tweek::SubjectManager::SubjectList* getAllSubjects()
      throw(CORBA::SystemException);

   /**
    * Returns a sequence of key/value pairs that provide site-specific
    * information about a given Subject Manager servant.
    */
   virtual tweek::SubjectManager::SubjectManagerInfoList* getInfo()
      throw(CORBA::SystemException);

   /**
    * Returns the name of this Subject Manager reference as it is registered
    * with the CORBA Naming Service.
    *
    * @note It might be possible for client code to look this up some other
    *       way, so this method may not really be needed except as a
    *       convenience.
    */
   virtual char* getName() throw(CORBA::SystemException);

   /**
    * Assigns the given value for the application name informational item.
    * This can be used to provide remote users with an application-specific
    * identifier for this Subject Manager instance.  If this method is not
    * used, the application name setting defaults to "unknown".
    *
    * @param appName The application name to be added to the collection of
    *                custom informational items.
    */
   void setApplicationName(const std::string& appName)
   {
      addInfoItem(std::string(APPNAME_KEY), appName);
   }

   /**
    * Assigns the given value for the username informational item.  This can
    * be used to provide remote users with an application-specific identifier
    * for this Subject Manager instance.  If this method is not used, the
    * username setting defaults to the value of the $USER environment varaible.
    *
    * @param userName The username to be added to the collection of custom
    *                 informational items.
    */
   void setUserName(const std::string& userName)
   {
      addInfoItem(USERNAME_KEY, userName);
   }

   /**
    * Returns a sequence of key/value pairs that provide site-specific
    * information about a given Subject Manager servant.
    */
   void addInfoItem(const std::string& key, const std::string& value)
   {
      vpr::Guard<vpr::Mutex> lock(mInfoMapMutex);
      mInfoMap[key] = value;
   }

protected:
   // Only this class can instantiate me.
   friend class tweek::CorbaManager;

   /**
    * Default constructor.  It is protected because only instances of
    * tweek::CorbaManager may create objects of this type.
    */
   SubjectManagerImpl(const CorbaManager& corbaMgr, const std::string& name);

   void storeSubject(Subject_ptr subject, const std::string& name);

   /**
    * Intializes the encapsulated map of key/value pairs for this Subject
    * Manager instance.
    */
   void initInfoMap();

private:
   const CorbaManager& mCorbaMgr;
   std::string         mName;

   typedef std::map<std::string, Subject_ptr> subject_map_t;
   subject_map_t mSubjects;
   vpr::Mutex    mSubjectsMutex;

   typedef std::map<std::string, PortableServer::ObjectId_var> subject_id_map_t;
   subject_id_map_t mSubjectIds;
   vpr::Mutex       mSubjectIdsMutex;

   std::map<std::string, std::string> mInfoMap;
   vpr::Mutex                         mInfoMapMutex;

   static const std::string USERNAME_KEY;
   static const std::string APPNAME_KEY;
};

} // End of tweek namespace

#endif /* _TWEEK_SUBJECT_MANAGER_IMPL_H_ */
