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

#include <boost/concept_check.hpp>

namespace vprTest
{
CPPUNIT_TEST_SUITE_REGISTRATION( SocketTest );

void SocketTest::testOpenCloseOpen_connector( void* data )
{
   boost::ignore_unused_variable_warning(data);

   int num_of_times_to_test = 9;
   vpr::Uint16 port = 6970;
   //const int backlog = 5;

   // make a new socket that will connect to port "port"
   vpr::InetAddr remote_addr;
   remote_addr.setPort(port);
   vpr::SocketStream connector_socket( vpr::InetAddr::AnyAddr, remote_addr );

      // run the test many times.
   for (int x = 0; x < num_of_times_to_test; ++x)
   {
      // open socket
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket::open() failed", connector_socket.open());

      // connect to the acceptor
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket::connect() failed", connector_socket.connect());

      // let acceptor accept it before closing
      vpr::System::msleep( 50 );

      // close the socket
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket::close() failed", connector_socket.close());

      // let the acceptor get a chance to start before connecting (sleep a while)
      vpr::System::usleep( 5000 );
   }
}

void SocketTest::testOpenCloseOpen_acceptor( void* data )
{
   boost::ignore_unused_variable_warning(data);

   int num_of_times_to_test = 3;
   vpr::Uint16 port = 6970;

   vpr::InetAddr local_addr;

   // make a new socket listening on port "port"
   local_addr.setPort(port);
   vpr::SocketStream acceptor_socket( local_addr, vpr::InetAddr::AnyAddr );

   // start/stop the acceptor many times...
   for (int x = 0; x < num_of_times_to_test; ++x)
   {
      //std::cout << "[acceptor open]" << std::flush;

      // open socket
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket::open() failed", acceptor_socket.open());

      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket::bind() failed", acceptor_socket.bind());

      // set the socket to listen
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket::listen() failed", acceptor_socket.listen());

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
         CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket::accept() failed", acceptor_socket.accept(child_socket));

         // close the child socket
         CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket::close() failed", child_socket.close());
      }

      // close the socket
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket::close() failed", acceptor_socket.close());

      //std::cout << "[acceptor close]\n" << std::flush;
   }
}

void SocketTest::testOpenCloseOpen ()
{
   //std::cout<<"]==================================================\n"<<std::flush;
   //std::cout<<" OpenCloseOpen Test: \n"<<std::flush;

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
   //connector_thread.join();
   //acceptor_thread.join();

}

void SocketTest::testSendRecv_connector( void* data )
{
  boost::ignore_unused_variable_warning(data);

   int num_of_times_to_test = 10;
   vpr::Uint16 port = 6940;
   //const int backlog = 5;
   vpr::InetAddr remote_addr;

   // make a new socket that will connect to port "port"
   remote_addr.setPort(port);
   vpr::SocketStream connector_socket( vpr::InetAddr::AnyAddr, remote_addr );

   // run the test many times.
   for (int x = 0; x < num_of_times_to_test; ++x)
   {
      // open socket
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket::open() failed", connector_socket.open());

      // connect to the acceptor
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket::connect() failed", connector_socket.connect());

      // find out how much data is coming...
      vpr::Uint32 size_of_data;
      vpr::Uint32 amount_read;
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("readn didn't read",
         connector_socket.readn(&size_of_data, sizeof(int), amount_read));
      CPPUNIT_ASSERT( amount_read == sizeof( int ) && "readn didn't read all" );

      // get that amount of data...
      std::string buffer;
      buffer.resize( size_of_data );
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("readn didn't read",
       connector_socket.readn(&buffer[0], size_of_data, amount_read));
      CPPUNIT_ASSERT( amount_read == size_of_data && "readn didn't read all" );

      //std::cout<<"Recieved buffer: "<<buffer<<"\n"<<std::flush;
      CPPUNIT_ASSERT( buffer == testSendRecv_buffer && "didn't recieve the right data" );

      // close the socket
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket::close() failed", connector_socket.close());

      // let the acceptor get a chance to start before connecting (sleep a while)
      vpr::System::usleep( 50000 );
   }
}

