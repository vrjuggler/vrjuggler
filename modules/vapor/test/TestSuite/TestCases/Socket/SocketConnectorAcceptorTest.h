#ifndef _SOCKETCONNECTOR_ACCEPTOR_TEST_H
#define _SOCKETCONNECTOR_ACCEPTOR_TEST_H

#include <stdio.h>
#include <string.h>
#include <iostream>

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

#include <IO/Socket/Socket.h>
#include <IO/Socket/SocketStream.h>
#include <IO/Socket/InetAddr.h>
#include <IO/Socket/SocketAcceptor.h>
#include <IO/Socket/SocketConnector.h>

#include <include/System.h>

#include <Threads/Thread.h>
#include <Threads/ThreadFunctor.h>
#include <Sync/Mutex.h>

#include <vector>


namespace vprTest
{

class SocketConnectorAcceptorTest : public TestCase
{
public:
   SocketConnectorAcceptorTest( std::string name )
   : mThreadAssertTest(false), TestCase (name)
   {;}

   virtual ~SocketConnectorAcceptorTest()
   {}

   virtual void setUp()
   {;}

   virtual void tearDown()
   {
   }
   
   // use this within your threads (CppUnit doesn't catch the assertTest there)
   // then test mThreadAssertTest with assertTest in your parent func.
   // then reset it to true.
   void threadAssertTest( bool testcase, std::string text = std::string("none") )
   {
       if(!testcase)
       {
           mThreadAssertTest = testcase; 
           std::cerr << "threadAssertTest: " << text << std::endl;
       }
       else
           std::cout << "." << std::flush;
   }
   void testAssertReset()
   { mThreadAssertTest = true; }


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
       testAssertReset();
       mRendevousPort = 47021;
       mNumItersA = 5;
       mMessageValue = std::string("The Data");
       mMessageLen = mMessageValue.length();
       //mSyncMutex.acquire();                    // Get it here so the acceptor can release it when ready
       mStartFlag = false;

       // Spawn acceptor thread
       vpr::ThreadMemberFunctor<SocketConnectorAcceptorTest> 
           acceptor_functor( this, &SocketConnectorAcceptorTest::testSpawnedAcceptor_acceptor );
       vpr::Thread acceptor_thread( &acceptor_functor);

       // Spawn connector thread
       vpr::ThreadMemberFunctor<SocketConnectorAcceptorTest> 
           connector_functor( this, &SocketConnectorAcceptorTest::testSpawnedAcceptor_connector );
       vpr::Thread connector_thread( &connector_functor);

       // Wait for threads
       acceptor_thread.join();
       connector_thread.join();

       assertTest( mThreadAssertTest );
   }
   void testSpawnedAcceptor_acceptor(void* arg)
   {
       vpr::InetAddr local_acceptor_addr;
       local_acceptor_addr.setAddress("localhost", mRendevousPort);
       vpr::SocketAcceptor acceptor;
       vpr::SocketStream* sock(NULL);
       bool ret_val(true);

       // Open the acceptor
       ret_val = acceptor.open(local_acceptor_addr);
       threadAssertTest((ret_val == true), "Acceptor did not open correctly");
       
       //mSyncMutex.release();                        // We are ready to recieve, so tell connector to go
       mStartFlag = true;

       for(int i=0;i<mNumItersA;i++)
       {                  
           sock = acceptor.accept();                // Accept a connection
           threadAssertTest((sock != NULL), "Accepted socket is null");
           ret_val = sock->write(mMessageValue, mMessageLen);      // Send a message           
           threadAssertTest((ret_val == true), "Problem writing in acceptor");
           
           //mSyncMutex.acquire();            // Wait for connector to close their side of the socket

           //ret_val = sock->isConnected();
           //threadAssertTest((ret_val == false), "Socket should not still be connected");
           
           ret_val = sock->close();                                // Close the socket
           threadAssertTest((ret_val == true), "Problem closing accepted socket");           
       }       
   }
   void testSpawnedAcceptor_connector(void* arg)
   {
       bool ret_val(true);
       vpr::InetAddr remote_addr;
       remote_addr.setAddress("localhost", mRendevousPort);
       vpr::SocketConnector connector;           // Connect to acceptor

       //mSyncMutex.acquire();                // Wait for acceptor to start up
       //mStartFlag = false;
       while(!mStartFlag)
       {;}

       for(int i=0;i<mNumItersA;i++)
       {
           vpr::SocketStream    con_sock;
           std::string      data;
            ret_val = connector.connect(con_sock, remote_addr, 100);
            threadAssertTest((ret_val == true), "Connector can't connect");
       
            vpr::Uint16 size = con_sock.read(data, mMessageLen);   // Recieve data
            threadAssertTest((size == mMessageLen), "Connector recieved message of wrong size" );

            //ret_val = con_sock.isConnected();
            //threadAssertTest((ret_val == false), "Socket should still be connected");            
            
            con_sock.close();                                   // Close socket
            //mSyncMutex.release();                   // Tell acceptor that I have closed my side
       }
   }
   
   static Test* suite()
   {
      TestSuite *test_suite = new TestSuite ("SocketConnectorAcceptorTest");

      test_suite->addTest( new TestCaller<SocketConnectorAcceptorTest>("testAcceptorConstruction", &SocketConnectorAcceptorTest::testAcceptorConstruction));
      test_suite->addTest( new TestCaller<SocketConnectorAcceptorTest>("testSpawnedAcceptor", &SocketConnectorAcceptorTest::testSpawnedAcceptor));      
      return test_suite;
   }

private:   
   bool             mThreadAssertTest; // true for no error

protected:
    vpr::Uint16     mRendevousPort;     // The port the acceptor will be waiting on
    int             mNumItersA;        // Number of primary iterations
    int             mNumItersB;        // Number of secondary iterations
    
    vpr::Uint16     mMessageLen;        // Length of message to send
    std::string     mMessageValue;      // The value of the message that is supposed to be sent (and recieved)

    vpr::Mutex      mSyncMutex;     // Mutex used to do synchronization
    bool            mStartFlag;
};

}       // namespace

#endif          
