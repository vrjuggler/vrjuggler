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
#include <Utils/Status.h>

#include <System.h>

#include <Threads/Thread.h>
#include <Threads/ThreadFunctor.h>
#include <Sync/Mutex.h>
#include <Sync/Cond.h>

#include <vector>
#include <map>
#include <numeric>
#include <algorithm>

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
       //else
       //    std::cout << "." << std::flush;
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
       mNumRendevousPorts = 13;
       mNumIters = 50;
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
      Status ret_val;
      ssize_t bytes_written;

      // make lots of acceptors, set their addresses, and open them... saving each handle in a std::map
      for(i=0;i<mNumRendevousPorts;i++)
      {
         vpr::SocketAcceptor* new_acceptor = new vpr::SocketAcceptor();          // Create acceptor
         vpr::InetAddr local_acceptor_addr;
         local_acceptor_addr.setAddress("localhost", (mRendevousPort+i));        // Set local address
         new_acceptor->open(local_acceptor_addr);                                // Open acceptor
         vpr::IOSys::Handle handle = new_acceptor->getHandle();                  // Get the Handle to register
         acceptorTable[handle] = new_acceptor;
         selector.addHandle(handle);                          // Add to selector
         selector.setIn(handle, (vpr::Selector::VPR_READ | vpr::Selector::VPR_WRITE | vpr::Selector::VPR_EXCEPT));         
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
          Status ret = selector.select(num_events, 50000);

          threadAssertTest((ret.success()) && "Selection did not return successfully");
          threadAssertTest((num_events == 1) && "There was not only 1 event");

          
          // get an acceptor that has a connection request pending (ready_acceptor)
          int num_found(0);
          vpr::SocketAcceptor* ready_acceptor = NULL;
          for(j=0;j<selector.getNumHandles();j++)
          {
            // if selector's out flag is VPR_READ|VPR_EXCEPT
            if(selector.getOut(selector.getHandle(j)) & (vpr::Selector::VPR_READ | vpr::Selector::VPR_EXCEPT))
            {
               threadAssertTest((acceptorTable.find(selector.getHandle(j)) != acceptorTable.end()) && "Handle not found int acceptor table");
               ready_acceptor = acceptorTable[selector.getHandle(j)];               
               num_found += 1;
            }
          }

          threadAssertTest((num_found == 1) && "Wrong number of acceptors found");
          threadAssertTest(ready_acceptor != NULL && "no ready acceptor");

          // ACCEPT connection (generate a sock)
          SocketStream sock;
          ret_val = ready_acceptor->accept(sock);
          threadAssertTest((ret_val.success()), "Accepted socket is null");
          threadAssertTest((sock.isOpen()), "Accepted socket should be open");
           
          // use the sock to write (send)... then close it...
          ret_val = sock.write(mMessageValue, mMessageLen, bytes_written);      // Send a message           
          threadAssertTest((ret_val.success()), "Problem writing in acceptor");
          ret_val = sock.close();         // Close the socket
          threadAssertTest((ret_val.success()), "Problem closing accepted socket");          
       }

       // Delete acceptors
       for( std::map<IOSys::Handle, vpr::SocketAcceptor*>::iterator a=acceptorTable.begin(); a != acceptorTable.end(); a++)
       {
          delete (*a).second;
       }
   }
   void testAcceptorPoolSelection_connector( void* arg )
   {
      Status ret_val;
      vpr::InetAddr remote_addr;
      vpr::SocketConnector connector;           // Connect to acceptor
      ssize_t bytes_read;

      // WAIT for READY 
      mCondVar.acquire();
      {
         while(mState != ACCEPTOR_READY)
            mCondVar.wait();
      }
      mCondVar.release();
      
      // Connect randomly to the rendevous ports
      for(unsigned int i=0;i<mNumIters;i++)
      {         
         vpr::Uint16 port_num = mRendevousPort+(random() % mNumRendevousPorts);
         //std::cout << " p: " << port_num << std::flush;
         remote_addr.setAddress("localhost", port_num);
         vpr::SocketStream con_sock;
         std::string       data;
         ret_val = connector.connect(con_sock, remote_addr, 100);
         threadAssertTest((ret_val.success()), "Connector can't connect");
         
         ret_val = con_sock.read(data, mMessageLen, bytes_read);   // Recieve data
         threadAssertTest((bytes_read == mMessageLen), "Connector recieved message of wrong size" );
         threadAssertTest((ret_val.success()), "Failure reading data");
         
         con_sock.close();                                   // Close socket
      }
   }
   

   // Test sending data to a group of recievers that are pooled
   // This test is based upon the idea of having a pool of
   // connected sockets.  The connector side is sending, and the acceptor side is recieving.
   //
   // The connector side selects some subgroup of the sockets, marks them as selected, and then sends data on them
   // The acceptor side then selects on the sockets and hopefully it gets that same group back as the ones with pending data
   //      
   void testSendThenPoll()
   {
       testAssertReset();
       mRendevousPort = 47100 + (random() % 71);     // Get a partially random port       
       mNumRendevousPorts = 17;
       mNumIters = 40;
       mMessageValue = std::string("The Data");
       mMessageLen = mMessageValue.length();
       
       mState = NOT_READY;                        // Initialize

       // Spawn acceptor thread
       vpr::ThreadMemberFunctor<SelectorTest> 
           acceptor_functor( this, &SelectorTest::testSendThenPoll_acceptor );
       vpr::Thread acceptor_thread( &acceptor_functor);

       // Spawn connector thread
       vpr::ThreadMemberFunctor<SelectorTest> 
           connector_functor( this, &SelectorTest::testSendThenPoll_connector );
       vpr::Thread connector_thread( &connector_functor);

       // Wait for threads
       acceptor_thread.join();
       connector_thread.join();

       assertTest( mThreadAssertTest );
   }
   void testSendThenPoll_acceptor(void* arg)
   {
      unsigned i,j;
      std::map<vpr::IOSys::Handle, vpr::Uint16> handleTable;      // Table mapping Handles to the socket index
      vpr::Selector selector;
      vector<vpr::SocketStream> socks;
      vector<vpr::SocketAcceptor*> acceptors;
      //bool ret_val(false);

      // Bind to the accepting ports
      for(i=0;i<mNumRendevousPorts;i++)
      {
         vpr::SocketAcceptor* new_acceptor = new vpr::SocketAcceptor();          // Create acceptor
         acceptors.push_back(new_acceptor);
         vpr::InetAddr local_acceptor_addr;
         local_acceptor_addr.setAddress("localhost", (mRendevousPort+i));        // Set local address
         new_acceptor->open(local_acceptor_addr);                                // Open acceptor                  
      }

      // READY - Tell everyone that we are ready to accept
      mCondVar.acquire();
      {
         mState = ACCEPTOR_READY;
         mCondVar.signal();       // Tell any waiting threads
      }
      mCondVar.release();      

      // Accept all the connections
      for(i=0;i<mNumRendevousPorts;i++)
      {
         SocketStream sock;
         Status ret_val = acceptors[i]->accept(sock);           // Accept the connection
         threadAssertTest((ret_val.success()), "Error accepting a connection");
         socks.push_back(sock);
         vpr::IOSys::Handle handle = sock.getHandle();       // Get the Handle to register
         handleTable[handle] = i;                           // Save handle index
         selector.addHandle(handle);                          // Add to selector
         selector.setIn(handle, (vpr::Selector::VPR_READ));       // Set it for waiting for VPR_READ
      }

      threadAssertTest((mNumRendevousPorts == selector.getNumHandles()), "We didn't add all ports correctly to selector");

      // Use selector to find the sock(s) to read on
      // Then read message after checking to verify subgroup                     
       for(i=0;i<mNumIters;i++)
       {
          // WAIT for data to be sent
         mCondVar.acquire();
         {
            while(mState != DATA_SENT)
               mCondVar.wait();         
         }
         mCondVar.release();
          
          // Get the events
          vpr::Uint16 num_events;
          Status ret = selector.select(num_events, 50000);

          threadAssertTest((ret.success()) && "Selection did not return successfully");
          threadAssertTest((num_events == mSelectedPorts.size()) && "There was an incorrect number of events");

          // mSelectedPorts holds the index numbers of the sockets that data was sent on          
          // Read from each selected socket
          unsigned num_found(0);
          for(j=0;j<selector.getNumHandles();j++)
          {
             // If have data to read
             if(selector.getOut(selector.getHandle(j)) & (vpr::Selector::VPR_READ))      
            {
               threadAssertTest((handleTable.find(selector.getHandle(j)) != handleTable.end()) && "Handle not found int acceptor table");
               vpr::Uint16 sock_index = handleTable[selector.getHandle(j)];
               threadAssertTest((std::find(mSelectedPorts.begin(), mSelectedPorts.end(), sock_index) != mSelectedPorts.end()), "Found a port that wasn't supposed to have data");
               num_found += 1;

               std::string data;
               int bytes_read;
               socks[j].read(data, mMessageLen, bytes_read);
               threadAssertTest((bytes_read == mMessageLen), "Data recieved is of wrong length");
            }
          }
          threadAssertTest((num_found == mSelectedPorts.size()) && "Wrong number of readers found");
          
          // Tell senders that we are DONE_READING
          mCondVar.acquire();
          {
             mState = DONE_READING;
             mCondVar.signal();
          }
          mCondVar.release();
       }

       // Close Sockets and delete acceptors
       for(i=0;i<socks.size();i++)
       {
          socks[i].close();          
       }
       for(i=0;i<mNumRendevousPorts;i++)
       {
          delete acceptors[i];
       }
   }
   void testSendThenPoll_connector(void* arg)
   {
      unsigned i,j;
      Status ret_val;
      ssize_t bytes_written;
      vpr::InetAddr remote_addr;
      vpr::SocketConnector connector;           // Connect to acceptor
      std::vector<vpr::SocketStream> sockets(mNumRendevousPorts);       // Initialize with the number of sockets needed
      std::vector<vpr::Uint16> port_indicies(mNumRendevousPorts);     // This vector holds a list of valid indices to send data to
      std::iota(port_indicies.begin(), port_indicies.end(), 0);    // Fill with (0...n)

      // WAIT for READY 
      mCondVar.acquire();
      {
         while(mState != ACCEPTOR_READY)
            mCondVar.wait();
      }
      mCondVar.release();

      // Connect all the sockets
      for(i=0;i<mNumRendevousPorts;i++)
      {
         remote_addr.setAddress("localhost", (mRendevousPort+i));             // Set remote port
         ret_val = connector.connect(sockets[i], remote_addr, 100);           // Connect to the port
         threadAssertTest((ret_val.success()), "Connector can't connect");
      }

      // Send data in random groupings
      // - First selecte a random subgroup of ports to send to
      // - Send the data
      // - Wait for recievers to say that it was good
      for(i=0;i<mNumIters;i++)
      { 
         vpr::Uint16 num_ports = (random() % mNumRendevousPorts);          // Select some number of ports to send to
         if(num_ports == 0)
            num_ports = 1;                                                 // Make sure we always send some
         
         mSelectedPorts = std::vector<vpr::Uint16>(num_ports, 0);          // Replace old data with new of given size
         std::random_sample(port_indicies.begin(), port_indicies.end(),      // Select the random subset
                            mSelectedPorts.begin(), mSelectedPorts.end());
  
         // Send data to the random subset of ports
         for(j=0;j<mSelectedPorts.size();j++)
         {
            threadAssertTest((mSelectedPorts[j] < sockets.size()));           // Make sure we are in range
            ret_val = sockets[mSelectedPorts[j]].write(mMessageValue, mMessageLen, bytes_written);    // Write the data
            threadAssertTest((ret_val.success()), "Error writing data on socket");
         }
         
         // SIGNAL that we have SENT_DATA
         mCondVar.acquire();
         {
            mState = DATA_SENT;
            mCondVar.signal();
         }
         mCondVar.release();

         // WAIT for DONE_READING
         mCondVar.acquire();
         {
            while(mState != DONE_READING)
               mCondVar.wait();
         }
         mCondVar.release();
      }

      for(i=0;i<sockets.size();i++)
      {
         sockets[i].close();
         //delete sockets[i];
      }
   }


   static Test* suite()
   {
      TestSuite *test_suite = new TestSuite ("SelectorTest");

      test_suite->addTest( new TestCaller<SelectorTest>("testAcceptorPoolSelection", &SelectorTest::testAcceptorPoolSelection));
      test_suite->addTest( new TestCaller<SelectorTest>("testSendThenPoll", &SelectorTest::testSendThenPoll));
                                         
      return test_suite;
   }

private:   
   bool             mThreadAssertTest; // true for no error

protected:
    unsigned     mNumIters;

    vpr::Uint16     mRendevousPort;       // The port the acceptor will be waiting on
    vpr::Uint16     mNumRendevousPorts;   // The number of ports to use
    
    vpr::Uint16     mMessageLen;        // Length of message to send
    std::string     mMessageValue;      // The value of the message that is supposed to be sent (and recieved)

    enum State { ACCEPTOR_READY, NOT_READY, CONNECTOR_CLOSED, DATA_SENT, DONE_READING };
    State           mState;         // State variable
    vpr::Cond       mCondVar;       // Condition variable

    std::vector<vpr::Uint16>     mSelectedPorts;      // List of ports that were selected to send data
    
};

}       // namespace

#endif          