void SocketTest::testSendRecv_acceptor( void* data )
{
   boost::ignore_unused_variable_warning(data);

   int num_of_times_to_test = 10;
   vpr::Uint16 port = 6940;
   vpr::InetAddr local_addr;

   // make a new socket listening on port "port"
   local_addr.setPort(port);
   vpr::SocketStream acceptor_socket( local_addr, vpr::InetAddr::AnyAddr );

   // open socket
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket::open() failed", acceptor_socket.open());

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket::bind() failed", acceptor_socket.bind());

   // set the socket to listen
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket::listen() failed", acceptor_socket.listen());

   vpr::SocketStream child_socket;

   // start/stop the child socket many times...
   for (int xx = 0; xx < num_of_times_to_test; ++xx)
   {
      // wait for a connect (blocking)
      // when someone connects to the server, and we accept the connection,
      // spawn a child socket to deal with the connection
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket::accept() failed",
         acceptor_socket.accept(child_socket));

      // send the size of the data to be sent
      vpr::Uint32 amount_sent;
      std::string buffer = testSendRecv_buffer;
      vpr::Uint32 size = buffer.size();
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("write didn't send",
         child_socket.write( &size, sizeof( int ), amount_sent ));
      CPPUNIT_ASSERT( amount_sent == sizeof( int ) && "write didn't send all" );

      // send the data...
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("write didn't send",
         child_socket.write( &buffer[0], buffer.size(), amount_sent));
      CPPUNIT_ASSERT( amount_sent == buffer.size() && "write didn't send all" );

      //std::cout<<"Sent buffer: "<<buffer<<"\n"<<std::flush;
      //std::cout<<"+"<<std::flush;

      // close the child socket
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket::close() failed", child_socket.close());
   }

   // close the socket
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket::close() failed", acceptor_socket.close());
}

void SocketTest::testSendRecv()
{
   //std::cout<<"]==================================================\n"<<std::flush;
   //std::cout<<" SendRecv Test: \n"<<std::flush;

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
}

void SocketTest::testOpenClose()
{
   //std::cout<<"]==================================================\n"<<std::flush;
   //std::cout<<" OpenClose Test: \n"<<std::flush;

   vpr::Uint16 port=15432;
   vpr::InetAddr local_addr;
   vpr::SocketStream*   sock;

   local_addr.setAddress("localhost", port);
   sock = new vpr::SocketStream(local_addr, vpr::InetAddr::AnyAddr);
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket can not be opened!", sock->open());
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket can not be bound!", sock->bind());
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket can not be closed!", sock->close());

   delete sock;
}

void SocketTest::bindAgainFailTest()
{
   //std::cout<<"]==================================================\n"<<std::flush;
   //std::cout<<" multiple bind failure Test: \n"<<std::flush;
   vpr::InetAddr local_addr;
   vpr::Uint16 port = 6976;

   local_addr.setAddress("localhost", port);
   vpr::SocketStream sock( local_addr, vpr::InetAddr::AnyAddr );

   // make sure aditional calls to bind() fails...
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("open() failed", sock.open());

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("bind() failed", sock.bind());

   for (int x = 0; x < 2; ++x)
   {
      CPPUNIT_ASSERT_THROW_MESSAGE("Socket was able to bind again, this is bad.",
         sock.bind(), vpr::IOException);
   }

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("close() failed", sock.close());
}

void SocketTest::sameAddressOpenBindCloseTest()
{
   vpr::InetAddr local_addr;
#ifdef VPR_OS_Windows
   long rand_num(rand());
#else
   long rand_num(random());
#endif
   vpr::Uint16 port = (rand_num % 20000) + 20000;

   local_addr.setAddress("localhost", port);
   vpr::SocketStream sock( local_addr, vpr::InetAddr::AnyAddr );

   // same address, open-bind-close
   const int runs = 10;
   for (int xx = 0; xx < runs; ++xx)
   {
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("open failed", sock.open());
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("bind failed", sock.bind());
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("close failed", sock.close());
      vpr::System::msleep(50);                           // XXX: Kludge to give OS time to close the descriptor
   }
}

void SocketTest::sameAddressOpenBindDestructTest()
{
   vpr::InetAddr local_addr;
#ifdef VPR_OS_Windows
   long rand_num(rand());
#else
   long rand_num(random());
#endif
   vpr::Uint16 port = (rand_num % 20000) + 20000;

   local_addr.setAddress("localhost", port);
   vpr::SocketStream sock( local_addr, vpr::InetAddr::AnyAddr );

   // same address, open-bind-close
   const int runs = 10;
   for (int xx = 0; xx < runs; ++xx)
   {
      vpr::SocketStream* sock = new vpr::SocketStream( local_addr, vpr::InetAddr::AnyAddr );

      CPPUNIT_ASSERT_NO_THROW_MESSAGE("open failed", sock->open());
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("bind_failed", sock->bind());
      // XXX: We tecnically should never throw an exception from
      //      a destructor. (ie destructor should never call IO methods.
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("close in destructor failed", delete sock);
      vpr::System::msleep(50);                           // XXX: Kludge to give OS time to close the descriptor
   }
}

