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
               void* buffer, const size_t length,
               ssize_t& bytes_read,
               const vpr::Interval timeout = vpr::Interval::NoTimeout)
  {
     if(status.success())
        incrementBytesRead(bytes_read);
  }

   /**
    * @pre  read must have already occured
    */
  virtual void readn_s(Status& status,
                void* buffer, const size_t length,
                ssize_t& bytes_read,
                const vpr::Interval timeout = vpr::Interval::NoTimeout)
  {
     if(status.success())
        incrementBytesRead(bytes_read);
  }

   /**
    * @pre  write must have already occured
    */
  virtual void write_s(Status& status,
                const void* buffer, const size_t length,
                ssize_t& bytes_written,
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


