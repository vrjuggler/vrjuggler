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

#ifndef BW_IOSTATS_H
#define BW_IOSTATS_H

#include <vpr/vprConfig.h>
#include <vpr/IO/Stats/BaseIOStatsStrategy.h>
#include <vpr/Util/DateTime.h>
#include <vpr/Util/Interval.h>

#include <deque>

namespace vpr
{

/**
 * Strategy for collecting bandwidth data about the block IO device
 * Holds all stats for Bandwidth collection
 */
class VPR_CLASS_API BandwidthIOStatsStrategy //: public BaseIOStatsStrategy_i
{
public:
   BandwidthIOStatsStrategy()
   {
      mMaxBwBufferSize = 100;
      mSendCount = mReadCount = 0;
      mSentBytes = mReadBytes = 0;

      mInstSendBW = mSTASendBW = mSTASendMaxBW = 0.0f;
      mInstReadBW = mSTAReadBW = mSTAReadMaxBW = 0.0f;
      mBWBurstiness = 0.0f;
   }

   virtual ~BandwidthIOStatsStrategy()
   {;}

   /**
    * @pre  read must have already occured
    */
  virtual void read_s(Status& status,
               void* buffer, const vpr::Uint32 length,
               vpr::Uint32& bytes_read,
               const vpr::Interval timeout = vpr::Interval::NoTimeout)
  {
     if(status.success())
        incrementBytesRead(bytes_read);
  }

   /**
    * @pre  read must have already occured
    */
  virtual void readn_s(Status& status,
                void* buffer, const vpr::Uint32 length,
                vpr::Uint32& bytes_read,
                const vpr::Interval timeout = vpr::Interval::NoTimeout)
  {
     if(status.success())
        incrementBytesRead(bytes_read);
  }

   /**
    * @pre  write must have already occured
    */
  virtual void write_s(Status& status,
                const void* buffer, const vpr::Uint32 length,
                vpr::Uint32& bytes_written,
                const vpr::Interval timeout = vpr::Interval::NoTimeout)
  {
     if(status.success())
        incrementBytesSent(bytes_written);
  }


protected:   
   void incrementBytesSent(vpr::Uint32 bytes);
   void incrementBytesRead(vpr::Uint32 bytes);

public:
   vpr::Uint32 getTotalBytesSent()
      { return mSentBytes; }
   vpr::Uint32 getTotalBytesRead()
      { return mReadBytes; }
   
   double getAverageSendBW();         
   double getAverageReadBW();
   
   double getSTASendBW()
      { return mSTASendBW;}
   double getSTAReadBW()
      { return mSTAReadBW; }
   double getInstSendBW()
      { return mInstSendBW; }
   double getInstReadBW()
      { return mInstReadBW; }
   double getMaxSTASendBW()
      { return mSTASendMaxBW;}
   double getMaxSTAReadBW()
      { return mSTAReadMaxBW;}


private:
   vpr::Uint32 mMaxBwBufferSize;
   vpr::Uint32 mSendCount, mReadCount;
   vpr::Uint32 mSentBytes, mReadBytes;

   double mInstSendBW, mSTASendBW, mSTASendMaxBW;
   double mInstReadBW, mSTAReadBW, mSTAReadMaxBW;
   double mBWBurstiness;

   vpr::DateTime  mInitialSendTime, mInitialReadTime;
   vpr::Interval  mPrevSendTime, mPrevReadTime;

   std::deque<double> mSendBWBuffer;     // Short term write bandwidth buffer -- Holds most recent N instAv vals
   std::deque<double> mReadBWBuffer;     // Short term read bw buffer -- Holds most recent N instAv vals
};

}; //namespace vpr

#endif //BWIOSTATS_H


