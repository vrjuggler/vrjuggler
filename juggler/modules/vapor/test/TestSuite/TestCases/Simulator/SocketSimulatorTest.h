#ifndef _SOCKET_SIMULATOR_TEST_H_
#define _SOCKET_SIMULATOR_TEST_H_

#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/ThreadTestCase.h>

#include <vpr/vprConfig.h>

#include <vpr/vpr.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Thread/ThreadFunctor.h>
#include <vpr/Sync/CondVar.h>
#include <vpr/IO/Socket/SocketAcceptor.h>
#include <vpr/IO/Socket/SocketConnector.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/Util/ReturnStatus.h>
#include <vpr/System.h>

#include <vpr/md/SIM/Controller.h>


namespace vprTest
{

class SocketSimulatorTest : public CppUnit::ThreadTestCase
{
public:
   SocketSimulatorTest (std::string name)
      : CppUnit::ThreadTestCase(name)
   {
      /* Do nothing. */ ;
   }

   SocketSimulatorTest ()
      : CppUnit::ThreadTestCase()
   {
      /* Do nothing. */ ;
   }

   virtual ~SocketSimulatorTest (void) {
      /* Do nothing. */ ;
   }

   void graphConstructionTest (void)
   {
      vpr::ReturnStatus status;
      vpr::sim::NetworkGraph::AddressList addrs;

      status = constructGraph();
      CPPUNIT_ASSERT(status.success() && "Could not construct network");

      status = vpr::sim::Controller::instance()->getNetworkGraph().getAllAddresses(addrs);
      CPPUNIT_ASSERT(status.success() && "Could not request all addresses");

      CPPUNIT_ASSERT(addrs.size() == vpr::sim::Controller::instance()->getNetworkGraph().getNodeCount());
   }

   void singleThreadTest (void)
   {
      vpr::SocketStream sender, acceptor, receiver;
      vpr::ReturnStatus status;

      status = acceptor.setLocalAddr(vpr::InetAddr(5556));
      CPPUNIT_ASSERT(status.success() && "Acceptor failed to set local address");

      status = acceptor.open();
      CPPUNIT_ASSERT(status.success() && "Failed to open acceptor");

      status = acceptor.bind();
      CPPUNIT_ASSERT(status.success() &&
                "Acceptor failed to find to local address");

      status = acceptor.listen();
      CPPUNIT_ASSERT(status.success() &&
                "Failed to set listening state for acceptor");

      status = acceptor.accept(receiver);
      CPPUNIT_ASSERT(status == vpr::ReturnStatus::WouldBlock &&
                "Acceptor should not have accepted");

      status = sender.setRemoteAddr(acceptor.getLocalAddr());
      CPPUNIT_ASSERT(status.success() && "Sender failed to set remote address");

      status = sender.open();
      CPPUNIT_ASSERT(status.success() && "Failed to open sender");

      status = sender.connect();
      CPPUNIT_ASSERT(status == vpr::ReturnStatus::InProgress &&
                 "Connection to acceptor should be in progress");

      status = acceptor.accept(receiver);
      CPPUNIT_ASSERT(status.success() && "Failed to accept waiting connection");

      // From this point on, we need the simulator to move messages around...
      std::string buffer("This is a message");
      vpr::Uint32 bytes_written;
      status = sender.write(buffer, buffer.length(), bytes_written);
      CPPUNIT_ASSERT(status.success() && "Failed to send buffer to receiver");

      char buffer2[20];
      vpr::Uint32 bytes_read;

      do
      {
         status = receiver.read(buffer2, sizeof(buffer2), bytes_read);
      } while (status == vpr::ReturnStatus::WouldBlock);

      CPPUNIT_ASSERT(status.success() && "Failed to receive buffer");
      CPPUNIT_ASSERT(bytes_read == buffer.length() && "Received message wrong size");

      status = receiver.close();
      CPPUNIT_ASSERT(status.success() && "Failed to close receiver");

      status = sender.close();
      CPPUNIT_ASSERT(status.success() && "Failed to close sender");

      status = acceptor.close();
      CPPUNIT_ASSERT(status.success() && "Failed to close acceptor");
   }

