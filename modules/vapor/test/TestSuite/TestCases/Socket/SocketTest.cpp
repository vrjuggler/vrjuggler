#include <stdio.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <string>

#include <cppunit/TestCaller.h>

#include <vpr/vpr.h>
#include <vpr/IO/Socket/Socket.h>
#include <vpr/IO/Socket/InetAddr.h>
#include <vpr/IO/Socket/SocketAcceptor.h>
#include <vpr/IO/Socket/SocketConnector.h>
#include <vpr/System.h>

#include <vpr/Thread/Thread.h>
#include <vpr/Thread/ThreadFunctor.h>

#include <TestCases/Socket/SocketTest.h>

namespace vprTest
{
CPPUNIT_TEST_SUITE_REGISTRATION( SocketTest );

void SocketTest::testOpenCloseOpen_connector( void* data )
{
   int num_of_times_to_test = 9;
   vpr::Uint16 port = 6970;
   //const int backlog = 5;
   bool result = 0;

   // make a new socket that will connect to port "port"
   vpr::InetAddr remote_addr;
   remote_addr.setPort(port);
   vpr::SocketStream connector_socket( vpr::InetAddr::AnyAddr, remote_addr );

      // run the test many times.
   for (int x = 0; x < num_of_times_to_test; ++x)
   {
      // open socket
      result = connector_socket.open().success();
      assertTestThread( (result != false) && "Socket::open() failed" );

      // connect to the acceptor
      result = connector_socket.connect().success();
      assertTestThread( result != false && "Socket::connect() failed" );

      // let acceptor accept it before closing
      vpr::System::msleep( 50 );

      // close the socket
      result = connector_socket.close().success();
      assertTestThread( result != false && "Socket::close() failed" );

      // let the acceptor get a chance to start before connecting (sleep a while)
      vpr::System::usleep( 5000 );
   }
}

void SocketTest::testOpenCloseOpen_acceptor( void* data )
{
   int num_of_times_to_test = 3;
   vpr::Uint16 port = 6970;
   bool result = 0;

   vpr::InetAddr local_addr;

   // make a new socket listening on port "port"
   local_addr.setPort(port);
   vpr::SocketStream acceptor_socket( local_addr, vpr::InetAddr::AnyAddr );

   // start/stop the acceptor many times...
   for (int x = 0; x < num_of_times_to_test; ++x)
   {
      //std::cout << "[acceptor open]" << std::flush;

      // open socket
      result = acceptor_socket.open().success();
      assertTestThread( result != false && "Socket::open() failed" );

      result = acceptor_socket.bind().success();
      assertTestThread( result != false && "Socket::bind() failed" );

      // set the socket to listen
      result = acceptor_socket.listen().success();
      assertTestThread( result != false && "Socket::listen() failed" );

      //std::cout << "[accepting "<<num_of_times_to_test<<" connections]" << std::flush;

      vpr::SocketStream child_socket;

      // start/stop the child socket many times...
      for (int xx = 0; xx < num_of_times_to_test; ++xx)
      {
         //std::cout << x* num_of_times_to_test + xx << "+\n" << std::flush;
         //std::cout << "+" << std::flush;

         // wait for a connect (blocking)
         // when someone connects to the server, and we accept the connection,
         // spawn a child socket to deal with the connection
         result = acceptor_socket.accept(child_socket).success();
         assertTestThread( result && "Socket::accept() failed" );

         // close the child socket
         result = child_socket.close().success();
         assertTestThread( result != false && "Socket::close() failed" );
      }

      // close the socket
      result = acceptor_socket.close().success();
      assertTestThread( result != false && "Socket::close() failed" );

      //std::cout << "[acceptor close]\n" << std::flush;
   }
}

void SocketTest::testOpenCloseOpen ()
{
   //std::cout<<"]==================================================\n"<<std::flush;
   //std::cout<<" OpenCloseOpen Test: \n"<<std::flush;

   threadAssertReset();

   // spawn an acceptor thread
   vpr::ThreadMemberFunctor<SocketTest>* acceptor_functor =
      new vpr::ThreadMemberFunctor<SocketTest>(this, &SocketTest::testOpenCloseOpen_acceptor);
   vpr::Thread acceptor_thread(acceptor_functor);

   // let the acceptor get a chance to start before connecting (sleep a while)
   vpr::System::msleep( 500 );

   // spawn a connector thread
   vpr::ThreadMemberFunctor<SocketTest>* connector_functor =
      new vpr::ThreadMemberFunctor<SocketTest>(this, &SocketTest::testOpenCloseOpen_connector);
   vpr::Thread connector_thread(connector_functor);

   // wait for both threads to terminate, then continue
   //vpr::System::sleep( 7 );
   acceptor_thread.join();
   connector_thread.join();

   //std::cout << " done\n" << std::flush;
   checkThreadAssertions();

   //connector_thread.join();
   //acceptor_thread.join();

}

void SocketTest::testSendRecv_connector( void* data )
{
   int num_of_times_to_test = 10;
   vpr::Uint16 port = 6940;
   //const int backlog = 5;
   bool result = 0;
   vpr::InetAddr remote_addr;

   // make a new socket that will connect to port "port"
   remote_addr.setPort(port);
   vpr::SocketStream connector_socket( vpr::InetAddr::AnyAddr, remote_addr );

   // run the test many times.
   for (int x = 0; x < num_of_times_to_test; ++x)
   {
      // open socket
      result = connector_socket.open().success();
      assertTestThread( result != false && "Socket::open() failed" );

      // connect to the acceptor
      result = connector_socket.connect().success();
      assertTestThread( result != false && "Socket::connect() failed" );

      // find out how much data is coming...
      vpr::Uint32 size_of_data;
      vpr::Uint32 amount_read;
      result = connector_socket.readn(&size_of_data, sizeof(int),
                                      amount_read).success();
      assertTestThread( result && "readn didn't read" );
      assertTestThread( amount_read == sizeof( int ) && "readn didn't read all" );

      // get that amount of data...
      std::string buffer;
      buffer.resize( size_of_data );
      result = connector_socket.readn(&buffer[0], size_of_data,
                                      amount_read).success();
      assertTestThread( result && "readn didn't read" );
      assertTestThread( amount_read == size_of_data && "readn didn't read all" );

      //std::cout<<"Recieved buffer: "<<buffer<<"\n"<<std::flush;
      assertTestThread( buffer == testSendRecv_buffer && "didn't recieve the right data" );

      // close the socket
      result = connector_socket.close().success();
      assertTestThread( result != false && "Socket::close() failed" );

      // let the acceptor get a chance to start before connecting (sleep a while)
      vpr::System::usleep( 50000 );
   }
}

void SocketTest::testSendRecv_acceptor( void* data )
{
   int num_of_times_to_test = 10;
   vpr::Uint16 port = 6940;
   bool result = 0;
   vpr::InetAddr local_addr;

   // make a new socket listening on port "port"
   local_addr.setPort(port);
   vpr::SocketStream acceptor_socket( local_addr, vpr::InetAddr::AnyAddr );

   // open socket
   result = acceptor_socket.open().success();
   assertTestThread( result != false && "Socket::open() failed" );

   result = acceptor_socket.bind().success();
   assertTestThread( result != false && "Socket::bind() failed" );

   // set the socket to listen
   result = acceptor_socket.listen().success();
   assertTestThread( result != false && "Socket::listen() failed" );

   vpr::SocketStream child_socket;

   // start/stop the child socket many times...
   for (int xx = 0; xx < num_of_times_to_test; ++xx)
   {
      // wait for a connect (blocking)
      // when someone connects to the server, and we accept the connection,
      // spawn a child socket to deal with the connection
      result = acceptor_socket.accept(child_socket).success();
      assertTestThread( result && "Socket::accept() failed" );

      // send the size of the data to be sent
      vpr::Uint32 amount_sent;
      std::string buffer = testSendRecv_buffer;
      vpr::Uint32 size = buffer.size();
      result = child_socket.write( &size, sizeof( int ), amount_sent ).success();
      assertTestThread( result && "write didn't send" );
      assertTestThread( amount_sent == sizeof( int ) && "write didn't send all" );

      // send the data...
      result = child_socket.write( &buffer[0], buffer.size(), amount_sent).success();
      assertTestThread( result && "write didn't send" );
      assertTestThread( amount_sent == buffer.size() && "write didn't send all" );

      //std::cout<<"Sent buffer: "<<buffer<<"\n"<<std::flush;
      //std::cout<<"+"<<std::flush;

      // close the child socket
      result = child_socket.close().success();
      assertTestThread( result != false && "Socket::close() failed" );
   }

   // close the socket
   result = acceptor_socket.close().success();
   assertTestThread( result != false && "Socket::close() failed" );
}

void SocketTest::testSendRecv()
{
   //std::cout<<"]==================================================\n"<<std::flush;
   //std::cout<<" SendRecv Test: \n"<<std::flush;
   threadAssertReset();

   // spawn an acceptor thread
   vpr::ThreadMemberFunctor<SocketTest>* acceptor_functor =
      new vpr::ThreadMemberFunctor<SocketTest>(this, &SocketTest::testSendRecv_acceptor);
   vpr::Thread acceptor_thread(acceptor_functor);

   // let the acceptor get a chance to start before connecting (sleep a while)
   vpr::System::msleep( 500 );

   // spawn a connector thread
   vpr::ThreadMemberFunctor<SocketTest>* connector_functor =
      new vpr::ThreadMemberFunctor<SocketTest>(this, &SocketTest::testSendRecv_connector);
   vpr::Thread connector_thread(connector_functor);

   // wait for both threads to terminate, then continue
   //vpr::System::sleep( 7 );
   connector_thread.join(); // join is broken.
   acceptor_thread.join();

   checkThreadAssertions();
}

void SocketTest::testOpenClose()
{
   //std::cout<<"]==================================================\n"<<std::flush;
   //std::cout<<" OpenClose Test: \n"<<std::flush;

   bool openSuccess( false );
   bool closeSuccess( false );
   bool bindSuccess( false );

   vpr::Uint16 port=15432;
   vpr::InetAddr local_addr;
   vpr::SocketStream*   sock;

   local_addr.setAddress("localhost", port);
   sock = new vpr::SocketStream(local_addr, vpr::InetAddr::AnyAddr);
   openSuccess=sock->open().success();
   if (openSuccess)
   {
      //std::cout<< " Open...";
      bindSuccess=(sock->bind().success());
   }
   closeSuccess=sock->close().success();

   CPPUNIT_ASSERT( openSuccess && "Socket can not be opened!");
   CPPUNIT_ASSERT( bindSuccess && "Socket can not be bound!");
   CPPUNIT_ASSERT( closeSuccess && "Socket can not be closed!");
   delete sock;
}

void SocketTest::bindAgainFailTest()
{
   //std::cout<<"]==================================================\n"<<std::flush;
   //std::cout<<" multiple bind failure Test: \n"<<std::flush;
   bool openSuccess( false );
   bool closeSuccess( false );
   bool bindSuccess( false );
   vpr::InetAddr local_addr;
   vpr::Uint16 port = 6976;

   local_addr.setAddress("localhost", port);
   vpr::SocketStream sock( local_addr, vpr::InetAddr::AnyAddr );

   // make sure aditional calls to bind() fails...
   openSuccess = sock.open().success();
   CPPUNIT_ASSERT( openSuccess == true && "open() failed");

   bindSuccess = sock.bind().success();
   CPPUNIT_ASSERT( bindSuccess == true && "bind() failed");

   for (int x = 0; x < 2; ++x)
   {
      bindSuccess = sock.bind().success();
      CPPUNIT_ASSERT( bindSuccess == false && "Socket was able to bind again, this is bad.");
   }

   closeSuccess = sock.close().success();
   CPPUNIT_ASSERT( closeSuccess == true && "close() failed");
}

void SocketTest::sameAddressOpenBindCloseTest()
{
   int openSuccess( 0 ), closeSuccess( 0 ), bindSuccess( 0 );
   vpr::InetAddr local_addr;
   vpr::Uint16 port = (random() % 20000) + 20000;

   local_addr.setAddress("localhost", port);
   vpr::SocketStream sock( local_addr, vpr::InetAddr::AnyAddr );

   // same address, open-bind-close
   const int runs = 10;
   for (int xx = 0; xx < runs; ++xx)
   {
      openSuccess += sock.open().success() ? 1 : 0;
      bindSuccess += sock.bind().success() ? 1 : 0;
      closeSuccess += sock.close().success() ? 1 : 0;
      vpr::System::msleep(50);                           // XXX: Kludge to give OS time to close the descriptor
   }
   const float success_percent = 1.0f;
   float minimum_for_success = float(runs) * success_percent;

   CPPUNIT_ASSERT( openSuccess >= minimum_for_success && "open() failed");
   CPPUNIT_ASSERT( bindSuccess >= minimum_for_success && "bind() failed");
   CPPUNIT_ASSERT( closeSuccess >= minimum_for_success && "close() failed");
}

void SocketTest::sameAddressOpenBindDestructTest()
{
   int openSuccess( 0 ), bindSuccess( 0 );
   vpr::InetAddr local_addr;
   vpr::Uint16 port = (random() % 20000) + 20000;

   local_addr.setAddress("localhost", port);
   vpr::SocketStream sock( local_addr, vpr::InetAddr::AnyAddr );

   // same address, open-bind-close
   const int runs = 10;
   for (int xx = 0; xx < runs; ++xx)
   {
      vpr::SocketStream* sock = new vpr::SocketStream( local_addr, vpr::InetAddr::AnyAddr );

      openSuccess += sock->open().success() ? 1 : 0;
      bindSuccess += sock->bind().success() ? 1 : 0;
      delete sock;
      vpr::System::msleep(50);                           // XXX: Kludge to give OS time to close the descriptor
   }
   const float success_percent = 1.0f;
   float minimum_for_success = float(runs) * success_percent;

   CPPUNIT_ASSERT( openSuccess >= minimum_for_success && "open() failed");
   CPPUNIT_ASSERT( bindSuccess >= minimum_for_success && "bind() failed");
}

void SocketTest::differentAddressOpenBindCloseTest()
{
   int openSuccess( 0 ), closeSuccess( 0 ), bindSuccess( 0 );
   vpr::InetAddr local_addr;
   vpr::Uint16 port = (random() % 20000) + 20000;

   // same address, open-bind-close
   const int runs = 10;
   for (int xx = 0; xx < runs; ++xx)
   {
      port++;
      CPPUNIT_ASSERT(local_addr.setAddress("localhost", port).success());

      vpr::SocketStream sock( local_addr, vpr::InetAddr::AnyAddr );

      openSuccess += sock.open().success() ? 1 : 0;
      bindSuccess += sock.bind().success() ? 1 : 0;
      closeSuccess += sock.close().success() ? 1 : 0;
      //vpr::System::msleep(50);
   }
   const float success_percent = 1.0f;
   float minimum_for_success = float(runs) * success_percent;

   CPPUNIT_ASSERT( openSuccess >= minimum_for_success && "open() failed");
   CPPUNIT_ASSERT( bindSuccess >= minimum_for_success && "bind() failed");
   CPPUNIT_ASSERT( closeSuccess >= minimum_for_success && "close() failed");
}

void SocketTest::reuseAddrSimpleTest()
{
   //std::cout<<"]==================================================\n"<<std::flush;
   //std::cout<<" Reuse Address Test (simple version)"<<endl;
   vpr::InetAddr addr1;
   //vpr::InetAddr addr2;
   vpr::SocketStream*   sock1;
   vpr::SocketStream*   sock2;
   vpr::SocketStream*   sock3;

   addr1.setPort(13768);
   //CPPUNIT_ASSERT(addr2("129.186.232.58", 5438));

   sock1 = new vpr::SocketStream(addr1, vpr::InetAddr::AnyAddr);
   sock2 = new vpr::SocketStream(addr1, vpr::InetAddr::AnyAddr);
   sock3 = new vpr::SocketStream(addr1, vpr::InetAddr::AnyAddr);
   if (sock1->open().success())
   {
      sock1->setReuseAddr(true);
      CPPUNIT_ASSERT(sock1->bind().success());
   }
   else CPPUNIT_ASSERT(false && "Cannot open sock1");
   if (sock2->open().success())
   {
      CPPUNIT_ASSERT(sock2->bind().success() && "Cannot bind second socket re-using addr");
   }
   else CPPUNIT_ASSERT(false && "Cannot open sock2");
   if (sock3->open().success())
      CPPUNIT_ASSERT(sock3->bind().success() && "Cannot bind third socket re-using addr");
   sock1->close();
   sock2->close();
   sock3->close();
   delete sock1;
   delete sock2;
   delete sock3;
}

void SocketTest::reuseAddrTest_connector( void* data )
{
   vpr::Uint16 port = 6667;
   bool result = 0;
   vpr::InetAddr remote_addr;

   // make a new socket that will connect to port "port"
   remote_addr.setPort(port);
   vpr::SocketStream connector_socket( vpr::InetAddr::AnyAddr, remote_addr );

   // open socket
   result = connector_socket.open().success();
   assertTestThread( result != false && "Socket::open() failed" );

   // connect to the acceptor
   result = connector_socket.connect().success();
   assertTestThread( result != false && "Socket::connect() failed" );

   // close the socket
   result = connector_socket.close().success();
   assertTestThread( result != false && "Socket::close() failed" );
}

void SocketTest::reuseAddrTest_acceptor( void* data )
{
   vpr::InetAddr addr1;

   assertTestThread(addr1.setAddress( "localhost", 6667 ).success());

   vpr::SocketStream sock1( addr1, vpr::InetAddr::AnyAddr );
   vpr::SocketStream sock2( addr1, vpr::InetAddr::AnyAddr );

   CPPUNIT_ASSERT( sock1.open().success() && "server Socket::open() failed" );
   sock1.setReuseAddr( true );
   assertTestThread( sock1.bind().success() && "server Socket::bind() failed" );
   assertTestThread( sock1.listen().success() && "server Socket::listen() failed" );

   vpr::SocketStream child_socket;
   vpr::ReturnStatus status = sock1.accept(child_socket);
   assertTestThread( status.success() && "child didn't spawn" );

      // assume server crashes, so lets restart it.
      assertTestThread( sock2.open().success() && "open(): server restart" );
      sock1.setReuseAddr( true ); // set the opt in-between for bind() to succeed
      bool result = sock2.bind().success();
      //std::cout<<"::::::::: " << result<<"\n"<<std::flush;
      assertTestThread( result == true && "bind(): server restart" );
      //std::cout<<"::::::::: " << result<<"\n"<<std::flush;

   // close the child socket
   assertTestThread( child_socket.close().success() && "child Socket::close() failed" );

   assertTestThread( sock2.close().success() && "restarted server Socket::close() failed" );
   assertTestThread( sock1.close().success() && "server Socket::close failed");
}

// XXX: Fails due to crashing thread (I think it is the connector thread)
void SocketTest::reuseAddrTest()
{
   //std::cout<<"]==================================================\n"<<std::flush;
   //std::cout<<" reuseAddr Test (cli/serv version): \n"<<std::flush;
   threadAssertReset();

   // spawn an acceptor thread
   vpr::ThreadMemberFunctor<SocketTest>* acceptor_functor =
      new vpr::ThreadMemberFunctor<SocketTest>(this, &SocketTest::reuseAddrTest_acceptor);
   vpr::Thread acceptor_thread(acceptor_functor);

   // let the acceptor get a chance to start before connecting (sleep a while)
   vpr::System::msleep( 500 );

   // spawn a connector thread
   vpr::ThreadMemberFunctor<SocketTest>* connector_functor =
      new vpr::ThreadMemberFunctor<SocketTest>(this, &SocketTest::reuseAddrTest_connector);
   vpr::Thread connector_thread(connector_functor);

   // wait for both threads to terminate, then continue
   //vpr::System::sleep( 1 );

   acceptor_thread.join();
   connector_thread.join(); // join is broken.

   checkThreadAssertions();

}

void SocketTest::testBlocking_connector(void* arg)
{
   vpr::Uint16 port = 7001;
   vpr::InetAddr remote_addr;

   remote_addr.setPort(port);

   //keep testing until acceptor says finish
   while (mFinishFlag == false)
   {
      char    buffer[40];
      bool    result = 0;
      char    buffer2[]="Oops!";
      vpr::Uint32 amount_read, amount_written;

      // make a new socket that will connect to port "port"
      vpr::SocketStream connector_socket(vpr::InetAddr::AnyAddr, remote_addr);

      // open socket
      connector_socket.setOpenBlocking();
      result = connector_socket.open().success();
      assertTestThread( result != false && "Socket::open() failed when test blocking" );

      // connect to the acceptor
      result = connector_socket.connect().success();
      assertTestThread( result != false && "Socket::connect() failed when test blocking" );

      connector_socket.setNoDelay(true);

      //sleep until acceptor says begin
      while (mStartFlag == false)
      {
         vpr::System::usleep(50);
      }

      //set socket to blocking/nonblocking mode as required
      if (mBlockingFlag == true)
         connector_socket.enableBlocking();
      else
         connector_socket.enableNonBlocking();

      //if the the flag of readn is true, use readn(), otherwise use read().
      if (mReadnFlag == true)
         connector_socket.readn(buffer, 20, amount_read);
      else
         connector_socket.read(buffer, 20, amount_read);

      if (amount_read>=0)
      {
         connector_socket.write(buffer, amount_read, amount_written);
      }
      else
      {
         connector_socket.write(buffer2, sizeof(buffer2), amount_written);
      }

      while (mStartFlag == true)
      {
         vpr::System::usleep(50);
      }

      result = connector_socket.close().success();
      assertTestThread( result != false && "Socket::close() failed when test blocking" );
   }

   // let the acceptor get a chance to start before connecting (sleep a while)
   vpr::System::usleep( 50000 );
}

void SocketTest::testBlocking_acceptor(void* arg)
{
   int num_of_times_to_test = 20;
   vpr::Uint16 port = 7001;
   bool  result = 0;
   char  buffer[40];
   char  buffer1[]="Hello, there!";
   char  buffer2[]="Hello";
   vpr::Uint32 amount_read (0), amount_written;
   vpr::InetAddr local_addr;

   local_addr.setPort(port);

   // make a new socket listening on port "port"
   vpr::SocketStream acceptor_socket( local_addr, vpr::InetAddr::AnyAddr );
   acceptor_socket.setOpenBlocking();

   // open socket
   result = acceptor_socket.open().success();

   assertTestThread( result != false && "Socket::open() failed in blocking test" );

   result = acceptor_socket.bind().success();
   assertTestThread( result != false && "Socket::bind() failed in blocking test" );

   mFinishFlag = false;

   // set the socket to listen
   result = acceptor_socket.listen().success();
   assertTestThread( result != false && "Socket::listen() failed in blocking test" );


   vpr::SocketStream child_socket;

   // switch between blocking/nonblocking and read/readn many times...
   for (int xx = 0; xx < num_of_times_to_test; xx++)
   {
      // wait for a connect (blocking)
      // when someone connects to the server, and we accept the connection,
      // spawn a child socket to deal with the connection

      int yy=xx%4;

      result = acceptor_socket.accept(child_socket).success();
      assertTestThread( result && "Socket::accept() failed in blocking test" );

      mFlagProtectionMutex.acquire();
         if(yy==0 || yy==2)
            mBlockingFlag=true;
         else mBlockingFlag=false;

         if(yy==0 || yy==1)
            mReadnFlag=false;
         else mReadnFlag=true;
         mStartFlag=true;
      mFlagProtectionMutex.release();

      vpr::System::msleep(50);

      child_socket.write(buffer1, sizeof(buffer1), amount_written);

      vpr::System::msleep(5);

      child_socket.write(buffer2, sizeof(buffer2), amount_written);

      child_socket.enableNonBlocking();
      child_socket.read(buffer, 40, amount_read);
      child_socket.enableBlocking();

      switch (yy)
      {
      case 0:
         assertTestThread(amount_read == 14 && "Should return 14");
         break;
      case 1:
         assertTestThread(amount_read == 6 && "Should return 6");
         break;
      case 2:
         assertTestThread(amount_read == 0 && "Should return 20");
         break;
      case 3:
         assertTestThread(amount_read==6 && "Should return 6");
      }

      mFlagProtectionMutex.acquire();
      mStartFlag=false;
      mFlagProtectionMutex.release();

      result = child_socket.close().success();
      assertTestThread( result != false && "Socket::close() failed in blocking test" );
   }

   // close the socket
   mFlagProtectionMutex.acquire();
      mFinishFlag = true;
   mFlagProtectionMutex.release();
   result = acceptor_socket.close().success();
   assertTestThread( result != false && "Socket::close() failed" );
}

void SocketTest::testBlocking()
{
   threadAssertReset();

   // spawn an acceptor thread
   vpr::ThreadMemberFunctor<SocketTest>* acceptor_functor =
      new vpr::ThreadMemberFunctor<SocketTest>(this, &SocketTest::testBlocking_acceptor);
   vpr::Thread acceptor_thread(acceptor_functor);

   // let the acceptor get a chance to start before connecting (sleep a while)
   vpr::System::msleep( 500 );

   // spawn a connector thread
   vpr::ThreadMemberFunctor<SocketTest>* connector_functor =
      new vpr::ThreadMemberFunctor<SocketTest>(this, &SocketTest::testBlocking_connector);
   vpr::Thread connector_thread(connector_functor);

   // wait for both threads to terminate, then continue

   //vpr::System::sleep( 10 );
   connector_thread.join(); // join is broken.
   acceptor_thread.join();

   checkThreadAssertions();
}


void SocketTest::testTcpConnection()
{
   mServerCheck=0;
   //Creat a thread to do server listen:
   vpr::ThreadMemberFunctor<SocketTest>* serverFunctor;
   serverFunctor=new vpr::ThreadMemberFunctor<SocketTest>(this, &SocketTest::serverFunc);
   vpr::Thread* serverThread;
   serverThread=new vpr::Thread(serverFunctor);

   vpr::System::sleep(1);

   //Creat a bunch of client thread.
   std::vector<vpr::ThreadMemberFunctor<SocketTest>*> clientFunctors(mNumClient);
   std::vector<vpr::Thread*> clientThreads(mNumClient);
   for (int t=0; t<mNumClient; t++){
      clientFunctors[t] = new vpr::ThreadMemberFunctor<SocketTest>(this, &SocketTest::clientFunc);
      clientThreads[t] = new vpr::Thread(clientFunctors[t]);
   }

   vpr::System::sleep(1);
   //serverThread->join();

   CPPUNIT_ASSERT(mOpenServerSuccess ==-1 && "Open server failed");
   CPPUNIT_ASSERT(mServerCheck==0 && "Not all connections are correct.");
   delete serverFunctor;
   delete serverThread;

   for (int t=0; t<mNumClient; t++){
      delete clientFunctors[t];
      delete clientThreads[t];
   }
}

void SocketTest::serverFunc(void* arg)
{
   vpr::Uint16 port=15432;
   vpr::Uint16 num=0;

   std::vector<vpr::ThreadMemberFunctor<SocketTest>*> sServerFunctors(mNumSServer);
   std::vector<vpr::Thread*> sServerThreads(mNumSServer);
   vpr::InetAddr local_addr;

   local_addr.setPort(port);
   vpr::SocketStream*   sock;
   sock = new vpr::SocketStream(local_addr, vpr::InetAddr::AnyAddr);
   if ( sock->openServer().success() ) {
      vpr::SocketStream client_sock;
      thread_args_t* tArgs;

      while ( num<mNumSServer) {
         sock->accept(client_sock);
         tArgs = new thread_args_t;
         tArgs->mSock= new vpr::SocketStream(client_sock);

         sServerFunctors[num]=new vpr::ThreadMemberFunctor<SocketTest>(this, &SocketTest::sServerFunc, tArgs);
         sServerFunctors[num]->setArg(tArgs);
         sServerThreads[num]= new vpr::Thread(sServerFunctors[num]);

         num++;
      }
      mOpenServerSuccess =-1;
   }
   else {
      mOpenServerSuccess = 1;
   }
   sock->close();
   delete sock;
}

void SocketTest::clientFunc(void* arg)
{
   vpr::SocketStream*   sock;
   vpr::InetAddr remote_addr;

   remote_addr.setAddress("localhost", 15432);
   sock = new vpr::SocketStream(vpr::InetAddr::AnyAddr, remote_addr);
   if ( sock->open().success() ) {
      char buffer1[40];
      //char buffer2[] = "What's up?";
      if ( sock->connect().success() ) {
         vpr::Uint32 bytes, bytes_written;
         sock->read(buffer1, 40, bytes);
      //sock->write(buffer2, sizeof(buffer2), bytes_written);
         sock->write(buffer1, bytes, bytes_written);
         mItemProtectionMutex.acquire();
         mClientCounter++;
         mItemProtectionMutex.release();
      }
   }
   sock->close();
   delete sock;
}

void SocketTest::sServerFunc(void* arg)
{
   _thread_args* tArg=(_thread_args *) arg;
   char buffer1[] = "Hello there!";
   char buffer2[40];
   vpr::Uint32 bytes, bytes_written;

   vpr::SocketStream* ss_sock;
   ss_sock=tArg->mSock;
   //send a string to client
   ss_sock->write(buffer1, sizeof(buffer1), bytes_written);
   //receive a string from client
   ss_sock->read(buffer2,40, bytes);
   CPPUNIT_ASSERT(bytes != 0);
   long compareString=strcmp(buffer1,buffer2);
   if (compareString!=0) {
      mServerCheck=1;
   }
   ss_sock->close();
   delete ss_sock;

   // Deleting void* is not "good"
   //delete arg;
}

void SocketTest::testReadnClient (void* arg)
{
   vpr::Uint16 port = *((vpr::Uint16*) arg);
   vpr::InetAddr remote_addr;

   assertTestThread(remote_addr.setAddress("localhost", port).success() && "Could not assign address");
   vpr::SocketStream client_sock(vpr::InetAddr::AnyAddr, remote_addr);
   char buffer[20];

   assertTestThread(client_sock.open().success() && "Client socket open failed");
   assertTestThread(client_sock.connect().success() && "Client could not connect");
   vpr::Uint32 bytes;
   client_sock.readn(buffer, sizeof(buffer), bytes);
   assertTestThread((bytes == sizeof(buffer)) && "readn didn't read enough!");
   client_sock.close();
}

void SocketTest::testReadn ()
{
   vpr::Uint16 server_port = 55446;
   vpr::InetAddr local_addr;
   local_addr.setPort(server_port);
   vpr::SocketStream server_sock(local_addr, vpr::InetAddr::AnyAddr);
   const unsigned int pkt_size = 5;
   vpr::Uint32 bytes;
   char buffer[pkt_size];

   //std::cout << "]==================================================\n"
   //          << std::flush;
   //std::cout <<" Readn Test:\n" << std::flush;

   CPPUNIT_ASSERT(server_sock.open().success() && "Server socket open failed");
   CPPUNIT_ASSERT(server_sock.bind().success() && "Server socket bind failed");
   CPPUNIT_ASSERT(server_sock.listen().success() && "Server socket listen failed");

   // Start the client thread.
   vpr::ThreadMemberFunctor<SocketTest>* func =
      new vpr::ThreadMemberFunctor<SocketTest>(this, &SocketTest::testReadnClient,
                                               (void*) &server_port);
   vpr::Thread* client_thread = new vpr::Thread(func);
   CPPUNIT_ASSERT(client_thread->valid() && "Server could not create client thread");

   vpr::SocketStream client_sock;
   vpr::ReturnStatus status = server_sock.accept(client_sock);
   CPPUNIT_ASSERT(status.success() && "Server could not accept connection");

   for ( unsigned int i = 0; i < 20; i += pkt_size )
   {
      memset((void*) buffer, 0, sizeof(buffer));
      snprintf(buffer, sizeof(buffer), "%04d", i);

      client_sock.write(buffer, pkt_size, bytes);
      CPPUNIT_ASSERT(bytes != 0 && "Server could not write to client");
   }

   client_sock.close();
   delete client_thread;
   server_sock.close();
}

void SocketTest::testIsConnected ()
{
   threadAssertReset();

   mState        = NOT_READY;                        // Initialize
   mAcceptorPort = 34568;

   // Spawn acceptor thread
   vpr::ThreadMemberFunctor<SocketTest>* acceptor_functor =
      new vpr::ThreadMemberFunctor<SocketTest>(this, &SocketTest::testIsConnected_acceptor);
   vpr::Thread acceptor_thread(acceptor_functor);

   // Spawn connector thread
   vpr::ThreadMemberFunctor<SocketTest>* connector_functor =
         new vpr::ThreadMemberFunctor<SocketTest>(this, &SocketTest::testIsConnected_connector);
   vpr::Thread connector_thread(connector_functor);

   // Wait for threads
   acceptor_thread.join();
   connector_thread.join();

   checkThreadAssertions();
}

void SocketTest::testIsConnected_acceptor (void* arg)
{
   vpr::ReturnStatus status;
   vpr::SocketAcceptor acceptor;
   vpr::SocketStream client_sock;
   vpr::InetAddr acceptor_addr;
   acceptor_addr.setPort(mAcceptorPort);

   status = acceptor.open(acceptor_addr);
   assertTestThread(status.success() && "Failed to open acceptor");

   // --- STATE: Acceptor Read --- //
   mCondVar.acquire();
   {
      mState = ACCEPTOR_READY;
      mCondVar.signal();
   }
   mCondVar.release();

   status = acceptor.accept(client_sock);
   assertTestThread(status.success() && "Accept failed");
   assertTestThread(client_sock.isOpen() && "Accepted socket should be open");
   assertTestThread(client_sock.isConnected() && "Connector not connected");

   // --- STATE: Acceptor Tested --- //
   mCondVar.acquire();
   {
      mState = ACCEPTOR_TESTED;
      mCondVar.signal();
   }
   mCondVar.release();

   // --- WAIT FOR: Connector closed -- //
   mCondVar.acquire();
   {
      while ( mState != CONNECTOR_CLOSED ) {
         mCondVar.wait();
      }
   }
   mCondVar.release();

   // We now have a one sided socket -- Read and write should return NotConnected
   char temp_buffer[100];
   vpr::Uint32 bytes_handled;
   vpr::ReturnStatus ret_val;

   ret_val = client_sock.recv((void*)temp_buffer, 50, bytes_handled);

   assertTestThread(bytes_handled == 0);
   assertTestThread(ret_val == vpr::ReturnStatus::NotConnected);

   // This should give an error too, but it doesn't seem to
   ret_val = client_sock.send((void*)temp_buffer, 50, bytes_handled);

   status = client_sock.close();
   assertTestThread(status.success() &&
                    "Could not close acceptor side of client socket");

   ret_val = client_sock.send((void*)temp_buffer, 50, bytes_handled);

   assertTestThread(!ret_val.success());

   assertTestThread(! client_sock.isConnected() && "Connector not disconnected");

   /*
   status = client_sock.close();
   assertTestThread(status.success() &&
                    "Could not close acceptor side of client socket");
   */

   status = acceptor.close();
   assertTestThread(status.success() && "Could not close acceptor");
}

void SocketTest::testIsConnected_connector (void* arg)
{
   vpr::ReturnStatus status;
   vpr::InetAddr remote_addr;
   vpr::SocketConnector connector;
   vpr::SocketStream con_sock;
   std::string data;

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

   status = connector.connect(con_sock, remote_addr,
                              vpr::Interval(5, vpr::Interval::Sec));
   assertTestThread(status.success() && "Connector can't connect");

   // --- WAIT FOR: Acceptor Tested --- //
   mCondVar.acquire();
   {
      while ( mState != ACCEPTOR_TESTED ) {
         mCondVar.wait();
      }
   }
   mCondVar.release();

   status = con_sock.close();
   assertTestThread(status.success() && "Could not close connector side of client socket");

   // --- STATE: Connector closed --- //
   mCondVar.acquire();
   {
      mState = CONNECTOR_CLOSED;
      mCondVar.signal();
   }
   mCondVar.release();

   assertTestThread(! con_sock.isConnected() && "Connect didn't disconnect?");
}


} // End of vprTest namespace
