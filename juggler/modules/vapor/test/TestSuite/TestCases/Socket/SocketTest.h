#ifndef _SOCKET_TEST_H
#define _SOCKET_TEST_H

#include <string.h>
#include <iostream>

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

#include <IO/Socket/Socket.h>
#include <IO/Socket/SocketStream.h>
#include <IO/Socket/InetAddr.h>
#include <include/System.h>

#include <Threads/Thread.h>
#include <Threads/ThreadFunctor.h>
#include <Sync/Mutex.h>

#include <vector>

/*****************************************************************
   This program creates a master thread listening the socket. If 
   there is client's request coming, the master accepts the request
   and generates a slave thread. The slave thread deals with the 
   connection. It sends a string --" Hello, there!". The client 
   reads the string and sends what it received back. The slve server
   then compare the return string and the sending one. If they are 
   different, gives an error information.
*******************************************************************/

struct _thread_args {
	vpr::SocketStream* mSock;
};

typedef struct _thread_args thread_args_t;

class SocketTest : public TestCase
{
public:
   SocketTest( std::string name ) : TestCase (name), testSendRecv_buffer( "we're sending DATA!!!" )
   {
      mOpenServerSuccess=0;
      mNumSServer=2;
      mNumClient=2;
   }

   virtual ~SocketTest()
   {
   }

   
         
