#ifndef _SOCKET_BANDWIDTH_IO_STATS_TEST_H
#define _SOCKET_BANDWIDTH_IO_STATS_TEST_H

#include <stdio.h>
#include <string.h>
#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/ThreadTestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <vpr/IO/Socket/Socket.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/IO/Socket/InetAddr.h>
#include <vpr/IO/Socket/SocketAcceptor.h>
#include <vpr/IO/Socket/SocketConnector.h>

#include <vpr/IO/Stats/BandwidthIOStatsStrategy.h>

#include <vpr/Util/Interval.h>

#include <vpr/System.h>

#include <vpr/Thread/Thread.h>
#include <vpr/Thread/ThreadFunctor.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/CondVar.h>

#include <vector>
#include <typeinfo>


namespace vprTest
{

class SocketBandwidthIOStatsTest : public CppUnit::ThreadTestCase
{
public:
   SocketBandwidthIOStatsTest()
   : CppUnit::ThreadTestCase ()
   {;}

   SocketBandwidthIOStatsTest(std::string name)
   : CppUnit::ThreadTestCase (name)
   {;}

   virtual ~SocketBandwidthIOStatsTest()
   {;}

   virtual void setUp()
   {;}

   virtual void tearDown()
   {;}


   // Based on: SocketConnectorAcceptor::Test Acceptor connector
   // This tests spawns off a thread that starts up an acceptor.
   // Then the main thread starts using a connector to connect to the connector
   // After connection the acceptor sends a message, the connector recieves it and closes the socket
   void testBandwidth()
   {
       threadAssertReset();
       mRendevousPort = 47000 + (random() % 71);     // Get a partially random port
       mNumItersA = 1;
       mNumItersB = 5000;
       mMessageValue = std::vector<vpr::Uint8>(1000, 21);      // 10000 bytes of data
       mMessageLen = mMessageValue.size();

       mState = NOT_READY;                        // Initialize

       // Spawn acceptor thread
       vpr::ThreadMemberFunctor<SocketBandwidthIOStatsTest>* acceptor_functor =
               new vpr::ThreadMemberFunctor<SocketBandwidthIOStatsTest>( this, &SocketBandwidthIOStatsTest::testBandwidth_acceptor );
       vpr::Thread acceptor_thread( acceptor_functor);

       // Spawn connector thread
       vpr::ThreadMemberFunctor<SocketBandwidthIOStatsTest>* connector_functor =
               new vpr::ThreadMemberFunctor<SocketBandwidthIOStatsTest>( this, &SocketBandwidthIOStatsTest::testBandwidth_connector );
       vpr::Thread connector_thread( connector_functor);

       CPPUNIT_ASSERT( acceptor_thread.valid() && "Invalid acceptor thread");
       CPPUNIT_ASSERT( connector_thread.valid() && "Invalid connector thread");

       if(!acceptor_thread.valid())
          std::cerr << "Invalid acceptor thread\n";
       if(!connector_thread.valid())
          std::cerr << "Invalid connector_thread\n";

       // Wait for threads
       acceptor_thread.join();
       connector_thread.join();

       checkThreadAssertions();
   }
   void testBandwidth_acceptor(void* arg)
   {
       vpr::InetAddr local_acceptor_addr;
       local_acceptor_addr.setAddress("localhost", mRendevousPort);
       vpr::SocketAcceptor acceptor;
       vpr::SocketStream* sock(NULL);
       vpr::ReturnStatus ret_val;
       vpr::Uint32 bytes_written;

       // Open the acceptor
       ret_val = acceptor.open(local_acceptor_addr);
       assertTestThread((ret_val.success()) && "Acceptor did not open correctly");

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
          ret_val = acceptor.accept(*sock, vpr::Interval::NoTimeout );
           assertTestThread((ret_val.success()) && "Accepting socket failed");

           assertTestThread((sock->isOpen()) && "Accepted socket should be open");
           //assertTestThread((sock->isConnected()), "Accepted socket should be connected");
           for(int j=0;j<mNumItersB;j++)
           {
             ret_val = sock->write(mMessageValue, mMessageLen, bytes_written);      // Send a message
             assertTestThread((ret_val.success()) && "Problem writing in acceptor");
             assertTestThread((bytes_written == mMessageLen) && "Didn't send entire messag");
           }

           // WAIT for close
           mCondVar.acquire();
           while(mState != CONNECTOR_CLOSED)
              mCondVar.wait();
           mCondVar.release();

           vpr::BaseIOStatsStrategy<>* stats = sock->getIOStatStrategy();
           BandwidthIOStatsStrategy* bw_interface = dynamic_cast<BandwidthIOStatsStrategy*>(stats );

           if(bw_interface != NULL)
           {
              // Dump out write stats
              std::cout << "Socket Write bandwidth stats ---\n";
              std::cout << "stats type: " << typeid(stats).name() << std::endl;
              std::cout << "      sent bytes: " << bw_interface->writeStats().getTotal() << std::endl
                        << "         av send: " << bw_interface->writeStats().getMean()/1024.0f << " k/s" << std::endl
                        << "        STA send: " << bw_interface->writeStats().getSTA()/1024.0f << " k/s" << std::endl
                        << "       Inst send: " << bw_interface->writeStats().getInstAverage()/1024.0f << " k/s" << std::endl
                        << "    Max STA send: " << bw_interface->writeStats().getMaxSTA()/1024.0f << " k/s" << std::endl
                        
                        << "      read bytes: " << bw_interface->readStats().getTotal() << std::endl
                        << "         av read: " << bw_interface->readStats().getMean()/1024.0f << " k/s" << std::endl
                        << "        STA read: " << bw_interface->readStats().getSTA()/1024.0f << " k/s" << std::endl
                        << "       Inst read: " << bw_interface->readStats().getInstAverage()/1024.0f << " k/s" << std::endl
                        << "    Max STA read: " << bw_interface->readStats().getMaxSTA()/1024.0f << " k/s" << std::endl
                        << std::endl;
           }
           else
           {
              std::cout << "SocketBWTest: Don't have BW Stats on stats. type is: " << typeid(stats).name() << std::endl;
           }

           ret_val = sock->close();                                // Close the socket
           assertTestThread((ret_val.success()) && "Problem closing accepted socket");
       }
   }
   void testBandwidth_connector(void* arg)
   {
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
         std::string          data;
         ret_val = connector.connect(con_sock, remote_addr, vpr::Interval::NoTimeout );
         assertTestThread((ret_val.success()) && "Connector can't connect");

         for(int j=0;j<mNumItersB;j++)
         {
            ret_val = con_sock.readn(data, mMessageLen, bytes_read);   // Recieve data
            assertTestThread((ret_val.success()) && "Read failed");
            assertTestThread((bytes_read == mMessageLen) && "Connector recieved message of wrong size" );
         }

         vpr::BaseIOStatsStrategy<>* stats = con_sock.getIOStatStrategy();
          BandwidthIOStatsStrategy* bw_interface = dynamic_cast<BandwidthIOStatsStrategy*>(stats );

           if(bw_interface != NULL)
           {
               // Dump out write stats
           std::cout << "Socket Read bandwidth stats ---\n";
           std::cout << "stats type: " << typeid(stats).name() << std::endl;
           std::cout << "      sent bytes: " << bw_interface->writeStats().getTotal() << std::endl
                        << "         av send: " << bw_interface->writeStats().getMean()/1024.0f << " k/s" << std::endl
                        << "        STA send: " << bw_interface->writeStats().getSTA()/1024.0f << " k/s" << std::endl
                        << "       Inst send: " << bw_interface->writeStats().getInstAverage()/1024.0f << " k/s" << std::endl
                        << "    Max STA send: " << bw_interface->writeStats().getMaxSTA()/1024.0f << " k/s" << std::endl
                        
                        << "      read bytes: " << bw_interface->readStats().getTotal() << std::endl
                        << "         av read: " << bw_interface->readStats().getMean()/1024.0f << " k/s" << std::endl
                        << "        STA read: " << bw_interface->readStats().getSTA()/1024.0f << " k/s" << std::endl
                        << "       Inst read: " << bw_interface->readStats().getInstAverage()/1024.0f << " k/s" << std::endl
                        << "    Max STA read: " << bw_interface->readStats().getMaxSTA()/1024.0f << " k/s" << std::endl
                        << std::endl;
           }
           else
          {
             std::cout << "SocketBWTest: Don't have BW Stats on stats. type is: " << typeid(stats).name() << std::endl;
          }


         con_sock.close();                                   // Close socket

         // Tell everyone that we closed
         mCondVar.acquire();
         {
            mState = CONNECTOR_CLOSED;
            mCondVar.signal();       // Tell any waiting threads
         }
         mCondVar.release();
      }
   }

   static CppUnit::Test* suite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("SocketBandwidthIOStatsTest");

      //test_suite->addTest( new CppUnit::TestCaller<SocketConnectorAcceptorTest>("testAcceptorConstruction", &SocketConnectorAcceptorTest::testAcceptorConstruction));
      //test_suite->addTest( new CppUnit::TestCaller<SocketConnectorAcceptorTest>("testSpawnedAcceptor", &SocketConnectorAcceptorTest::testSpawnedAcceptor));

      return test_suite;
   }

   static CppUnit::Test* metric_suite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("SocketBandwidthIOStatsTest_metric");

      test_suite->addTest( new CppUnit::TestCaller<SocketBandwidthIOStatsTest>("testBandwidth", &SocketBandwidthIOStatsTest::testBandwidth));

      return test_suite;
   }


protected:
    vpr::Uint16     mRendevousPort;     // The port the acceptor will be waiting on
    int             mNumItersA;        // Number of primary iterations
    int             mNumItersB;        // Number of secondary iterations

    vpr::Uint16               mMessageLen;        // Length of message to send
    std::vector<vpr::Uint8>   mMessageValue;      // The value of the message that is supposed to be sent (and recieved)

    vpr::CondVar    mCondVar;       // Condition variable
    vpr::Mutex      mSyncMutex;     // Mutex used to do synchronization

    enum State { READY, NOT_READY, CONNECTOR_CLOSED
    };
    State           mState;         // State variable

    bool            mReadyFlag1;
    bool            mReadyFlag2;
};

};

#endif
