#include <stdio.h>
#include <string.h>
#include <iostream>
#include <typeinfo>

#include <vpr/vpr.h>
#include <vpr/IO/Socket/Socket.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/IO/Socket/InetAddr.h>
#include <vpr/IO/Socket/SocketAcceptor.h>
#include <vpr/IO/Socket/SocketConnector.h>

#include <vpr/IO/Stats/BandwidthIOStatsStrategy.h>

#include <vpr/Util/Interval.h>

#include <vpr/System.h>

#include <vpr/Thread/Thread.h>

#include <TestCases/IO/Stats/SocketBandwidthIOStatsTest.h>


namespace vprTest
{

void SocketBandwidthIOStatsTest::testBandwidth()
{
#ifdef VPR_OS_Windows
   long rand_num(rand());
#else
   long rand_num(random());
#endif
   mRendevousPort = 47000 + (rand_num % 71);     // Get a partially random port
   mNumItersA = 1;
   mNumItersB = 5000;
   mMessageValue = std::vector<vpr::Uint8>(1000, 21);      // 10000 bytes of data
   mMessageLen = mMessageValue.size();

   mState = NOT_READY;                        // Initialize

   // Spawn acceptor thread
   vpr::Thread acceptor_thread(
      boost::bind(&SocketBandwidthIOStatsTest::testBandwidth_acceptor, this)
   );

   // Spawn connector thread
   vpr::Thread connector_thread(
      boost::bind(&SocketBandwidthIOStatsTest::testBandwidth_connector, this)
   );

   CPPUNIT_ASSERT( acceptor_thread.valid() && "Invalid acceptor thread");
   CPPUNIT_ASSERT( connector_thread.valid() && "Invalid connector thread");

   if(!acceptor_thread.valid())
      std::cerr << "Invalid acceptor thread\n";
   if(!connector_thread.valid())
      std::cerr << "Invalid connector_thread\n";

   // Wait for threads
   acceptor_thread.join();
   connector_thread.join();
}

void SocketBandwidthIOStatsTest::testBandwidth_acceptor()
{
   vpr::InetAddr local_acceptor_addr;
   local_acceptor_addr.setAddress("localhost", mRendevousPort);
   vpr::SocketAcceptor acceptor;
   vpr::SocketStream* sock(NULL);
   vpr::ReturnStatus ret_val;
   vpr::Uint32 bytes_written;

   // Open the acceptor
   ret_val = acceptor.open(local_acceptor_addr);
   CPPUNIT_ASSERT((ret_val.success()) && "Acceptor did not open correctly");

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
      CPPUNIT_ASSERT((ret_val.success()) && "Accepting socket failed");

      CPPUNIT_ASSERT((sock->isOpen()) && "Accepted socket should be open");
      CPPUNIT_ASSERT((sock->isConnected()) && "Accepted socket should be connected");
      for(int j=0;j<mNumItersB;j++)
      {
         ret_val = sock->write(mMessageValue, mMessageLen, bytes_written);      // Send a message
         CPPUNIT_ASSERT((ret_val.success()) && "Problem writing in acceptor");
         CPPUNIT_ASSERT((bytes_written == mMessageLen) && "Didn't send entire messag");
      }

      // WAIT for close
      mCondVar.acquire();
      while(mState != CONNECTOR_CLOSED)
         mCondVar.wait();
      mCondVar.release();

      vpr::BaseIOStatsStrategy* stats = sock->getIOStatStrategy();
      vpr::BandwidthIOStatsStrategy* bw_interface = dynamic_cast<vpr::BandwidthIOStatsStrategy*>(stats );

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
      CPPUNIT_ASSERT((ret_val.success()) && "Problem closing accepted socket");
   }
}

void SocketBandwidthIOStatsTest::testBandwidth_connector()
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
      CPPUNIT_ASSERT((ret_val.success()) && "Connector can't connect");

      for(int j=0;j<mNumItersB;j++)
      {
         ret_val = con_sock.readn(data, mMessageLen, bytes_read);   // Recieve data
         CPPUNIT_ASSERT((ret_val.success()) && "Read failed");
         CPPUNIT_ASSERT((bytes_read == mMessageLen) && "Connector recieved message of wrong size" );
      }

      vpr::BaseIOStatsStrategy* stats = con_sock.getIOStatStrategy();
      vpr::BandwidthIOStatsStrategy* bw_interface = dynamic_cast<vpr::BandwidthIOStatsStrategy*>(stats );

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

} // End of vprTest namespace