   void networkCommTest (void)
   {
      vpr::ReturnStatus status;
      vpr::sim::NetworkGraph::AddressList addrs;

//      status = constructGraph();
//      CPPUNIT_ASSERT(status.success() && "Could not construct network");

      status =
         vpr::sim::Controller::instance()->getNetworkGraph().getAllAddresses(addrs);
      CPPUNIT_ASSERT(status.success() && "Could not request all addresses");

      const unsigned int node_count =
         vpr::sim::Controller::instance()->getNetworkGraph().getNodeCount();
      CPPUNIT_ASSERT(node_count > 0 && "No nodes in the graph");

      std::vector<vpr::Uint32> addr_vec;
      addr_vec.resize(node_count);
      std::iota(addr_vec.begin(), addr_vec.end(), 0);
      std::random_shuffle(addr_vec.begin(), addr_vec.end());

      // Create a vector with min(20, node_count) sockets, all with default
      // addresses,
      const unsigned int socket_count = (20 < node_count ? 20 : node_count);
      std::vector<vpr::SocketStream> sockets;

      vpr::InetAddr local_addr;

      // Create a bunch of sockets with different addresses to ensure that they
      // are bound to different nodes on the network.
      for ( unsigned int i = 0; i < socket_count; i++ )
      {
         vpr::sim::NetworkGraph::net_vertex_t node =
            vpr::sim::Controller::instance()->getNetworkGraph().getNode(addr_vec[i]);
         vpr::sim::NetworkNodePtr node_prop =
            vpr::sim::Controller::instance()->getNetworkGraph().getNodeProperty(node);

         // All the sockets can have the same port number because they will be
         // on different nodes in the network.
         local_addr.setAddress(node_prop->getIpAddress(), 40000);

         sockets.push_back(vpr::SocketStream(local_addr,
                                             vpr::InetAddr::AnyAddr));
         CPPUNIT_ASSERT(sockets[i].getLocalAddr() == local_addr && "Address assignment failed");

         status = sockets[i].open();
         CPPUNIT_ASSERT(status.success() && "Failed to open new socket");

         status = sockets[i].bind();
         CPPUNIT_ASSERT(status.success() && "Failed to bind socket to local address");
      }

      std::vector<vpr::SocketStream> source_sockets, dest_sockets;

      // Put the even-numbered sockets in a listening state, and connect the
      // odd-numbered sockets to them.  That is, socket(n) is an acceptor,
      // and socket(n + 1) connects to it (where n is even).
      for ( unsigned int i = 0; i + 1 < socket_count; i += 2 )
      {
         const unsigned int acceptor = i, connector = i + 1;

         status = sockets[acceptor].listen();
         CPPUNIT_ASSERT(status.success() && "Failed to go into listening state");

         sockets[connector].setRemoteAddr(sockets[acceptor].getLocalAddr());
         status = sockets[connector].connect();
         CPPUNIT_ASSERT(status == vpr::ReturnStatus::InProgress &&
                        "Connection to acceptor should be in progress");

         vpr::SocketStream new_sock;
         status = sockets[acceptor].accept(new_sock);
         CPPUNIT_ASSERT(status.success() && "Failed to accept waiting connection");

         source_sockets.push_back(sockets[connector]);
         dest_sockets.push_back(new_sock);
      }

      CPPUNIT_ASSERT(source_sockets.size() == dest_sockets.size() &&
                     "Different number of source and destination sockets");

      std::string msg("Here is a packet");

      // Now we send a message from each source to its connected destination.
      for ( unsigned int i = 0; i < source_sockets.size(); i++ )
      {
         vpr::Uint32 bytes_written;
         status = source_sockets[i].send(msg, msg.length(), bytes_written);
         CPPUNIT_ASSERT(status.success() && "Failed to send buffer to receiver");

         char buffer[20];
         vpr::Uint32 bytes_read;

         do
         {
            status = dest_sockets[i].recv(buffer, sizeof(buffer), bytes_read);
         } while (status == vpr::ReturnStatus::WouldBlock);

         buffer[bytes_read] = '\0';

         CPPUNIT_ASSERT(status.success() && "Failed to receive buffer");

         CPPUNIT_ASSERT(buffer == msg && "Got the wrong message");
      }

      for ( std::vector<vpr::SocketStream>::iterator i = sockets.begin();
            i != sockets.end();
            i++ )
      {
         (*i).close();
      }
   }

