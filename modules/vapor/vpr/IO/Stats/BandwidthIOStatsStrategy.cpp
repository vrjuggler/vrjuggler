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

#include <vpr/IO/Stats/BandwidthIOStatsStrategy.h>

double vpr::BandwidthIOStatsStrategy::getAverageSendBW()
{ 
   if(0 == mSentBytes)
      return 0.0f;
   
   vpr::DateTime cur_date_time, diff_date_time;
   double av_sent_bw, diff_secs;

   cur_date_time.setNow();
   diff_date_time = cur_date_time - mInitialSendTime;
   diff_secs = diff_date_time.getSecondsf();

   std::cout << "-- send bw --" << std::endl;
   std::cout << "diff secs: " << diff_secs << std::endl;
   std::cout << "bytes sent: " << mSentBytes << std::endl;

   av_sent_bw = double(mSentBytes)/diff_secs;
   
   return av_sent_bw;
}

double vpr::BandwidthIOStatsStrategy::getAverageReadBW()
{ 
   if(0 == mReadBytes)
      return 0.0f;
   
   vpr::DateTime cur_date_time, diff_date_time;
   double av_read_bw, diff_secs;
   
   cur_date_time.setNow();
   diff_date_time = cur_date_time - mInitialReadTime;
   diff_secs = diff_date_time.getSecondsf();

   std::cout << " -- read bw -- " << std::endl;
   std::cout << "diff secs: " << diff_secs << std::endl;
   std::cout << "bytes sent: " << mReadBytes << std::endl;

   av_read_bw = double(mReadBytes)/diff_secs;
   
   return av_read_bw;
}


void vpr::BandwidthIOStatsStrategy::incrementBytesRead(vpr::Uint32 bytes)
{
   mReadBytes += bytes;
   mReadCount += 1;
   
   vpr::Interval cur_time, diff_time;
   double diff_sec;                       // Num secs different in send times
   cur_time.setNow();                     // Set current time
   
   // Compute inst read bandwitdth
   if(mReadCount == 1)              // First send -- INIT TIMES
   {
      mInitialReadTime.setNow();    // Initialize first read time      
   }
   else
   {
      // Compute -- INST BANDWIDTH
      diff_time = cur_time - mPrevReadTime;
      diff_sec = diff_time.secf();
      if(diff_sec > 0)
      {
        mInstReadBW = double(bytes)/diff_sec;
      }
            
      // Compute -- STA BANDWIDTH
      double bw_sum(0.0f);
      
      if(mReadBWBuffer.size() == mMaxBwBufferSize)          // If already full, then pop off old one
        mReadBWBuffer.pop_back();
      mReadBWBuffer.push_front(mInstReadBW);                 // Put new one in
      
      for(vpr::Uint16 i=0;i<mReadBWBuffer.size();i++)       // Sum the total bandwidth up
        bw_sum += mReadBWBuffer[i];
      
      mSTAReadBW = bw_sum/double(mReadBWBuffer.size());     // Calc the new average
      
      if(mSTAReadBW > mSTAReadMaxBW)                        // Check for new max
           mSTAReadMaxBW = mSTAReadBW;
   }
   
   mPrevReadTime = cur_time;        // Set the prev time to use next time  
}

void vpr::BandwidthIOStatsStrategy::incrementBytesSent(vpr::Uint32 bytes)
{
   mSentBytes += bytes;
   mSendCount += 1;

   vpr::Interval cur_time, diff_time;
   double diff_sec;                       // Num secs different in send times
   cur_time.setNow();                     // Set current time
      
   // Compute inst send bandwitdth
   if(mSendCount == 1)     // First send -- INIT TIMES
   {
      mInitialSendTime.setNow();        // Initialize first send time      
   }
   else
   {
      // Compute -- INST BANDWIDTH
      diff_time = cur_time - mPrevSendTime;
      diff_sec = diff_time.secf();
      if(diff_sec > 0)
      {
         mInstSendBW = double(bytes)/diff_sec;
      }

      // Compute -- STA BANDWIDTH
      double bw_sum(0.0f);

      if(mSendBWBuffer.size() == mMaxBwBufferSize)          // If already full, then pop off old one
         mSendBWBuffer.pop_back();
      mSendBWBuffer.push_front(mInstSendBW);                 // Put new one in

      for(vpr::Uint16 i=0;i<mSendBWBuffer.size();i++)       // Sum the total bandwidth up
         bw_sum += mSendBWBuffer[i];

      mSTASendBW = bw_sum/double(mSendBWBuffer.size());     // Calc the new average
      
      if(mSTASendBW > mSTASendMaxBW)                        // Check for new max
         mSTASendMaxBW = mSTASendBW;
   }

   mPrevSendTime = cur_time;        // Set the prev time to use next time
}
