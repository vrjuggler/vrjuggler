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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VRJ_PERFORMANCE_MONITOR_SUBJECT_IMPL_H_
#define _VRJ_PERFORMANCE_MONITOR_SUBJECT_IMPL_H_

#include <vrj/Performance/PluginConfig.h>

#include <tweek/CORBA/SubjectImpl.h>

#include <PerformanceMonitorSubject.h>

namespace vrj
{

class PerformanceMonitorSubjectImpl
   : public POA_vrj::PerformanceMonitorSubject
   , public tweek::SubjectImpl
{
public:

   PerformanceMonitorSubjectImpl()
   {;}

   ~PerformanceMonitorSubjectImpl()
   {;}

   virtual vrj::PerformanceMonitorSubject::StringList* getAllSections()
      throw(CORBA::SystemException);

   virtual CORBA::Float getValue(CORBA::Float param)
      throw(CORBA::SystemException);

   /**
    * This function prepares a map of profiles to samples.
    *
    * @return   the newly created SampleTimeMap
    */
   virtual vrj::PerformanceMonitorSubject::SampleValueMap* getValueMap()
      throw(CORBA::SystemException);

   /**
    * This overriding method is needed so that the correct type is returned
    * when the _this() method is invoked.  Without this method, an object of
    * type tweek::Subject_ptr would be returned.
    *
    * XXX: It may be possible to remove this requirement in the future.
    */
   vrj::PerformanceMonitorSubject_ptr _this()
   {
      return POA_vrj::PerformanceMonitorSubject::_this();
   }

};

} // namespace vrj

#endif
