#ifndef _SOCKETCONNECTOR_ACCEPTOR_TEST_H
#define _SOCKETCONNECTOR_ACCEPTOR_TEST_H

#include <stdio.h>
#include <string.h>
#include <iostream>

#include <TestCase.h>
#include <ThreadTestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

#include <vpr/IO/Socket/Socket.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/IO/Socket/InetAddr.h>
#include <vpr/IO/Socket/SocketAcceptor.h>
#include <vpr/IO/Socket/SocketConnector.h>

#include <vpr/Util/Interval.h>

#include <vpr/System.h>

#include <vpr/Thread/Thread.h>
#include <vpr/Thread/ThreadFunctor.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/CondVar.h>

#include <vector>


namespace vprTest
{

class SocketConnectorAcceptorTest : public ThreadTestCase
{
public:
   SocketConnectorAcceptorTest( std::string name )
   : ThreadTestCase (name)
   {;}

   virtual ~SocketConnectorAcceptorTest()
   {}

   virtual void setUp()
   {;}

   virtual void tearDown()
   {;}
   
   // Test the acceptor wrapper construction
   // Things to test
   //  -- Opening acceptor
   //    - With all param type
   //    - Constructor and non-constructor open
   //  -- Closing
   void testAcceptorConstruction()
   {
       const vpr::Uint16 server_port(12021);
       vpr::InetAddr local_addr;
       local_addr.setPort(server_port);
       vpr::SocketAcceptor* test_acceptor(NULL);

       // Test address in constructor
       test_acceptor = new vpr::SocketAcceptor(local_addr);
       assertTest(test_acceptor != NULL);
       assertTest(test_acceptor->getSocket().isOpen());
       test_acceptor->close();
       delete(test_acceptor);

       // Test address in constructor (with backlog)
       test_acceptor = new vpr::SocketAcceptor(local_addr,7);
       assertTest(test_acceptor != NULL);
       assertTest(test_acceptor->getSocket().isOpen());
       test_acceptor->close();
       delete(test_acceptor);

       // Test default constructor
       test_acceptor = new vpr::SocketAcceptor();
       assertTest(test_acceptor != NULL);
       test_acceptor->open(local_addr);
       assertTest(test_acceptor->getSocket().isOpen());
       test_acceptor->close();
       delete(test_acceptor);

       // Test default constructor (with backlog)
       test_acceptor = new vpr::SocketAcceptor();
       assertTest(test_acceptor != NULL);
       test_acceptor->open(local_addr,7);
       assertTest(test_acceptor->getSocket().isOpen());
       test_acceptor->close();
       delete(test_acceptor);
   }

