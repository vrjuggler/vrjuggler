#ifndef _VPR_DURATION_STAT_COLLECTOR_H_
#define _VPR_DURATION_STAT_COLLECTOR_H_

#include <vpr/vprConfig.h>
#include <vpr/Util/SampleLimitedStatCollector.h>

namespace vpr
{

template
class DurationStatCollector : public SampleLimitedStatCollector<double, false>
{
public:
   DurationStatCollector(vpr::Interval::Unit units = vpr::Interval::Sec, unsigned sampleLimit = 100)
    : SampleLimitedStatCollector(sampleLimit)
   {
      mUnits = units;
      mStartTimeNeededNext = true;
   }

   void startSample()
   {
      vprASSERT(mStartTimeNeededNext);
      mStartTimeNeededNext = false;

      mSampleStartTime.now();
   }

   void stopSample()
   {
      vprASSERT(!mStartTimeNeededNext);
      mStartTimeNeededNext = true;

      // Compute time difference
      mSampleStopTime.now();
      vpr::Interval diff(mSampleStartTime - mSampleStopTime);
      double sample(0.0f);

      switch(timeUnit)
      {
      case vpr::Interval::Sec:
         sample = diff.secf();
         break;
      case vpr::Interval::Msec:
         sample = diff.msecf();
         break;
      case vpr::Interval::Usec:
         sample = diff.usecf();
         break;
      default:
         vprASSERT(false && "Invalid units");
         break;
      }

      addSample(sample);
   }

protected:
   vpr::Interval::Unit  mUnits;

   vpr::Interval  mSampleStartTime;
   vpr::Interval  mSampleStopTime;

   bool mStartTimeNeededNext;       // For helping with debugging
}

}; // namespace

#endif
