#include <stdio.h>
#include <string.h>
#include <iostream>
#include <map>
#include <numeric>
#include <algorithm>

#if defined(__GNUC__) && \
    (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
#  include <ext/numeric>
#  include <ext/algorithm>
#endif

#include <vpr/IO/Socket/Socket.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/IO/Socket/InetAddr.h>
#include <vpr/IO/Socket/SocketAcceptor.h>
#include <vpr/IO/Socket/SocketConnector.h>
#include <vpr/IO/TimeoutException.h>
#include <vpr/IO/IOSys.h>
#include <vpr/IO/Selector.h>
#include <vpr/Util/ReturnStatus.h>

#include <vpr/System.h>

#include <vpr/Thread/Thread.h>
#include <vpr/Thread/ThreadFunctor.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/CondVar.h>

#include <TestCases/IO/SelectorTest.h>

#include <boost/concept_check.hpp>


namespace vprTest
{

CPPUNIT_TEST_SUITE_REGISTRATION( SelectorTest );

void SelectorTest::testAcceptorPoolSelection ()
{
#ifdef VPR_OS_Windows
	long rand_num(rand());
#else
	long rand_num(random());
#endif
    mRendevousPort = 30000 + (rand_num % 30000);     // Get a partially random port

    mNumRendevousPorts = 13;
    mNumIters = 50;
    mMessageValue = std::string("The Data");
    mMessageLen = mMessageValue.length();

    mState = NOT_READY;                        // Initialize

    // Spawn acceptor thread
    vpr::ThreadMemberFunctor<SelectorTest>* acceptor_functor =
        new vpr::ThreadMemberFunctor<SelectorTest>(this, &SelectorTest::testAcceptorPoolSelection_acceptor);
    vpr::Thread acceptor_thread(acceptor_functor);

    // Spawn connector thread
    vpr::ThreadMemberFunctor<SelectorTest>* connector_functor =
        new vpr::ThreadMemberFunctor<SelectorTest>(this, &SelectorTest::testAcceptorPoolSelection_connector);
    vpr::Thread connector_thread(connector_functor);

    // Wait for threads
    acceptor_thread.join();
    connector_thread.join();
}

void SelectorTest::testAcceptorPoolSelection_acceptor (void* arg)
{
   boost::ignore_unused_variable_warning(arg);

   unsigned i,j;
   std::map<vpr::IOSys::Handle, vpr::SocketAcceptor*> acceptorTable;
   vpr::Selector selector;
   //vpr::SocketStream* sock(NULL);
   ReturnStatus ret_val;
   vpr::Uint32 bytes_written;
   vpr::Uint16 num_events = 0;

   // make lots of acceptors, set their addresses, and open them... saving each handle in a std::map
   for(i=0;i<mNumRendevousPorts;i++)
   {
      vpr::SocketAcceptor* new_acceptor = new vpr::SocketAcceptor();          // Create acceptor
      vpr::InetAddr local_acceptor_addr;
      local_acceptor_addr.setAddress("localhost", (mRendevousPort+i));        // Set local address
      new_acceptor->open(local_acceptor_addr);                                // Open acceptor
      vpr::IOSys::Handle handle = new_acceptor->getHandle();                  // Get the Handle to register
      acceptorTable[handle] = new_acceptor;
      //selector.addHandle(handle);
      selector.addHandle(handle, (vpr::Selector::Read | vpr::Selector::Write | vpr::Selector::Except) );                      // Add to selector
      //selector.setIn(handle, (vpr::Selector::Read | vpr::Selector::Write | vpr::Selector::Except));
   }

   // First test selector TIMEOUT
   CPPUNIT_ASSERT_THROW(selector.select(num_events, vpr::Interval(5,vpr::Interval::Usec)), vpr::TimeoutException);
   //CPPUNIT_ASSERT(ret_val.timeout() && "Selection did not timeout like it should");
   CPPUNIT_ASSERT((0 == num_events) && "Num events should have been 0");

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
       num_events = 0;
       
       CPPUNIT_ASSERT_NO_THROW_MESSAGE("Selection did not return successfully", selector.select(num_events, vpr::Interval::NoTimeout ));
       CPPUNIT_ASSERT((num_events == 1) && "There was not only 1 event");

       // get an acceptor that has a connection request pending
       // (ready_acceptor)
       int num_found(0);
       vpr::SocketAcceptor* ready_acceptor = NULL;
       for(j=0;j<selector.getNumHandles();j++)
       {
         // if selector's out flag is VPR_READ|VPR_EXCEPT
         if(selector.getOut(selector.getHandle(j)) & (vpr::Selector::Read | vpr::Selector::Except))
         {
            CPPUNIT_ASSERT((acceptorTable.find(selector.getHandle(j)) != acceptorTable.end()) &&
                             "Handle not found int acceptor table");
            ready_acceptor = acceptorTable[selector.getHandle(j)];
            num_found += 1;
         }
       }

       CPPUNIT_ASSERT((num_found == 1) && "Wrong number of acceptors found");
       CPPUNIT_ASSERT(ready_acceptor != NULL && "no ready acceptor");

       // ACCEPT connection (generate a sock)
       SocketStream sock;
       CPPUNIT_ASSERT_NO_THROW_MESSAGE("Accepted socket is null", ready_acceptor->accept(sock));
       CPPUNIT_ASSERT((sock.isOpen()) && "Accepted socket should be open");

       // use the sock to write (send)... then close it...
       // Send a message
       CPPUNIT_ASSERT_NO_THROW_MESSAGE("Problem writing in acceptor", sock.write(mMessageValue, mMessageLen, bytes_written));
       // Close the socket
       CPPUNIT_ASSERT_NO_THROW_MESSAGE("Problem closing accepted socket", sock.close());
    }

    // Delete acceptors (which in turn closes's and deletes their sockets)
    for( std::map<IOSys::Handle, vpr::SocketAcceptor*>::iterator a=acceptorTable.begin(); a != acceptorTable.end(); a++)
    {
       delete (*a).second;
    }
}

void SelectorTest::testAcceptorPoolSelection_connector( void* arg )
{
   boost::ignore_unused_variable_warning(arg);

   ReturnStatus ret_val;
   vpr::InetAddr remote_addr;
   vpr::SocketConnector connector;           // Connect to acceptor
   vpr::Uint32 bytes_read;

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
#ifdef VPR_OS_Windows
      long rand_num(rand());
#else
      long rand_num(random());
#endif
      vpr::Uint16 port_num = mRendevousPort+(rand_num % mNumRendevousPorts);
      //std::cout << " p: " << port_num << std::flush;
      remote_addr.setAddress("localhost", port_num);
      vpr::SocketStream con_sock;
      std::string       data;
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Connector can't connect", connector.connect(con_sock, remote_addr, vpr::Interval(5, vpr::Interval::Sec) ));

      // Recieve data
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failure reading data", con_sock.read(data, mMessageLen, bytes_read));
      CPPUNIT_ASSERT((bytes_read == mMessageLen) && "Connector recieved message of wrong size" );

      con_sock.close();                                   // Close socket
   }
}

