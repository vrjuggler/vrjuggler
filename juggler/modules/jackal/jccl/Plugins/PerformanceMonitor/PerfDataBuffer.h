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




#ifndef _JCCL_PERF_DATA_BUFFER_H_
#define _JCCL_PERF_DATA_BUFFER_H_

#include <jccl/jcclConfig.h>
#include <jccl/Performance/TimeStamp.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Util/Debug.h>

namespace jccl {

//---------------------------------------------------------------
//: temporary storage for performance data
//
// Problem: gathering up the performance data and shipping it out
//          to wherever should happen independently of the 
//          process that's generating the data.
// <p>
// Solution: the PerfDataBuffer is used as a temporary storage
//           for perfdata.  Each unit in the buffer contains
//           an integer index and a timestamp. the index is used
//           in case there are multiple points inside the 
//           process body where timestamps are generated
//           (i.e. 1 = start of frame, 2 = before sync, 
//           3 = between sync and buffer swap.
// <p>
// One process can write to the buffer (using the set() fn) 
// while another simultaneously reads from it using the 
// write() function.  The system is implemented so that the
// writing process never has to wait.  However, this means 
// that if the writer gets far enough ahead of the reader 
// that there are no more free buffers, the PerfDataBuffer
// will start throwing away data until there is a free buffer.
// The 'lost' field approximates the number of data samples
// lost; it is reported and reset at the conclusion of every
// write() call.
//
//----------------------------------------------------------------
class VJ_CLASS_API PerfDataBuffer {

    struct buf_entry {

        //: an index for the point in the proc. that we're at
        int              phase;
        
        //: time stamp associated with this point.
        TimeStamp      ts;
        
        buf_entry() {
            phase = 0;
        }
    };



    buf_entry*  buffer;
    int         numbufs;
    int         lost;
    vpr::Mutex     lost_lock;

    int         read_begin;
    int         write_pos;

    //: Buffer is currently active
    bool        active;

public:
    std::string name;
    int         nindex;

 public:

    //: constructor
    //! PRE: true
    //! POST: self is created and has _numbufs buffers
    //! ARGS: _numbufs - number of buffers to allocate
    //+       (default 50)
    PerfDataBuffer (const std::string& _name, int _numbufs, int _nindex);


    //: destructor
    //: POST: all memory & buffers have been freed.
    virtual ~PerfDataBuffer ();



    virtual const std::string& getName() const {
        return name;
    }

    //: activates the buffer
    //! POST: once this call is made, the buffer will start 
    //+       storing data whenever a set() is made and
    //+       writing available data when requested.
    void activate();



    //: deactivates the buffer
    //! POST: once this call is made, the buffer will,
    //+       essentially, do nothing.  set() will not store
    //+       any information and the write calls won't
    //+       write anything.
    void deactivate();



    //: is the buffer active?
    //! RETURNS: True - buffer is currently active
    //! RETURNS: False - buffer is not active
    bool isActive();


    //: writes a new time entry to the buffer
    //! POST: if a buffer is available, it is stamped with 
    //+       the current time and _phase.  If not, the
    //+       'lost' counter is incremented.
    //! ARGS: _phase - an integer index used to differentiate
    //+       between different stamping points in the process
    //+       that calls set. e.g. 1 = point right before
    //+       entering some big computation, and 2 = point
    //+       right after.
    void set (int _phase);

    void set (int _phase, TimeStamp& _value);

    // for below: need a version w/ max # buffers to write

    //: writes buffer contents to an ostream
    //! POST: As many buffers as available are written to
    //+       the ostream out and released so they can be
    //+       used again by the writer.
    //! ARGS: out - an ostream to write contents to.
    //! NOTE: The format for a buffer is 'ind timestamp\n',
    //+       e.g.: (for four buffers, say we have 3 indices)
    //+       <br>1 15
    //+       <br>2 25
    //+       <br>3 27
    //+       <br>1 42
    void write (std::ostream& out);


    //: just deletes all the current info in buffer.
    //! NOTE: this is mainly a utility used in testing performance
    //+       of the perf data collection
    //! RETURNS: x - time in usecs between first and last points.
    //+          which is only useful if you know how many pts 
    //+          there are...
    void writeTotal (std::ostream& out, int preskip, int postskip, float discrep);


    //: just empties out the buffer & throws away the data.
    void dumpData();

private:
   
    PerfDataBuffer (const PerfDataBuffer& o) {;}
    void operator= (const PerfDataBuffer& o) {;}
};

}; // namespace jccl

#endif
