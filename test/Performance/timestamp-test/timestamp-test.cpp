/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */

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

   vjTimeStamp begin, end, buf[5000];

   cout << "VJTimeStamp performance testing\n"
	<< "Host is " << getenv("HOST") << '\n';
   cout << "Timer resolution is " << begin.getResolution() << " us\n";

   // hopefully just get everything relevant in cache & stuff
   for (i = 0; i < 5000; i++)
       buf[i].set();

   begin.set();
   for (i = 0; i < 5000; ) {
       buf[i++].set();
       buf[i++].set();
       buf[i++].set();
       buf[i++].set();
       buf[i++].set();
       buf[i++].set();
       buf[i++].set();
       buf[i++].set();
       buf[i++].set();
       buf[i++].set();
   }
   end.set();

   total = end - begin;
   cout << "Test 1: 5000 timestamp sets in an unrolled loop\n"
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



}
