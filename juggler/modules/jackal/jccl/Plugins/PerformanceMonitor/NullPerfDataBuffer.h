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
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */




#include <iostream.h>


//---------------------------------------------------------------
//: Dummy standin for vjPerfDataBuffer
class vjPerfDataBuffer {


 public:

    //: constructor
    //! PRE: true
    //! POST: self is created and has _numbufs buffers
    //! ARGS: _numbufs - number of buffers to allocate
    //+       (default 0)
    vjPerfDataBuffer (int _numbufs=0) {
    }

    //: destructor
    //: POST: all memory & buffers have been freed.
    ~vjPerfDataBuffer () {
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
    inline void set(int _phase) {
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
    inline void write (ostream& out) {
    }

};



