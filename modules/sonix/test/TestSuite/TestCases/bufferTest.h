
#include <syn/AudioFormat.h>
#include <syn/SignalBuffer.h>


#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>


namespace synTest
{

   class bufferTest : public CppUnit::TestCase
   {
   public:
      bufferTest() 
      {
      }

      bufferTest( std::string name ) : CppUnit::TestCase( name )
      {
      }

      virtual ~bufferTest()
      {
      }

      // test the signal buffer class....
      void testme()
      {
         syn::SignalBuffer buf;

         syn::AudioFormat fmt;
         fmt.channels = 1;
         fmt.samplingRateHz = 44100;
         fmt.datatype = syn::AudioFormat::FLOAT32;

         buf.setFormat( fmt );

         // empty container with no capacity
         CPPUNIT_ASSERT( buf.full() == true );
         CPPUNIT_ASSERT( buf.empty() == true );
         CPPUNIT_ASSERT( buf.free() == 0 );
         CPPUNIT_ASSERT( buf.size() == 0 );
         CPPUNIT_ASSERT( buf.capacity() == 0 );
         CPPUNIT_ASSERT( buf.readPosition == 0 );
         CPPUNIT_ASSERT( buf.writePosition == 0 );

         // empty container with 20000 capacity
         buf.recapacity( 20000 );
         CPPUNIT_ASSERT( buf.full() == false );
         CPPUNIT_ASSERT( buf.empty() == true );
         CPPUNIT_ASSERT( buf.free() == 20000 );
         CPPUNIT_ASSERT( buf.size() == 0 );
         CPPUNIT_ASSERT( buf.capacity() == 20000 );
         CPPUNIT_ASSERT( buf.readPosition == 0 );
         CPPUNIT_ASSERT( buf.writePosition == 0 );

         // read/write tests...

         // init some data
         float f[100], fr[100]; int x;
         for (x = 0; x < 100; ++x) f[x] = 0.69f;
         for (x = 0; x < 100; ++x) fr[x] = 0.34f;

         // verify the buffer has some expected stuff.
         CPPUNIT_ASSERT( ((float*)buf.datar())[0] != 0.69f ); 
         CPPUNIT_ASSERT( ((float*)buf.datar())[99] != 0.69f ); 
         CPPUNIT_ASSERT( ((float*)buf.datar())[100] != 0.69f ); 
         float this_shouldnt_change = ((float*)buf.datar())[100];

         // fill the buffer, verify that new stuff was written in the right places...
         buf.fill( f, 100 );
         CPPUNIT_ASSERT( buf.full() == false );
         CPPUNIT_ASSERT( buf.empty() == false );
         CPPUNIT_ASSERT( buf.free() == 20000 - 100 );
         CPPUNIT_ASSERT( buf.size() == 100 );
         CPPUNIT_ASSERT( buf.capacity() == 20000 );
         CPPUNIT_ASSERT( buf.readPosition == 0 );
         CPPUNIT_ASSERT( buf.writePosition == 100 );

         CPPUNIT_ASSERT( ((float*)buf.datar())[0] == 0.69f ); 
         CPPUNIT_ASSERT( ((float*)buf.datar())[99] == 0.69f ); 
         CPPUNIT_ASSERT( ((float*)buf.datar())[100] != 0.69f ); 
         CPPUNIT_ASSERT( ((float*)buf.datar())[100] == this_shouldnt_change ); 

         // fill in some more... make sure buffer stays sane...
         buf.fill( fr, 20 );
         CPPUNIT_ASSERT( buf.full() == false );
         CPPUNIT_ASSERT( buf.empty() == false );
         CPPUNIT_ASSERT( buf.free() == 20000 - 120 );
         CPPUNIT_ASSERT( buf.size() == 120 );
         CPPUNIT_ASSERT( buf.capacity() == 20000 );
         CPPUNIT_ASSERT( buf.readPosition == 0 );
         CPPUNIT_ASSERT( buf.writePosition == 120 );

         CPPUNIT_ASSERT( ((float*)buf.datar())[0] == 0.69f ); 
         CPPUNIT_ASSERT( ((float*)buf.datar())[99] == 0.69f ); 
         CPPUNIT_ASSERT( ((float*)buf.datar())[100] == 0.34f ); 
         CPPUNIT_ASSERT( ((float*)buf.datar())[119] == 0.34f ); 
         CPPUNIT_ASSERT( ((float*)buf.datar())[120] != 0.34f && ((float*)buf.datar())[120] != 0.69f );

         // read
         float r1[101], r2[101], r3[101];
         for (x = 0; x < 101; ++x) r1[x] = 0.44f;
         for (x = 0; x < 101; ++x) r2[x] = 0.44f;
         for (x = 0; x < 101; ++x) r3[x] = 0.44f;
         buf.drain( r1, 9 );
         CPPUNIT_ASSERT( buf.full() == false );
         CPPUNIT_ASSERT( buf.empty() == false );
         CPPUNIT_ASSERT( buf.free() == 20000 - 120 ); // not circular, so free stays the same
         CPPUNIT_ASSERT( buf.size() == 111 );
         CPPUNIT_ASSERT( buf.capacity() == 20000 );
         CPPUNIT_ASSERT( buf.readPosition == 9 );
         CPPUNIT_ASSERT( buf.writePosition == 120 );


         buf.drain( r2, 100 );
         CPPUNIT_ASSERT( buf.full() == false );
         CPPUNIT_ASSERT( buf.empty() == false );
         CPPUNIT_ASSERT( buf.free() == 20000 - 120 );
         CPPUNIT_ASSERT( buf.size() == 11 );
         CPPUNIT_ASSERT( buf.capacity() == 20000 );
         CPPUNIT_ASSERT( buf.readPosition == 109 );
         CPPUNIT_ASSERT( buf.writePosition == 120 );


         buf.drain( r3, 11 );
         CPPUNIT_ASSERT( buf.full() == false );
         CPPUNIT_ASSERT( buf.empty() == true );
         CPPUNIT_ASSERT( buf.free() == 20000 - 120 );
         CPPUNIT_ASSERT( buf.size() == 0 );
         CPPUNIT_ASSERT( buf.capacity() == 20000 );
         CPPUNIT_ASSERT( buf.readPosition == 120 );
         CPPUNIT_ASSERT( buf.writePosition == 120 );


         // check the extents for error...
         CPPUNIT_ASSERT( r1[0] == 0.69f );
         CPPUNIT_ASSERT( r1[8] == 0.69f );
         CPPUNIT_ASSERT( r1[9] == 0.44f ); // off end
         CPPUNIT_ASSERT( r2[0] == 0.69f );
         CPPUNIT_ASSERT( r2[90] == 0.69f );
         CPPUNIT_ASSERT( r2[91] == 0.34f );
         CPPUNIT_ASSERT( r2[99] == 0.34f );
         CPPUNIT_ASSERT( r1[100] == 0.44f ); // off end
         CPPUNIT_ASSERT( r3[0] == 0.34f );
         CPPUNIT_ASSERT( r3[10] == 0.34f );
         CPPUNIT_ASSERT( r3[11] == 0.44f ); // off end

         // make sure it can be full
         float* temp = new float[buf.free()];
         buf.fill( temp, buf.free() );
         CPPUNIT_ASSERT( buf.full() == true );
         CPPUNIT_ASSERT( buf.empty() == false );
         CPPUNIT_ASSERT( buf.free() == 0 );
         CPPUNIT_ASSERT( buf.size() == 20000 - 120 );
         CPPUNIT_ASSERT( buf.capacity() == 20000 );
         CPPUNIT_ASSERT( buf.readPosition == 120 );
         CPPUNIT_ASSERT( buf.writePosition == 20000 );


         // make sure reset does something
         buf.reset();
         CPPUNIT_ASSERT( buf.full() == false );
         CPPUNIT_ASSERT( buf.empty() == true );
         CPPUNIT_ASSERT( buf.free() == 20000 );
         CPPUNIT_ASSERT( buf.size() == 0 );
         CPPUNIT_ASSERT( buf.capacity() == 20000 );
         CPPUNIT_ASSERT( buf.readPosition == 0 );
         CPPUNIT_ASSERT( buf.writePosition == 0 );


         // test out the direct write (TODO: make this better)...
         int samples = 100;
         int bytes = buf.format().sampsize() * samples;
         memset( buf.dataw(), 0, bytes );
         buf.updateWritePointer( samples );
         CPPUNIT_ASSERT( ((float*)buf.datar())[99] == 0.0f );
         CPPUNIT_ASSERT( ((float*)buf.datar())[100] == 0.34f ); // off end
         CPPUNIT_ASSERT( buf.full() == false );
         CPPUNIT_ASSERT( buf.empty() == false );
         CPPUNIT_ASSERT( buf.free() == 20000 - 100 );
         CPPUNIT_ASSERT( buf.size() == 100 );
         CPPUNIT_ASSERT( buf.capacity() == 20000 );
         CPPUNIT_ASSERT( buf.readPosition == 0 );
         CPPUNIT_ASSERT( buf.writePosition == 100 );


         // test the direct read (TODO: make this better)...
         float bok[100];
         samples = 100;
         bytes = buf.format().sampsize() * samples;
         memcpy( bok, buf.datar(), bytes );
         buf.updateReadPointer( samples );
         CPPUNIT_ASSERT( bok[0] == 0.0f );
         CPPUNIT_ASSERT( bok[99] == 0.0f );
         CPPUNIT_ASSERT( buf.full() == false );
         CPPUNIT_ASSERT( buf.empty() == true );
         CPPUNIT_ASSERT( buf.free() == 20000 - 100 );
         CPPUNIT_ASSERT( buf.size() == 0 );
         CPPUNIT_ASSERT( buf.capacity() == 20000 );
         CPPUNIT_ASSERT( buf.readPosition == 100 );
         CPPUNIT_ASSERT( buf.writePosition == 100 );
      }


      static CppUnit::Test* suite()
      {
         CppUnit::TestSuite* test_suite = new CppUnit::TestSuite ("buffer tests");
         test_suite->addTest( new CppUnit::TestCaller<bufferTest>("buffer_test", &bufferTest::testme));

         return test_suite;
      }

   protected:

   };

};

