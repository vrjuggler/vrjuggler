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

#ifndef CPPUNIT_TEST_H
#define CPPUNIT_TEST_H

#include <string>

class TestResult;


class Test
{
public:
    virtual                ~Test () = 0;

    virtual void           run (TestResult *result)    = 0;
    virtual int            countTestCases ()           = 0;
    virtual std::string    toString ()                 = 0;
};

inline Test::~Test ()
{}



// Runs a test and collects its result in a TestResult instance.
inline void Test::run (TestResult *result)
{}


// Counts the number of test cases that will be run by this test.
inline int Test::countTestCases ()
{ return 0; }


// Returns the name of the test instance. 
inline std::string Test::toString ()
{ return ""; }


#endif
