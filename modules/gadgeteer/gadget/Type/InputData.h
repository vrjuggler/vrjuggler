/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/


#ifndef _GADGET_INPUTDATA_H_
#define _GADGET_INPUTDATA_H_

#include <gadget/gadgetConfig.h>
#include <jccl/Plugins/PerformanceMonitor/TimeStamp.h>
#include <vpr/Util/Interval.h>

namespace gadget {

    /** Base class for all input data returned by Gadgeteer.
     *  This provides all input data with a standard timestamping
     *  system used to calculate input latency.
     *  Note that currently two timestamping systems are supported,
     *  vpr::Interval and jccl::TimeStamp.  It's likely that the
     *  TimeStamp interface will be deprecated in favor of 
     *  Interval, but in the short term TimeStamp is needed to
     *  provide an appearance of consistency between measurements
     *  taken with VR Juggler 1.0.x.
     */
class GADGET_CLASS_API InputData {

public:

    /** Constructor. */
    InputData () {;}


    /** Record the current time using the jccl::TimeStamp. */
    void setTime () {
        mTimeStamp.set();
    }

    /** Record the given time. */
    void setTime (const jccl::TimeStamp& ts) {
        mTimeStamp = ts;
    }

    /** Return the last marked time using the jccl::TimeStamp. */
    jccl::TimeStamp getTime () {
        return mTimeStamp;
    }

    // uncomment these & comment out the above to switch from 
    // jccl::TimeStamp to vpr::Interval.

//     /** Record the current time using the vpr::Interval. */
//     void setTime () {
//         mInterval.setNow();
//     }

//     /** Record the given time. */
//     void setTime (const vpr::Interval& iv) {
//         mInterval = iv;
//     }

//     /** Return the last marked time using the vpr::Interval. */
//     vpr::Interval getTime () {
//         return mInterval;
//     }

protected:

    void copy (const InputData& id) {
        mInterval = id.mInterval;
        mTimeStamp = id.mTimeStamp;
    }

    vpr::Interval mInterval;
    jccl::TimeStamp mTimeStamp;


}; // class InputData


}; // namespace gadget


#endif
