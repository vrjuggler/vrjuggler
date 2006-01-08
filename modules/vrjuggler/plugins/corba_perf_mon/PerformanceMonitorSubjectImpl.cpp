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

#include <string>
#include <list>
#include <sstream>

#include <vpr/vpr.h>
#include <vpr/Util/Debug.h>
#include <vpr/Perf/ProfileManager.h>

#include <vrj/Util/Debug.h>

#include <corba_perf_mon/PerformanceMonitorSubjectImpl.h>

namespace vrj
{
   PerformanceMonitorSubject::StringList* PerformanceMonitorSubjectImpl::getAllSections()
      throw(CORBA::SystemException)
   {
      // Create the sequence.
      vrj::PerformanceMonitorSubject::StringList* strings =
         new vrj::PerformanceMonitorSubject::StringList();

      return strings;
   }

   CORBA::Float PerformanceMonitorSubjectImpl::getValue(CORBA::Float param)
      throw(CORBA::SystemException)
   {
      return param;
   }

   vrj::PerformanceMonitorSubject::SampleValueMap* PerformanceMonitorSubjectImpl::getValueMap()
      throw(CORBA::SystemException)
   {
      // TODO: Safeguard from changing tree...ask for names to lookup and return them
      // or keep a master list to compare against.
      vpr::ProfileManager::ProfileSampleResult sample_time_map = vpr::ProfileManager::getSampleResult();

      vpr::ProfileManager::ProfileSampleResult::iterator itr;
      unsigned long i = 0;
      vrj::PerformanceMonitorSubject::SampleValueMap* value_map = 
         new vrj::PerformanceMonitorSubject::SampleValueMap();

      value_map->length(sample_time_map.size());

      for ( itr = sample_time_map.begin(); itr != sample_time_map.end(); ++itr )
      {
         (*value_map)[i].mName = CORBA::string_dup(itr->first.c_str());
         (*value_map)[i].mSampleTime = itr->second.msecf();
         i++;
      }
      return value_map;
   }
} // namespace jccl
