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

#ifndef MULTICASTERTEST_H
#define MULTICASTERTEST_H

#include "Multicaster.h"

#ifndef CPPUNIT_TESTCASE_H
#include "TestCase.h"
#endif





class MulticasterTest : public TestCase
{
public:
    class Observer : public MulticastObserver
    {
    public:
        int             m_state;
        std::string     m_lastAddressReceived;

                        Observer () : m_state (0) {}
                        Observer (std::string initialAddress, int state) 
                            : m_lastAddressReceived (initialAddress), m_state (state) {}

        virtual void    accept (std::string address, Value Value)
        { m_lastAddressReceived = address; m_state++; }

    };

    Multicaster                 *m_multicaster;
    Observer                    *m_o1;
    Observer                    *m_o2;
    Observer                    *m_o3;
    Observer                    *m_o4;


protected:
    void                        setUp ();
    void                        tearDown ();

    void                        testSinglePublish ();
    void                        testMultipleHomogenousPublish ();
    void                        testMultipleHeterogenousPublish ();
    void                        testSingleUnsubscribe ();
    void                        testMultipleUnsubscribe ();
    void                        testSimpleUnsubscribeAll ();
    void                        testComplexUnsubscribeAll ();

public:
                                MulticasterTest (std::string name);
    virtual                     ~MulticasterTest ();

    static Test                 *suite ();

};


inline bool operator== (const MulticasterTest::Observer& o1, const MulticasterTest::Observer& o2)
{ return o1.m_state == o2.m_state && o1.m_lastAddressReceived == o2.m_lastAddressReceived; }


#endif
