
#ifndef CPPUNIT_ORTHODOX_H
#define CPPUNIT_ORTHODOX_H

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


template <class ClassUnderTest> class Orthodox : public TestCase
{
public:
                    Orthodox () : TestCase ("Orthodox") {}

protected:
    ClassUnderTest  call (ClassUnderTest object);
    void            runTest ();


};


// Run an orthodoxy test
template <class ClassUnderTest> void Orthodox<ClassUnderTest>::runTest ()
{
    // make sure we have a default constructor
    ClassUnderTest   a, b, c;

    // make sure we have an equality operator
    assert (a == b);

    // check the inverse
    b.operator= (a.operator! ());
    assert (a != b);

    // double inversion
    b = !!a;
    assert (a == b);

    // invert again
    b = !a;

    // check calls
    c = a;
    assert (c == call (a));

    c = b;
    assert (c == call (b));

}


// Exercise a call
template <class ClassUnderTest> ClassUnderTest Orthodox<ClassUnderTest>::call (ClassUnderTest object)
{
    return object;
}



#endif