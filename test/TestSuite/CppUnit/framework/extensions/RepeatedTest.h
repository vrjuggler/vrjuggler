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

#ifndef CPPUNIT_REPEATEDTEST_H
#define CPPUNIT_REPEATEDTEST_H

#ifndef CPPUNIT_GUARDS_H
#include "Guards.h"
#endif

#ifndef CPPUNIT_TESTDECORATOR_H
#include "TestDecorator.h"
#endif

class Test;
class TestResult;


class RepeatedTest : public TestDecorator 
{
    REFERENCEOBJECT (RepeatedTest)

public:
                        RepeatedTest (Test *test, int timesRepeat)
                            : TestDecorator (test), m_timesRepeat (timesRepeat) {}

    int                 countTestCases ();
    std::string         toString ();
    void                run (TestResult *result);

private:
    const int           m_timesRepeat;


};


// Counts the number of test cases that will be run by this test.
inline RepeatedTest::countTestCases ()
{ return TestDecorator::countTestCases () * m_timesRepeat; }

// Returns the name of the test instance. 
inline std::string RepeatedTest::toString ()
{ return TestDecorator::toString () + " (repeated)"; }

// Runs a repeated test
inline void RepeatedTest::run (TestResult *result)
{
    for (int n = 0; n < m_timesRepeat; n++) {
        if (result->shouldStop ())
            break;

        TestDecorator::run (result);
    }
}


#endif
