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

#include <iostream>
#include <vector>

#include <cppunit/TestSuite.h>
#include <cppunit/TextTestResult.h>

#include <TestCases/Math/QuatSelfTest.h>
#include <vrj/Util/Debug.h>


static void addNoninteractive (CppUnit::TestSuite* suite)
{
   std::vector<vrjTest::JugglerTest*> tests;

   // Add new tests to this vector.
   tests.push_back(new vrjTest::QuatSelfTest());

   for ( std::vector<vrjTest::JugglerTest*>::iterator i = tests.begin();
         i != tests.end();
         i++ )
   {
      (*i)->registerTests(suite);
   }
}

static void addInteractive (CppUnit::TestSuite* suite)
{
}

static void addMetrics (CppUnit::TestSuite* suite)
{
}

int main (int ac, char **av)
{
   vjDEBUG(0,0) << "Starting test\n" << vjDEBUG_FLUSH;       // Do this here to get init text out of the way

   CppUnit::TestSuite suite;

   if ( ac > 1 && strcmp(av[1], "all") != 0 )
   {
      for ( int i = 1; i < ac; i++ )
      {
         // -------------------------------
         // NON-INTERACTIVE
         // -------------------------------
         if ( strcmp(av[i], "noninteractive") == 0 )
         {
            addNoninteractive(&suite);
         }
         // ------------------------------
         // METRICS
         // ------------------------------
         else if ( strcmp(av[i], "metrics") == 0 )
         {
            addMetrics(&suite);
         }
         // -------------------------------
         // INTERACTIVE
         // -------------------------------
         else if ( strcmp(av[i], "interactive") == 0 )
         {
            addInteractive(&suite);
         }
         else
         {
            std::cerr << "WARNING: Unknown suite name " << av[i] << std::endl;
         }
      }
   }
   else
   {
      addNoninteractive(&suite);
      addMetrics(&suite);
      addInteractive(&suite);
   }

   CppUnit::TextTestResult result;
   suite.run(&result);
   result.print(std::cout);

   return 0;
}
