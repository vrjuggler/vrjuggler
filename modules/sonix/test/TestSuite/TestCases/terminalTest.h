#include <iostream>
//#include <assert.h>
#include <syn/AudioFormat.h>
#include <syn/Terminal.h>
#include <syn/InputTerminal.h>
#include <syn/OutputTerminal.h>



#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>


namespace synTest
{

   class terminalTest : public CppUnit::TestCase
   {
   public:
      terminalTest() 
      {
      }

      terminalTest( std::string name ) : CppUnit::TestCase( name )
      {
      }

      virtual ~terminalTest()
      {
      }

      void testme()
      {
         {
         // make sure it inits ok.
         syn::InputTerminalPtr it = syn::InputTerminalPtr( new syn::InputTerminal ("dummy"));
         syn::SignalBufferQueuePtr q, q2;
         syn::InputTerminalPtr in;
         syn::OutputTerminalPtr out;
         CPPUNIT_ASSERT( it->isConnected() == false );
         CPPUNIT_ASSERT( it->getConnection( out ) == false );
         CPPUNIT_ASSERT( it->isConnectionValid() == false );
         CPPUNIT_ASSERT( it->getQueue( q ) == false );
         CPPUNIT_ASSERT( q.get() == NULL );
         }

         {
         // good connection
         syn::InputTerminalPtr it = syn::InputTerminalPtr( new syn::InputTerminal ("dummy"));
         syn::OutputTerminalPtr ot = syn::OutputTerminalPtr( new syn::OutputTerminal ("dummy"));
         syn::SignalBufferQueuePtr q, q2;
         syn::InputTerminalPtr in;
         syn::OutputTerminalPtr out;
         it->connect( ot );
         ot->connect( it );
         CPPUNIT_ASSERT( it->isConnected() == true );
         CPPUNIT_ASSERT( it->getConnection( out ) == true );
         CPPUNIT_ASSERT( out.get() == ot.get() );
         CPPUNIT_ASSERT( it->isConnectionValid() == true );
         CPPUNIT_ASSERT( it->getQueue( q ) == true );
         CPPUNIT_ASSERT( q.get() != NULL );
         CPPUNIT_ASSERT( ot->isConnected() == true );
         CPPUNIT_ASSERT( ot->getConnection( in ) == true );
         CPPUNIT_ASSERT( in.get() == it.get() );
         CPPUNIT_ASSERT( ot->isConnectionValid() == true );
         CPPUNIT_ASSERT( (q2 = ot->queue()).get() != NULL );
         CPPUNIT_ASSERT( q.get() == q2.get() );
         }

         {
         // bad connection
         syn::InputTerminalPtr it = syn::InputTerminalPtr( new syn::InputTerminal ("dummy"));
         syn::OutputTerminalPtr ot = syn::OutputTerminalPtr( new syn::OutputTerminal ("dummy"));
         syn::SignalBufferQueuePtr q, q2;
         syn::InputTerminalPtr in;
         syn::OutputTerminalPtr out;
         it->connect( ot );
         //ot->connect( it );
         CPPUNIT_ASSERT( it->isConnected() == true );
         CPPUNIT_ASSERT( it->getConnection( out ) == true );
         CPPUNIT_ASSERT( out.get() == ot.get() );
         CPPUNIT_ASSERT( it->isConnectionValid() == false );
         CPPUNIT_ASSERT( it->getQueue( q ) == true );
         CPPUNIT_ASSERT( q.get() != NULL );
         CPPUNIT_ASSERT( ot->isConnected() == false );
         CPPUNIT_ASSERT( ot->getConnection( in ) == false );
         CPPUNIT_ASSERT( in.get() != it.get() );
         CPPUNIT_ASSERT( ot->isConnectionValid() == false );
         CPPUNIT_ASSERT( (q2 = ot->queue()).get() != NULL );// never NULL
         CPPUNIT_ASSERT( q.get() == q2.get() );
         }   

         {
         // bad connection (opposite)
         syn::InputTerminalPtr it = syn::InputTerminalPtr( new syn::InputTerminal ("dummy"));
         syn::OutputTerminalPtr ot = syn::OutputTerminalPtr( new syn::OutputTerminal ("dummy"));
         syn::SignalBufferQueuePtr q, q2;
         syn::InputTerminalPtr in;
         syn::OutputTerminalPtr out;
         //it->connect( ot );
         ot->connect( it );
         CPPUNIT_ASSERT( it->isConnected() == false );
         CPPUNIT_ASSERT( it->getConnection( out ) == false );
         CPPUNIT_ASSERT( out.get() != ot.get() );
         CPPUNIT_ASSERT( it->isConnectionValid() == false );
         CPPUNIT_ASSERT( it->getQueue( q ) == false );
         CPPUNIT_ASSERT( q.get() == NULL );
         CPPUNIT_ASSERT( ot->isConnected() == true );
         CPPUNIT_ASSERT( ot->getConnection( in ) == true );
         CPPUNIT_ASSERT( in.get() == it.get() );
         CPPUNIT_ASSERT( ot->isConnectionValid() == false );
         CPPUNIT_ASSERT( (q2 = ot->queue()).get() != NULL ); // never NULL
         CPPUNIT_ASSERT( q.get() != q2.get() );
         }   
      }



      static CppUnit::Test* suite()
      {
         CppUnit::TestSuite* test_suite = new CppUnit::TestSuite ("terminal tests");
         test_suite->addTest( new CppUnit::TestCaller<terminalTest>("terminal_test", &terminalTest::testme));

         return test_suite;
      }

   protected:

   };

};
