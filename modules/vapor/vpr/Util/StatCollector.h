/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#ifndef _VPR_STAT_COLLECTOR_H_
#define _VPR_STAT_COLLECTOR_H_

#include <vpr/vprConfig.h>

#include <vpr/Util/DateTime.h>
#include <vpr/Util/Interval.h>

#include <deque>
#include <utility>
#include <typeinfo>

namespace vpr
{

/** \class StatCollector StatCollector.h vpr/Util/StatCollector.h
 *
 * Statistics collection class.
 *
 * STA: Short Term Average (a time-limited average).
 *
 * @param TYPE      The sample type.
 * @param TimeBased Should the return values be /per second or just "normal"
 *                  stats?  This is basically the difference between discrete
 *                  and continuous values (or interpretation of the values).
 */
template<class TYPE, bool TimeBased>
class StatCollector
{
public:
   /** Constructor
    * @param staMaxTime  The max age for samples in the sta computation
    */
   StatCollector(vpr::Interval staMaxTime = vpr::Interval(5, vpr::Interval::Sec) )
      : mSTAMaxTime(staMaxTime)
   {
      reset();
   }

   void reset()
   {
      mCurTotal = 0;
      mSampleCount = 0;

      mMaxSTA = 0.0;
      mPrevSample1 = 0;
      mPrevSample2 = 0;

      mSampleBuffer.clear();
   }

   void addSample(const TYPE sample);

   TYPE getTotal() const
   {
      return mCurTotal;
   }

   vpr::Uint32 getNumSamples() const
   {
      return mSampleCount;
   }

   /** Return Mean (value/second) */
   double getMean();
   double getInstAverage();
   double getSTA();
   double getMaxSTA() const
   {
      return mMaxSTA;
   }

   void print(std::ostream& out);

private:
   TYPE        mCurTotal;     /**< Running total of the data */
   vpr::Uint32 mSampleCount;  /**< Number of samples taken */

   vpr::Interval  mSTAMaxTime;   /**< Max age of a value used in short term average (STA) */
   double         mMaxSTA;

   vpr::DateTime mInitialSampleTime;   /**< Time of first sample */
   vpr::Interval mPrevSampleTime1, mPrevSampleTime2; /**< Time of last 2 samples (mPrevST1 < mPrevSt2) */
   TYPE          mPrevSample1, mPrevSample2;         /**< Previous samples */

   std::deque< std::pair<TYPE,vpr::Interval> >  mSampleBuffer;    /**< Buffer of samples used to calc STA */
};

template <class TYPE, bool TimeBased>
void StatCollector<TYPE, TimeBased>::print(std::ostream& out)
{
   out << "type: " << typeid(TYPE).name() << "   time based:"
       << (TimeBased ? "Y" : "N") << std::endl
       << "total: " << mCurTotal << "   samples:" << mSampleCount << std::endl
       << "mean: " << getMean() << std::endl
       << "staMaxTime: " << mSTAMaxTime.secf() << "s    Initial Sample Time:"
       << mInitialSampleTime.getMinutesf() << std::endl
       << "prev sampTime: " << mPrevSampleTime1.secf() << "s   prev sampTime2:"
       << mPrevSampleTime2.secf() << std::endl
       << "prev samp: " << mPrevSample1 << "   prev samp2:" << mPrevSample2
       << std::endl
       << " --- data --- time --- " << std::endl;

   for ( typename std::deque< std::pair<TYPE,vpr::Interval> >::iterator i = mSampleBuffer.begin();
          i != mSampleBuffer.end();
          ++i )
   {
      out << (*i).first << "   " << (*i).second.msec() << "ms\n";
   }
}

template <class TYPE, bool TimeBased>
void StatCollector<TYPE, TimeBased>::addSample(const TYPE sample)
{
   mCurTotal += sample;
   mSampleCount += 1;

   vpr::Interval cur_time;
   cur_time.setNow();                     // Set current time

   // Set Init time
   if(mSampleCount == 1)              // First send -- INIT TIMES
   {
      mInitialSampleTime.setNow();    // Initialize first read time
   }

   // Update recent sample values for INST average
   mPrevSampleTime1 = mPrevSampleTime2;      // Time of last 2 samples (mPrevST1 < mPrevSt2)
   mPrevSampleTime2 = cur_time;
   mPrevSample1 = mPrevSample2;
   mPrevSample2 = sample;

   // Add value to sample buffer
   mSampleBuffer.push_front( std::pair<TYPE,vpr::Interval>(sample, cur_time) );
}

template <class TYPE, bool TimeBased>
double StatCollector<TYPE, TimeBased>::getMean()
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
      mean_result = double(mCurTotal)/double(mSampleBuffer.size());
   }

   return mean_result;
}

template <class TYPE, bool TimeBased>
double StatCollector<TYPE, TimeBased>::getInstAverage()
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
double StatCollector<TYPE, TimeBased>::getSTA()
{
   vpr::Interval cur_time, diff_time;
   double diff_sec;                       // Num secs different in send times
   cur_time.setNow();                     // Set current time

   // --- CULL OFF old values --- //
   bool got_cull_point(false);
   typename std::deque< std::pair<TYPE,vpr::Interval> >::iterator cull_from;
   cull_from = mSampleBuffer.end();

   // Go until I am pointing at an element that is within range
   // OR I am at the head of the queue
   while((!got_cull_point) && (cull_from != mSampleBuffer.begin()))
   {
      --cull_from;   // Go to next one to check
      diff_time = cur_time - (*cull_from).second;
      if(diff_time < mSTAMaxTime)
         got_cull_point = true;
   }

   // Cull the values
   // - If at beginning, then just clear the whole thing
   // - Else clear from cull pt to end
   if(cull_from == mSampleBuffer.begin())
   {
      mSampleBuffer.clear();
   }
   else
   {
      ++cull_from;         // Increment to get to the first value to cull
      if(cull_from != mSampleBuffer.end())
      {
         mSampleBuffer.erase(cull_from, mSampleBuffer.end());     // Erase the values
      }
   }

   // Compute -- STA BANDWIDTH
   double sta_value(0.0f);

   if(mSampleBuffer.size() > 0)
   {
      double sum(0.0f);
      vpr::Interval first_sample_time(mSampleBuffer[0].second);

      typename std::deque< std::pair<TYPE,vpr::Interval> >::iterator cur_sample(mSampleBuffer.begin());

      for( ; cur_sample != mSampleBuffer.end(); ++cur_sample)       // Sum the total bandwidth up
      {  sum += (*cur_sample).first; }

      if(TimeBased)
      {
         diff_time = cur_time - first_sample_time;
         diff_sec = diff_time.secf();
         sta_value = sum/diff_sec;
      }
      else
      {
         sta_value = sum/double(mSampleBuffer.size());      // Compute as average of the buffered samples
      }

      if(sta_value > mMaxSTA)                        // Check for new max
         mMaxSTA = sta_value;
   }

   return sta_value;
}

} // namespace vpr


#endif
