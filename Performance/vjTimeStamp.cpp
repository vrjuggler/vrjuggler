
#include <Performance/vjTimeStamp.h>

#ifdef _VJ_SGI_TIMING_
/*
 * SGI cyclecounter timing
 *
 * NOTE: to understand this code, it's helpful to read the
 * syssgi man page (search for SGI_QUERY_CYCLECNTR)
 *
 */



#include <sys/syssgi.h>
#include <sys/errno.h>
#include <Kernel/vjDebug.h>

void vjTimeStamp::initialize() {
    int cyclevalue;
    static unsigned long long enodev_dummy_address = 0;

    cyclecntrsize = syssgi(SGI_CYCLECNTR_SIZE);
    maxval = (1 << cyclecntrsize) -1;
  
    poffmask = getpagesize() - 1;
    phys_addr = syssgi(SGI_QUERY_CYCLECNTR, &cyclevalue);
    if (phys_addr == ENODEV) {
	cerr << "ERROR: vjTimeStamp: Cycle Counter not supported "
	     << "by this machine."<< endl;
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

    vjDEBUG(3) << "vjTimeStamp system initialized.\n"
	       << vjDEBUG_FLUSH;
}



float vjTimeStamp::usecs () {
    /* returns timestamp value in usecs from intialize() */
    return val*resolution;
}



/* these need to be here to avoid "Unresolved data symbols" 
 * problem w/ linker.
 */
__psunsigned_t vjTimeStamp::phys_addr = 0;
__psunsigned_t vjTimeStamp::raddr = 0;
volatile void* vjTimeStamp::iotimer_addr;
volatile unsigned long long vjTimeStamp::longcount;
int vjTimeStamp::fd, 
    vjTimeStamp::poffmask;
float vjTimeStamp::resolution; // in usecs.
int vjTimeStamp::cyclecntrsize;  // either 32 or 64 bits. depends on hardware
long long vjTimeStamp::initval;
long long vjTimeStamp::maxval;


#else

/*
 * gettimeofday version 
 */

float vjTimeStamp::initval = 0.0;

#endif









