/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#ifndef _VRJ_TEST_TESTCASE_H
#define _VRJ_TEST_TESTCASE_H

#include <vpr/vpr.h>
#include <vrj/vrjConfig.h>

#include <string>
#include <vector>

#include <vrj/Test/Test.h>

namespace vrj {
namespace test
{

/**
*
* Base class for normal single test case.
* 
*/
class TestCase : public Test
{
public:
   TestCase(std::string name)
      : mName(name)
   {;}

   virtual void setApp(vrj::App* app)
   { mApp = app; }

   virtual std::string getName()
   { return mName; }

protected:
   vrj::App*   mApp;          /**< Pointer to the application to be testing */
   std::string mName;         /**< Name of the test */
};

} }

#endif

