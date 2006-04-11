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

#ifndef _VPR_SAMPLE_LIMITED_STAT_COLLECTOR_H_
#define _VPR_SAMPLE_LIMITED_STAT_COLLECTOR_H_

#include <vpr/vprConfig.h>

#include <vpr/Util/DateTime.h>
#include <vpr/Util/Interval.h>

#include <deque>
#include <utility>
#include <typeinfo>

namespace vpr
{

/** Statistics collection class
*
* STA - Short Term Average (a time limited average)
*
* @param TimeBased  Should the return values be /per second or just "normal" stats
*
*  This is basically the difference between discrete and continuous values (or interpretation of the values)
*/
template<class TYPE, bool TimeBased>
class SampleLimitedStatCollector
{
public:
   /** Constructor
   * @param staMaxTime  The max age for samples in the sta computation
   */
   SampleLimitedStatCollector(unsigned sampleLimit = 100 )
   {
      mSTASampleLimit = sampleLimit;
      reset();
   }

   void reset()
   {
      mCurTotal = 0;
      mSampleCount = 0;

      mMaxSTA = 0.0;
      mRunningSTATotal = 0.0f;
      mPrevSample1 = 0;
      mPrevSample2 = 0;

      mCurSampleIndex = 0;
      mSampleBuffer.clear();
      mSampleBuffer.resize(mSTASampleLimit);
   }

   void addSample(const TYPE sample);

   TYPE getTotal() const
   { return mCurTotal; }
   vpr::Uint32 getNumSamples() const
   { return mSampleCount; }

   /** Return Mean (value/second) */
   double getMean();
   double getInstAverage();
   double getSTA();
   double getMaxSTA() const
   {  return mMaxSTA; }

   void print(std::ostream& out);

private:
   TYPE        mCurTotal;     // Running total of the data
   vpr::Uint32 mSampleCount;  // Number of samples taken

   unsigned       mSTASampleLimit;   // Max age of a value used in short term average (STA)
   double         mMaxSTA;
   double         mRunningSTATotal; // The running total of the STA

   vpr::DateTime mInitialSampleTime;   // Time of first sample
   vpr::Interval mPrevSampleTime1, mPrevSampleTime2;      // Time of last 2 samples (mPrevST1 < mPrevSt2)
   TYPE          mPrevSample1, mPrevSample2;              // Previous samples