void SocketTest::differentAddressOpenBindCloseTest()
{
   vpr::InetAddr local_addr;
#ifdef VPR_OS_Windows
   long rand_num(rand());
#else
   long rand_num(random());
#endif
   vpr::Uint16 port = (rand_num % 20000) + 20000;

   // same address, open-bind-close
   const int runs = 10;
   for (int xx = 0; xx < runs; ++xx)
   {
      port++;
      CPPUNIT_ASSERT_NO_THROW(local_addr.setAddress("localhost", port));

      vpr::SocketStream sock( local_addr, vpr::InetAddr::AnyAddr );

      CPPUNIT_ASSERT_NO_THROW_MESSAGE("open failed", sock.open());
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("bind failed", sock.bind());
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("close failed", sock.close());
      //vpr::System::msleep(50);
   }
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

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Cannot open sock1", sock1->open());
   sock1->setReuseAddr(true);
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Cannot bind sock1", sock1->bind());

   // Try failed bind
   sock2->open();
   CPPUNIT_ASSERT_THROW_MESSAGE("Bind on same address should fail.", sock2->bind(), vpr::SocketException);
   sock2->close();

   // Close and then try reuse
   sock1->close();

   sock3->open();
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Bind on same (closed) address should not fail.", sock3->bind());

   sock3->close();
   delete sock1;
   delete sock2;
   delete sock3;
}

void SocketTest::reuseAddrTest_connector( void* data )
{
   boost::ignore_unused_variable_warning(data);

   vpr::Uint16 port = 6667;
   vpr::InetAddr remote_addr;

   // make a new socket that will connect to port "port"
   remote_addr.setPort(port);
   vpr::SocketStream connector_socket( vpr::InetAddr::AnyAddr, remote_addr );

   // open socket
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket::open() failed", connector_socket.open());

   // connect to the acceptor
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket::connect() failed", connector_socket.connect());

   // close the socket
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket::close() failed", connector_socket.close());
}

void SocketTest::reuseAddrTest_acceptor( void* data )
{
   boost::ignore_unused_variable_warning(data);

   vpr::InetAddr addr1;

   CPPUNIT_ASSERT_NO_THROW(addr1.setAddress( "localhost", 6667 ));

   vpr::SocketStream sock1( addr1, vpr::InetAddr::AnyAddr );
   vpr::SocketStream sock2( addr1, vpr::InetAddr::AnyAddr );

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("server Socket::open() failed", sock1.open());
   sock1.setReuseAddr( true );
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("server Socket::bind() failed", sock1.bind());
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("server Socket::listen() failed", sock1.listen());

   vpr::SocketStream child_socket;
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("child didn't spawn", sock1.accept(child_socket));

      // assume server crashes, so lets restart it.
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("open(): server restart", sock2.open());
      sock1.setReuseAddr( true ); // set the opt in-between for bind() to succeed
      //std::cout<<"::::::::: " << result<<"\n"<<std::flush;
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("bind(): server restart", sock2.bind());
      //std::cout<<"::::::::: " << result<<"\n"<<std::flush;

   // close the child socket
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("child Socket::close() failed", child_socket.close());

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("restarted server Socket::close() failed", sock2.close());
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("server Socket::close failed", sock1.close());
}

// XXX: Fails due to crashing thread (I think it is the connector thread)
void SocketTest::reuseAddrTest()
{
   //std::cout<<"]==================================================\n"<<std::flush;
   //std::cout<<" reuseAddr Test (cli/serv version): \n"<<std::flush;

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
}

