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

#include "MulticasterTest.h"
#include "TestSuite.h"


MulticasterTest::MulticasterTest (std::string name)
: TestCase (name)
{
}


MulticasterTest::~MulticasterTest()
{
}

void MulticasterTest::setUp ()
{
    m_multicaster = new Multicaster;
    m_o1          = new Observer;
    m_o2          = new Observer;
    m_o3          = new Observer;
    m_o4          = new Observer;

}


void MulticasterTest::tearDown ()
{
    delete m_o4;
    delete m_o3;
    delete m_o2;
    delete m_o1;
    delete m_multicaster;
}



void MulticasterTest::testSinglePublish ()
{
    // Make sure we can subscribe and publish to an address
    Value value;

    assert (m_multicaster->subscribe (m_o1, "alpha"));
    assert (m_multicaster->publish (NULL, "alpha", value));

    assert (*m_o1 == Observer ("alpha", 1));

}


void MulticasterTest::testMultipleHomogenousPublish ()
{
    // Make sure we can multicast to an address
    Value value;

    assert (m_multicaster->subscribe (m_o1, "alpha"));
    assert (m_multicaster->subscribe (m_o2, "alpha"));
    assert (m_multicaster->subscribe (m_o3, "alpha"));
    assert (m_multicaster->subscribe (m_o4, "alpha"));
    assert (m_multicaster->publish (NULL, "alpha", value));

    assert (*m_o1 == Observer ("alpha", 1));
    assert (*m_o2 == Observer ("alpha", 1));
    assert (*m_o3 == Observer ("alpha", 1));
    assert (*m_o4 == Observer ("alpha", 1));


}

void MulticasterTest::testMultipleHeterogenousPublish ()
{
    // Make sure we can multicast to several addresses at once
    Value value;

    assert (m_multicaster->subscribe (m_o1, "alpha"));
    assert (m_multicaster->subscribe (m_o2, "beta"));
    assert (m_multicaster->subscribe (m_o3, "alpha"));
    assert (m_multicaster->subscribe (m_o4, "beta"));
    assert (m_multicaster->publish (NULL, "alpha", value));

    assert (*m_o1 == Observer ("alpha", 1));
    assert (*m_o2 == Observer ());
    assert (*m_o3 == Observer ("alpha", 1));
    assert (*m_o4 == Observer ());

}

void MulticasterTest::testSingleUnsubscribe ()
{
    // Make sure we can unsubscribe one of two observers on the same address
    Value value;

    assert (m_multicaster->subscribe (m_o1, "alpha"));
    assert (m_multicaster->subscribe (m_o2, "alpha"));
    assert (m_multicaster->unsubscribe (m_o1, "alpha"));
    assert (m_multicaster->publish (NULL, "alpha", value));

    assert (*m_o1 == Observer ());
    assert (*m_o2 == Observer ("alpha", 1));

}


void MulticasterTest::testMultipleUnsubscribe ()
{
    // Make sure we unsubscribe all occurrences of an observer on the same address
    Value value;

    assert (m_multicaster->subscribe (m_o1, "alpha"));
    assert (m_multicaster->subscribe (m_o1, "alpha"));
    assert (m_multicaster->unsubscribe (m_o1, "alpha"));
    assert (m_multicaster->publish (NULL, "alpha", value));
    assert (*m_o1 == Observer ());

}


void MulticasterTest::testSimpleUnsubscribeAll ()
{
    // Make sure we unsubscribe all occurrences of an observer on all addresses
    Value value;

    assert (m_multicaster->subscribe (m_o1, "alpha"));
    assert (m_multicaster->subscribe (m_o1, "beck"));
    assert (m_multicaster->subscribe (m_o1, "gamma"));
    m_multicaster->unsubscribeFromAll (m_o1);
    assert (m_multicaster->publish (NULL, "alpha", value));
    assert (m_multicaster->publish (NULL, "beck", value));
    assert (m_multicaster->publish (NULL, "gamma", value));
    assert (*m_o1 == Observer ());

}


void MulticasterTest::testComplexUnsubscribeAll ()
{
    // Make sure we unsubscribe all occurrences of an observer on all addresses
    // in the presence of many observers
    Value value;

    assert (m_multicaster->subscribe (m_o1, "alpha"));
    assert (m_multicaster->subscribe (m_o1, "beck"));
    assert (m_multicaster->subscribe (m_o1, "gamma"));
    assert (m_multicaster->subscribe (m_o2, "beck"));
    assert (m_multicaster->subscribe (m_o2, "gamma"));
    assert (m_multicaster->subscribe (m_o2, "demeter"));
    m_multicaster->unsubscribeFromAll (m_o2);

    assert (m_multicaster->publish (NULL, "alpha", value));
    assert (m_multicaster->publish (NULL, "beck", value));
    assert (m_multicaster->publish (NULL, "gamma", value));
    assert (m_multicaster->publish (NULL, "demeter", value));
    assert (*m_o1 == Observer ("gamma", 3));
    assert (*m_o2 == Observer ());

}


Test *MulticasterTest::suite ()
{
    TestSuite *suite = new TestSuite ("Multicaster");

    suite->addTest (new MulticasterTest("testSinglePublish"));
    suite->addTest (new MulticasterTest("testMultipleHomogenousPublish"));
    suite->addTest (new MulticasterTest("testMultipleHeterogenousPublish"));
    suite->addTest (new MulticasterTest("testSingleUnsubscribe"));
    suite->addTest (new MulticasterTest("testMultipleUnsubscribe"));
    suite->addTest (new MulticasterTest("testSimpleUnsubscribeAll"));
    suite->addTest (new MulticasterTest("testComplexUnsubscribeAll"));

    return suite;

}

