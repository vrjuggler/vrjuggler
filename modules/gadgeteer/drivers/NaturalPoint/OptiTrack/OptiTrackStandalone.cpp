#include <drivers/NaturalPoint/OptiTrack/OptiTrackStandalone.h>
#include <winsock2.h>
#include <ws2tcpip.h>

OptiTrackStandalone::OptiTrackStandalone() :
   mActive(false)
{

}

bool OptiTrackStandalone::open(std::string address)
{
   if (!mActive)
   {
      // Winsock startup code
      WSADATA WsaData; 

      if (WSAStartup(0x202, &WsaData) == SOCKET_ERROR)
      {
         vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
            << clrOutNORM(clrRED,"ERROR:")
            << " gadget::OptiTrackStandalone::OptiTrackStandalone() WSAStartup "
            << "failed!\n" << vprDEBUG_FLUSH;
         WSACleanup();
      }  
      
      in_addr myAddr, multiCastAddr;
      GetLocalIPAddresses((unsigned long *)&myAddr, 1);
      multiCastAddr.S_un.S_addr = inet_addr(MULTICAST_ADDRESS);

      mSocket = socket(AF_INET, SOCK_DGRAM, 0);

      struct sockaddr_in mySocketAddr;
      memset(&mySocketAddr, 0, sizeof(mySocketAddr));
      mySocketAddr.sin_family = AF_INET;
      mySocketAddr.sin_port = htons(PORT_MULTICASTGROUP);
      mySocketAddr.sin_addr = myAddr;
      bind(mSocket, (struct sockaddr *)&mySocketAddr, sizeof(struct sockaddr));

      // join multicat group
      struct ip_mreq mreq;
      mreq.imr_multiaddr = multiCastAddr;
      mreq.imr_interface = myAddr;
      setsockopt(mSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq));

      mActive = true;
      return true;
   }

   return false;
}

bool OptiTrackStandalone::close()
{
   closesocket(mSocket);
   mActive = false;
   return true;
}

bool OptiTrackStandalone::updateData()
{
   char szData[20000];
   int addrLen = sizeof(struct sockaddr);
   sockaddr_in theirAddr;

   recvfrom(mSocket, szData, sizeof(szData), 0, (sockaddr *)&theirAddr, &addrLen);
   unpack(szData);
   return true;
}

void OptiTrackStandalone::unpack(char * szData)
{
   char *ptr = szData;

   // message ID
   int id = 0;
   memcpy(&id, ptr, 2); ptr += 2;

   // size
   int nBytes = 0;
   memcpy(&nBytes, ptr, 2); ptr += 2;

   if (id == 7)
   {
      // Frame number
      mPacket.iFrame = 0;
      memcpy(&(mPacket.iFrame), ptr, 4); ptr +=4;

      // Number of data sets
      mPacket.nMarkerSets = 0;
      memcpy(&(mPacket.nMarkerSets), ptr, 4); ptr+=4;

      for (int i = 0; i < mPacket.nMarkerSets; i++)
      {
         // Marker name
         strcpy_s(mPacket.MocapData[i].szName, ptr);
         int nDataBytes = (int) strlen(mPacket.MocapData[i].szName) + 1;
         ptr += nDataBytes;

         // Marker Data
         mPacket.MocapData[i].nMarkers = 0;
         memcpy(&(mPacket.MocapData[i].nMarkers), ptr, 4); ptr += 4;

         int nBytes = mPacket.MocapData[i].nMarkers * 3 * sizeof(float);

         ptr += nBytes;
      }

      // Unidentified Markers
      mPacket.nOtherMarkers = 0;
      memcpy(&(mPacket.nOtherMarkers), ptr, 4); ptr += 4;
        
      int nBytes = mPacket.nOtherMarkers * 3 * sizeof(float);
      ptr += nBytes;

      // Rigid Bodies
      mPacket.nRigidBodies = 0;
      memcpy(&(mPacket.nRigidBodies), ptr, 4); ptr += 4;

      for (int i = 0; i < mPacket.nRigidBodies; i++)
      {
         // ID
         mPacket.RigidBodies[i].ID = 0;
         memcpy(&(mPacket.RigidBodies[i].ID), ptr, 4); ptr += 4;
         // X
         mPacket.RigidBodies[i].x = 0;
         memcpy(&(mPacket.RigidBodies[i].x), ptr, 4); ptr += 4;
         // Y
         mPacket.RigidBodies[i].y = 0;
         memcpy(&(mPacket.RigidBodies[i].y), ptr, 4); ptr += 4;
         // Z
         mPacket.RigidBodies[i].z = 0;
         memcpy(&(mPacket.RigidBodies[i].z), ptr, 4); ptr += 4;
         // qX
         mPacket.RigidBodies[i].qx = 0;
         memcpy(&(mPacket.RigidBodies[i].qx), ptr, 4); ptr += 4;
         // qY
         mPacket.RigidBodies[i].qy = 0;
         memcpy(&(mPacket.RigidBodies[i].qy), ptr, 4); ptr += 4;
         // qZ
         mPacket.RigidBodies[i].qz = 0;
         memcpy(&(mPacket.RigidBodies[i].qz), ptr, 4); ptr += 4;
         // qW
         mPacket.RigidBodies[i].qw = 0;
         memcpy(&(mPacket.RigidBodies[i].qw), ptr, 4); ptr += 4;

         // Add data to mRBMap
         if (mPacket.RigidBodies[i].x != 0 && mPacket.RigidBodies[i].y != 0 &&
             mPacket.RigidBodies[i].z != 0 && mPacket.RigidBodies[i].qx != 0 &&
             mPacket.RigidBodies[i].qy != 0 && mPacket.RigidBodies[i].qz != 0 &&
             mPacket.RigidBodies[i].qw != 0)
         {
            mRBMap[mPacket.RigidBodies[i].ID].mX = mPacket.RigidBodies[i].x;
            mRBMap[mPacket.RigidBodies[i].ID].mY = mPacket.RigidBodies[i].y;
            mRBMap[mPacket.RigidBodies[i].ID].mZ = mPacket.RigidBodies[i].z;

            mRBMap[mPacket.RigidBodies[i].ID].mQX = mPacket.RigidBodies[i].qx;
            mRBMap[mPacket.RigidBodies[i].ID].mQY = mPacket.RigidBodies[i].qy;
            mRBMap[mPacket.RigidBodies[i].ID].mQZ = mPacket.RigidBodies[i].qz;
            mRBMap[mPacket.RigidBodies[i].ID].mQW = mPacket.RigidBodies[i].qw;
         }

         int nRigidMarkers = 0;
         memcpy(&nRigidMarkers, ptr, 4); ptr += 4;

         int nBytes = nRigidMarkers*3*sizeof(float);
         ptr += nBytes;
            
         // associated marker IDs
         nBytes = nRigidMarkers*sizeof(int);
         ptr += nBytes;

         // associated marker sizes
         nBytes = nRigidMarkers*sizeof(float);
         ptr += nBytes;
        }
    }
}

