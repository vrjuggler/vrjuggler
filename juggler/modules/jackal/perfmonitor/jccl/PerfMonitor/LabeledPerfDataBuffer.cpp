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


#include <jccl/Plugins/PerformanceMonitor/LabeledPerfDataBuffer.h>
//#include <vpr/Util/Debug.h>

namespace jccl {

LabeledPerfDataBuffer::LabeledPerfDataBuffer (const std::string& _name, int _numbufs,
                                int _nindex) {
    name = _name;
    numbufs = _numbufs;
    buffer = new buf_entry[numbufs];
    read_begin = 0;
    write_pos = 1;
    lost = 0;
    active = false;
    nindex = _nindex;
}



//: destructor
//: POST: all memory & buffers have been freed.
LabeledPerfDataBuffer::~LabeledPerfDataBuffer () {
    active = false;
    delete buffer;
}



//: activates the buffer
//! POST: once this call is made, the buffer will start
//+       storing data whenever a set() is made and
//+       writing available data when requested.
void LabeledPerfDataBuffer::activate() {
    active = true;
    vprDEBUG(jcclDBG_PERFORMANCE,1) << "Performance Buffer " << name << 
	" activated.\n" << vprDEBUG_FLUSH;
}



//: deactivates the buffer
//! POST: once this call is made, the buffer will,
//+       essentially, do nothing.  set() will not store
//+       any information and the write calls won't
//+       write anything.
void LabeledPerfDataBuffer::deactivate() {
    active = 0;
    /* deactivate maybe should reset the buffer so it's reactivated
     * with a clean slate, thusly:
     */
    read_begin = 0;
    write_pos = 1;
    lost = 0;
    vprDEBUG(jcclDBG_PERFORMANCE,1) << "Performance Buffer " << name << 
	" deactivated.\n" << vprDEBUG_FLUSH;

}



bool LabeledPerfDataBuffer::isActive() {
    return active;
}


void LabeledPerfDataBuffer::set (vpr::GUID category, 
                                 const std::string& index_name) {
    int tw;

    if (active && isCategoryActive (category)) {

        if (write_pos == read_begin) {
            if (lost_lock.acquire() != -1) {
                lost++;
                lost_lock.release();
            }
            else
                vprDEBUG(vprDBG_ALL,2) 
                    << "LabeledPerfDataBuffer: lock acquire "
                    << "failed\n" << vprDEBUG_FLUSH;
            tw = (write_pos + numbufs - 1) % numbufs;
            buffer[tw].category = category;
            buffer[tw].index = &index_name;
            buffer[tw].stamp.set();
        }
        else {
            buffer[write_pos].category = category;
            buffer[write_pos].index = &index_name;
            buffer[write_pos].stamp.set();
            write_pos = (write_pos+1)%numbufs;
        }
    }

}





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
void LabeledPerfDataBuffer::write (std::ostream& out) {
//     // the only tricky part of this is that the region we
//     // want to print out might wrap back around to the
//     // beginning of the list.  That's what the 2nd big
//     // case is for.
//     int begin, end, i, tlost;
//     buf_entry* b;

//     //out.width(13);

//     if (!active)
// 	return;

//     //out << "PerfData " << name << "\n";
//     begin = read_begin;
//     end = (write_pos - 1 + numbufs)%numbufs;
//     //cout << "begin/end are " << begin <<' '<< end << endl;
//     if (begin == end)
// 	return;
//     out << "PerfData1 \"" << name << "\" " << nindex << std::endl;
//     if (begin < end) {
// 	for (i = begin; i < end; i++) {
// 	    b = &(buffer[i]);
// 	    out << b->phase << ' '
// 		<< std::setiosflags(std::ios::fixed) << b->ts << '\n';
// 	}
//     }
//     else { /* wraparound */
// 	for (i = begin; i < numbufs; i++) {
// 	    b = &(buffer[i]);
// 	    out << b->phase << ' ' << std::setiosflags(std::ios::fixed)
// 		<< b->ts << '\n';
// 	}
// 	for (i = 0; i < end; i++) {
// 	    b = &(buffer[i]);
// 	    out << b->phase << ' ' << std::setiosflags(std::ios::fixed)
// 		<< b->ts << '\n';
// 	}
//     }
	
//     lost_lock.acquire();
//     tlost = lost;
//     lost = 0;
//     lost_lock.release();
//     read_begin = end;

//     out << -1 << ' ' << tlost << std::endl;

}


};
