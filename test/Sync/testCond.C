/// TOP OF TEST_C

#include <iostream.h>
#include <stdio.h>
#include <ulocks.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#include <Sync/vjCond.h>
#include <Kernel/vjDebug.h>
#include <Threads/vjThread.h>

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
   vjCond syncCond;     // condition var
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
      //cerr << setw(5) << vjThread::self() << "P1: Before Trigger" << endl;   

         vjDEBUG(0) << "main: trigger\n" << vjDEBUG_FLUSH;
      syncer.trigger();    // Trigger the beginning of frame drawing
         vjDEBUG(3) << "main: trigger done\n" << vjDEBUG_FLUSH;

      //cerr << setw(5) << vjThread::self() << "P1: Between" << endl;

         vjDEBUG(0) << "main: sync up\n" << vjDEBUG_FLUSH;
      syncer.sync();    // Block until drawing is done
         vjDEBUG(0) << "main: sync done\n" << vjDEBUG_FLUSH;
      
      syncer.decValue();
      vjDEBUG(0) << "VAL: " << syncer.getValue() << endl << vjDEBUG_FLUSH;
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
   vjThreadMemberFunctor<SyncIncrementer>* memberFunctor = 
   new vjThreadMemberFunctor<SyncIncrementer>(this, &SyncIncrementer::main, NULL);

   vjThreadId* controlPid = vjThread::spawn(memberFunctor, 0);

   vjDEBUG(0) << "SyncIncrementer::start: Just started main loop.  "
              << *controlPid << endl << vjDEBUG_FLUSH;

   return 1;
}


//: Trigger and increment 
void SyncIncrementer::trigger()
{
     // Allow the processes to draw
   syncCond.acquire();        // Get exclusive access
   {
      cerr << setw(5) << vjThread::self() << "  P1: Signal" << endl;

      go = true;          // Signal that rendering can happen
      syncCond.signal();
         vjDEBUG(0) << "Trigger signaled\n" << vjDEBUG_FLUSH;
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
         cerr << setw(5) << vjThread::self() << "  P1: Wait" << endl;
         syncCond.wait();
      }
      /* Do nothing */
         vjDEBUG(0) << "Sync: Completed. trigger == false\n" << vjDEBUG_FLUSH;
         //syncCond.dump();
      cerr << setw(5) << vjThread::self() << "  P1: Exit wait" << endl;
   }
   syncCond.release();
}


//: This is the main loop that incs
void SyncIncrementer::main(void* nullParam)
{
   while (1)
   {
      syncCond.acquire();
      {
         vjDEBUG(0) << "Wait for trigger\n" << vjDEBUG_FLUSH;
         
         // Wait for trigger == true
         while (go == false)
         {
            cerr << setw(5) << vjThread::self() << "  P2: Wait" << endl;
            syncCond.wait();
         }
         
         cerr << setw(5) << vjThread::self() << "  P2: Wait done" << endl;
         //syncCond.dump();
         // THEN --- Do Work --- //
         vjDEBUG(0) << "Incrementing\n" << vjDEBUG_FLUSH;
	
         incValue();

         vjDEBUG(0) << "Var Incremented - Set trigger FALSE and SIGNAL\n" << vjDEBUG_FLUSH;

         go = false;   // We are done rendering

         cerr << setw(5) << vjThread::self() << "  P2: Signal" << endl;
         syncCond.signal();

         //syncCond.dump();
      }
      syncCond.release();

      //cerr << "P2: Out of lock" << endl;
   }
}

