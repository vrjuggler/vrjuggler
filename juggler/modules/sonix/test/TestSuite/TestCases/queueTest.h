
#include <syn/AudioFormat.h>
#include <syn/SignalBufferQueue.h>

#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>


namespace synTest
{

   class queueTest : public CppUnit::TestCase
   {
   public:
      queueTest() 
      {
      }

      queueTest( std::string name ) : CppUnit::TestCase( name )
      {
      }

      virtual ~queueTest()
      {
      }


      syn::SignalBuffer k;

      // test the signal buffer queue class....

      void testme()
      {
         syn::SignalBufferQueue q;

         syn::AudioFormat fmt;
         fmt.channels = 1;
         fmt.samplingRateHz = 44100;
         fmt.datatype = syn::AudioFormat::FLOAT32;

         q.setFormat( fmt );

         // empty container with no capacity
         CPPUNIT_ASSERT( q.size() == 0 );
         CPPUNIT_ASSERT( q.capacity() == 2 );
         CPPUNIT_ASSERT( q.bufferCapacity() == 1000 );
         CPPUNIT_ASSERT( q.highMark() == 2 );
         CPPUNIT_ASSERT( q.lowMark() == 1 );
         CPPUNIT_ASSERT( q.high() == false );
         CPPUNIT_ASSERT( q.low() == true );
         CPPUNIT_ASSERT( q.empty() == true );

         // make sure that when there is nothing in the queue, pop returns false
         CPPUNIT_ASSERT( q.pop() == false );

         // obtain a write buffer
         syn::SignalBuffer& buf = q.write();

         // check writebuffer's params for expected values
         CPPUNIT_ASSERT( buf.full() == false );
         CPPUNIT_ASSERT( buf.empty() == true );
         CPPUNIT_ASSERT( buf.free() == 1000 );
         CPPUNIT_ASSERT( buf.size() == 0 );
         CPPUNIT_ASSERT( buf.capacity() == 1000 );
         CPPUNIT_ASSERT( buf.readPosition == 0 );
         CPPUNIT_ASSERT( buf.writePosition == 0 );


         // make sure that a second call to write gives the same buffer back
         syn::SignalBuffer& buf2 = q.write();
         CPPUNIT_ASSERT( &buf == &buf2 );

         // init some data
         float f[100], fr[100]; int x;
         for (x = 0; x < 100; ++x) f[x] = 0.69f;
         for (x = 0; x < 100; ++x) fr[x] = 0.34f;
         buf.fill( f, 100 );
         buf.fill( fr, 100 );
         CPPUNIT_ASSERT( buf.full() == false );
         CPPUNIT_ASSERT( buf.empty() == false );
         CPPUNIT_ASSERT( buf.free() == 1000 - 200 );
         CPPUNIT_ASSERT( buf.size() == 200 );
         CPPUNIT_ASSERT( buf.capacity() == 1000 );
         CPPUNIT_ASSERT( buf.readPosition == 0 );
         CPPUNIT_ASSERT( buf.writePosition == 200 );

         CPPUNIT_ASSERT( ((float*)buf.datar())[0] == 0.69f ); 
         CPPUNIT_ASSERT( ((float*)buf.datar())[99] == 0.69f ); 
         CPPUNIT_ASSERT( ((float*)buf.datar())[100] != 0.69f ); 

         // make sure that a third call to write gives the same buffer back
         syn::SignalBuffer& buf3 = q.write();
         CPPUNIT_ASSERT( &buf == &buf2 && &buf == &buf3 );

         // push the write buffer into the queue..
         bool result = q.push();
         CPPUNIT_ASSERT( result == true );

         // queue should have one thing in it now.
         CPPUNIT_ASSERT( q.size() == 1 );
         CPPUNIT_ASSERT( q.capacity() == 2 );
         CPPUNIT_ASSERT( q.bufferCapacity() == 1000 );
         CPPUNIT_ASSERT( q.highMark() == 2 );
         CPPUNIT_ASSERT( q.lowMark() == 1 );
         CPPUNIT_ASSERT( q.high() == false );
         CPPUNIT_ASSERT( q.low() == true );
         CPPUNIT_ASSERT( q.empty() == false );

         // make sure we cannot safepop()
         CPPUNIT_ASSERT( q.safepop() == false );

         // push another write buffer.
         syn::SignalBuffer& buf4 = q.write();

         // make sure this buf is a new one (different than any we've seen so far)
         CPPUNIT_ASSERT( &buf4 != &buf && &buf4 != &buf2 && &buf4 != &buf3 );

         buf4.fill( f, 100 );
         buf4.fill( fr, 100 );
         CPPUNIT_ASSERT( buf4.full() == false );
         CPPUNIT_ASSERT( buf4.empty() == false );
         CPPUNIT_ASSERT( buf4.free() == 1000 - 200 );
         CPPUNIT_ASSERT( buf4.size() == 200 );
         CPPUNIT_ASSERT( buf4.capacity() == 1000 );
         CPPUNIT_ASSERT( buf4.readPosition == 0 );
         CPPUNIT_ASSERT( buf4.writePosition == 200 );
         CPPUNIT_ASSERT( ((float*)buf4.datar())[0] == 0.69f ); 
         CPPUNIT_ASSERT( ((float*)buf4.datar())[99] == 0.69f ); 
         CPPUNIT_ASSERT( ((float*)buf4.datar())[100] != 0.69f );


         // push the second write buffer into the queue..
         result = q.push();
         CPPUNIT_ASSERT( result == true );

         // queue should have 2 things in it now.
         CPPUNIT_ASSERT( q.size() == 2 );
         CPPUNIT_ASSERT( q.capacity() == 2 );
         CPPUNIT_ASSERT( q.bufferCapacity() == 1000 );
         CPPUNIT_ASSERT( q.highMark() == 2 );
         CPPUNIT_ASSERT( q.lowMark() == 1 );
         CPPUNIT_ASSERT( q.high() == true );
         CPPUNIT_ASSERT( q.low() == false );
         CPPUNIT_ASSERT( q.empty() == false );

         // make sure a second push (i.e. no write buffer available) yields false
         result = q.push();
         CPPUNIT_ASSERT( result == false );
      }



      static CppUnit::Test* suite()
      {
         CppUnit::TestSuite* test_suite = new CppUnit::TestSuite ("queue tests");
         test_suite->addTest( new CppUnit::TestCaller<queueTest>("queue_test", &queueTest::testme));

         return test_suite;
      }

   protected:

   };

};
