
#ifndef CPPUNIT_TESTCALLER_H
#define CPPUNIT_TESTCALLER_H

#ifndef CPPUNIT_GUARDS_H
#include "Guards.h"
#endif

#ifndef CPPUNIT_TESTCASE_H
#include "TestCase.h"
#endif

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


template <class Fixture> class TestCaller : public TestCase
{ 
   REFERENCEOBJECT (TestCaller)

   typedef void             (Fixture::*TestMethod)();
    
public:
                            TestCaller (std::string name, TestMethod test)
                            : TestCase (name), m_fixture (new Fixture (name)), m_test (test)
                            {}

protected:
    void                    runTest () 
                            { (m_fixture.get ()->*m_test)(); }  

    void                    setUp ()
                            { m_fixture.get ()->setUp (); }

    void                    tearDown ()
                            { m_fixture.get ()->tearDown (); }

private:
   TestMethod               m_test;
   std::auto_ptr<Fixture>   m_fixture;

};



#endif
