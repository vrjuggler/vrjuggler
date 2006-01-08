/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#ifndef _VRJ_TEST_TEST_H
#define _VRJ_TEST_TEST_H

#include <vpr/vpr.h>
#include <vrj/vrjConfig.h>
#include <vrj/Kernel/App.h>

#include <string>
#include <vector>
#include "TestFailure.h"

namespace vrj {
namespace test
{

/** \class Test Test.h vrj/Test/Test.h
 * 
 * Base interface for what all tests must look like.
 */
class Test
{
public:
   Test()
    {;}

   /**
    * Sets the application that we are testing.
    * To do anything meaningful you will probably have to dynamic_cast the
    * app to you application type.
    */
   virtual void setApp(vrj::App* app) = 0;
   
   /** Do any setup needed before the test starts. */
   virtual void setUp()
   {;}

   /** Do any cleanup needed after running. */
   virtual void tearDown()
   {;}

   /**
    * Gives the test time to process (that is, run & test).
    * This method is called by the runner at then end of vrj::App:;preFrame()
    * until isDone() is true.
    *
    * @pre isDone() == false
    * @throws vrj::test::TestFailure Thrown if the test fails.
    */
   virtual void processTest() = 0;

   /**
    * Returns true when the test is done processing.
    * testing 
    */
   virtual bool isDone() = 0;

   /** Gets the name of the test. */
   virtual std::string getName() = 0;

private:            

   /** @link dependency 
    * @stereotype throws*/
   /*# TestFailure lnkTestFailure; */
};

} }

#endif

