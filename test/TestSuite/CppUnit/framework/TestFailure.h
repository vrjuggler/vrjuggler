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

#ifndef CPPUNIT_TESTFAILURE_H
#define CPPUNIT_TESTFAILURE_H

#ifndef CPPUNIT_GUARDS_H
#include "Guards.h"
#endif

#ifndef CPPUNIT_EXCEPTION_H
#include "CppUnitException.h"
#endif

class Test;



class TestFailure 
{
    REFERENCEOBJECT (TestFailure)

public:
                        TestFailure (Test *failedTest, CppUnitException *thrownException);
                        ~TestFailure ();

    Test                *failedTest ();
    CppUnitException    *thrownException ();
    std::string         toString ();

protected:
    Test                *m_failedTest;
    CppUnitException    *m_thrownException;

};


// Constructs a TestFailure with the given test and exception.
inline TestFailure::TestFailure (Test *failedTest, CppUnitException *thrownException)
 : m_failedTest (failedTest), m_thrownException (thrownException) 
{}


// Deletes the owned exception.
inline TestFailure::~TestFailure ()
{ delete m_thrownException; }


// Gets the failed test.
inline Test *TestFailure::failedTest ()
{ return m_failedTest; }


// Gets the thrown exception.
inline CppUnitException *TestFailure::thrownException ()
{ return m_thrownException; }


#endif