   unsigned      mCurSampleIndex;                                  // Index of the NEXT sample to write
   std::vector< std::pair<TYPE,vpr::Interval> >  mSampleBuffer;    // Buffer of samples used to calc STA
};

template <class TYPE, bool TimeBased>
void SampleLimitedStatCollector<TYPE, TimeBased>::print(std::ostream& out)
{
   out << "type: " << typeid(TYPE).name() << "   time based:" << ( TimeBased ? "Y" : "N" ) << std::endl
       << "total: " << mCurTotal << "   samples:" << mSampleCount << std::endl
       << "mean: " << getMean() << "    sampleLimit:" << mSTASampleLimit << std::endl
       << "Initial Sample Time:" << mInitialSampleTime.getMinutesf() << std::endl
       << "prev sampTime: " << mPrevSampleTime1.secf() << "s   prev sampTime2:" << mPrevSampleTime2.secf() << std::endl
       << "prev samp: " << mPrevSample1 << "   prev samp2:" << mPrevSample2 << std::endl
       << " --- data --- time --- " << std::endl;

   for(typename std::vector< std::pair<TYPE,vpr::Interval> >::iterator i = mSampleBuffer.begin();
       i!= mSampleBuffer.end(); ++i)
   {
      out << (*i).first << "   " << (*i).second.msec() << "ms\n";
   }
}


template <class TYPE, bool TimeBased>
void SampleLimitedStatCollector<TYPE, TimeBased>::addSample(const TYPE sample)
{
   mCurTotal += sample;
   mSampleCount += 1;

   vpr::Interval cur_time;

   if(TimeBased)
   {
      cur_time.setNow();                     // Set current time

      // Set Init time
      if(mSampleCount == 1)              // First send -- INIT TIMES
      {
         mInitialSampleTime.setNow();    // Initialize first read time
      }

      mPrevSampleTime1 = mPrevSampleTime2;      // Time of last 2 samples (mPrevST1 < mPrevSt2)
      mPrevSampleTime2 = cur_time;
   }

   // Update recent sample values for INST average
   mPrevSample1 = mPrevSample2;
   mPrevSample2 = sample;

   // Add value to sample buffer
   mRunningSTATotal += sample;                           // Add on new value
   mRunningSTATotal -= mSampleBuffer[mCurSampleIndex].first;   // Subtract off old value (note: this is 0 when not set yet)
   mSampleBuffer[mCurSampleIndex] = std::pair<TYPE,vpr::Interval>(sample, cur_time);

   // Goto next index
   ++mCurSampleIndex;
   if(mCurSampleIndex == mSTASampleLimit)
   {  mCurSampleIndex = 0; }
   vprASSERT(mCurSampleIndex < mSTASampleLimit && "Should never get larger then SampleLimit");

   // --- UPDATE MAXES ---- //
   double sta_value = getSTA();
   if(sta_value > mMaxSTA)                        // Check for new max
   {  mMaxSTA = sta_value; }
}

template <class TYPE, bool TimeBased>
double SampleLimitedStatCollector<TYPE, TimeBased>::getMean()
{
   if(0 == mCurTotal)
      return 0.0f;

   double mean_result(0.0);

   if(TimeBased)
   {
      vpr::DateTime cur_date_time, diff_date_time;
      double diff_secs;

      cur_date_time.setNow();
      diff_date_time = cur_date_time - mInitialSampleTime;
      diff_secs = diff_date_time.getSecondsf();

      mean_result = double(mCurTotal)/diff_secs;
   }
   else
   {
      mean_result = double(mCurTotal)/double(mSampleCount);
   }

   return mean_result;
}

template <class TYPE, bool TimeBased>
double SampleLimitedStatCollector<TYPE, TimeBased>::getInstAverage()
{
   double inst_average(0.0);

   if(TimeBased)
   {
      //
      //     |
      //     |                  |
      //     |                  |
      // mPrevTime1        mPrevTime2             cur_time
      vpr::Interval cur_time, diff_time;
      double diff_sec;                       // Num secs different in send times
      cur_time.setNow();                     // Set current time
      diff_time = cur_time - mPrevSampleTime1;     // Get time to compute the average over
      diff_sec = diff_time.secf();

      // Compute -- INST BANDWIDTH
      if(diff_sec > 0)
      {
        inst_average = double(mPrevSample1 + mPrevSample2)/diff_sec;
      }
      if(diff_time > mSTAMaxTime)   // Haven't had sample in quite a while, so clamp to zero
      {
         inst_average = 0.0;
      }
   }
   else
   {
      // Just approximate it with the average of the last two entries
      inst_average = double(mPrevSample1 + mPrevSample2)/2.0;
   }

   return inst_average;
}

template <class TYPE, bool TimeBased>
double SampleLimitedStatCollector<TYPE, TimeBased>::getSTA()
{
   // Compute -- STA BANDWIDTH
   double sta_value(0.0f);

   if(mSampleBuffer.size() > 0)
   {
      if(TimeBased)
      {
         vpr::Interval cur_time, diff_time;
         double diff_sec;                       // Num secs different in send times
         cur_time.setNow();                     // Set current time

         vpr::Interval first_sample_time( mSampleBuffer.back().second );
         diff_time = cur_time - first_sample_time;
         diff_sec = diff_time.secf();
         sta_value = mRunningSTATotal/diff_sec;
      }
      else
      {
         sta_value = mRunningSTATotal/double(mSampleBuffer.size());      // Compute as average of the buffered samples
      }
   }

   return sta_value;
}


}; // namespace vpr

#endif

