#ifndef GADGET_CLOCK_SYNC_H
#define GADGET_CLOCK_SYNC_H

// The Remote Input Manager allows a proxy or device on one machine to point to a device on
// on another machine

#include <gadget/gadgetConfig.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <gadget/RemoteInputManager/NetConnection.h>

namespace gadget
{

enum SyncState { SYNC_NONE,       
                 SYNC_INITIATE,   /** Our clock will be used in sync() **/
                 SYNC_RESPOND} ;  /** Will receive a clock in sync() **/


/** This Struct stores information for a pending time sync **/

struct _SyncConnection{ 
  SyncState state;  // do we start exchange or respond
  NetConnection *connection;
  std::vector<char> recv_buffer;
} ;
typedef struct _SyncConnection SyncConnection;



/** This class is used to synchronize the base performance 
  * clocks on two vrjuggler instances connected through the 
  * remote input manager.
  * The initiator's clock will be stored in the responder.
  * 
  * To Initiate sync:
  * Two nodes call SyncWithMyClock() and SyncWithOtherClock()
  * during the same frame (usually after just connecting) and
  *  both will sync() at end of remote input manager's frame.
  *
  * In synchronizing:
  * The first step sends the name of the clock's original
  * host to verify the responder needs synchronizing.
  * A loop then passes messages containing clock/time info
  * back and forth until the responder believes its clock
  * is synchronized well enough with the initiator.  
  * 
  * Not yet Implemented: after the setting of its clock,
  * the responder could attempt to synchronize its new clock with
  * all of its existing connections -- although much recursive 
  * synchronizing would be detrimental to clock accuracy.  The need
  * for recursive syncs can be avoided by adding connections one at
  * a time -- all this is only necessary if using clock sync
  * performance measurements.
  */

class GADGET_CLASS_API NetClockSync{

public:
   NetClockSync(){ mErrorThreshold = 0.00005; } // start at .5 milliseconds

   /** Set the host/manager that our clock originated from **/
   void setClockSrcId(const vpr::GUID& id){ mSrcManagerId = id; }

   /** Stores information to sync clocks at end of this frame **/
   void syncWithMyClock(NetConnection* connection);

   /** Stores information to sync clocks at end of this frame **/
   void syncWithOtherClock(NetConnection* connection);

   /** Is sync needed **/
   bool syncNeeded();

   /** Will start a sync exchange if mConnections contains any connections. **/
   void sync();

protected:

/* Information about the manager/host that our clock originated 
 * from.  Lets us we know if we are updated already.
 */
   vpr::GUID mSrcManagerId;

   double mErrorThreshold; /// target clock error threshold in seconds
   std::vector<SyncConnection*> mConnections; /// connections that need sync-ing


   /** ---internal functions called from sync()--------------- **/

   void initiateExchange(SyncConnection &sconn);
   void respondExchange(SyncConnection &sconn);
   void firstIterationAsInitiator(SyncConnection& sconn);
   float firstIterationAsResponder(SyncConnection& sconn, float& returned_clock1);
   bool iterationLoopAsInitiator(SyncConnection& sconn);
   float iterationLoopAsResponder(SyncConnection& sconn, const float& clock1, float& clock2, float& returned_clock1, float& returned_clock2, const bool is_clock_synced);

};

}  // end namespace gadget

#endif


