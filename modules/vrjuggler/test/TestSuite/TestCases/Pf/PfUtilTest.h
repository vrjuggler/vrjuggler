
#ifndef PFUTILS_TESTS
#define PFUTILS_TESTS


#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <vrj/Math/Math.h>
#include <vrj/Math/Quat.h>
#include <vrj/Draw/Pf/PfUtil.h>

#include <JugglerTest.h>


namespace vrjTest
{

class PfUtilTests : public CppUnit::TestCase, public JugglerTest
{
public:
   PfUtilTests() : CppUnit::TestCase ()
   {
   }
   
   
   virtual ~PfUtilTests()
   {
   }
   void testMatrixConversion()
   {
         vjMatrix mat;
         mat.postTrans( mat, 1, 2, 3 );
         mat.postRot( mat, 90, vjVec3( 0, 1, 0 ) );

         pfMatrix pf_mat = vjGetPfMatrix( mat );
         vjMatrix vj_mat = vjGetVjMatrix( pf_mat );

         std::cout << pf_mat << "\n\n" << std::flush;
            std::cout << vj_mat << "\n\n" << std::flush;
            std::cout << mat << "\n===============\n" << std::flush;
         if (!vrj::isEqual( vj_mat, mat ))
         {
            std::cout << "BAD!!!\n" << std::flush;
         }  
         else
         {
            std::cout << "GOOD!!!\n" << std::flush;
         }                
         assert( vrj::isEqual( vj_mat, mat ) && "bug in juggler" );
   }     
   void registerTests( CppUnit::TestSuite* suite )
   {
      suite->addTest( new CppUnit::TestCaller<PfUtilTests>( "test pf --> vj matrix conversion", &PfUtilTests::testMatrixConversion));
   }
};

}

#endif

             