   void
   multiThreadTest (void) {
      vpr::sim::Controller simulator;
      vpr::ReturnStatus status;

      mState        = NOT_READY;
      mAcceptorPort = 34567;
      mMessage      = "The sixth sheik's sixth sheep's sick";
      mMessageLen   = mMessage.length();

      status = vpr::sim::Controller::instance()->constructNetwork("test_network.tiers");
      CPPUNIT_ASSERT(status.success() && "Could not construct network");

      vpr::ThreadMemberFunctor<SocketSimulatorTest>* acceptor_func =
         new vpr::ThreadMemberFunctor<SocketSimulatorTest>(this, &SocketSimulatorTest::multiThreadTest_acceptor);
      vpr::Thread acceptor(acceptor_func);

      vpr::ThreadMemberFunctor<SocketSimulatorTest>* connector_func =
         new vpr::ThreadMemberFunctor<SocketSimulatorTest>(this, &SocketSimulatorTest::multiThreadTest_connector);
      vpr::Thread connector(connector_func);

      // Run, Lola, run.
      while ( vpr::sim::Controller::instance()->isRunning() ) {
         vpr::sim::Controller::instance()->processNextEvent();
      }

      acceptor.join();
      connector.join();

      checkThreadAssertions();
   }

   void
   multiThreadTest_acceptor (void* arg) {
      vpr::ReturnStatus status;
      vpr::Uint32 bytes_written;
      vpr::SocketAcceptor acceptor;
      vpr::SocketStream client_sock;
      vpr::InetAddr acceptor_addr(mAcceptorPort);

      status = acceptor.open(acceptor_addr);
      assertTestThread(status.success() && "Failed to open acceptor");

      // The acceptor must be non-blocking so that the connected socket it
      // returns will also be non-blocking.  *sigh*
      status = acceptor.getSocket().enableNonBlocking();
      assertTestThread(status.success() &&
                       "Failed to enable non-blocking for accepted socket");

      mCondVar.acquire();
      {
         mState = ACCEPTOR_READY;
         mCondVar.signal();
      }
      mCondVar.release();

      do {
         status = acceptor.accept(client_sock);
      } while ( status == vpr::ReturnStatus::WouldBlock );

      assertTestThread(status.success() && "Accept failed");

      assertTestThread(client_sock.isOpen() && "Accepted socket should be open");
      assertTestThread(client_sock.getNonBlocking() &&
                       "Connected client socket should be non-blocking");

      // Wait until the connector knows that it's connected before we proceed.
      mCondVar.acquire();
      {
         while ( mState != CONNECTOR_READY ) {
            mCondVar.wait();
         }
      }
      mCondVar.release();

      client_sock.setNoDelay(true);
      status = client_sock.send(mMessage, mMessageLen, bytes_written);
      assertTestThread(! status.failure() &&
                       "Failed to send message to client");

      mCondVar.acquire();
      {
         mState = DATA_SENT;
         mCondVar.signal();
      }
      mCondVar.release();

      status = client_sock.close();
      assertTestThread(status.success() &&
                       "Could not close acceptor side of connection");

      status = acceptor.close();
      assertTestThread(status.success() && "Could not close acceptor");
   }

