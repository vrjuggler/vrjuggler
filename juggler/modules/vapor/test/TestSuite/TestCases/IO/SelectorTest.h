#ifndef _SELECTOR_TEST_H
#define _SELECTOR_TEST_H

#include <stdio.h>
#include <string.h>
#include <iostream>

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

#include <vprConfig.h>

#include <IO/Socket/Socket.h>
#include <IO/Socket/SocketStream.h>
#include <IO/Socket/InetAddr.h>
#include <IO/Socket/SocketAcceptor.h>
#include <IO/Socket/SocketConnector.h>
#include <IO/IOSys.h>
#include <IO/Selector.h>

#include <include/System.h>

#include <Threads/Thread.h>
#include <Threads/ThreadFunctor.h>
#include <Sync/Mutex.h>
#include <Sync/Cond.h>

#include <vector>
#include <map>

using namespace vpr;

namespace vprTest
{

class SelectorTest : public TestCase
{
public:
   SelectorTest( std::string name )
   : mThreadAssertTest(false), TestCase (name)
   {;}

   virtual ~SelectorTest()
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
   

   // Test an acceptor pool
   // This test is based upon the idea of having a pool of acceptors that
   // Another process keeps connecting too.  A selector is used to determine
   // which acceptor is actually being connected to so that only a single
   // accept is needed (that never should really block)
   void testAcceptorPoolSelection()
   {
       testAssertReset();
       mRendevousPort = 47000 + (random() % 71);     // Get a partially random port       
       mNumRendevousPorts = 10;
       mNumIters = 30;
       mMessageValue = std::string("The Data");
       mMessageLen = mMessageValue.length();
       
       mState = NOT_READY;                        // Initialize

       // Spawn acceptor thread
       vpr::ThreadMemberFunctor<SelectorTest> 
           acceptor_functor( this, &SelectorTest::testAcceptorPoolSelection_acceptor );
       vpr::Thread acceptor_thread( &acceptor_functor);

       // Spawn connector thread
       vpr::ThreadMemberFunctor<SelectorTest> 
           connector_functor( this, &SelectorTest::testAcceptorPoolSelection_connector );
       vpr::Thread connector_thread( &connector_functor);

       // Wait for threads
       acceptor_thread.join();
       connector_thread.join();

       assertTest( mThreadAssertTest );
   }
   void testAcceptorPoolSelection_acceptor(void* arg)
   {
      unsigned i,j;
      std::map<vpr::IOSys::Handle, vpr::SocketAcceptor*> acceptorTable;
      vpr::Selector selector;
      vpr::SocketStream* sock(NULL);
      bool ret_val(false);

      for(i=0;i<mNumRendevousPorts;i++)
      {
         vpr::SocketAcceptor* new_acceptor = new vpr::SocketAcceptor();          // Create acceptor
         vpr::InetAddr local_acceptor_addr;
         local_acceptor_addr.setAddress("localhost", (mRendevousPort+i));        // Set local address
         new_acceptor->open(local_acceptor_addr);                                // Open acceptor
         vpr::IOSys::Handle handle = new_acceptor->getHandle();                  // Get the Handle to register
         acceptorTable[handle] = new_acceptor;
         selector.addHandle(handle);                          // Add to selector
         selector.setIn(handle, (vpr::Selector::READ | vpr::Selector::WRITE | vpr::Selector::EXCEPT));         
      }
       
      // READY - Tell everyone that we are ready to accept
      mCondVar.acquire();
      {
         mState = ACCEPTOR_READY;
         mCondVar.signal();       // Tell any waiting threads
      }
      mCondVar.release();

      // Use selector to find the acceptor to accept on
      // Then accept and send a message back                     
       for(i=0;i<mNumIters;i++)
       {
          vpr::Uint16 num_events;
          bool ret = selector.select(num_events, 50000);

          threadAssertTest((true == ret) && "Selection did not return successfully");
          threadAssertTest((num_events == 1) && "There was not only 1 event");

          int num_found(0);
          vpr::SocketAcceptor* ready_acceptor;

          for(j=0;j<selector.getNumHandles();j++)
          {
            if(selector.getOut(selector.getHandle(j)) & (vpr::Selector::READ | vpr::Selector::EXCEPT))
            {
               threadAssertTest((acceptorTable.find(selector.getHandle(j)) != acceptorTable.end()) && "Handle not found int acceptor table");
               ready_acceptor = acceptorTable[selector.getHandle(j)];               
               num_found += 1;
            }
          }

          threadAssertTest((num_found == 1) && "Wrong number of acceptors found");

          // ACCEPT connection 
          sock = ready_acceptor->accept();
          threadAssertTest((sock != NULL), "Accepted socket is null");
          threadAssertTest((sock->isOpen()), "Accepted socket should be open");
           
          ret_val = sock->write(mMessageValue, mMessageLen);      // Send a message           
          threadAssertTest((ret_val == true), "Problem writing in acceptor");
          ret_val = sock->close();                                // Close the socket
          threadAssertTest((ret_val == true), "Problem closing accepted socket");
          delete sock;
          sock = NULL;
       }

       // Delete acceptors
       for( std::map<IOSys::Handle, vpr::SocketAcceptor*>::iterator a=acceptorTable.begin(); a != acceptorTable.end(); a++)
       {
          delete (*a).second;
       }
   }
   void testAcceptorPoolSelection_connector(void* arg)
   {
      bool ret_val(true);
      vpr::InetAddr remote_addr;
      vpr::SocketConnector connector;           // Connect to acceptor

      // WAIT for READY 
      mCondVar.acquire();
      {
         while(mState != ACCEPTOR_READY)
            mCondVar.wait();
      }
      mCondVar.release();
      
      // Connect randomly to the rendevous ports
      for(int i=0;i<mNumIters;i++)
      {         
         PRUint16 port_num = mRendevousPort+(i % mNumRendevousPorts);
         remote_addr.setAddress("localhost", port_num);
         vpr::SocketStream con_sock;
         std::string       data;
         ret_val = connector.connect(con_sock, remote_addr, 100);
         threadAssertTest((ret_val == true), "Connector can't connect");
         
         vpr::Uint16 size = con_sock.read(data, mMessageLen);   // Recieve data
         threadAssertTest((size == mMessageLen), "Connector recieved message of wrong size" );
         
         con_sock.close();                                   // Close socket
      }
   }
   
   static Test* suite()
   {
      TestSuite *test_suite = new TestSuite ("SelectorTest");

      test_suite->addTest( new TestCaller<SelectorTest>("testAcceptorPoolSelection", &SelectorTest::testAcceptorPoolSelection));
      
      return test_suite;
   }

private:   
   bool             mThreadAssertTest; // true for no error

protected:
    int     mNumIters;

    vpr::Uint16     mRendevousPort;       // The port the acceptor will be waiting on
    vpr::Uint16     mNumRendevousPorts;   // The number of ports to use
    
    vpr::Uint16     mMessageLen;        // Length of message to send
    std::string     mMessageValue;      // The value of the message that is supposed to be sent (and recieved)

    enum State { ACCEPTOR_READY, NOT_READY, CONNECTOR_CLOSED };
    State           mState;         // State variable
    vpr::Cond       mCondVar;       // Condition variable
    
};

}       // namespace

#endif          
