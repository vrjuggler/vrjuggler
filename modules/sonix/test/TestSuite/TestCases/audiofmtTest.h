#include <iostream>
//#include <assert.h>
#include <syn/AudioFormat.h>

#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>


namespace synTest
{

   class audiofmtTest : public CppUnit::TestCase
   {
   public:
      audiofmtTest() 
      {
      }

      audiofmtTest( std::string name ) : CppUnit::TestCase( name )
      {
      }

      virtual ~audiofmtTest()
      {
      }

      void testme()
      {
         unsigned int bytes = 0;
         syn::AudioFormat fmt;
         fmt.datatype = syn::AudioFormat::FLOAT32;
         fmt.channels = 1;
         fmt.samplingRateHz = 44100;

         syn::bytes_calculator( fmt, 1, bytes );
         CPPUNIT_ASSERT( bytes == 4 );

         syn::bytes_calculator( fmt, 2, bytes );
         CPPUNIT_ASSERT( bytes == 8 );

         syn::bytes_calculator( fmt, 3, bytes );
         CPPUNIT_ASSERT( bytes == 12 );

         syn::bytes_calculator( fmt, 4, bytes );
         CPPUNIT_ASSERT( bytes == 16 );


         // channels shouldn't change it...
         fmt.channels = 2;
         syn::bytes_calculator( fmt, 1, bytes );
         CPPUNIT_ASSERT( bytes == 4 );

         syn::bytes_calculator( fmt, 2, bytes );
         CPPUNIT_ASSERT( bytes == 8 );

         syn::bytes_calculator( fmt, 3, bytes );
         CPPUNIT_ASSERT( bytes == 12 );

         syn::bytes_calculator( fmt, 4, bytes );
         CPPUNIT_ASSERT( bytes == 16 );
      }


      static CppUnit::Test* suite()
      {
         CppUnit::TestSuite* test_suite = new CppUnit::TestSuite ("audiofmt tests");
         test_suite->addTest( new CppUnit::TestCaller<audiofmtTest>("audiofmt_test", &audiofmtTest::testme));

         return test_suite;
      }

   protected:

   };

};
