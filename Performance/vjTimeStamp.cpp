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



#include <vjConfig.h>

// these should all be in separate .cpp files.  fix later

#if VJ_PERFORMANCE == VJ_PERF_SGI

#include <Performance/vjTimeStampSGI.h>

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
#include <Kernel/vjDebug.h>

void vjTimeStampSGI::initialize() {
    int cyclevalue;
    static unsigned long long enodev_dummy_address = 0;

    cyclecntrsize = syssgi(SGI_CYCLECNTR_SIZE);
    maxval = (1 << cyclecntrsize) -1;

    poffmask = getpagesize() - 1;
    phys_addr = syssgi(SGI_QUERY_CYCLECNTR, &cyclevalue);
    if (phys_addr == ENODEV) {
	vjDEBUG (vjDBG_ERROR,0) << clrOutNORM(clrRED, "ERROR:") << " vjTimeStamp: Cycle Counter not "
	    "supported by this machine.\n" << vjDEBUG_FLUSH;
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

    vjDEBUG(vjDBG_PERFORMANCE,3) << "vjTimeStamp system initialized.\n"
	       << vjDEBUG_FLUSH;
}



float vjTimeStampSGI::usecs () {
    /* returns timestamp value in usecs from intialize() */
    return val*resolution;
}


vjTimeStampSGI::vjTimeStampSGI() {
    set();
}


float vjTimeStampSGI::operator - (const vjTimeStampSGI& t2) const {
    /* returns self - t2, in usecs */
    return (val - t2.val)*resolution;
}


std::ostream& operator << (std::ostream& out, vjTimeStampSGI& ts) {
    out << ts.usecs();
    return out;
}



float vjTimeStampSGI::getResolution() {
    return resolution;
}


/* these need to be here to avoid "Unresolved data symbols"
 * problem w/ linker.
 */
__psunsigned_t vjTimeStampSGI::phys_addr = 0;
__psunsigned_t vjTimeStampSGI::raddr = 0;
volatile void* vjTimeStampSGI::iotimer_addr;
volatile unsigned long long vjTimeStampSGI::longcount;
int vjTimeStampSGI::fd,
    vjTimeStampSGI::poffmask;
float vjTimeStampSGI::resolution; // in usecs.
int vjTimeStampSGI::cyclecntrsize;  // either 32 or 64 bits. depends on hardware
long long vjTimeStampSGI::initval;
long long vjTimeStampSGI::maxval;


#elif VJ_PERFORMANCE == VJ_PERF_POSIX

/*
 * gettimeofday version
 */
#include <Performance/vjTimeStampPosix.h>

vjTimeStampPosix& vjTimeStampPosix::operator= (const vjTimeStampPosix& t2) {
    val = t2.val;
    return *this;
}


float vjTimeStampPosix::initval = 0.0;

#elif VJ_PERFORMANCE == VJ_PERF_NONE
// nothing to do here
#endif // VJ_PERF_NONE
