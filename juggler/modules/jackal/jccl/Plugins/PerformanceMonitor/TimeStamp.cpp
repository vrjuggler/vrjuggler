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
#include <jccl/Plugins/PerformanceMonitor/TimeStamp.h>

// these should all be in separate .cpp files.  fix later

#if JCCL_PERFORMANCE == JCCL_PERF_SGI

//#include <jccl/Plugins/PerformanceMonitor/TimeStampSGI.h>

/*
 * SGI cyclecounter timing
 *
 * NOTE: to understand this code, it's helpful to read the
 * syssgi man page (search for SGI_QUERY_CYCLECNTR)
 *
 */

#include <stddef.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>


#include <sys/syssgi.h>
#include <sys/errno.h>
#include <jccl/Util/Debug.h>

namespace jccl {

void TimeStampSGI::initialize() {
    int cyclevalue;
    static unsigned long long enodev_dummy_address = 0;

    cyclecntrsize = syssgi(SGI_CYCLECNTR_SIZE);
    maxval = (1 << cyclecntrsize) -1;

    poffmask = getpagesize() - 1;
    phys_addr = syssgi(SGI_QUERY_CYCLECNTR, &cyclevalue);
    if (phys_addr == ENODEV) {
	vprDEBUG (vprDBG_ERROR,0) << clrOutNORM(clrRED, "ERROR:") << " TimeStamp: Cycle Counter not "
	    "supported by this machine.\n" << vprDEBUG_FLUSH;
	iotimer_addr = &enodev_dummy_address;
    }
    else {
	resolution = (float)cyclevalue/1000000.0;
	raddr = phys_addr & ~poffmask;
	fd = open("/dev/mmem", O_RDONLY);

	iotimer_addr =
	    (volatile void *)mmap(0, poffmask, PROT_READ,
				  MAP_PRIVATE, fd, (off_t)raddr);
	iotimer_addr =
	    (volatile void *)((__psunsigned_t)iotimer_addr +
			      (phys_addr & poffmask));
    }

    if (cyclecntrsize == 64)
	initval = *(unsigned long long*)iotimer_addr;
    else
	initval = *(unsigned int*)iotimer_addr;

    vprDEBUG(jcclDBG_PERFORMANCE,3) << "TimeStamp system initialized.\n"
	       << vprDEBUG_FLUSH;
}



float TimeStampSGI::usecs () {
    /* returns timestamp value in usecs from intialize() */
    return val*resolution;
}


TimeStampSGI::TimeStampSGI() {
    TimeStampInitializer::instance();
    set();
}


float TimeStampSGI::operator - (const TimeStampSGI& t2) const {
    /* returns self - t2, in usecs */
    return (val - t2.val)*resolution;
}


std::ostream& operator << (std::ostream& out, TimeStampSGI& ts) {
    out << ts.usecs();
    return out;
}



float TimeStampSGI::getResolution() {
    return resolution;
}


/* these need to be here to avoid "Unresolved data symbols"
 * problem w/ linker.
 */
__psunsigned_t TimeStampSGI::phys_addr = 0;
__psunsigned_t TimeStampSGI::raddr = 0;
volatile void* TimeStampSGI::iotimer_addr;
volatile unsigned long long TimeStampSGI::longcount;
int TimeStampSGI::fd,
    TimeStampSGI::poffmask;
float TimeStampSGI::resolution; // in usecs.
int TimeStampSGI::cyclecntrsize;  // either 32 or 64 bits. depends on hardware
long long TimeStampSGI::initval;
long long TimeStampSGI::maxval;

}; // namespace jccl

#elif JCCL_PERFORMANCE == JCCL_PERF_POSIX

/*
 * gettimeofday version
 */
#include <jccl/Plugins/PerformanceMonitor/TimeStampPosix.h>

namespace jccl {

    TimeStampPosix::TimeStampPosix() {
        TimeStampInitializer::instance();
	set();
    }


TimeStampPosix& TimeStampPosix::operator= (const TimeStampPosix& t2) {
    val = t2.val;
    return *this;
}


float TimeStampPosix::initval = 0.0;

}; // namespace jccl

#elif JCCL_PERFORMANCE == JCCL_PERF_NONE
// nothing to do here
#endif // JCCL_PERF_NONE


namespace jccl {
    vprSingletonImp(TimeStampInitializer);
};

