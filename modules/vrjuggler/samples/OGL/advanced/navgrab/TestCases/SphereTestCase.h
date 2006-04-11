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
         gmtl::Point3f correct_center(7.07136, 3.10896, -2.16408);
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

