#include <gfxConfig.h>
#include <iostream>

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

#include <Performer/pf.h>

#include <SGA/PfTriangleListAdapter.h>

#include <gfxConfig.h>
#include <Util/Assert.h>


namespace sgaTest
{

class PfTriangleListAdapterTest : public TestCase
{
public:
   PfTriangleListAdapterTest( std::string name )
   : TestCase (name)
   //PfTriangleListAdapterTest()
   {;}

   virtual ~PfTriangleListAdapterTest()
   {}

   virtual void setUp()
   {
      if(!pfIsInited())
      {
         pfInit();
         pfInitState(NULL);
      }

      // Set it up as coords of a strip of tris
      //  1 3 5 7 9
      //
      //  0 2 4 6 8 10
      mCoords = new pfVec3[11];
      mCoords[0].set( 0.0f,  0.0f,  0.0f);
      mCoords[1].set( 0.0f,  1.0f,  0.0f);
      mCoords[2].set( 1.0f,  0.0f,  0.0f);
      mCoords[3].set( 1.0f,  1.0f,  0.0f);
      mCoords[4].set( 2.0f,  0.0f,  0.0f);
      mCoords[5].set( 2.0f,  1.0f,  0.0f);
      mCoords[6].set( 3.0f,  0.0f,  0.0f);
      mCoords[7].set( 3.0f,  1.0f,  0.0f);
      mCoords[8].set( 4.0f,  0.0f,  0.0f);
      mCoords[9].set( 4.0f,  1.0f,  0.0f);
      mCoords[10].set( 5.0f,  0.0f,  0.0f);

      mPoint3Coords = new gmtl::Point3[11];
      for(unsigned i=0;i<11;i++)
         mPoint3Coords[i] = sga::getPoint3(mCoords[i]);

/*
       pfGeoSet* geom = new pfGeoSet;
       ushort* cindex = new ushort[];
       int*    prim_len = new int[];

       cindex[0]  = 0; cindex[1]  = 1; cindex[2]  = 2; cindex[3]  = 3;

       geom->setAttr(PFGS_COORD3, PFGS_PER_VERTEX, scoords, vindex);
       geom->setPrimType(PFGS_QUADS);
       geom->setNumPrims(6);
*/
   }

   virtual void tearDown()
   {
   }

   void testNonIndexedTris()
   {
      pfGeoSet* geom = new pfGeoSet;
      geom->setAttr(PFGS_COORD3, PFGS_PER_VERTEX, mCoords, NULL);
      geom->setPrimType(PFGS_TRIS);
      geom->setNumPrims(3);

      sga::PfTriangleListAdapter tri_adapter(geom);

      gmtl::Tri tri0(mPoint3Coords[0], mPoint3Coords[1], mPoint3Coords[2]);
      gmtl::Tri tri1(mPoint3Coords[3], mPoint3Coords[4], mPoint3Coords[5]);
      gmtl::Tri tri2(mPoint3Coords[6], mPoint3Coords[7], mPoint3Coords[8]);


      assertTest(tri_adapter.size() == 3);
      assertTest(tri_adapter.tri(0) == tri0);
      assertTest(tri_adapter.tri(1) == tri1);
      assertTest(tri_adapter.tri(2) == tri2);
   }

   void testIndexedTris()
   {
      pfGeoSet* geom = new pfGeoSet;
      ushort* cindex = new ushort[6];

      cindex[0] = 0; cindex[1] = 1; cindex[2] = 2;
      cindex[3] = 1; cindex[4] = 2; cindex[5] = 3;

      geom->setAttr(PFGS_COORD3, PFGS_PER_VERTEX, mCoords, cindex);
      geom->setPrimType(PFGS_TRIS);
      geom->setNumPrims(2);

      sga::PfTriangleListAdapter tri_adapter(geom);

      gmtl::Tri tri0(mPoint3Coords[cindex[0]], mPoint3Coords[cindex[1]], mPoint3Coords[cindex[2]]);
      gmtl::Tri tri1(mPoint3Coords[cindex[3]], mPoint3Coords[cindex[4]], mPoint3Coords[cindex[5]]);

      assertTest(tri_adapter.size() == 2);
      assertTest(tri_adapter.tri(0) == tri0);
      assertTest(tri_adapter.tri(1) == tri1);
   }

   void testNonIndexedTriStrips()
   {
      // Strips
      // 1 - 0,1,2,3    --> 2 tris
      // 2 - 4,5,6      --> 1 tri
      // 3 - 7,8,9,10   --> 2 tris
      pfGeoSet* geom = new pfGeoSet;
      geom->setAttr(PFGS_COORD3, PFGS_PER_VERTEX, mCoords, NULL);
      int*    prim_len = new int[3];
      prim_len[0] = 4;
      prim_len[1] = 3;
      prim_len[2] = 4;
      geom->setPrimLengths(prim_len);
      geom->setPrimType(PFGS_TRISTRIPS );
      geom->setNumPrims(3);

      sga::PfTriangleListAdapter tri_adapter(geom);

      gmtl::Tri tri0(mPoint3Coords[0], mPoint3Coords[1], mPoint3Coords[2]);
      gmtl::Tri tri1(mPoint3Coords[1], mPoint3Coords[2], mPoint3Coords[3]);

      gmtl::Tri tri2(mPoint3Coords[4], mPoint3Coords[5], mPoint3Coords[6]);

      gmtl::Tri tri3(mPoint3Coords[7], mPoint3Coords[8], mPoint3Coords[9]);
      gmtl::Tri tri4(mPoint3Coords[8], mPoint3Coords[9], mPoint3Coords[10]);

      assertTest(tri_adapter.size() == 5);
      assertTest(tri_adapter.tri(0) == tri0);
      assertTest(tri_adapter.tri(1) == tri1);
      assertTest(tri_adapter.tri(2) == tri2);
      assertTest(tri_adapter.tri(3) == tri3);
      assertTest(tri_adapter.tri(4) == tri4);
   }

