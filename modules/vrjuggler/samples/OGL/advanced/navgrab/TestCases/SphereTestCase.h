/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _SPHERE_TEST_CASE_H
#define _SPHERE_TEST_CASE_H

#include <vrj/Test/TestCase.h>
#include <vrj/Test/Asserter.h>

#include <iostream>
#include <string>

#include <gadget/InputManager.h>
#include <gadget/InputLogger.h>

#include <NavGrabApp.h>
#include <gmtl/VecOps.h>
#include <gmtl/Output.h>

                     
/** A very small class that demonstrates how to 
* fail a test.
*/
class SphereTestCase : public vrj::test::TestCase
{
public:
   SphereTestCase()
      : vrj::test::TestCase("SphereTestCase"), mNavGrabApp(NULL), mCurState(Done)
   {;}

   virtual ~SphereTestCase()
   {;}

   /** Do any setup needed before the test starts */
   virtual void setUp()
   {
      vprASSERT(mApp != NULL);

      mNavGrabApp = dynamic_cast<NavGrabApp*>(mApp);
      vprASSERT(NULL != mNavGrabApp && "App is of wrong type");

      mCurState = Running;

      // Reset the application and start the logger
      mNavGrabApp->reset();
      gadget::InputManager* input_mgr = gadget::InputManager::instance();
      mInputLogger = input_mgr->getInputLogger();

      mInputLogger->load("data/sphere_test.xml");
      mInputLogger->play();
   }

   /** Do any cleanup needed after running */
   virtual void tearDown()
   {
      std::cout << "FailureTestCase::tearDown: Nothing to tear down\n";
   }

   /** Gives the test time to process (ie. run & test)
   * This method is called by the runner at then end
   * of preFrame until isDone is true.
   * @pre isDone() == false
   * @throws vrj::test::TestFailure - Thrown if the test fails
   */
   virtual void processTest()
   {
      //VRJTEST_ASSERT(false && "This is supposed to fail");
    
      std::string cur_stamp = mInputLogger->getStamp();

      if("selected" == cur_stamp)
      {
         std::cout << "SphereTestCase: stamp: selected:\n" << std::flush;

         bool is_selected = mNavGrabApp->mSphereIsect;
         VRJTEST_ASSERT(true == is_selected);
      }
      else if("moved" == cur_stamp)
      {
         std::cout << "SphereTestCase: stamp: moved:\n" << std::flush;
         
         gmtl::Point3f sphere_center = mNavGrabApp->mSphere.getCenter();
         gmtl::Point3f correct_center(7.07136f, 3.10896f, -2.16408f);
         gmtl::Vec3f diff = sphere_center-correct_center;
         std::cout << "SphereTestCase: moved: diff:" << diff << std::endl;

         bool is_equal = gmtl::isEqual(sphere_center, correct_center, 0.01f);
         if(!is_equal)
         {
            std::cout << "--- failed state ---\n";
            std::cout << "sphere center: " << sphere_center << std::endl;
            std::cout << "correct center: " << correct_center << std::endl;
            std::cout << "cur state: \n";
            mNavGrabApp->dumpState();
         }

         VRJTEST_ASSERT(is_equal && "Move didn't work");
      
         mCurState = Done;
      }
   }

   /** Return true when the test is done processing
   * testing 
   */
   virtual bool isDone()
   {
      return (Done == mCurState);     /* should just error out instead */
   }

protected:
   enum CurState
   {
      Done,
      Running
   };
protected:
   NavGrabApp*             mNavGrabApp;      /**< Pointer the real type of the app */
   CurState                mCurState;        /**< The current state of the test */
   gadget::InputLoggerPtr  mInputLogger;     /**< Ptr to the system input logger */

};


#endif

