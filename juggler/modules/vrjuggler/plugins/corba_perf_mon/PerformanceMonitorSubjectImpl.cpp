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

#include <vrj/Performance/PluginConfig.h>
#include <vpr/Perf/ProfileManager.h>

#include <string>
#include <list>
#include <sstream>

#include <corba_perf_mon/PerformanceMonitorSubjectImpl.h>

namespace vrj
{
   PerformanceMonitorSubject::StringList* PerformanceMonitorSubjectImpl::getAllSections()
   {
      // Create the sequence and size it.
      vrj::PerformanceMonitorSubject::StringList* strings =
         new vrj::PerformanceMonitorSubject::StringList();

      return strings;
   }

   CORBA::Float PerformanceMonitorSubjectImpl::getValue(CORBA::Float param)
   {
      return param;
   }
   
   vrj::PerformanceMonitorSubject::SampleValueMap* PerformanceMonitorSubjectImpl::getValueMap()
   {
      vrj::PerformanceMonitorSubject::SampleValueMap* value_map = 
                        new vrj::PerformanceMonitorSubject::SampleValueMap();
      std::map<std::string, float> sample_time_map = vpr::ProfileManager::getValueMap();
      std::map<std::string, float>::iterator itr;
      unsigned long i = 0;
      for (itr = sample_time_map.begin(); itr != sample_time_map.end(); ++itr,++i)
      {
         (*value_map)[i].mName = itr->first.c_str();
         (*value_map)[i].mSampleTime = itr->second;
      }
      return value_map;
   }
} // namespace jccl
