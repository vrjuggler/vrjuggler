#ifndef _VPR_DURATION_STAT_COLLECTOR_H_
#define _VPR_DURATION_STAT_COLLECTOR_H_

#include <vpr/vprConfig.h>
#include <vpr/Util/SampleLimitedStatCollector.h>

namespace vpr
{

class DurationStatCollector : public SampleLimitedStatCollector<double, false>
{
public:
   DurationStatCollector(vpr::Interval::Unit units = vpr::Interval::Sec, unsigned sampleLimit = 100)
    : SampleLimitedStatCollector<double, false>(sampleLimit)
   {
      mUnits = units;
      mStartTimeNeededNext = true;
   }

   void startSample()
   {
      vprASSERT(mStartTimeNeededNext);
      mStartTimeNeededNext = false;

      mSampleStartTime.setNow();
      //std::cout << "sta: " << mSampleStartTime.usec() << "\n";
   }

   void stopSample()
   {
      vprASSERT(!mStartTimeNeededNext);
      mStartTimeNeededNext = true;

      // Compute time difference
      mSampleStopTime.setNow();
      //std::cout << "sto: " << mSampleStopTime.usec() << "\n";
      vpr::Interval diff(mSampleStartTime - mSampleStopTime);
      //std::cout << "d: " << diff.usec() << "\n";
      double sample(0.0f);

      switch(mUnits)
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

      //std::cout << "s: " << sample << "\n";
      addSample(sample);
   }

protected:
   vpr::Interval::Unit  mUnits;

   vpr::Interval  mSampleStartTime;
   vpr::Interval  mSampleStopTime;

   bool mStartTimeNeededNext;       // For helping with debugging
};

}; // namespace

#endif
