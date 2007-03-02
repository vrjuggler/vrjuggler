#include <stdio.h>
#include <boost/bind.hpp>

#include <vpr/vpr.h>
#include <vpr/IO/Socket/Socket.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/IO/Socket/InetAddr.h>

#include <vpr/System.h>

#include <vpr/Thread/Thread.h>

#include <TestCases/Socket/SocketCopyConstructorTest.h>

#include <boost/concept_check.hpp>


namespace vprTest
{
CPPUNIT_TEST_SUITE_REGISTRATION( SocketCopyConstructorTest );

void SocketCopyConstructorTest::simpleTest ()
{
   const vpr::Uint16 remote_port(13579);
   vpr::InetAddr remote_addr;
   remote_addr.setPort(remote_port);

   vpr::SocketStream*	sock(NULL);
   vpr::SocketStream*  newSock1(NULL);
   vpr::SocketStream*  newSock2(NULL);
   sock = new vpr::SocketStream( vpr::InetAddr::AnyAddr, remote_addr );

   newSock1= new vpr::SocketStream (*sock);
   newSock2= new vpr::SocketStream (*sock);
   CPPUNIT_ASSERT(sock->getType() == newSock1->getType());
   CPPUNIT_ASSERT(sock->getRemoteAddr() == newSock1->getRemoteAddr());
   CPPUNIT_ASSERT(sock->getLocalAddr() == newSock1->getLocalAddr());

   delete sock;
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Can not open copied socket1", newSock1->open());
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Can not open copied socket2", newSock2->open());

   CPPUNIT_ASSERT(newSock2->getType() == newSock1->getType());
   CPPUNIT_ASSERT(newSock2->getRemoteAddr() == newSock1->getRemoteAddr());
   CPPUNIT_ASSERT(newSock2->getLocalAddr() == newSock1->getLocalAddr());

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Can not close copied socket1", newSock1->close());
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Can not close copied socket2", newSock2->close());

   delete newSock1;
   delete newSock2;
}

void SocketCopyConstructorTest::testCopyConnectedSocket ()
{
   mState = NOT_READY;                        // Initialize

   // Spawn acceptor thread
   vpr::Thread acceptor_thread(
      boost::bind(&SocketCopyConstructorTest::testCopyConstructor_acceptor,
                  this)
   );

   // spawn a connector thread
   vpr::Thread connector_thread(
      boost::bind(&SocketCopyConstructorTest::testCopyConstructor_connector,
                  this)
   );

   connector_thread.join();
   acceptor_thread.join();
}

void SocketCopyConstructorTest::testCopyConstructor_connector()
{
   const vpr::Uint16 port(13579);
   vpr::InetAddr remote_addr;
   char buffer[40];
   remote_addr.setPort(port);
   vpr::SocketStream connector_socket(vpr::InetAddr::AnyAddr, remote_addr);
   vpr::SocketStream* copy_connector(NULL);

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Can not open connector socket", connector_socket.open());

   // WAIT for READY
   mCondVar.acquire();
   {
      while(mState != READY)
         mCondVar.wait();
   }
   mCondVar.release();

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("connect fails", connector_socket.connect());

   copy_connector = new vpr::SocketStream (connector_socket);

   CPPUNIT_ASSERT_NO_THROW(copy_connector->read (buffer, 40));
   copy_connector->close();

   CPPUNIT_ASSERT(copy_connector->getType() == connector_socket.getType());
   CPPUNIT_ASSERT(copy_connector->getRemoteAddr() == connector_socket.getRemoteAddr());
   CPPUNIT_ASSERT(copy_connector->getLocalAddr() == connector_socket.getLocalAddr());

   connector_socket.close();
   copy_connector->close();

   // Tell everyone that we closed
   mCondVar.acquire();
   {
      mState = CONNECTOR_CLOSED;
      mCondVar.signal();       // Tell any waiting threads
   }
   mCondVar.release();

   delete copy_connector;
}

void SocketCopyConstructorTest::testCopyConstructor_acceptor()
{
   const vpr::Uint16 port(13579);
   char buffer[]="Hello!";
   vpr::InetAddr local_addr;
   local_addr.setPort(port);
   vpr::SocketStream acceptor_socket(local_addr, vpr::InetAddr::AnyAddr);

   vpr::SocketStream child_socket;
   vpr::SocketStream* copy_acceptor(NULL);
   vpr::SocketStream* copy_child(NULL);

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Can not open acceptor socket", acceptor_socket.open());;

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Can not bind acceptor socket", acceptor_socket.bind());

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("acceptor listening fails", acceptor_socket.listen());
   // READY - Tell everyone that we are ready to accept
      mCondVar.acquire();
      {
         mState = READY;
         mCondVar.signal();       // Tell any waiting threads
      }
      mCondVar.release();
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("accept() fails", acceptor_socket.accept(child_socket));

   copy_acceptor = new vpr::SocketStream (acceptor_socket);
   copy_child = new vpr::SocketStream (child_socket);

   CPPUNIT_ASSERT(copy_acceptor->getType() == acceptor_socket.getType());
   CPPUNIT_ASSERT(copy_acceptor->getRemoteAddr() == acceptor_socket.getRemoteAddr());
   CPPUNIT_ASSERT(copy_acceptor->getLocalAddr() == acceptor_socket.getLocalAddr());

   if (copy_child != NULL)
   {
      copy_child->write(buffer, sizeof(buffer));
      CPPUNIT_ASSERT(copy_child->getType() == child_socket.getType());
      CPPUNIT_ASSERT(copy_child->getRemoteAddr() == child_socket.getRemoteAddr());
      CPPUNIT_ASSERT(copy_child->getLocalAddr() == child_socket.getLocalAddr());
   }

   // WAIT for close
   mCondVar.acquire();
      while(mState != CONNECTOR_CLOSED)
         mCondVar.wait();
   mCondVar.release();

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("acceptor close fails", acceptor_socket.close());
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("child close fails", child_socket.close());

   copy_acceptor->close();
   if (copy_child != NULL) copy_child->close();

   delete copy_acceptor;
   if (copy_child != NULL) delete copy_child;
}

} // End of vprTest namespace
