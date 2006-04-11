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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/
#ifndef _JCCL_LABELED_PERF_DATA_BUFFER_H_
#define _JCCL_LABELED_PERF_DATA_BUFFER_H_

#include <jccl/jcclConfig.h>
#include <jccl/PerfMonitor/PerformanceCategories.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Util/GUID.h>
#include <jccl/Util/Debug.h>
#include <vpr/Util/Singleton.h>
#include <vpr/Util/Interval.h>

namespace jccl
{

/** Storage buffer for performance data.
 *  LabeledPerfDataBuffer is responsible for collecting performance data
 *  so that is can be periodically written out to a file or network
 *  connection.  The goal is to keep the act of collecting a sample
 *  as lightweight as possible, and to make sure that data can be
 *  collected without blocking even while another thread is writing
 *  the data out.
 *  <p>
 *  The LabeledPerfDataBuffer is a circular buffer
 *  for perfdata.  It can store data for multiple points inside
 *  the control loop of a thread, each loop identified by a text
 *  string.  Samples can also be tagged with categories.
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
class JCCL_CLASS_API LabeledPerfDataBuffer
{

   struct BufEntry
   {

      const std::string*  mIndexString;
      char*               mIndexCString;
      const PerformanceCategory*    mCategory;

      vpr::Interval       mStamp;

      BufEntry()
      {
         mIndexString = 0;
         mIndexCString = 0;
      }
   };


   BufEntry*   mBuffer;        /**< Buffer array. */
   int         mBufferCount;   /**< Length of mBuffer. */
   int         mLost;          /**< Count of 'lost' samples. */
   vpr::Mutex  mLostLock;

   int         mReadBegin;     /**< Index of first unread sample. */
   int         mWritePos;      /**< Index to write next sample. */

   std::string mName;

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
   LabeledPerfDataBuffer (const std::string& _name, int _numbufs);


   /** Default constructor. */
   LabeledPerfDataBuffer ();


   /** Destructor. */
   virtual ~LabeledPerfDataBuffer ();


   void setBufferSize (int n);


   /** Returns the name of this instance. */
   virtual const std::string& getName() const
   {
      return mName;
   }


   virtual void setName (std::string& _name)
   {
      mName = _name;
   }


   /** Records a new data point.
    *  If self is active and the buffer is not full, a new data
    *  point is recorded with the given index and the current
    *  time.
    *  <p>
    *  If the buffer is full, the lost counter is incremented.
    *
    *  @param category Category of this data point.
    *  @param index_name A name indicating where in the calling
    *                    thread's code the data point was gathered.
    */
   void set (PerformanceCategory &category, const std::string& index_name);

   void set (PerformanceCategory &category, char* index_name);


   /** Records a new data point.
    *  If self is active and the buffer is not full, a new data
    *  point is recorded with the given index and the given
    *  time.
    *  <p>
    *  If the buffer is full, the lost counter is incremented.
    *
    *  @param category Category of this data point.
    *  @param index_name A name indicating where in the calling
    *                    thread's code the data point was gathered.
    *  @param _value A TimeStamp.
    */
   void set (PerformanceCategory &category, const std::string& index_name,
             vpr::Interval& _value);

   void set (PerformanceCategory &category, char* index_name,
             vpr::Interval& _value);


//        /** Marks the "top" of the calling thread's loop.
//         *  This is necessary for some analysis and graphing of the
//         *  performance data.
//         */
//        void setBeginCycle (const vpr::GUID& category);


   /** Writes buffer contents to an ostream.
    *  As many buffers as available are written to the ostream
    *  and released so that they can be used again by set.
    *  The format for the output is, well, not yet defined.
    *  The stamp values are written in microseconds.
    *
    *  @param out An std::ostream that data is written to.
    */
   void write (std::ostream& out, const std::string& pad);



private:
   // win32 dlls need these for no good reason.
   LabeledPerfDataBuffer (const LabeledPerfDataBuffer& o) {;}
   void operator= (const LabeledPerfDataBuffer& o) {;}
};

} // namespace jccl


#endif
