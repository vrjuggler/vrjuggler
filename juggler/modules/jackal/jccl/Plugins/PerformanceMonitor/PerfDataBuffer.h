

#include <Performance/vjTimeStamp.h>
#include <Sync/vjMutex.h>
#include <iostream.h>
#include <Kernel/vjDebug.h>

//---------------------------------------------------------------
//: temporary storage for performance data
//
// Problem: gathering up the performance data and shipping it out
//          to wherever should happen independently of the 
//          process that's generating the data.
// <p>
// Solution: the vjPerfDataBuffer is used as a temporary storage
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
class vjPerfDataBuffer {

    struct buf_entry {

	//: an index for the point in the proc. that we're at
	int              phase;

	//: probly not used.
	int              lost;

	//: time stamp associated with this point.
	vjTimeStamp      ts;

	_buf() {
	    phase = lost = 0;
	}
    };



    buf_entry*  buffer;
    int         numbufs;
    int         lost;
    vjMutex     lost_lock;

    int         read_begin;
    int         write_pos;

 public:

    //: constructor
    //! PRE: true
    //! POST: self is created and has _numbufs buffers
    //! ARGS: _numbufs - number of buffers to allocate
    //+       (default 25)
    vjPerfDataBuffer (int _numbufs=25) {
	numbufs = _numbufs;
	buffer = new buf_entry[numbufs];
	read_begin = 0;
	write_pos = 1;
	lost = 0;
    }

    //: destructor
    //: POST: all memory & buffers have been freed.
    ~vjPerfDataBuffer () {
	delete buffer;
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
    void set(int _phase) {
	if (write_pos == read_begin) {
	    if (lost_lock.acquire() != -1) {
		lost++;
		lost_lock.release();
	    }
	    else
		vjDEBUG(2) << "vjPerfDataBuffer: lock acquire "
			   << "failed\n" << vjDEBUG_FLUSH;
	}
	else {
	    buffer[write_pos].phase = _phase;
	    buffer[write_pos].ts.set();
	    write_pos = (write_pos+1)%numbufs;
	}
    }



    // for below: need a version w/ max buffers to write

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
    void write (ostream& out) {
	int begin, end, i, tlost;
	buf_entry* b;
	begin = read_begin;
	end = (write_pos - 1 + numbufs)%numbufs;
cout << "begin/end are " << begin <<' '<< end << endl;
	if (begin == end)
	    return;
	else if (begin < end) {
	    for (i = begin; i < end; i++) {
		b = &(buffer[i]);
		out <<i<<' '<< b->phase << ' ' << b->lost << ' ' 
		    << b->ts.usecs() << '\n';
		b->lost = 0;
	    }
	    //read_begin = end;
	}
	else { /* wraparound */
	    for (i = begin; i < numbufs; i++) {
		b = &(buffer[i]);
		out <<i<<' '<< b->phase << ' ' << b->lost << ' ' 
		    << b->ts.usecs() << '\n';
	    }
	    for (i = 0; i < end; i++) {
		b = &(buffer[i]);
		out <<i<<' '<< b->phase << ' ' << b->lost << ' ' 
		    << b->ts.usecs() << '\n';
	    }
	    //read_begin = end;

	}

	lost_lock.acquire();
	tlost = lost;
	lost = 0;
	lost_lock.release();
	read_begin = end;

	out << -1 << ' ' << tlost << '\n';
    }


};



