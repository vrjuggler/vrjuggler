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

#include <stdexcept>
#include <math.h>

#include "TestCase.h"
#include "TestResult.h"
#include "estring.h"



// Create a default TestResult
TestResult *TestCase::defaultResult ()
{ return new TestResult; } 


// Check for a failed general assertion 
void TestCase::assertImplementation (bool          condition,
                                     std::string   conditionExpression,
                                     long          lineNumber,
                                     std::string   fileName)
{ 
    if (!condition) 
        throw CppUnitException (conditionExpression, lineNumber, fileName); 
}


// Check for a failed equality assertion 
void TestCase::assertEquals (long        expected, 
                             long        actual,
                             long        lineNumber,
                             std::string fileName)
{ 
    if (expected != actual) 
        assertImplementation (false, notEqualsMessage(expected, actual), lineNumber, fileName); 
}


// Check for a failed equality assertion
void TestCase::assertEquals (double        expected, 
                             double        actual, 
                             double        delta,
                             long          lineNumber,
                             std::string   fileName)
{ 
    if (fabs (expected - actual) > delta) 
        assertImplementation (false, notEqualsMessage(expected, actual), lineNumber, fileName); 

}


// Run the test and catch any exceptions that are triggered by it 
void TestCase::run (TestResult *result)
{
    result->startTest (this);

    setUp ();

    try {
        runTest ();

    }
    catch (CppUnitException e) {
        CppUnitException *copy = new CppUnitException (e);
        result->addFailure (this, copy);

    }
    catch (std::exception e) {
        result->addError (this, new CppUnitException (e.what ()));

    }
    catch (...) {
        CppUnitException *e = new CppUnitException ("unknown exception");
        result->addError (this, e);

    }

    tearDown ();

    result->endTest (this);

}


// A default run method 
TestResult *TestCase::run ()
{
    TestResult *result = defaultResult ();

    run (result);
    return result;

}


// All the work for runTest is deferred to subclasses 
void TestCase::runTest ()
{
}


// Build a message about a failed equality check 
std::string TestCase::notEqualsMessage (long expected, long actual)
{ 
    return "expected: " + estring (expected) + " but was: " + estring (actual); 
}


// Build a message about a failed equality check 
std::string TestCase::notEqualsMessage (double expected, double actual)
{ 
    return "expected: " + estring (expected) + " but was: " + estring (actual); 
}
