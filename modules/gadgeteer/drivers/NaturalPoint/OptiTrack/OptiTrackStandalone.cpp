#include <drivers/NaturalPoint/OptiTrack/OptiTrackStandalone.h>
#include <vpr/vpr.h>
#include <vpr/IO/Socket/SocketDatagram.h>

OptiTrackStandalone::OptiTrackStandalone() :
   mActive(false)
{

}

bool OptiTrackStandalone::open(int port)
{
   if (!mActive)
   {

      vpr::InetAddr myAddr, multiCastAddr;

      myAddr.setPort(port);

      multiCastAddr.setAddress(MULTICAST_ADDRESS);
      multiCastAddr.setFamily(vpr::SocketTypes::INET);

      vpr::McastReq multiCastReq = vpr::McastReq(multiCastAddr, myAddr);

      mSocket = new vpr::SocketDatagram();
      mSocket->setLocalAddr(myAddr);
     
      mSocket->open();    
      mSocket->setMcastInterface(myAddr);
      try
      {
         mSocket->bind();
      }
      catch (vpr::IOException&)
      {
         mActive = false;
         return false;
      }
      mSocket->addMcastMember(multiCastReq);

      mActive = true;
      return true;
   }

   return false;
}

bool OptiTrackStandalone::close()
{
   mSocket->close();
   delete mSocket;
   mActive = false;
   return true;
}

bool OptiTrackStandalone::updateData()
{
   char szData[20000];
   vpr::InetAddr theirAddr;
   vpr::Interval timeout = vpr::Interval::Interval(1000000, vpr::Interval::Unit::Msec);
   vpr::Uint32 bytes = mSocket->recvfrom(szData, sizeof(szData), theirAddr, timeout);
   if (bytes > 0)
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
         strcpy(mPacket.MocapData[i].szName, ptr);
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
         
         // Mean Marker Error
         ptr += 4;
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