void SocketTest::testBlocking_connector(void* arg)
{
   boost::ignore_unused_variable_warning(arg);

   vpr::Uint16 port = 7001;
   vpr::InetAddr remote_addr;

   remote_addr.setPort(port);

   //keep testing until acceptor says finish
   while (mFinishFlag == false)
   {
      char    buffer[40];
      char    buffer2[]="Oops!";
      vpr::Uint32 amount_read, amount_written;

      // make a new socket that will connect to port "port"
      vpr::SocketStream connector_socket(vpr::InetAddr::AnyAddr, remote_addr);

      // open socket
      connector_socket.setBlocking(true);
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket::open() failed when test blocking", connector_socket.open());

      // connect to the acceptor
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket::connect() failed when test blocking", connector_socket.connect());

      connector_socket.setNoDelay(true);

      //sleep until acceptor says begin
      while (mStartFlag == false)
      {
         vpr::System::usleep(50);
      }

      //set socket to blocking/nonblocking mode as required
      connector_socket.setBlocking(mBlockingFlag);

      //if the the flag of readn is true, use readn(), otherwise use read().
      // Try to read 20 bytes
      if (mReadnFlag == true)
      {
         connector_socket.readn(buffer, 20, amount_read);
      }
      else
      {
         connector_socket.read(buffer, 20, amount_read);
      }

      if (amount_read>0)
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

      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket::close() failed when test blocking",
         connector_socket.close());
   }

   // let the acceptor get a chance to start before connecting (sleep a while)
   vpr::System::usleep( 50000 );
}

void SocketTest::testBlocking_acceptor(void* arg)
{
   boost::ignore_unused_variable_warning(arg);

   int num_of_times_to_test = 20;
   vpr::Uint16 port = 7001;
   char  buffer[40];
   char  buffer1[]="Hello, there!";
   char  buffer2[]="Hello";
   vpr::Uint32 amount_read (0), amount_written;
   vpr::InetAddr local_addr;

   local_addr.setPort(port);

   // make a new socket listening on port "port"
   vpr::SocketStream acceptor_socket( local_addr, vpr::InetAddr::AnyAddr );
   acceptor_socket.setBlocking(true);

   // open socket
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket::open() failed in blocking test",
      acceptor_socket.open());

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket::bind() failed in blocking test", acceptor_socket.bind());

   mFinishFlag = false;

   // set the socket to listen
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket::listen() failed in blocking test", acceptor_socket.listen());


   vpr::SocketStream child_socket;

   // switch between blocking/nonblocking and read/readn many times...
   for (int xx = 0; xx < num_of_times_to_test; xx++)
   {
      // wait for a connect (blocking)
      // when someone connects to the server, and we accept the connection,
      // spawn a child socket to deal with the connection

      int yy=xx%4;

      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket::accept() failed in blocking test", acceptor_socket.accept(child_socket));

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

      child_socket.setBlocking(false);
      child_socket.read(buffer, 40, amount_read);
      child_socket.setBlocking(true);

      switch (yy)
      {
      case 0:
         CPPUNIT_ASSERT(amount_read == 14 && "Should return 14");
         break;
      case 1:
         CPPUNIT_ASSERT(amount_read == 6 && "Should return 6");
         break;
      case 2:
         CPPUNIT_ASSERT(amount_read == 0 && "Should return 20");
         break;
      case 3:
         CPPUNIT_ASSERT(amount_read==6 && "Should return 6");
      }

      mFlagProtectionMutex.acquire();
      mStartFlag=false;
      mFlagProtectionMutex.release();

      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket::close() failed in blocking test", child_socket.close());
   }

   // close the socket
   mFlagProtectionMutex.acquire();
      mFinishFlag = true;
   mFlagProtectionMutex.release();
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Socket::close() failed", acceptor_socket.close());
}