void SelectorTest::testSendThenPoll ()
{
#ifdef VPR_OS_Windows
	long rand_num(rand());
#else
	long rand_num(random());
#endif
    mRendevousPort = 30000 + (rand_num % 30000);     // Get a partially random port
    mNumRendevousPorts = 37;
    mNumIters = 10;
    mMessageValue = std::string("The Data");
    mMessageLen = mMessageValue.length();

    mState = NOT_READY;                        // Initialize

    // Spawn acceptor thread
    vpr::ThreadMemberFunctor<SelectorTest>* acceptor_functor =
        new vpr::ThreadMemberFunctor<SelectorTest>(this, &SelectorTest::testSendThenPoll_acceptor);
    vpr::Thread acceptor_thread(acceptor_functor);

    // Spawn connector thread
    vpr::ThreadMemberFunctor<SelectorTest>* connector_functor =
        new vpr::ThreadMemberFunctor<SelectorTest>(this, &SelectorTest::testSendThenPoll_connector);
    vpr::Thread connector_thread(connector_functor);

    // Wait for threads
    acceptor_thread.join();
    connector_thread.join();
}

void SelectorTest::testSendThenPoll_acceptor (void* arg)
{
   boost::ignore_unused_variable_warning(arg);

   unsigned i;    // j;
   std::map<vpr::IOSys::Handle, vpr::Uint16> handleTable;      // Table mapping Handles to the socket index
   vpr::Selector selector;
   std::vector<vpr::SocketStream>     socks;                        // The acceptor side sockets
   std::vector<vpr::SocketAcceptor*>  acceptors;
   vpr::ReturnStatus ret_val;
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
      // Accept the connection
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Error accepting a connection", acceptors[i]->accept(sock));
      socks.push_back(sock);                          // Pushes a copy of the socket onto the socket list
      vpr::IOSys::Handle handle = sock.getHandle();         // Get the Handle to register
      handleTable[handle] = i;                              // Save handle index
      selector.addHandle(handle);                           // Add to selector
      selector.setIn(handle, (vpr::Selector::Read));        // Set it for waiting for VPR_READ
   }

   CPPUNIT_ASSERT((mNumRendevousPorts == selector.getNumHandles()) && "We didn't add all ports correctly to selector");

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

      //std::cout << "iter: " << i << std::endl;

      //std::cout << "num expected events: " << mSelectedPorts.size() << std::endl;
      //vpr::System::msleep(50);

       // Get the events
       vpr::Uint16 num_events;
       
       CPPUNIT_ASSERT_NO_THROW_MESSAGE("Selection did not return successfully", selector.select(num_events, vpr::Interval::NoTimeout));

       //vpr::System::msleep(50);
       CPPUNIT_ASSERT((num_events == mSelectedPorts.size()) &&
                        "There was an incorrect number of events");
       if(num_events != mSelectedPorts.size())
       {
          std::cout << "Expected [ " << mSelectedPorts.size() << " ] events. Have [ " << num_events << " ] instead." << std::endl;
       }

       // mSelectedPorts holds the index numbers of the sockets that data was sent on
       // Read from each selected socket
       //std::cout << "Reading: ";

       unsigned num_found(0);
       for(unsigned s_idx=0;s_idx<selector.getNumHandles();s_idx++)
       {
          // If have data to read
          if(selector.getOut(selector.getHandle(s_idx)) & (vpr::Selector::Read))
          {
            CPPUNIT_ASSERT((handleTable.find(selector.getHandle(s_idx)) != handleTable.end()) &&
                             "Handle not found int acceptor table");
            vpr::Uint16 sock_index = handleTable[selector.getHandle(s_idx)];

            //std::cout << sock_index << ", ";

            CPPUNIT_ASSERT(sock_index == s_idx && "Indices out of sync");
            CPPUNIT_ASSERT((std::find(mSelectedPorts.begin(), mSelectedPorts.end(), sock_index) != mSelectedPorts.end()) &&
                             "Found a port that wasn't supposed to have data");
            num_found += 1;

            std::string data;
            vpr::Uint32 bytes_read;
            
            CPPUNIT_ASSERT_NO_THROW_MESSAGE("Problems reading data", socks[s_idx].read(data, mMessageLen, bytes_read)); 
            CPPUNIT_ASSERT((bytes_read == mMessageLen) && "Data recieved is of wrong length");
         }
       }
       //std::cout << std::endl;

       CPPUNIT_ASSERT((num_found == mSelectedPorts.size()) &&
                        "Wrong number of readers found");

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