   // =========================================================================
   // open-close-open test
   // =========================================================================
   void testOpenCloseOpen_connector( void* data )
   {
      int num_of_times_to_test = 100;
      vpr::Uint16 port = 6970;
      const int backlog = 5;
      bool result = 0;
      
      // make a new socket that will connect to port "port"
      vpr::SocketStream	connector_socket( vpr::InetAddr::AnyAddr, vpr::InetAddr(port) );
      
      // run the test many times.
      for (int x = 0; x < num_of_times_to_test; ++x)
      {
         // open socket
         result = connector_socket.open();
         assertTest( result != false && "Socket::open() failed" );

         // connect to the acceptor
         result = connector_socket.connect();
         assertTest( result != false && "Socket::connect() failed" );

         // close the socket
         result = connector_socket.close();
         assertTest( result != false && "Socket::close() failed" );

         // let the acceptor get a chance to start before connecting (sleep a while)
         vpr::System::usleep( 50000 );
      }
   }
   void testOpenCloseOpen_acceptor( void* data )
   {
      int num_of_times_to_test = 10;
      vpr::Uint16 port = 6970;
      const int backlog = 5;
      bool result = 0;
      
      // make a new socket listening on port "port"
      vpr::SocketStream	acceptor_socket( vpr::InetAddr(port), vpr::InetAddr::AnyAddr );
      
      // start/stop the acceptor many times...
      for (int x = 0; x < num_of_times_to_test; ++x)
      {
         std::cout << "[acceptor open]" << std::flush;

         // open socket
         result = acceptor_socket.open();
         assertTest( result != false && "Socket::open() failed" );

         result = acceptor_socket.bind();
         assertTest( result != false && "Socket::bind() failed" );

         // set the socket to listen
         result = acceptor_socket.listen( backlog );
         assertTest( result != false && "Socket::listen() failed" );

         std::cout << "[accepting "<<num_of_times_to_test<<" connections]" << std::flush;

         // start/stop the child socket many times...
         for (int xx = 0; xx < num_of_times_to_test; ++xx)
         {
            //std::cout << x* num_of_times_to_test + xx << "+\n" << std::flush;
            std::cout << "+" << std::flush;

            // wait for a connect (blocking)
            // when someone connects to the server, and we accept the connection, 
            // spawn a child socket to deal with the connection
            vpr::SocketStream* child_socket = acceptor_socket.accept();
            assertTest( child_socket != NULL && "Socket::accept() failed" );

            // close the child socket
            result = child_socket->close();
            assertTest( result != false && "Socket::close() failed" );

            // clean up any memory i've made...
            delete child_socket;
            child_socket = NULL;
         }

         // close the socket
         result = acceptor_socket.close();
         assertTest( result != false && "Socket::close() failed" );

         std::cout << "[acceptor close]\n" << std::flush;
      }
   }
   void testOpenCloseOpen()
   {
      // spawn an acceptor thread
      vpr::ThreadMemberFunctor<SocketTest> acceptor_functor( this, &SocketTest::testOpenCloseOpen_acceptor );
      vpr::Thread acceptor_thread( &acceptor_functor );
      
      // let the acceptor get a chance to start before connecting (sleep a while)
      vpr::System::msleep( 500 );
      
      // spawn a connector thread
      vpr::ThreadMemberFunctor<SocketTest> connector_functor( this, &SocketTest::testOpenCloseOpen_connector );
      vpr::Thread connector_thread( &connector_functor );
      
      // wait for both threads to terminate, then continue
      vpr::System::sleep( 7 );

      std::cout << " done\n" << std::flush;

      //connector_thread.join();
      //acceptor_thread.join();
   }
   // =========================================================================
   
   
   // =========================================================================
   // send-recv test
   // =========================================================================
   const std::string testSendRecv_buffer;
   void testSendRecv_connector( void* data )
   {
      int num_of_times_to_test = 10;
      vpr::Uint16 port = 6940;
      const int backlog = 5;
      bool result = 0;
      
      // make a new socket that will connect to port "port"
      vpr::SocketStream	connector_socket( vpr::InetAddr::AnyAddr, vpr::InetAddr(port) );
      
      // run the test many times.
      for (int x = 0; x < num_of_times_to_test; ++x)
      {
         // open socket
         result = connector_socket.open();
         assertTest( result != false && "Socket::open() failed" );

         // connect to the acceptor
         result = connector_socket.connect();
         assertTest( result != false && "Socket::connect() failed" );

         
            // find out how much data is coming...
            int size_of_data;
            int amount_read;
            amount_read = connector_socket.readn( &size_of_data, sizeof( int ) );
            assertTest( amount_read == sizeof( int ) && "readn didn't read all" );
            
            // get that amount of data...
            std::string buffer;
            buffer.resize( size_of_data );
            amount_read = connector_socket.readn( &buffer[0], size_of_data );
            assertTest( amount_read == size_of_data && "readn didn't read all" );
            
            //std::cout<<"Recieved buffer: "<<buffer<<"\n"<<std::flush;
            assertTest( buffer == testSendRecv_buffer && "didn't recieve the right data" );
            
         // close the socket
         result = connector_socket.close();
         assertTest( result != false && "Socket::close() failed" );

         // let the acceptor get a chance to start before connecting (sleep a while)
         vpr::System::usleep( 50000 );
      }
   }
   void testSendRecv_acceptor( void* data )
   {
      int num_of_times_to_test = 10;
      vpr::Uint16 port = 6940;
      const int backlog = 5;
      bool result = 0;
      
      // make a new socket listening on port "port"
      vpr::SocketStream	acceptor_socket( vpr::InetAddr(port), vpr::InetAddr::AnyAddr );
      
      // open socket
      result = acceptor_socket.open();
      assertTest( result != false && "Socket::open() failed" );

      result = acceptor_socket.bind();
      assertTest( result != false && "Socket::bind() failed" );

      // set the socket to listen
      result = acceptor_socket.listen( backlog );
      assertTest( result != false && "Socket::listen() failed" );

      // start/stop the child socket many times...
      for (int xx = 0; xx < num_of_times_to_test; ++xx)
      {
         // wait for a connect (blocking)
         // when someone connects to the server, and we accept the connection, 
         // spawn a child socket to deal with the connection
         vpr::SocketStream* child_socket = acceptor_socket.accept();
         assertTest( child_socket != NULL && "Socket::accept() failed" );

            // send the size of the data to be sent
            int amount_sent;
            std::string buffer = testSendRecv_buffer;
            int size = buffer.size();
            amount_sent = child_socket->write( &size, sizeof( int ) );
            assertTest( amount_sent == sizeof( int ) && "write didn't send all" );
            
            // send the data...
            amount_sent = child_socket->write( &buffer[0], buffer.size() );
            assertTest( amount_sent == buffer.size() && "write didn't send all" );
            
            //std::cout<<"Sent buffer: "<<buffer<<"\n"<<std::flush;
            std::cout<<"+"<<std::flush;
         
         // close the child socket
         result = child_socket->close();
         assertTest( result != false && "Socket::close() failed" );

         // clean up any memory i've made...
         delete child_socket;
      }

      // close the socket
      result = acceptor_socket.close();
      assertTest( result != false && "Socket::close() failed" );
   }
   void testSendRecv()
   {
      // spawn an acceptor thread
      vpr::ThreadMemberFunctor<SocketTest> acceptor_functor( this, &SocketTest::testSendRecv_acceptor );
      vpr::Thread acceptor_thread( &acceptor_functor );
      
      // let the acceptor get a chance to start before connecting (sleep a while)
      vpr::System::msleep( 500 );
      
      // spawn a connector thread
      vpr::ThreadMemberFunctor<SocketTest> connector_functor( this, &SocketTest::testSendRecv_connector );
      vpr::Thread connector_thread( &connector_functor );
      
      // wait for both threads to terminate, then continue
      vpr::System::sleep( 7 );
      //connector_thread.join(); // join is broken.
      //acceptor_thread.join();
      std::cout << " done\n" << std::flush;
   }
   // =========================================================================
   
   
   // =========================================================================
   // open-close test
   // =========================================================================
   void openCloseTest()
   { 
      std::cout<<" Open/Close Test: "; 
      bool openSuccess( false );
      bool closeSuccess( false );
      bool bindSuccess( false );
      
      vpr::Uint16 port=5432;
      vpr::SocketStream*	sock;
      sock = new vpr::SocketStream(vpr::InetAddr("localhost",port), vpr::InetAddr::AnyAddr);	
      openSuccess=sock->open();
      if (openSuccess){
         std::cout<< " Open...";
         bindSuccess=(sock->bind());
      }
      closeSuccess=sock->close();
      std::cout<< " Close..."<<endl;
      assertTest( openSuccess && "Socket can not be opened!");
      assertTest( bindSuccess && "Socket can not be bound!");
      assertTest( closeSuccess && "Socket can not be closed!");
      delete sock;
   }
   // =========================================================================
   // bind again should fail test
   // =========================================================================
   void bindAgainFailTest()
   { 
      std::cout<<" bind again fails Test: \n"<<std::flush; 
      bool openSuccess( false );
      bool closeSuccess( false );
      bool bindSuccess( false );
      
      vpr::Uint16 port = 6976;
      vpr::SocketStream sock( vpr::InetAddr("localhost",port), vpr::InetAddr::AnyAddr );	
      
      // make sure aditional calls to bind() fails...
      openSuccess = sock.open();
      assertTest( openSuccess == true && "open() failed");
      
      bindSuccess = sock.bind();
      assertTest( bindSuccess == true && "bind() failed");
      
      for (int x = 0; x < 100; ++x)
      {
         bindSuccess = sock.bind();
         assertTest( bindSuccess == false && "Socket was able to bind again, this is bad.");
      }
      
      closeSuccess = sock.close();
      assertTest( closeSuccess == true && "close() failed");
   }
   // =========================================================================
   // same-address-open-bind-close test
   // =========================================================================
   void sameAddressOpenBindCloseTest()
   { 
      std::cout<<" same-address-open-bind-close Test: \n"<<std::flush; 
      bool openSuccess( false );
      bool closeSuccess( false );
      bool bindSuccess( false );
      
      vpr::Uint16 port = 6977;
      vpr::SocketStream sock( vpr::InetAddr("localhost",port), vpr::InetAddr::AnyAddr );	
      
      // same address, open-bind-close
      for (int xx = 0; xx < 100; ++xx)
      {
         std::cout<<xx<<"\r"<<std::flush;
         openSuccess = sock.open();
         assertTest( openSuccess == true && "open() failed");

         bindSuccess = sock.bind();
         assertTest( bindSuccess == true && "bind() failed");

         closeSuccess = sock.close();
         assertTest( closeSuccess == true && "close() failed");
      }
   }
   // =========================================================================
   // different-address-open-bind-close test
   // =========================================================================
   void differentAddressOpenBindCloseTest()
   { 
      std::cout<<" different-address-open-bind-close Test: \n"<<std::flush; 
      bool openSuccess( false );
      bool closeSuccess( false );
      bool bindSuccess( false );
      
      // same address, open-bind-close
      for (int xx = 0; xx < 100; ++xx)
      {
         vpr::Uint16 port = 5977 + xx;
         vpr::SocketStream sock( vpr::InetAddr("localhost", port), vpr::InetAddr::AnyAddr );	
      
         openSuccess = sock.open();
         assertTest( openSuccess == true && "open() failed");

         bindSuccess = sock.bind();
         assertTest( bindSuccess == true && "bind() failed");

         closeSuccess = sock.close();
         assertTest( closeSuccess == true && "close() failed");
      }
   }
   // =========================================================================
   // reuse address test
   // =========================================================================
   void reuseAddrTest()
   {
      std::cout<<" Reuse Address Test"<<endl;
      vpr::InetAddr addr1(13768);
      vpr::InetAddr addr2("129.186.232.58", 5438);
      vpr::SocketStream*	sock1;
      vpr::SocketStream*	sock2;
      vpr::SocketStream*	sock3;
      sock1 = new vpr::SocketStream(addr1, vpr::InetAddr::AnyAddr);
      sock2 = new vpr::SocketStream(addr1, vpr::InetAddr::AnyAddr);
      sock3 = new vpr::SocketStream(addr1, vpr::InetAddr::AnyAddr);
      if (sock1->open()){
         sock1->setReuseAddr(true);
         assertTest(sock1->bind());
      }
      else assert(false && "Cannot open sock1");
      if (sock2->open()){
         assertTest(sock2->bind());
      }
      else assert(false && "Cannot open sock2");
      if (sock3->open())
         assertTest(sock3->bind());
      sock1->close();
      sock2->close();
      sock3->close();
      delete sock1;
      delete sock2;
      delete sock3;
   }
   
