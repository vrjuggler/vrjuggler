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
#include <jccl/Plugins/PerformanceMonitor/TimeStamp.h>
#include <vpr/Sync/Mutex.h>
#include <jccl/Util/Debug.h>

namespace jccl {

/** Storage buffer for performance data.
 *  PerfDataBuffer is responsible for collecting performance data so
 *  that is can be periodically written out to a file or network
 *  connection.  The goal is to keep the act of collecting a sample
 *  as lightweight as possible, and to make sure that data can be 
 *  collected without blocking even while another thread is writing
 *  the data out.
 *  <p>
 *  The PerfDataBuffer is a circular buffer
 *  for perfdata.  Each unit in the buffer contains
 *  an integer index and a timestamp. the index is used
 *  in case there are multiple points inside the 
 *  process body where timestamps are generated
 *  (i.e. 1 = start of frame, 2 = before sync, 
 *  3 = between sync and buffer swap.
 *  <p>
 *  One process can write to the buffer (using the set() fn) 
 *  while another simultaneously reads from it using the 
 *  write() function.  The system is implemented so that the
 *  writing process never has to wait.  However, this means 
 *  that if the writer gets far enough ahead of the reader 
 *  that there are no more free buffers, the PerfDataBuffer
 *  will start throwing away data until there is a free buffer.
 *  The 'lost' field approximates the number of data samples
 *  lost; it is reported and reset at the conclusion of every
 *  write() call.
 * 
 *  @version $Revision$, $Date$
 */
class JCCL_CLASS_API PerfDataBuffer {

    struct buf_entry {

        /** Index marking where in the loop this point was collected. */
        int              phase;
        
        /** Timestamp associated with this point. */
        TimeStamp      ts;
        
        buf_entry() {
            phase = 0;
        }
    };



    buf_entry*  buffer;
    int         numbufs;
    int         lost;
    vpr::Mutex  lost_lock;

    int         read_begin;
    int         write_pos;

    bool        active;

public:
    std::string name;
    int         nindex;

 public:

    /** Constructor.
     *  Creates a new, inactive PerfDataBuffer.  The caller is 
     *  responsible for deciding the size of the buffer and the 
     *  number of indices that will be stored in it via set().
     *
     *  @param _name String identifier for this buffer.
     *  @param _numbufs Number of samples to store (default = 50).
     *  @param _nindex Number of unique indices this buffer will use.
     */
    PerfDataBuffer (const std::string& _name, int _numbufs, int _nindex);


    /** Destructor. */
    virtual ~PerfDataBuffer ();


    /** Returns the name of this instance. */
    virtual const std::string& getName() const {
        return name;
    }


    /** Activates the PerfDataBuffer.
     *  Once this method is called, the buffer will store a data 
     *  point whenever a set() is made.
     */
    void activate();



    /** Deactivates the PerfDataBuffer.
     *  When the buffer is deactivated, the set() and write calls
     *  will return without performing any actions.
     */
    void deactivate();



    /** Returns whether the buffer is active.
     *  @return True iff the buffer is active.
     */
    bool isActive();


    /** Records a new data point.
     *  If self is active and the buffer is not full, a new data
     *  point is recorded with the given index and the current
     *  time.
     *  <p>
     *  If the buffer is full, the lost counter is incremented.
     *
     *  @param _phase  An integer index used to differentiate
     *                 between different stamping points in the 
     *                 process that calls set(). e.g. 1 = point 
     *                 right before entering some big computation, 
     *                 and 2 = point right after.
     */
    void set (int _phase);


    /** Records a new data point with the given time value.
     *  If self is active and the buffer is not full, a new data
     *  point is recorded with the given index and the specified
     *  time.  This version of set() is useful for recording 
     *  data latency and other such values.
     *  <p>
     *  If the buffer is full, the lost counter is incremented.
     *
     *  @param _phase  An integer index used to differentiate
     *                 between different stamping points in the 
     *                 process that calls set(). e.g. 1 = point 
     *                 right before entering some big computation, 
     *                 and 2 = point right after.
     *  @param _value  A TimeStamp.
     */
    void set (int _phase, TimeStamp& _value);


    /** Writes buffer contents to an ostream.
     *  As many buffers as available are written to the ostream
     *  and released so that they can be used again by set.
     *  The format for the output is 'index timestamp\n', 
     *  followed by the number -1 and the lost count.
     *  For example:
     *  <pre>
     *     1 15.105
     *     2 17.333
     *     3 19.1919
     *     -1 0
     *  </pre>
     *  The stamp values are written in microseconds.
     *
     *  @param out An std::ostream that data is written to.
     */
    void write (std::ostream& out);


    /** Debugging method to write out all data in self.
     *  @param out An std::ostream that data is written to.
     *  @param preskip Skip the first n samples.
     *  @param postskip Skip the last n samples.
     *  @param discrep Proportion of error from average to note.
     */
    void writeTotal (std::ostream& out, int preskip, int postskip, float discrep);


    /** Empties the buffer and throws away the data. */
    void dumpData();

private:
   
    PerfDataBuffer (const PerfDataBuffer& o) {;}
    void operator= (const PerfDataBuffer& o) {;}
};

}; // namespace jccl

#endif
