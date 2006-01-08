/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <iostream>
#include <stdio.h>
#include <ulocks.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#include <vpr/vpr.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Sync/CondVar.h>
#include <vpr/Util/Debug.h>

class SyncIncrementer
{
public:
   SyncIncrementer() : value(0), go(false)
      {;}
   
   int start();     // Start the thingie
   void trigger();   // Trigger increment
   void sync();      // Wait for completion
   void main(void* nullParam);   // Main loop
   
   void incValue()
      { value++;}
   void decValue()
      { value--;}
   int getValue()
      { return value; }

private:
   vpr::CondVar syncCond;     // condition var
   bool   go;
   int    value;
};


// ------------------------------ //
// -----    MAIN   -------------- //
// ------------------------------ //
int main(void)
{
   SyncIncrementer syncer;    // The test syncer
   
   // Spawn incrementer
   syncer.start();

   while (1)
   {         
      std::cerr << std::setw(5) << vpr::Thread::self() << "P1: Before Trigger"
                << std::endl;

         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "main: trigger\n" << vprDEBUG_FLUSH;
      syncer.trigger();    // Trigger the beginning of frame drawing
         vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "main: trigger done\n" << vprDEBUG_FLUSH;

      std::cerr << std::setw(5) << vpr::Thread::self() << "P1: Between"
                << std::endl;

         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "main: sync up\n" << vprDEBUG_FLUSH;
      syncer.sync();    // Block until drawing is done
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "main: sync done\n" << vprDEBUG_FLUSH;
      
      syncer.decValue();
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "VAL: " << syncer.getValue() << std::endl
                            << vprDEBUG_FLUSH;
   }

   return 1;
}


/// -------------------- ///
/// ---- Members ------- ///
/// -------------------- ///
    
//: Start the main function
int SyncIncrementer::start()
{
   // Create a new thread to handle the control
   vpr::ThreadMemberFunctor<SyncIncrementer>* memberFunctor = 
   new vpr::ThreadMemberFunctor<SyncIncrementer>(this, &SyncIncrementer::main, NULL);

   vpr::Thread* control_thread = new vpr::Thread(memberFunctor);

   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "SyncIncrementer::start: Just started main loop.  "
                           << control_thread << std::endl << vprDEBUG_FLUSH;

   return 1;
}


//: Trigger and increment 
void SyncIncrementer::trigger()
{
     // Allow the processes to draw
   syncCond.acquire();        // Get exclusive access
   {
      std::cerr << std::setw(5) << vpr::Thread::self() << "  P1: Signal"
                << std::endl;

      go = true;          // Signal that rendering can happen
      syncCond.signal();
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "Trigger signaled\n" << vprDEBUG_FLUSH;
         //syncCond.dump();
   }
   syncCond.release();
}
    
//: Wait for completion
void SyncIncrementer::sync()
{
   syncCond.acquire();
   {   // Wait for triggerRender == false
      while (go == true)
      {
         std::cerr << std::setw(5) << vpr::Thread::self() << "  P1: Wait"
                   << std::endl;
         syncCond.wait();
      }
      /* Do nothing */
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "Sync: Completed. trigger == false\n"
                                 << vprDEBUG_FLUSH;
         //syncCond.dump();
      std::cerr << std::setw(5) << vpr::Thread::self() << "  P1: Exit wait"
                << std::endl;
   }
   syncCond.release();
}


//: This is the main loop that incs
void SyncIncrementer::main(void* nullParam) {
   while (1)
   {
      syncCond.acquire();
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "Wait for trigger\n" << vprDEBUG_FLUSH;
         
         // Wait for trigger == true
         while (go == false)
         {
            std::cerr << std::setw(5) << vpr::Thread::self() << "  P2: Wait"
                      << std::endl;
            syncCond.wait();
         }
         
         std::cerr << std::setw(5) << vpr::Thread::self() << "  P2: Wait done"
                   << std::endl;
         //syncCond.dump();
         // THEN --- Do Work --- //
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "Incrementing\n" << vprDEBUG_FLUSH;
	
         incValue();

         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "Var Incremented - Set trigger FALSE and SIGNAL\n"
                                 << vprDEBUG_FLUSH;

         go = false;   // We are done rendering

         std::cerr << std::setw(5) << vpr::Thread::self() << "  P2: Signal"
                   << std::endl;
         syncCond.signal();

         //syncCond.dump();
      }
      syncCond.release();

      //cerr << "P2: Out of lock" << endl;
   }
}