void SocketTest::testBlocking()
{
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
   boost::ignore_unused_variable_warning(arg);

   vpr::Uint16 port=15432;
   vpr::Uint16 num=0;

   std::vector<vpr::ThreadMemberFunctor<SocketTest>*> sServerFunctors(mNumSServer);
   std::vector<vpr::Thread*> sServerThreads(mNumSServer);
   vpr::InetAddr local_addr;

   local_addr.setPort(port);
   vpr::SocketStream*   sock;
   sock = new vpr::SocketStream(local_addr, vpr::InetAddr::AnyAddr);
   CPPUNIT_ASSERT_NO_THROW(sock->openServer());
   {
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
   sock->close();
   delete sock;
}

void SocketTest::clientFunc(void* arg)
{
   boost::ignore_unused_variable_warning(arg);

   vpr::SocketStream*   sock;
   vpr::InetAddr remote_addr;

   remote_addr.setAddress("localhost", 15432);
   sock = new vpr::SocketStream(vpr::InetAddr::AnyAddr, remote_addr);
   CPPUNIT_ASSERT_NO_THROW( sock->open() );
   {
      char buffer1[40];
      //char buffer2[] = "What's up?";
      CPPUNIT_ASSERT_NO_THROW( sock->connect() );
      {
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

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Could not assign address", remote_addr.setAddress("localhost", port));
   vpr::SocketStream client_sock(vpr::InetAddr::AnyAddr, remote_addr);
   char buffer[20];

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Client socket open failed", client_sock.open());
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Client could not connect", client_sock.connect());
   vpr::Uint32 bytes;
   client_sock.readn(buffer, sizeof(buffer), bytes);
   CPPUNIT_ASSERT((bytes == sizeof(buffer)) && "readn didn't read enough!");
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

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Server socket open failed", server_sock.open());
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Server socket bind failed", server_sock.bind());
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Server socket listen failed", server_sock.listen());

   // Start the client thread.
   vpr::ThreadMemberFunctor<SocketTest>* func =
      new vpr::ThreadMemberFunctor<SocketTest>(this, &SocketTest::testReadnClient,
                                               (void*) &server_port);
   vpr::Thread* client_thread = new vpr::Thread(func);
   CPPUNIT_ASSERT(client_thread->valid() && "Server could not create client thread");

   vpr::SocketStream client_sock;
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Server could not accept connection", server_sock.accept(client_sock));

   for ( unsigned int i = 0; i < 20; i += pkt_size )
   {
      memset((void*) buffer, 0, sizeof(buffer));
#ifdef VPR_OS_Windows
      sprintf(buffer, "%04d", i);
#else
      snprintf(buffer, sizeof(buffer), "%04d", i);
#endif

      client_sock.write(buffer, pkt_size, bytes);
      CPPUNIT_ASSERT(bytes != 0 && "Server could not write to client");
   }

   client_sock.close();
   delete client_thread;
   server_sock.close();
}

void SocketTest::testIsConnected ()
{
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
}

void SocketTest::testIsConnected_acceptor (void* arg)
{
   boost::ignore_unused_variable_warning(arg);

   vpr::ReturnStatus status;
   vpr::SocketAcceptor acceptor;
   vpr::SocketStream client_sock;
   vpr::InetAddr acceptor_addr;
   acceptor_addr.setPort(mAcceptorPort);

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to open acceptor", acceptor.open(acceptor_addr));

   // --- STATE: Acceptor Read --- //
   mCondVar.acquire();
   {
      mState = ACCEPTOR_READY;
      mCondVar.signal();
   }
   mCondVar.release();

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Accept failed", acceptor.accept(client_sock));
   CPPUNIT_ASSERT(client_sock.isOpen() && "Accepted socket should be open");
   CPPUNIT_ASSERT(client_sock.isConnected() && "Connector not connected");

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

   CPPUNIT_ASSERT_NO_THROW(client_sock.recv((void*)temp_buffer, 50, bytes_handled));

   CPPUNIT_ASSERT(bytes_handled == 0);

   // This should give an error too, but it doesn't seem to
   CPPUNIT_ASSERT_NO_THROW(client_sock.send((void*)temp_buffer, 50, bytes_handled));

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Could not close acceptor side of client socket", client_sock.close());

   CPPUNIT_ASSERT_NO_THROW(client_sock.send((void*)temp_buffer, 50, bytes_handled));

   CPPUNIT_ASSERT( !client_sock.isConnected() && "Connector not disconnected");

   /*
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Could not close acceptor side of client socket", client_sock.close());
   */

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Could not close acceptor", acceptor.close());
}

void SocketTest::testIsConnected_connector (void* arg)
{
   boost::ignore_unused_variable_warning(arg);

   vpr::ReturnStatus status;
   vpr::InetAddr remote_addr;
   vpr::SocketConnector connector;
   vpr::SocketStream con_sock;
   std::string data;

   remote_addr.setAddress("localhost", mAcceptorPort);

   mCondVar.acquire();
   {
      while ( mState != ACCEPTOR_READY )
      {
         mCondVar.wait();
      }
   }
   mCondVar.release();

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to open connector socket", con_sock.open());

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Connector can't connect",
      connector.connect(con_sock, remote_addr, vpr::Interval(5, vpr::Interval::Sec)));

   // --- WAIT FOR: Acceptor Tested --- //
   mCondVar.acquire();
   {
      while ( mState != ACCEPTOR_TESTED )
      {
         mCondVar.wait();
      }
   }
   mCondVar.release();

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Could not close connector side of client socket", con_sock.close());

   // --- STATE: Connector closed --- //
   mCondVar.acquire();
   {
      mState = CONNECTOR_CLOSED;
      mCondVar.signal();
   }
   mCondVar.release();

   CPPUNIT_ASSERT(! con_sock.isConnected() && "Connect didn't disconnect?");
}


} // End of vprTest namespace
