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


#include <vjConfig.h>

#include <Sync/vjCondGeneric.h>


// Wait for possible condition change
// POST: The condition has been modifed, but may not be satisfied.
// NOTE: The call blocks until a condition has been signaled
int
vjCondGeneric::wait () {
   cerr << setw(5) << getpid() << "  Wait: Begin:" << endl;
   // ASSERT:  We have been locked
   if (condMutex->test() == 0)    // Not locked
      cerr << " vjCondGeneric::wait: INCORRECT USAGE: Mutex was not locked when wait invoked!!!" << endl;

   waiters++;              // We have lock already

   condMutex->release();   // Release it

   sema.acquire();         // Wait for a while
   sema.dump();

   // We must now regain the lock so that the condition can be re checked upon exit
   // We also need it to decrement waiters
   condMutex->acquire();
   waiters--;

   cerr << setw(5) << getpid() << "  Wait: end:" << endl;
   return 0;
}
