//----------------------------------------
// Test OpenGL C2Lib program
//
// ptest.C
//----------------------------------------
#include <nullApp.h>

      // --- Lib Stuff --- //
#include <Kernel/vjKernel.h>

int main(int argc, char* argv[])
{
   vjKernel* kernel = vjKernel::instance();        // Get the kernel
   nullApp* application = new nullApp(kernel);   // Declare an instance of the app
   kernel->setApplication(application);         // Set application
   
   vjTimeStamp::initialize();

   int i, j;
   float total, diff, avg;

   vjTimeStamp begin, end;
   vjPerfDataBuffer buf("test data buffer", 7100, 5, 1);

   cout << "vjPerfDataBuffer performance testing\n"
	<< "Host is " << getenv("HOST") << '\n';
   cout << "Timer resolution is " << begin.getResolution() << " us\n";

   buf.activate();

   // hopefully just get everything relevant in cache & stuff
   //   for (i = 0; i < 5000; i++)
   //  buf.set(5001);
   //buf.dumpData();

   begin.set();
   for (i = 0; i < 700; i++ ) {
       buf.set(i);
       buf.set(i);
       buf.set(i);
       buf.set(i);
       buf.set(i);
       buf.set(i);
       buf.set(i);
       buf.set(i);
       buf.set(i);
       buf.set(i);
   }
   end.set();

   //buf.write(cout);
   buf.writeTotal (cout, 1000, 1000, 1.0);

#if 0
   total = end - begin;
   cout << "Test 1: 5000 timestamp set()s in an unrolled loop\n"
	<< "    Total Time: " << total << endl
	<< "    Avg Time:   " << total/5000 << endl;

   // discrepency check
   cout << "    Discrepencies\n";
   avg = total/5000.0;
   for (i = 1; i < 5000; i++) {
       diff = buf[i] - buf[i-1];
       if (abs(diff-avg) > avg*1.0) {
	   cout << "        " << diff << " us at time " 
		<< buf[i-1] << endl;
       }
   }
#endif


}
