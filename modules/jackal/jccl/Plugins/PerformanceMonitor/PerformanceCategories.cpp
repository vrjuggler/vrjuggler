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



#include <jccl/Plugins/PerformanceMonitor/PerformanceCategories.h>
#include <jccl/Plugins/PerformanceMonitor/LabeledPerfDataBuffer.h>

namespace jccl {

    void PerformanceCategories::addBuffer (LabeledPerfDataBuffer* buffer) {
        mBuffersLock.acquire();
        mBuffers.push_back (buffer);
        mBuffersLock.release();
    }

    void PerformanceCategories::removeBuffer (LabeledPerfDataBuffer* buffer) {
        mBuffersLock.acquire();
        std::vector<LabeledPerfDataBuffer*>::iterator i = mBuffers.begin();
        while (i != mBuffers.end()) {
            if (*i == buffer) {
                mBuffers.erase(i);
                break;
            }
        }
        mBuffersLock.release();
    }

    void PerformanceCategories::writeAllBuffers (std::ostream& out, 
                                                 const std::string& pad /*=""*/) {
        mBuffersLock.acquire();
        for (int i = 0; i < mBuffers.size(); i++) {
            mBuffers[i]->write (out, pad);
        }
        mBuffersLock.release();
    }


    vprSingletonImp (PerformanceCategories);

    jcclREGISTER_PERF_CATEGORY(jcclPERF_JACKAL, PERF_JACKAL);


};
