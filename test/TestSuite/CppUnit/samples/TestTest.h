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

#ifndef CPPUNIT_TESTTEST_H
#define CPPUNIT_TESTTEST_H

#ifndef CPPUNIT_TESTCASE_H
#include "TestCase.h"
#endif

#ifndef CPPUNIT_TESTSUITE_H
#include "TestSuite.h"
#endif

#ifndef CPPUNIT_TESTRESULT_H
#include "TestResult.h"
#endif

#ifndef CPPUNIT_TESTCALLER_H
#include "TestCaller.h"
#endif



class TestTest : public TestCase
{
protected:
    TestCase            *m_failure;
    TestCase            *m_error;
    TestCase            *m_success;

public:
                        TestTest (std::string name) : TestCase (name) {}

    void                testFailure ();
    void                testError ();
    void                testSuccess ();
    static Test         *suite ();

    void                setUp ();
    void                tearDown ();

private:
    class FailureTestCase : public TestCase 
    { public: FailureTestCase (std::string name) : TestCase (name) {} 
      protected: void runTest () { assert (false); }; };

    class ErrorTestCase : public TestCase 
    { public: ErrorTestCase (std::string name) : TestCase (name) {} 
    protected: void runTest () { int zero = 0; int result = 8 / zero; }; };

    class SuccessTestCase : public TestCase 
    { public: SuccessTestCase (std::string name) : TestCase (name) {} 
      protected: void runTest () { assert (true); }; };

};

inline void TestTest::setUp ()
{ 
    m_failure   = new FailureTestCase ("failure");
    m_error     = new ErrorTestCase ("error");
    m_success   = new SuccessTestCase ("success");
}

inline void TestTest::tearDown ()
{ 
    delete m_failure;
    delete m_error;
    delete m_success;
}


inline void TestTest::testFailure ()
{
    std::auto_ptr<TestResult>    result (m_failure->run ());

    assert (result->runTests () == 1);
    assert (result->testFailures () == 1);
    assert (result->testErrors () == 0);
    assert (!result->wasSuccessful ());

}

inline void TestTest::testError ()
{
    std::auto_ptr<TestResult> result (m_error->run ());

    assert (result->runTests () == 1);
    assert (result->testFailures () == 0);
    assert (result->testErrors () == 1);
    assert (!result->wasSuccessful ());

}


inline void TestTest::testSuccess ()
{
    std::auto_ptr<TestResult> result (m_success->run ());

    assert (result->runTests () == 1);
    assert (result->testFailures () == 0);
    assert (result->testErrors () == 0);
    assert (result->wasSuccessful ());

}


inline Test *TestTest::suite ()
{
    TestSuite *suite = new TestSuite ("TestTest");

    suite->addTest (new TestCaller<TestTest> ("testFailure", testFailure));
    suite->addTest (new TestCaller<TestTest> ("testError", testError));
    suite->addTest (new TestCaller<TestTest> ("testSuccess", testSuccess));

    return suite;
}

#endif
