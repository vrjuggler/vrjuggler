/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#include <Performance/vjPerfDataBuffer.h>
#include <Kernel/vjDebug.h>


vjPerfDataBuffer::vjPerfDataBuffer (char* _name, int _numbufs,
				    int _nindex) {
    name = strdup (_name);
    numbufs = _numbufs;
    buffer = new buf_entry[numbufs];
    read_begin = 0;
    write_pos = 1;
    lost = 0;
    active = 0;
    nindex = _nindex;
}



//: destructor
//: POST: all memory & buffers have been freed.
vjPerfDataBuffer::~vjPerfDataBuffer () {
    active = 0;
    delete buffer;
    delete name;
}



//: activates the buffer
//! POST: once this call is made, the buffer will start
//+       storing data whenever a set() is made and
//+       writing available data when requested.
void vjPerfDataBuffer::activate() {
    active = 1;
    vjDEBUG(vjDBG_PERFORMANCE,1) << "Performance Buffer " << name << 
	" activated.\n" << vjDEBUG_FLUSH;
}



//: deactivates the buffer
//! POST: once this call is made, the buffer will,
//+       essentially, do nothing.  set() will not store
//+       any information and the write calls won't
//+       write anything.
void vjPerfDataBuffer::deactivate() {
    active = 0;
    /* deactivate maybe should reset the buffer so it's reactivated
     * with a clean slate, thusly:
     */
    read_begin = 0;
    write_pos = 1;
    lost = 0;
    vjDEBUG(vjDBG_PERFORMANCE,1) << "Performance Buffer " << name << 
	" deactivated.\n" << vjDEBUG_FLUSH;

}



bool vjPerfDataBuffer::isActive() {
    return active;
}



//: writes a new time entry to the buffer
//! POST: if a buffer is available, it is stamped with
//+       the current time and _phase.  If not, the
//+       'lost' counter is incremented.
//! ARGS: _phase - an integer index used to differentiate
//+       between different stamping points in the process
//+       that calls set. e.g. 1 = point right before
//+       entering some big computation, and 2 = point
//+       right after.
void vjPerfDataBuffer::set(int _phase) {
    int tw;

    if (!active)
	return;

    if (write_pos == read_begin) {
	if (lost_lock.acquire() != -1) {
	    lost++;
	    lost_lock.release();
	}
	else
	    vjDEBUG(vjDBG_ALL,2) << "vjPerfDataBuffer: lock acquire "
		       << "failed\n" << vjDEBUG_FLUSH;
	tw = (write_pos + numbufs - 1) % numbufs;
	buffer[tw].phase = _phase;
	buffer[tw].ts.set();
    }
    else {
	buffer[write_pos].phase = _phase;
	buffer[write_pos].ts.set();
	write_pos = (write_pos+1)%numbufs;
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
void vjPerfDataBuffer::write (ostream& out) {
    // the only tricky part of this is that the region we
    // want to print out might wrap back around to the
    // beginning of the list.  That's what the 2nd big
    // case is for.
    int begin, end, i, tlost;
    buf_entry* b;

    //out.width(13);

    if (!active)
	return;

    //out << "PerfData " << name << "\n";
    begin = read_begin;
    end = (write_pos - 1 + numbufs)%numbufs;
    //cout << "begin/end are " << begin <<' '<< end << endl;
    if (begin == end)
	return;
    out << "PerfData1 \"" << name << "\" " << nindex << endl;
    if (begin < end) {
	for (i = begin; i < end; i++) {
	    b = &(buffer[i]);
	    out << b->phase << ' '
		<< setiosflags(ios::fixed) << b->ts << '\n';
	}
    }
    else { /* wraparound */
	for (i = begin; i < numbufs; i++) {
	    b = &(buffer[i]);
	    out << b->phase << ' ' << setiosflags(ios::fixed)
		<< b->ts << '\n';
	}
	for (i = 0; i < end; i++) {
	    b = &(buffer[i]);
	    out << b->phase << ' ' << setiosflags(ios::fixed)
		<< b->ts << '\n';
	}
    }
	
    lost_lock.acquire();
    tlost = lost;
    lost = 0;
    lost_lock.release();
    read_begin = end;

    out << -1 << ' ' << tlost << endl;

}



// this probably isn't safe in a multitasking environment
void vjPerfDataBuffer::writeTotal(ostream& out, int preskip, int postskip, float discrep) {
    int begin = read_begin;
    int end = (write_pos - 1 + numbufs)%numbufs;
    int last = (end + numbufs-1) %numbufs;
    float retval;
    int nump;
    int i;
    float diff, avg;

    retval = buffer[(end-postskip)%numbufs].ts - buffer[(begin+preskip)%numbufs].ts;
    /*
    cout << "begin is " << begin
	 << "\nend is " << end
	 << "\nlast is " << last << endl;
    */
    if (last > begin)
	nump = last - begin;
    else
	nump = last + (numbufs - begin);

    avg = retval/(nump - preskip - postskip);

    out << "Dumping buffer: " << nump << " samples, total: "
	<< retval << " us, avg: " << avg << "us\n";
    if (discrep > 0) {
	out << "Reporting Discrepencies:\n";
    }
    end = (end-postskip)%numbufs;
    begin = (begin + preskip)%numbufs;

	if (begin == end)
	    return;
	else if (begin < end) {
	    for (i = begin+1; i < end; i++) {
		diff = buffer[i].ts - buffer[i-1].ts;
		if (abs(diff - avg) > avg * discrep) {
		    out << "    " << diff << " us at time "
			<< buffer[i-1].ts << " us\n";
		}
	    }
	}
	else { /* wraparound */
	    for (i = begin+1; i < numbufs; i++) {
		diff = buffer[i].ts - buffer[i-1].ts;
		if (abs(diff - avg) > avg * discrep) {
		    out << "    " << diff << " us at time "
			<< buffer[i-1].ts << " us\n";

		}
	    }
	    diff = buffer[0].ts - buffer[numbufs].ts;
	    if (abs(diff - avg) > avg * discrep) {
		out << "    " << diff << " us at time "
		    << buffer[numbufs].ts << " us\n";
	    }
	    for (i = 1; i < end; i++) {
		diff = buffer[i].ts - buffer[i-1].ts;
		if (abs(diff - avg) > avg * discrep) {
		    out << "    " << diff << " us at time "
			<< buffer[i-1].ts << " us\n";
		}
	    }
	
	}


    lost_lock.acquire();
    lost = 0;
    lost_lock.release();
    read_begin = end;

}


void vjPerfDataBuffer::dumpData() {

    lost_lock.acquire();
    read_begin = 0;
    write_pos = 1;
    lost = 0;
    lost_lock.release();

}
