#ifndef _VJ_SYSTEM_DATA_
#define _VJ_SYSTEM_DATA_

#include <vjConfig.h>

/**
 * vjSystemData: Globabl data that many objects need access to.
 * 
 * This object is designed to encapsulte a variety of data
 * at the Kernel level that needs to be access by many objects
 * at several levels.  This object will provide a single point
 * of access.
 */
class vjSystemData
{

public:
    vjInputManager*	    inputManager;
};

#endif