   // Test Acceptor connector
   // This tests spawns off a thread that starts up an acceptor.
   // Then the main thread starts using a connector to connect to the connector
   // After connection the acceptor sends a message, the connector recieves it and closes the socket
   void testSpawnedAcceptor()
   {
       threadAssertReset();
       mRendevousPort = 47000 + (random() % 71);     // Get a partially random port
       mNumItersA = 5;
       mMessageValue = std::string("The Data");
       mMessageLen = mMessageValue.length();

       mState = NOT_READY;                        // Initialize

       // Spawn acceptor thread
       vpr::ThreadMemberFunctor<SocketConnectorAcceptorTest>
           acceptor_functor( this, &SocketConnectorAcceptorTest::testSpawnedAcceptor_acceptor );
       vpr::Thread acceptor_thread( &acceptor_functor);

       // Spawn connector thread
       vpr::ThreadMemberFunctor<SocketConnectorAcceptorTest>
           connector_functor( this, &SocketConnectorAcceptorTest::testSpawnedAcceptor_connector );
       vpr::Thread connector_thread( &connector_functor);

       assertTest( acceptor_thread.valid() && "Invalid acceptor thread");
       assertTest( connector_thread.valid() && "Invalid connector thread");

       if(!acceptor_thread.valid())
          std::cerr << "Invalid acceptor thread\n";
       if(!connector_thread.valid())
          std::cerr << "Invalid connector_thread\n";

       // Wait for threads
       acceptor_thread.join();
       connector_thread.join();

       checkThreadAssertions();
   }
   void testSpawnedAcceptor_acceptor(void* arg)
   {
       vpr::InetAddr local_acceptor_addr;
       local_acceptor_addr.setAddress("localhost", mRendevousPort);
       vpr::SocketAcceptor acceptor;
       vpr::SocketStream* sock(NULL);
       vpr::Status ret_val;
       vpr::Uint32 bytes_written;

       // Open the acceptor
       ret_val = acceptor.open(local_acceptor_addr);
       assertTestThread((ret_val.success()) && "Acceptor did not open correctly");

       for(int i=0;i<mNumItersA;i++)
       {
          // READY - Tell everyone that we are ready to accept
          mCondVar.acquire();
          {
              mState = READY;
              mCondVar.signal();       // Tell any waiting threads
          }
          mCondVar.release();

          // ACCEPT connection
          sock = new vpr::SocketStream;
          ret_val = acceptor.accept(*sock, vpr::Interval::NoTimeout );
           assertTestThread((ret_val.success()) && "Accepting socket failed");

           assertTestThread((sock->isOpen()) && "Accepted socket should be open");
           //assertTestThread((sock->isConnected()), "Accepted socket should be connected");

           ret_val = sock->write(mMessageValue, mMessageLen, bytes_written);      // Send a message
           assertTestThread((ret_val.success()) && "Problem writing in acceptor");
           assertTestThread((bytes_written == mMessageLen) && "Didn't send entire messag");

           // WAIT for close
           mCondVar.acquire();
           while(mState != CONNECTOR_CLOSED)
              mCondVar.wait();
           mCondVar.release();

           //ret_val = sock->isConnected();
           //assertTestThread((ret_val == false), "Socket should not still be connected");

           ret_val = sock->close();                                // Close the socket
           assertTestThread((ret_val.success()) && "Problem closing accepted socket");
       }
   }
   void testSpawnedAcceptor_connector(void* arg)
   {
      vpr::Status ret_val;
      vpr::Uint32 bytes_read;
      vpr::InetAddr remote_addr;
      remote_addr.setAddress("localhost", mRendevousPort);
      vpr::SocketConnector connector;           // Connect to acceptor

      for(int i=0;i<mNumItersA;i++)
      {
         // WAIT for READY
         mCondVar.acquire();
         {
            while(mState != READY)
               mCondVar.wait();
         }
         mCondVar.release();

         vpr::SocketStream    con_sock;
         std::string      data;
         ret_val = connector.connect(con_sock, remote_addr, vpr::Interval::NoTimeout );
         assertTestThread((ret_val.success()) && "Connector can't connect");

         ret_val = con_sock.read(data, mMessageLen, bytes_read);   // Recieve data
         assertTestThread((ret_val.success()) && "Read failed");
         assertTestThread((bytes_read == mMessageLen) && "Connector recieved message of wrong size" );

         //ret_val = con_sock.isConnected();
         //assertTestThread((ret_val == false), "Socket should still be connected");

         con_sock.close();                                   // Close socket

         // Tell everyone that we closed
         mCondVar.acquire();
         {
            mState = CONNECTOR_CLOSED;
            mCondVar.signal();       // Tell any waiting threads
         }
         mCondVar.release();
      }
   }

   static Test* suite()
   {
      TestSuite *test_suite = new TestSuite ("SocketConnectorAcceptorTest");

      test_suite->addTest( new TestCaller<SocketConnectorAcceptorTest>("testAcceptorConstruction", &SocketConnectorAcceptorTest::testAcceptorConstruction));
      test_suite->addTest( new TestCaller<SocketConnectorAcceptorTest>("testSpawnedAcceptor", &SocketConnectorAcceptorTest::testSpawnedAcceptor));
      return test_suite;
   }

protected:
    vpr::Uint16     mRendevousPort;     // The port the acceptor will be waiting on
    int             mNumItersA;        // Number of primary iterations
    int             mNumItersB;        // Number of secondary iterations

    vpr::Uint16     mMessageLen;        // Length of message to send
    std::string     mMessageValue;      // The value of the message that is supposed to be sent (and recieved)

    vpr::CondVar    mCondVar;       // Condition variable
    vpr::Mutex      mSyncMutex;     // Mutex used to do synchronization

    enum State { READY, NOT_READY, CONNECTOR_CLOSED
    };
    State           mState;         // State variable

    bool            mReadyFlag1;
    bool            mReadyFlag2;
};

}       // namespace

#endif
