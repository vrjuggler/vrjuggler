#include <stdio.h>
#include <string>
#include <iostream>

#include <vpr/vpr.h>
#include <vpr/IO/Socket/Socket.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/IO/Socket/InetAddr.h>
#include <vpr/IO/Socket/SocketAcceptor.h>
#include <vpr/IO/Socket/SocketConnector.h>

#include <vpr/Util/Interval.h>

#include <vpr/System.h>

#include <vpr/Thread/Thread.h>
#include <vpr/Thread/ThreadFunctor.h>

#include <SocketConnectorAcceptorTest.h>


namespace vprTest
{

void SocketConnectorAcceptorTest::testAcceptorConstruction ()
{
   const vpr::Uint16 server_port(12021);
   vpr::InetAddr local_addr;
   local_addr.setPort(server_port);
   vpr::SocketAcceptor* test_acceptor(NULL);

   // Test address in constructor
   test_acceptor = new vpr::SocketAcceptor(local_addr);
   CPPUNIT_ASSERT(test_acceptor != NULL);
   CPPUNIT_ASSERT(test_acceptor->getSocket().isOpen());
   test_acceptor->close();
   delete(test_acceptor);

   // Test address in constructor (with backlog)
   test_acceptor = new vpr::SocketAcceptor(local_addr,7);
   CPPUNIT_ASSERT(test_acceptor != NULL);
   CPPUNIT_ASSERT(test_acceptor->getSocket().isOpen());
   test_acceptor->close();
   delete(test_acceptor);

   // Test default constructor
   test_acceptor = new vpr::SocketAcceptor();
   CPPUNIT_ASSERT(test_acceptor != NULL);
   test_acceptor->open(local_addr);
   CPPUNIT_ASSERT(test_acceptor->getSocket().isOpen());
   test_acceptor->close();
   delete(test_acceptor);

   // Test default constructor (with backlog)
   test_acceptor = new vpr::SocketAcceptor();
   CPPUNIT_ASSERT(test_acceptor != NULL);
   test_acceptor->open(local_addr,7);
   CPPUNIT_ASSERT(test_acceptor->getSocket().isOpen());
   test_acceptor->close();
   delete(test_acceptor);
}

void SocketConnectorAcceptorTest::testSpawnedAcceptor ()
{
   threadAssertReset();
   mRendevousPort = 47000 + (random() % 71);     // Get a partially random port
   mNumItersA = 5;
   mMessageValue = std::string("The Data");
   mMessageLen = mMessageValue.length();

   mState = NOT_READY;                        // Initialize

   // Spawn acceptor thread
   vpr::ThreadMemberFunctor<SocketConnectorAcceptorTest>* acceptor_functor =
        new vpr::ThreadMemberFunctor<SocketConnectorAcceptorTest>( this, &SocketConnectorAcceptorTest::testSpawnedAcceptor_acceptor );
   vpr::Thread acceptor_thread( acceptor_functor);

   // Spawn connector thread
   vpr::ThreadMemberFunctor<SocketConnectorAcceptorTest>* connector_functor =
      new vpr::ThreadMemberFunctor<SocketConnectorAcceptorTest>( this, &SocketConnectorAcceptorTest::testSpawnedAcceptor_connector );
   vpr::Thread connector_thread( connector_functor);

   CPPUNIT_ASSERT( acceptor_thread.valid() && "Invalid acceptor thread");
   CPPUNIT_ASSERT( connector_thread.valid() && "Invalid connector thread");

   if(!acceptor_thread.valid())
      std::cerr << "Invalid acceptor thread\n";
   if(!connector_thread.valid())
      std::cerr << "Invalid connector_thread\n";

   // Wait for threads
   acceptor_thread.join();
   connector_thread.join();

   checkThreadAssertions();
}

void SocketConnectorAcceptorTest::testSpawnedAcceptor_acceptor (void* arg)
{
   vpr::InetAddr local_acceptor_addr;
   local_acceptor_addr.setAddress("localhost", mRendevousPort);
   vpr::SocketAcceptor acceptor;
   vpr::SocketStream* sock(NULL);
   vpr::ReturnStatus ret_val;
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

void SocketConnectorAcceptorTest::testSpawnedAcceptor_connector (void* arg)
{
   vpr::ReturnStatus ret_val;
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

} // End of vprTest namespace
