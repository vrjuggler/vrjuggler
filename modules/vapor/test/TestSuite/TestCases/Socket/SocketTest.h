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
   SocketTest( std::string name ) : TestCase (name)
   {
      mOpenServerSuccess=0;
      mNumSServer=2;
      mNumClient=2;
   }

   virtual ~SocketTest()
   {
   }

   virtual void setUp()
   {
   }

   virtual void tearDown()
   {
   }

   void openCloseTest()
   {  
      bool openSuccess=false;
      bool closeSuccess=false;
      bool bindSuccess=false;
      
      vpr::Uint16 port=5432;
      vpr::SocketStream*	sock;
      sock = new vpr::SocketStream(vpr::InetAddr("localhost",port), vpr::InetAddr::AnyAddr);	
      openSuccess=sock->open();
      if (openSuccess)
         bindSuccess=(sock->bind());
      closeSuccess=sock->close();
      assertTest( openSuccess && "Socket can not be opened!");
      assertTest( bindSuccess && "Socket can not be bound!");
      assertTest( closeSuccess && "Socket can not be closed!");
      delete sock;
   }
   
/*   void ReuseAddr()
   {
      sock = new vpr::SocketStream(vpr::InetAddr::AnyAddr, vpr::InetAddr::AnyAddr);
      sock->setReuse(true);
      openSuccess=sock->open();
//      
   }
*/   
   void testTcpConnection()
   {
      mServerCheck=0;
      //Creat a thread to do server listen:
      vpr::ThreadMemberFunctor<SocketTest>* serverFunctor;
      serverFunctor=new vpr::ThreadMemberFunctor<SocketTest>(this, &SocketTest::serverFunc);
      vpr::Thread* serverThread;
      serverThread=new vpr::Thread(serverFunctor);

      sleep(1);

      //Creat a bunch of client thread.
      std::vector<vpr::ThreadMemberFunctor<SocketTest>*> clientFunctors(mNumClient);
      std::vector<vpr::Thread*> clientThreads(mNumClient);
      for (int t=0; t<mNumClient; t++){
         clientFunctors[t] = new vpr::ThreadMemberFunctor<SocketTest>(this, &SocketTest::clientFunc);
         clientThreads[t] = new vpr::Thread(clientFunctors[t]);
      }

      sleep(1);

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
//      sock->setReuseAddr(true);
      if ( sock->openServer() ) {
         vpr::SocketStream* client_sock;
         thread_args_t* tArgs;

         sock->setReuseAddr(true);

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
