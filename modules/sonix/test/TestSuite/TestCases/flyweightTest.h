#include <string>
//#include <assert.h>

#include <syn/AudioFormat.h>
#include <syn/FlyWeightPool.h>


#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>


namespace synTest
{

   class flyweightTest : public CppUnit::TestCase
   {
   public:
      flyweightTest() 
      {
      }

      flyweightTest( std::string name ) : CppUnit::TestCase( name )
      {
      }

      virtual ~flyweightTest()
      {
      }

      // flyweight pool test.

      class bok
      {
      public:
         bok()
         {
            name = "bok";
         }
         ~bok()
         {
            name = "deleted";
         }
         std::string name;
      };

      void testme()
      {
         syn::FlyWeightPool<bok> pool;
         CPPUNIT_ASSERT( pool.size() == 0 );
         CPPUNIT_ASSERT( pool.available() == 0 );
         CPPUNIT_ASSERT( pool.empty() == true );

         pool.resize( 5 );
         CPPUNIT_ASSERT( pool.size() == 5 );
         CPPUNIT_ASSERT( pool.available() == 5 );
         CPPUNIT_ASSERT( pool.empty() == false );

         bok *a1, *a2, *a3, *a4, *a5;

         pool.take( a1 );
         CPPUNIT_ASSERT( pool.size() == 5 );
         CPPUNIT_ASSERT( pool.available() == 4 );
         CPPUNIT_ASSERT( pool.empty() == false );
         CPPUNIT_ASSERT( a1->name == "bok" );

         pool.take( a2 );
         CPPUNIT_ASSERT( pool.size() == 5 );
         CPPUNIT_ASSERT( pool.available() == 3 );
         CPPUNIT_ASSERT( pool.empty() == false );
         CPPUNIT_ASSERT( a2->name == "bok" );

         pool.take( a3 );
         CPPUNIT_ASSERT( pool.size() == 5 );
         CPPUNIT_ASSERT( pool.available() == 2 );
         CPPUNIT_ASSERT( pool.empty() == false );
         CPPUNIT_ASSERT( a3->name == "bok" );

         pool.take( a4 );
         pool.take( a5 );
         CPPUNIT_ASSERT( pool.size() == 5 );
         CPPUNIT_ASSERT( pool.available() == 0 );
         CPPUNIT_ASSERT( pool.empty() == true );
         CPPUNIT_ASSERT( a4->name == "bok" );
         CPPUNIT_ASSERT( a5->name == "bok" );

         pool.putback( a4 );
         pool.putback( a5 );
         CPPUNIT_ASSERT( pool.size() == 5 );
         CPPUNIT_ASSERT( pool.available() == 2 );
         CPPUNIT_ASSERT( pool.empty() == false );

         pool.putback( a2 );
         pool.putback( a1 );
         pool.putback( a3 );
         CPPUNIT_ASSERT( pool.size() == 5 );
         CPPUNIT_ASSERT( pool.available() == 5 );
         CPPUNIT_ASSERT( pool.empty() == false );


         // test out an autogrowing fw pool.
         syn::FlyWeightPool<bok> pool2;
         pool2.setAutoGrow( true );

         CPPUNIT_ASSERT( pool2.available() == 0 );
         CPPUNIT_ASSERT( pool2.size() == 0 );
         bok* z[15];
         pool2.take( z[0] );         // 1
         CPPUNIT_ASSERT( pool2.available() == 1 );
         CPPUNIT_ASSERT( pool2.size() == 2 );
         CPPUNIT_ASSERT( pool2.empty() == false );

         pool2.take( z[1] );         // 2
         CPPUNIT_ASSERT( pool2.available() == 0 );
         CPPUNIT_ASSERT( pool2.size() == 2 );
         CPPUNIT_ASSERT( pool2.empty() == true );

         pool2.take( z[2] );         // 3
         CPPUNIT_ASSERT( pool2.available() == 3 );
         CPPUNIT_ASSERT( pool2.size() == 6 );
         CPPUNIT_ASSERT( pool2.empty() == false );

         pool2.take( z[3] );         // 4
         CPPUNIT_ASSERT( pool2.available() == 2 );
         CPPUNIT_ASSERT( pool2.size() == 6 );
         CPPUNIT_ASSERT( pool2.empty() == false );

         pool2.take( z[4] );         // 5
         CPPUNIT_ASSERT( pool2.available() == 1 );
         CPPUNIT_ASSERT( pool2.size() == 6 );
         CPPUNIT_ASSERT( pool2.empty() == false );

         pool2.take( z[5] );         // 6
         CPPUNIT_ASSERT( pool2.available() == 0 );
         CPPUNIT_ASSERT( pool2.size() == 6 );
         CPPUNIT_ASSERT( pool2.empty() == true );

         pool2.take( z[6] );         // 7
         CPPUNIT_ASSERT( pool2.available() == 7 );
         CPPUNIT_ASSERT( pool2.size() == 14 );
         CPPUNIT_ASSERT( pool2.empty() == false );

         pool2.take( z[7] );         // 8
         CPPUNIT_ASSERT( pool2.available() == 6 );
         CPPUNIT_ASSERT( pool2.size() == 14 );
         CPPUNIT_ASSERT( pool2.empty() == false );

         pool2.take( z[8] );         // 9
         CPPUNIT_ASSERT( pool2.available() == 5 );
         CPPUNIT_ASSERT( pool2.size() == 14 );
         CPPUNIT_ASSERT( pool2.empty() == false );

         pool2.take( z[9] );         // 10
         CPPUNIT_ASSERT( pool2.available() == 4 );
         CPPUNIT_ASSERT( pool2.size() == 14 );
         CPPUNIT_ASSERT( pool2.empty() == false );

         pool2.take( z[10] );         // 11
         CPPUNIT_ASSERT( pool2.available() == 3 );
         CPPUNIT_ASSERT( pool2.size() == 14 );
         CPPUNIT_ASSERT( pool2.empty() == false );

         pool2.take( z[11] );         // 12
         CPPUNIT_ASSERT( pool2.available() == 2 );
         CPPUNIT_ASSERT( pool2.size() == 14 );
         CPPUNIT_ASSERT( pool2.empty() == false );

         pool2.take( z[12] );         // 13
         CPPUNIT_ASSERT( pool2.available() == 1 );
         CPPUNIT_ASSERT( pool2.size() == 14 );
         CPPUNIT_ASSERT( pool2.empty() == false );

         pool2.take( z[13] );         // 14
         CPPUNIT_ASSERT( pool2.available() == 0 );
         CPPUNIT_ASSERT( pool2.size() == 14 );
         CPPUNIT_ASSERT( pool2.empty() == true );

         pool2.take( z[14] );         // 15
         CPPUNIT_ASSERT( pool2.available() == 15 );
         CPPUNIT_ASSERT( pool2.size() == 30 );
         CPPUNIT_ASSERT( pool2.empty() == false );


         for ( int xx = 0; xx < 15; ++xx)
         {
            pool2.putback( z[xx] );
            CPPUNIT_ASSERT( pool2.available() == (16 + xx) );

            // pool doesn't shrink unless requested with a resize()
            CPPUNIT_ASSERT( pool2.size() == 30 );
         }

         CPPUNIT_ASSERT( pool2.size() == 30 );
         CPPUNIT_ASSERT( pool2.available() == 30 );
         CPPUNIT_ASSERT( pool2.empty() == false );
      }

      static CppUnit::Test* suite()
      {
         CppUnit::TestSuite* test_suite = new CppUnit::TestSuite ("flyweight tests");
         test_suite->addTest( new CppUnit::TestCaller<flyweightTest>("flyweight_test", &flyweightTest::testme));

         return test_suite;
      }

   protected:

   };

};