#ifdef VPR_OS_Windows
// SGI STL extension adapted from GCC's ext/algorithm.
template<typename _InputIter, typename _RandomAccessIter, typename _Distance>
_RandomAccessIter
__random_sample(_InputIter __first, _InputIter __last,
		_RandomAccessIter __out,
		const _Distance __n)
{
   _Distance __m = 0;
   _Distance __t = __n;
   for ( ; __first != __last && __m < __n; ++__m, ++__first) 
   {
      __out[__m] = *__first;
   }

   while (__first != __last)
   {
      ++__t;
      _Distance __M = rand() % __t;
      if (__M < __n)
      {
         __out[__M] = *__first;
      }
      ++__first;
   }

   return __out + __m;
}

template<typename _InputIter, typename _RandomAccessIter>
_RandomAccessIter
random_sample(_InputIter __first, _InputIter __last,
              _RandomAccessIter __out_first, _RandomAccessIter __out_last) 
{
   return __random_sample(__first, __last, __out_first,
                          __out_last - __out_first);
}
#endif

void SelectorTest::testSendThenPoll_connector (void* arg)
{
   boost::ignore_unused_variable_warning(arg);

   unsigned i,j;
   ReturnStatus ret_val;
   vpr::Uint32 bytes_written;
   vpr::InetAddr remote_addr;
   vpr::SocketConnector connector;           // Connect to acceptor
   std::vector<vpr::SocketStream> sockets;                        // The sockets that we are using.
   std::vector<vpr::Uint16> port_indicies(mNumRendevousPorts);     // This vector holds a list of valid indices to send data to
#ifdef VPR_OS_Windows
   std::vector<vpr::Uint16>::iterator k;
   vpr::Uint16 val;

   // Fill with (0..n)
   for ( k = port_indicies.begin(), val = 0;
         k != port_indicies.end();
         ++k, ++val )
   {
      *k = val;
   }
#else
   std::iota(port_indicies.begin(), port_indicies.end(), 0);    // Fill with (0...n)
#endif

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
      vpr::SocketStream cur_sock;                                          // The current socket to connect
      // Connect to the port
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Connector can't connect", connector.connect(cur_sock, remote_addr, vpr::Interval::NoTimeout ));
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to set nodelay on socket", cur_sock.setNoDelay(true));
      sockets.push_back(cur_sock);                                         // Copy the connected socket handle into list
   }

   // Send data in random groupings
   // - First selecte a random subgroup of ports to send to
   // - Send the data
   // - Wait for recievers to say that it was good
   for(i=0;i<mNumIters;i++)
   {
#ifdef VPR_OS_Windows
      long rand_num(rand());
#else
      long rand_num(random());
#endif
      vpr::Uint16 num_ports = (rand_num % mNumRendevousPorts);          // Select some number of ports to send to
      if(num_ports == 0)
         num_ports = 1;                                                 // Make sure we always send some

      mSelectedPorts = std::vector<vpr::Uint16>(num_ports, 0);          // Replace old data with new of given size

#ifdef VPR_OS_Windows
      random_sample(port_indicies.begin(), port_indicies.end(),      // Select the random subset
                    mSelectedPorts.begin(), mSelectedPorts.end());
#else
      std::random_sample(port_indicies.begin(), port_indicies.end(),      // Select the random subset
                         mSelectedPorts.begin(), mSelectedPorts.end());
#endif

      //std::cout << "writing on: ";

      // Send data to the random subset of ports
      for(j=0;j<mSelectedPorts.size();j++)
      {
         //std::cout << mSelectedPorts[j] << ", ";
         CPPUNIT_ASSERT((mSelectedPorts[j] < sockets.size()));           // Make sure we are in range
         // Write the data
         CPPUNIT_ASSERT_NO_THROW_MESSAGE("Error writing data on socket", sockets[mSelectedPorts[j]].write(mMessageValue, mMessageLen, bytes_written));
         CPPUNIT_ASSERT((mMessageLen == bytes_written) && "Wrong num bytes written");
      }

      //std::cout << std::endl;

      // Give it time to send the data
      vpr::System::usleep(10000);
      vpr::Thread::yield();         // Give up the CPU

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

} // End of vprTest namespace
