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
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#ifndef _VPR_DURATION_STAT_COLLECTOR_H_
#define _VPR_DURATION_STAT_COLLECTOR_H_

#include <vpr/vprConfig.h>
#include <vpr/Util/SampleLimitedStatCollector.h>

namespace vpr
{

/** \class DurationStatCollector DurationStatCollector.h vpr/Util/DurationStatCollector.h
 *
 * Duration statistics collector class.
 */
class DurationStatCollector : public SampleLimitedStatCollector<double, false>
{
public:
   DurationStatCollector(vpr::Interval::Unit units = vpr::Interval::Sec,
                         unsigned sampleLimit = 100)
      : SampleLimitedStatCollector<double, false>(sampleLimit)
      , mUnits(units)
      , mStartTimeNeededNext(true)
   {
      ;
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
      vpr::Interval diff(mSampleStopTime - mSampleStartTime);
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

} // namespace


#endif