   void testIndexedTriStrips()
   {
      // Strips
      // 1 - 0,3,4,5,8  --> 3 tris
      // 2 - 2,7,10      --> 1 tri
      // 3 - 3,4,5,6,7   --> 3 tris
      pfGeoSet* geom = new pfGeoSet;

      ushort* cindex = new ushort[13];
      cindex[0] = 0; cindex[1] = 3; cindex[2] = 4; cindex[3] = 5; cindex[4] = 8;
      cindex[5] = 2; cindex[6] = 7; cindex[7] = 10;
      cindex[8] = 3; cindex[9] = 4; cindex[10] = 5; cindex[11] = 6; cindex[12] = 7;

      geom->setAttr(PFGS_COORD3, PFGS_PER_VERTEX, mCoords, cindex);
      int*    prim_len = new int[3];
      prim_len[0] = 5;
      prim_len[1] = 3;
      prim_len[2] = 5;
      geom->setPrimLengths(prim_len);
      geom->setPrimType(PFGS_TRISTRIPS );
      geom->setNumPrims(3);

      sga::PfTriangleListAdapter tri_adapter(geom);

      gmtl::Tri tri0(mPoint3Coords[cindex[0]], mPoint3Coords[cindex[1]], mPoint3Coords[cindex[2]]);
      gmtl::Tri tri1(mPoint3Coords[cindex[1]], mPoint3Coords[cindex[2]], mPoint3Coords[cindex[3]]);
      gmtl::Tri tri2(mPoint3Coords[cindex[2]], mPoint3Coords[cindex[3]], mPoint3Coords[cindex[4]]);

      gmtl::Tri tri3(mPoint3Coords[cindex[5]], mPoint3Coords[cindex[6]], mPoint3Coords[cindex[7]]);

      gmtl::Tri tri4(mPoint3Coords[cindex[8]], mPoint3Coords[cindex[9]], mPoint3Coords[cindex[10]]);
      gmtl::Tri tri5(mPoint3Coords[cindex[9]], mPoint3Coords[cindex[10]], mPoint3Coords[cindex[11]]);
      gmtl::Tri tri6(mPoint3Coords[cindex[10]], mPoint3Coords[cindex[11]], mPoint3Coords[cindex[12]]);

      assertTest(tri_adapter.size() == 7);
      assertTest(tri_adapter.tri(0) == tri0);
      assertTest(tri_adapter.tri(1) == tri1);
      assertTest(tri_adapter.tri(2) == tri2);
      assertTest(tri_adapter.tri(3) == tri3);
      assertTest(tri_adapter.tri(4) == tri4);
      assertTest(tri_adapter.tri(5) == tri5);
      assertTest(tri_adapter.tri(6) == tri6);
   }

   static Test* suite()
   {
      TestSuite* test_suite = new TestSuite ("PfTriangleListAdapterTest");
      test_suite->addTest( new TestCaller<PfTriangleListAdapterTest>("testNonIndexedTris", &PfTriangleListAdapterTest::testNonIndexedTris));
      test_suite->addTest( new TestCaller<PfTriangleListAdapterTest>("testIndexedTris", &PfTriangleListAdapterTest::testIndexedTris));
      test_suite->addTest( new TestCaller<PfTriangleListAdapterTest>("testNonIndexedTriStrips", &PfTriangleListAdapterTest::testNonIndexedTriStrips));
      test_suite->addTest( new TestCaller<PfTriangleListAdapterTest>("testIndexedTriStrips", &PfTriangleListAdapterTest::testIndexedTriStrips));

      return test_suite;
   }

   static Test* interactiveSuite()
   {
      TestSuite* test_suite = new TestSuite ("InteractivePfTriangleListAdapterTest");
      //test_suite->addTest( new TestCaller<ThreadTest>("interactiveCPUGrind", &ThreadTest::interactiveTestCPUGrind));
      return test_suite;
   }

   // Static function that allows me to test it in a Pf App directly
   static void testIt()
   {
      PfTriangleListAdapterTest me("PfTriangleListAdapterTest");
      std::cout << "Testing: PfTriangleListAdapterTest: " << std::flush;

      me.setUp();
      me.testNonIndexedTris();      std::cout << "." << std::flush;

      std::cout << "  done." << std::flush;
   }

protected:

   pfVec3*        mCoords;       // Vertex coordinates
   gmtl::Point3*  mPoint3Coords;


};

};