float OptiTrackStandalone::xMarkerPos(const int i)
{
   int index = i - MARKER_INDEX_OFFSET;
   if(mPacket.MocapData[0].nMarkers > index) 
      return mPacket.MocapData[0].Markers[index][0];
   return 0.0f;
}

float OptiTrackStandalone::yMarkerPos(const int i)
{
   int index = i - MARKER_INDEX_OFFSET; 
   if(mPacket.MocapData[0].nMarkers > index) 
      return mPacket.MocapData[0].Markers[index][1];
   return 0.0f;
}

float OptiTrackStandalone::zMarkerPos(const int i)
{
   int index = i - MARKER_INDEX_OFFSET; 
   if(mPacket.MocapData[0].nMarkers > index) 
      return mPacket.MocapData[0].Markers[index][2];
   return 0.0f;
}

float OptiTrackStandalone::xRBPos(const int i)
{
   return mRBMap[i].mX;
}

float OptiTrackStandalone::yRBPos(const int i)
{
   return mRBMap[i].mY;
}

float OptiTrackStandalone::zRBPos(const int i)
{
   return mRBMap[i].mZ;
}

float OptiTrackStandalone::xRBQuat(const int i)
{
   return mRBMap[i].mQX;
}

float OptiTrackStandalone::yRBQuat(const int i)
{
   return mRBMap[i].mQY;
}

float OptiTrackStandalone::zRBQuat(const int i)
{
   return mRBMap[i].mQZ;
}

float OptiTrackStandalone::wRBQuat(const int i)
{
   return mRBMap[i].mQW;
}

bool OptiTrackStandalone::IPAddress_StringToAddr(char *szNameOrAddress, struct in_addr *Address)
{
   int retVal;
   struct sockaddr_in saGNI;
   char hostName[256];
   char servInfo[256];
   u_short port;
   port = 0;

   // Set up sockaddr_in structure which is passed to the getnameinfo function
   saGNI.sin_family = AF_INET;
   saGNI.sin_addr.s_addr = inet_addr(szNameOrAddress);
   saGNI.sin_port = htons(port);

   // getnameinfo
   if ((retVal = getnameinfo((SOCKADDR *)&saGNI, sizeof(sockaddr), hostName, 256, servInfo, 256, NI_NUMERICSERV)) != 0)
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << " [OptiTrackStandalone::IPAddress_StringToAddr()] getnameinfo failed!\n"
         << " Error code = " << WSAGetLastError() << "\n" << vprDEBUG_FLUSH;
      return false;
   }

   Address->S_un.S_addr = saGNI.sin_addr.S_un.S_addr;

   return true;
}

int OptiTrackStandalone::GetLocalIPAddresses(unsigned long Addresses[], int nMax)
{
   unsigned long  NameLength = 128;
   char szMyName[1024];
   struct addrinfo aiHints;
   struct addrinfo *aiList = NULL;
   struct sockaddr_in addr;
   int retVal = 0;
   char* port = "0";
    
   if (GetComputerName(szMyName, &NameLength) != TRUE)
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << " [OptiTrackStandalone::GetLocalIPAddresses()] GetComputerName failed!\n"
         << " Error code = " << WSAGetLastError() << "\n" << vprDEBUG_FLUSH; 
      return 0;
   }

   memset(&aiHints, 0, sizeof(aiHints));
   aiHints.ai_family = AF_INET;
   aiHints.ai_socktype = SOCK_DGRAM;
   aiHints.ai_protocol = IPPROTO_UDP;
   if ((retVal = getaddrinfo(szMyName, port, &aiHints, &aiList)) != 0) 
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << " [OptiTrackStandalone::GetLocalIPAddresses()] getaddrinfo failed!\n"
         << " Error code = " << WSAGetLastError() << "\n" << vprDEBUG_FLUSH; 
      return 0;
   }

   memcpy(&addr, aiList->ai_addr, aiList->ai_addrlen);
   freeaddrinfo(aiList);
   Addresses[0] = addr.sin_addr.S_un.S_addr;

   return 1;
}
