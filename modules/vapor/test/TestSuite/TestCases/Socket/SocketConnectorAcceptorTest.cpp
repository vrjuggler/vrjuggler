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

#include <TestCases/Socket/SocketConnectorAcceptorTest.h>

#include <boost/concept_check.hpp>


namespace vprTest
{

CPPUNIT_TEST_SUITE_REGISTRATION( SocketConnectorAcceptorTest );

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
   test_acceptor = new vpr::SocketAcceptor(local_addr, true, 7);
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
   test_acceptor->open(local_addr, true, 7);
   CPPUNIT_ASSERT(test_acceptor->getSocket().isOpen());
   test_acceptor->close();
   delete(test_acceptor);
}

void SocketConnectorAcceptorTest::testSpawnedAcceptor ()
{
#ifdef VPR_OS_Windows
   long rand_num(rand());
#else
   long rand_num(random());
#endif
   mRendevousPort = 47000 + (rand_num % 71);     // Get a partially random port
   mNumItersA = 5;
   mMessageValue = std::string("The Data");
   mMessageLen = mMessageValue.length();

   mState = NOT_READY;                        // Initialize

   // Spawn acceptor thread
   vpr::ThreadMemberFunctor<SocketConnectorAcceptorTest>* acceptor_functor =
      new vpr::ThreadMemberFunctor<SocketConnectorAcceptorTest>(this, &SocketConnectorAcceptorTest::testSpawnedAcceptor_acceptor);
   vpr::Thread acceptor_thread(acceptor_functor);

   // Spawn connector thread
   vpr::ThreadMemberFunctor<SocketConnectorAcceptorTest>* connector_functor =
      new vpr::ThreadMemberFunctor<SocketConnectorAcceptorTest>(this, &SocketConnectorAcceptorTest::testSpawnedAcceptor_connector);
   vpr::Thread connector_thread(connector_functor);

   CPPUNIT_ASSERT( acceptor_thread.valid() && "Invalid acceptor thread");
   CPPUNIT_ASSERT( connector_thread.valid() && "Invalid connector thread");

   if(!acceptor_thread.valid())
      std::cerr << "Invalid acceptor thread\n";
   if(!connector_thread.valid())
      std::cerr << "Invalid connector_thread\n";

   // Wait for threads
   acceptor_thread.join();
   connector_thread.join();
}

void SocketConnectorAcceptorTest::testSpawnedAcceptor_acceptor (void* arg)
{
   boost::ignore_unused_variable_warning(arg);

   vpr::InetAddr local_acceptor_addr;
   local_acceptor_addr.setAddress("localhost", mRendevousPort);
   vpr::SocketAcceptor acceptor;
   vpr::SocketStream* sock(NULL);
   vpr::ReturnStatus ret_val;
   vpr::Uint32 bytes_written;

   // Open the acceptor
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Acceptor did not open correctly",
      acceptor.open(local_acceptor_addr));

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
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Accepting socket failed",
         acceptor.accept(*sock, vpr::Interval::NoTimeout ));

      CPPUNIT_ASSERT((sock->isOpen()) && "Accepted socket should be open");
      CPPUNIT_ASSERT((sock->isConnected()) && "Accepted socket should be connected");

      // Send a message
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Problem writing in acceptor",
         sock->write(mMessageValue, mMessageLen, bytes_written));
      CPPUNIT_ASSERT((bytes_written == mMessageLen) && "Didn't send entire messag");

      // WAIT for close
      mCondVar.acquire();
      while(mState != CONNECTOR_CLOSED)
         mCondVar.wait();
      mCondVar.release();

      CPPUNIT_ASSERT(sock->isConnected() && "Socket should not still be connected");

      // Close the socket
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Problem closing accepted socket", sock->close());
   }
}

void SocketConnectorAcceptorTest::testSpawnedAcceptor_connector (void* arg)
{
   boost::ignore_unused_variable_warning(arg);

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
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Connector can't connect",
         connector.connect(con_sock, remote_addr, vpr::Interval::NoTimeout ));

      // Recieve data
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Read failed", con_sock.read(data, mMessageLen, bytes_read));
      CPPUNIT_ASSERT((bytes_read == mMessageLen) && "Connector recieved message of wrong size" );

      CPPUNIT_ASSERT(con_sock.isConnected() && "Socket should still be connected");

      // Close socket
      CPPUNIT_ASSERT_NO_THROW(con_sock.close());

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
