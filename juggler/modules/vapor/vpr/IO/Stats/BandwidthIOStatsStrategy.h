/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef VPR_BW_IOSTATS_H
#define VPR_BW_IOSTATS_H

#include <vpr/vprConfig.h>
#include <boost/concept_check.hpp>
#include <vpr/IO/Stats/BaseIOStatsStrategy.h>
#include <vpr/Util/DateTime.h>
#include <vpr/Util/Interval.h>
#include <vpr/Util/StatCollector.h>

#include <deque>

namespace vpr
{

/**
 * Strategy for collecting bandwidth data about the block IO device.
 * Holds all stats for Bandwidth collection.
 */
class BandwidthIOStatsStrategy //: public BaseIOStatsStrategy_i
{
public:
   BandwidthIOStatsStrategy()
   {;}

   virtual ~BandwidthIOStatsStrategy()
   {;}

   /**
    * @pre  read must have already occured.
    */
  virtual void read_s(ReturnStatus& status,
               void* buffer, const vpr::Uint32 length,
               vpr::Uint32& bytes_read,
               const vpr::Interval timeout = vpr::Interval::NoTimeout)
  {
     boost::ignore_unused_variable_warning(buffer);
     boost::ignore_unused_variable_warning(length);
     boost::ignore_unused_variable_warning(timeout);

     if(status.success())
     {
        mReadStats.addSample(bytes_read);
     }
  }

   /**
    * @pre  read must have already occured.
    */
  virtual void readn_s(ReturnStatus& status,
                void* buffer, const vpr::Uint32 length,
                vpr::Uint32& bytes_read,
                const vpr::Interval timeout = vpr::Interval::NoTimeout)
  {
     boost::ignore_unused_variable_warning(buffer);
     boost::ignore_unused_variable_warning(length);
     boost::ignore_unused_variable_warning(timeout);

     if(status.success())
     {
        mReadStats.addSample(bytes_read);
     }
  }

   /**
    * @pre  write must have already occured.
    */
  virtual void write_s(ReturnStatus& status,
                const void* buffer, const vpr::Uint32 length,
                vpr::Uint32& bytes_written,
                const vpr::Interval timeout = vpr::Interval::NoTimeout)
  {
     boost::ignore_unused_variable_warning(buffer);
     boost::ignore_unused_variable_warning(length);
     boost::ignore_unused_variable_warning(timeout);

     if(status.success())
     {
        mWriteStats.addSample(bytes_written);
     }
  }

public:
   vpr::StatCollector<vpr::Uint32, true>& readStats()
      { return mReadStats; }
   vpr::StatCollector<vpr::Uint32, true>& writeStats()
      { return mWriteStats; }

private:
   vpr::StatCollector<vpr::Uint32, true> mReadStats;
   vpr::StatCollector<vpr::Uint32, true> mWriteStats;
};

} //namespace vpr


#endif //VPR_BWIOSTATS_H
