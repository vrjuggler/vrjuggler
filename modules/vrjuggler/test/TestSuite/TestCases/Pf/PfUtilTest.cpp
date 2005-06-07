#include <iostream>

#include <gmtl/Math.h>
#include <gmtl/Vec.h>
#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/Xforms.h>

#include <vrj/Draw/Pf/PfUtil.h>

#include <PfUtilTest.h>


namespace vrjTest
{

void PfUtilTest::testMatrixConversion()
{
   gmtl::Matrix44f mat;
   gmtl::postMult( mat, gmtl::makeTrans<gmtl::Matrix44f>( gmtl::Vec3f(1, 2, 3 )));
   gmtl::postMult( mat, gmtl::makeRot<gmtl::Matrix44f>( 90, gmtl::Vec3f( 0, 1, 0 ) ));

   pfMatrix pf_mat = vrj::GetPfMatrix( mat );
   gmtl::Matrix44f vj_mat = vrj::GetVjMatrix( pf_mat );

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
   CPPUNIT_ASSERT( gmtl::isEqual( vj_mat, mat ) && "test pf --> vj matrix conversion failed" );
}

} // End of vrjTest namespace
