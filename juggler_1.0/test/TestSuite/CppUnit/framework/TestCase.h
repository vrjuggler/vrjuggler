
#ifndef CPPUNIT_TESTCASE_H
#define CPPUNIT_TESTCASE_H

#include <string>
#include <typeinfo> // for std::type_info

#ifndef CPPUNIT_GUARDS_H
#include "Guards.h"
#endif

#ifndef CPPUNIT_TEST_H
#include "Test.h"
#endif

#ifndef CPPUNIT_CPPUNITEXCEPTION_H
#include "CppUnitException.h"
#endif

class TestResult;



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


class TestCase : public Test 
{
    REFERENCEOBJECT (TestCase)

public:
                        TestCase         (std::string Name);
                        ~TestCase        ();

    virtual void        run              (TestResult *result);
    virtual TestResult  *run             ();
    virtual int         countTestCases   ();
    std::string         name             ();
    std::string         toString         ();

    virtual void        setUp            ();
    virtual void        tearDown         ();

protected:
    virtual void        runTest          ();

    TestResult          *defaultResult   ();
    void                assertImplementation 
                                         (bool         condition, 
                                          std::string  conditionExpression = "",
                                          long         lineNumber = CPPUNIT_UNKNOWNLINENUMBER,
                                          std::string  fileName = CPPUNIT_UNKNOWNFILENAME);

    void                assertEquals     (long         expected, 
                                          long         actual,
                                          long         lineNumber = CPPUNIT_UNKNOWNLINENUMBER,
                                          std::string  fileName = CPPUNIT_UNKNOWNFILENAME);

    void                assertEquals     (double       expected, 
                                          double       actual, 
                                          double       delta, 
                                          long         lineNumber = CPPUNIT_UNKNOWNLINENUMBER,
                                          std::string  fileName = CPPUNIT_UNKNOWNFILENAME);

    std::string         notEqualsMessage (long         expected, 
                                          long         actual);

    std::string         notEqualsMessage (double       expected, 
                                          double       actual);
    
private:
    const std::string   m_name;



};


// Constructs a test case
inline TestCase::TestCase (std::string name) 
: m_name (name) 
{}


// Destructs a test case
inline TestCase::~TestCase ()
{}


// Returns a count of all the tests executed
inline int TestCase::countTestCases ()
{ return 1; }


// Returns the name of the test case
inline std::string TestCase::name ()
{ return m_name; }


// A hook for fixture set up
inline void TestCase::setUp ()
{}


// A hook for fixture tear down
inline void TestCase::tearDown ()
{}


// Returns the name of the test case instance
inline std::string TestCase::toString ()
{ const std::type_info& thisClass = typeid (*this); return std::string (thisClass.name ()) + "." + name (); }



// A set of macros which allow us to get the line number
// and file name at the point of an error.
// Just goes to show that preprocessors do have some
// redeeming qualities.

#define CPPUNIT_SOURCEANNOTATION

#ifdef CPPUNIT_SOURCEANNOTATION

    #undef assert
    #define assert(condition)\
    (this->assertImplementation ((condition),(#condition),\
        __LINE__, __FILE__))

#else

    #undef assert
    #define assert(condition)\
    (this->assertImplementation ((condition),"",\
        __LINE__, __FILE__))

#endif


// Macros for primitive value comparisons
#define assertDoublesEqual(expected,actual,delta)\
(this->assertEquals ((expected),\
        (actual),(delta),__LINE__,__FILE__))

#define assertLongsEqual(expected,actual)\
(this->assertEquals ((expected),\
        (actual),__LINE__,__FILE__))


#endif