   void testTcpConnection()
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

      //Stop the master server thread 
//      serverThread->kill();
      //	assertTest(mClientCounter==mNumClient && "Not all client are connect");
	   assertTest(mOpenServerSuccess ==-1 && "Open server failed");
      assertTest(mServerCheck==0 && "Not all connections are correct.");
      delete serverFunctor;
      delete serverThread;

      for (int t=0; t<mNumClient; t++){
         delete clientFunctors[t];
         delete clientThreads[t];
      }
   }

   //Function for master server thread
   void serverFunc(void* arg)
   {
      vpr::Uint16 port=5432;
      vpr::Uint16 num=0;

      std::vector<vpr::ThreadMemberFunctor<SocketTest>*> sServerFunctors(mNumSServer);
      std::vector<vpr::Thread*> sServerThreads(mNumSServer);

      vpr::SocketStream*	sock;
      sock = new vpr::SocketStream(vpr::InetAddr(port), vpr::InetAddr::AnyAddr);	
      if ( sock->openServer() ) {
         vpr::SocketStream* client_sock;
         thread_args_t* tArgs;

         while ( num<mNumSServer) {
            client_sock = sock->accept();
//            client_sock->setReuseAddr(true);
            tArgs = new thread_args_t;
            tArgs->mSock=client_sock;

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
   
   //function for creating client threads
   void clientFunc(void* arg)
   {
      vpr::SocketStream*	sock;

      sock = new vpr::SocketStream(vpr::InetAddr::AnyAddr, vpr::InetAddr("localhost", 5432));
      if ( sock->open() ) {
         char buffer1[40];
//         char buffer2[] = "What's up?";
         if ( sock->connect() ) {
            ssize_t bytes;
            bytes = sock->read(buffer1, 40);
//            sock->write(buffer2, sizeof(buffer2));
            sock->write(buffer1, bytes);
            mItemProtectionMutex.acquire();
            mClientCounter++;
            mItemProtectionMutex.release();
         }
      }
      sock->close();
      delete sock;
   }
   
   //function for creating slave server thread, the server sends message to client, gets the client's 
   //echo, and then compares the two strings.
   void sServerFunc(void* arg)
   {
      _thread_args* tArg=(_thread_args *) arg;
      char buffer1[] = "Hello there!";
      char buffer2[40];
      ssize_t bytes;

      vpr::SocketStream* ss_sock;
      ss_sock=tArg->mSock;
      //send a string to client
      ss_sock->write(buffer1, sizeof(buffer1));
      //receive a string from client
      bytes=ss_sock->read(buffer2,40);
      long compareString=strcmp(buffer1,buffer2);
      if (compareString!=0) {
         mServerCheck=1;
      }
      ss_sock->close();
      delete ss_sock;
      delete arg;
   }

   static Test* suite()
   {
      TestSuite *test_suite = new TestSuite ("SocketTest");
      test_suite->addTest( new TestCaller<SocketTest>("Open/CloseTest", &SocketTest::openCloseTest));
      
      test_suite->addTest( new TestCaller<SocketTest>("bind-Again Failure Test", &SocketTest::bindAgainFailTest));
      test_suite->addTest( new TestCaller<SocketTest>("same-Address-Open-Bind-Close Test", &SocketTest::sameAddressOpenBindCloseTest));
      test_suite->addTest( new TestCaller<SocketTest>("different-Address-Open-Bind-Close Test", &SocketTest::differentAddressOpenBindCloseTest));
      
      test_suite->addTest( new TestCaller<SocketTest>("ReuseAddrTest", &SocketTest::reuseAddrTest));
      test_suite->addTest( new TestCaller<SocketTest>("testOpenCloseOpen", &SocketTest::testOpenCloseOpen));
      test_suite->addTest( new TestCaller<SocketTest>("testSendRecv", &SocketTest::testSendRecv));
      test_suite->addTest( new TestCaller<SocketTest>("testTcpConnection", &SocketTest::testTcpConnection));
      return test_suite;
   }

protected:
   vpr::Mutex     mItemProtectionMutex;         // Protect an exclusive item
   long           mOpenServerSuccess;
   long           mNumSServer;
   long           mNumClient;
   long           mClientCounter;
   long           mServerCheck;
};

#endif          
