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


#include <jccl/jcclConfig.h>

namespace jccl {

//---------------------------------------------------------------
//: Dummy standin for PerfDataBuffer
class PerfDataBuffer {


 public:

    //: constructor
    //! PRE: true
    //! POST: self is created and has _numbufs buffers
    //! ARGS: _numbufs - number of buffers to allocate
    //+       (default 0)
    PerfDataBuffer (int _numbufs=0) {
    }

    //: destructor
    //: POST: all memory & buffers have been freed.
    ~PerfDataBuffer () {
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

}; // namespace jccl