   void
   multiThreadTest_connector (void* arg) {
      vpr::ReturnStatus status;
      vpr::InetAddr remote_addr;
      vpr::SocketConnector connector;
      vpr::SocketStream con_sock;
      std::string data;
      vpr::Uint32 bytes_read;

      remote_addr.setAddress("localhost", mAcceptorPort);

      mCondVar.acquire();
      {
         while ( mState != ACCEPTOR_READY ) {
            mCondVar.wait();
         }
      }
      mCondVar.release();

      status = con_sock.open();
      assertTestThread(status.success() && "Failed to open connector socket");

      status = con_sock.enableNonBlocking();
      assertTestThread(status.success() &&
                       "Failed to enable non-blocking for connector");

      status = connector.connect(con_sock, remote_addr,
                                 vpr::Interval(5, vpr::Interval::Sec));

      // XXX: For now, it's hard to test when the connection completes
      // with sim sockets--at least in this test.  The acceptor can easily
      // do all of its work and close its side of the connection before we
      // even have a chance to test if the connection has been completed.
      // When this happens, we think the connection hasn't completed and just
      // keep waiting.  Timeouts would probably help a little...
      if ( ! status.success() ) {
         vpr::Selector selector;
         vpr::Uint16 num_events;

         selector.addHandle(con_sock.getHandle(), vpr::Selector::Write);

         do {
             status = selector.select(num_events);
         } while ( ! status.success() );

         connector.complete(con_sock);
      }

      assertTestThread(status.success() && "Connector can't connect");

      assertTestThread(con_sock.getNonBlocking() &&
                       "Connector should be non-blocking");

      // Tell the acceptor that we are ready and it can do whatever it wants.
      mCondVar.acquire();
      {
         mState = CONNECTOR_READY;
         mCondVar.signal();
      }
      mCondVar.release();

      // Wait until the data has been sent.
      mCondVar.acquire();
      {
         while ( mState != DATA_SENT ) {
            mCondVar.wait();
         }
      }
      mCondVar.release();

/*
      while ( ! con_sock.isReadBlocked(vpr::Interval(50, vpr::Interval::Msec)) )
      {
         vpr::Thread::yield();  // XXX: Remove me if timeouts work
      }
*/

      do {
         status = con_sock.recv(data, mMessageLen, bytes_read);
         vpr::Thread::yield();  // XXX: Remove me if timeouts work
      } while ( status == vpr::ReturnStatus::WouldBlock );

      assertTestThread(bytes_read == mMessageLen &&
                       "Connector received message of wrong size");

      con_sock.close();
   }

   static CppUnit::Test*
   suite (void) {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite ("SocketSimulatorTest");
      test_suite->addTest( new CppUnit::TestCaller<SocketSimulatorTest>("graphConstructionTest", &SocketSimulatorTest::graphConstructionTest));
      test_suite->addTest( new CppUnit::TestCaller<SocketSimulatorTest>("singleThreadTest", &SocketSimulatorTest::singleThreadTest));
      test_suite->addTest( new CppUnit::TestCaller<SocketSimulatorTest>("networkCommTest", &SocketSimulatorTest::networkCommTest));
//      test_suite->addTest(new CppUnit::TestCaller<SocketSimulatorTest>("multiThreadTest", &SocketSimulatorTest::multiThreadTest));

      return test_suite;
   }

protected:
   vpr::ReturnStatus constructGraph (void)
   {
      std::string path_base;
      vpr::ReturnStatus status;

      status = vpr::System::getenv("VPR_TEST_DIR", path_base);
      CPPUNIT_ASSERT(status.success() && "Could not find VPR_TEST_DIR environment variable");

      status = vpr::sim::Controller::instance()->constructNetwork(path_base.append("/test_network.tiers"));
      CPPUNIT_ASSERT(status.success() && "Could not construct network");

      return status;
   }

   enum State {
      ACCEPTOR_READY,
      NOT_READY,
      CONNECTOR_CLOSED,
      CONNECTOR_READY,
      DATA_SENT,
      DONE_READING
   };

   State        mState;
   vpr::CondVar mCondVar;
   vpr::Uint16  mAcceptorPort;
   std::string  mMessage;
   vpr::Uint16  mMessageLen;
};

}

#endif
