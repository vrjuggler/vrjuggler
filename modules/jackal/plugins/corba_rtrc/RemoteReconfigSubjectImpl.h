/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#ifndef _JCCL_REMOTE_RECONFIG_SUBJECT_IMPL_H_
#define _JCCL_REMOTE_RECONFIG_SUBJECT_IMPL_H_

#include <jccl/Plugins/PluginConfig.h>

#include <tweek/CORBA/SubjectImpl.h>

#include <RemoteReconfigSubject.h>


namespace jccl 
{

/** CORBA interface subject implementation to the dynamic reconfiguration 
 *  management plugin. Use these functions to view and modify the elements in
 *  the Config Manager.
 *
 *  @date July 31, 2002
 */
class RemoteReconfigSubjectImpl
   : public POA_jccl::RemoteReconfigSubject
   , public tweek::SubjectImpl
{
public:

   RemoteReconfigSubjectImpl()
   {;}

   ~RemoteReconfigSubjectImpl()
   {;}

   /**
    * Adds a string containing an XML definition of a configuration.
    */
   virtual void add(const char* configurationString)
      throw(CORBA::SystemException);

   /**
    * Removes a string containing an XML definition of a configuration.
    */
   virtual void remove(const char* configurationString)
      throw(CORBA::SystemException);

   /** 
    * Swap in/out a string containing XML definitions of configurations.
    */
   virtual void swap(const char* addConfigurationString,
                     const char* removeConfigurationString)    
      throw(CORBA::SystemException);

   /**
    * Gets the current elements in the config system.
    */
   virtual char* getElements() throw(CORBA::SystemException);

   /**
    * Get the current configuration definitions in the config system.
    */
   virtual char* getDefinitions() throw(CORBA::SystemException);

   /**
    * This overriding method is needed so that the correct type is returned
    * when the _this() method is invoked.  Without this method, an object of
    * type tweek::Subject_ptr would be returned.
    *
    * XXX: It may be possible to remove this requirement in the future.
    */
   jccl::RemoteReconfigSubject_ptr _this()
   {
      return POA_jccl::RemoteReconfigSubject::_this();
   }

//private:

};

} // namespace jccl

#endif
