/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _JCCL_RTRC_INTERFACE_SUBJECT_IMPL_H_
#define _JCCL_RTRC_INTERFACE_SUBJECT_IMPL_H_

#include <jccl/jcclConfig.h>

#include <tweek/CORBA/SubjectImpl.h>

#include <jccl/RTRC/RTRCInterfaceSubject.h>


namespace jccl 
{

/** CORBA interface subject implementation to the dynamic reconfiguration 
 *  management plugin. Use these functions to view and modify the chunks in
 *  the config manager.
 *
 *  @date July 31, 2002
 */
class JCCL_CLASS_API RTRCInterfaceSubjectImpl : public POA_jccl::RTRCInterfaceSubject,
                                                public tweek::SubjectImpl
{
public:

   RTRCInterfaceSubjectImpl()
   {;}

   ~RTRCInterfaceSubjectImpl()
   {;}

   /**
    * Add a string containing an XML definition of a config chunk DB
    */
   virtual void add( const char* chunkDBString );

   /**
    * Remove a string containing an XML definition of a config chunk DB
    */
   virtual void remove( const char* chunkDBString );

   /** 
    * Swap in/out a string containing XML definitions of config chunk DBs
    */
   virtual void swap( const char* addChunkDBString, const char* removeChunkDBString );

   /**
    * Get the current chunks in the config system
    */
   virtual char* getChunks();

   /**
    * Get the current chunk descs in the config system
    */
   virtual char* getChunkDescs();

   /**
    * This overriding method is needed so that the correct type is returned
    * when the _this() method is invoked.  Without this method, an object of
    * type tweek::Subject_ptr would be returned.
    *
    * XXX: It may be possible to remove this requirement in the future.
    */
   jccl::RTRCInterfaceSubject_ptr _this()
   {
      return POA_jccl::RTRCInterfaceSubject::_this();
   }

//private:

};

} // namespace jccl

#endif
