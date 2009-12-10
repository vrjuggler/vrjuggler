#include <vpr/vpr.h>
#include <vpr/vprTypes.h>
#include <vpr/IO/Socket/SocketDatagram.h>
#include "natnettypes.h"

#define PORT_MULTICASTGROUP		1001   
#define MULTICAST_ADDRESS		"224.0.0.1"
#define PORT_COMMAND            1510
#define MARKER_INDEX_OFFSET     1000

class RigidBodyData
{
public:
   RigidBodyData()
   {
      mX = 0.0f;
      mY = 0.0f;
      mZ = 0.0f;
      mQX = 0.0f;
      mQY = 0.0f;
      mQZ = 0.0f;
      mQW = 0.0f;
   }

   RigidBodyData(float x, float y, float z, float qx, float qy, float qz, float qw)
   {
      mX = 0.0f;
      mY = 0.0f;
      mZ = 0.0f;
      mQX = 0.0f;
      mQY = 0.0f;
      mQZ = 0.0f;
      mQW = 0.0f;
   }

   ~RigidBodyData() {;}

   float mX, mY, mZ;
   float mQX, mQY, mQZ, mQW;
};

class OptiTrackStandalone
{
public:
   OptiTrackStandalone();
   ~OptiTrackStandalone() {;}

   bool open(std::string address);
   bool close();

   bool updateData();
    
   // Marker Data
   /** Gets the x position of the i'th marker. */
   float xMarkerPos(const int i);

   /** Gets the y position of the i'th marker. */
   float yMarkerPos(const int i);

   /** Gets the z position of the i'th marker. */
   float zMarkerPos(const int i);

   //Rigid Body data
   /** Gets the x position of the i'th rigid body. */
   float xRBPos(const int i);

   /** Gets the y position of the i'th rigid body. */
   float yRBPos(const int i);

   /** Gets the z position of the i'th rigid body. */
   float zRBPos(const int i);

   /** Gets the x quaternion value of the i'th rigid body. */
   float xRBQuat(const int i);

   /** Gets the y quaternion value of the i'th rigid body. */
   float yRBQuat(const int i);

   /** Gets the z quaternion value of the i'th rigid body. */
   float zRBQuat(const int i);

   /** Gets the w quaternion value of the i'th rigid body. */
   float wRBQuat (const int i);

   bool isActive()
   {
      return mActive;
   }

private:
   void unpack(char * pData);
   bool IPAddress_StringToAddr(char *szNameOrAddress, struct in_addr *Address);
   int GetLocalIPAddresses(unsigned long Addresses[], int nMax);

   bool                 mActive;  /**< If the driver is active. */
   SOCKET               mSocket;
   vpr::InetAddr        mAddress; /**< Address of TrackingTools. */
   sFrameOfMocapData    mPacket;
   std::map<int, RigidBodyData> mRBMap;
};
