#include <vpr/IO/Stats/BandwidthIOStatsStrategy.h>

namespace vpr
{

double BandwidthIOStatsStrategy::getAverageSendBW()
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

double BandwidthIOStatsStrategy::getAverageReadBW()
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


void BandwidthIOStatsStrategy::incrementBytesRead(vpr::Uint32 bytes)
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

void BandwidthIOStatsStrategy::incrementBytesSent(vpr::Uint32 bytes)
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

}; // namespace vpr

