
#ifndef VRJ_PFUTILS_TESTS
#define VRJ_PFUTILS_TESTS


#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <vrj/Math/Math.h>
#include <vrj/Math/Quat.h>
#include <vrj/Draw/Pf/PfUtil.h>


namespace vrjTest
{

class PfUtilTest : public CppUnit::TestCase
{
public:
   PfUtilTest (std::string name) : CppUnit::TestCase (name)
   {;}
   
   PfUtilTest() : CppUnit::TestCase ()
   {;}
   
   virtual ~PfUtilTest()
   {;}

   void testMatrixConversion()
   {
         vrj::Matrix mat;
         mat.postTrans( mat, 1, 2, 3 );
         mat.postRot( mat, 90, vrj::Vec3( 0, 1, 0 ) );

         pfMatrix pf_mat = vrj::GetPfMatrix( mat );
         vrj::Matrix vj_mat = vrj::GetVjMatrix( pf_mat );

         /*
         std::cout << pf_mat << "\n\n" << std::flush;
         std::cout << vj_mat << "\n\n" << std::flush;
         std::cout << mat << "\n===============\n" << std::flush;
         if (!vj_mat.isEqual( mat ))
         {
            std::cout << "BAD!!!\n" << std::flush;
         }  
         else
         {
            std::cout << "GOOD!!!\n" << std::flush;
         }                
         */
         assertTest( vj_mat.isEqual( mat ) && "test pf --> vj matrix conversion failed" );
   }     

   static CppUnit::Test* suite()
   {
      CppUnit::TestSuite* suite = new CppUnit::TestSuite ("PfUtilTest");
      suite->addTest( new CppUnit::TestCaller<PfUtilTest>( "test pf --> vj matrix conversion", &PfUtilTest::testMatrixConversion));

      return suite;
   }
};

}

#endif
