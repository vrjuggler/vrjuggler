/////////////////////////////////////////////////////////////////////////////
// vjInput.h
//
// Base class for all vj Input devices
//
// History:  
//
// Andy Himberger:    v0.1 (10/12/97) - Integeration to new libraries, support for
//              Abilities, Instance Names, Device Names, Config Chunk constructors
// Andy Himberger:    v0.0  - Inital version
////////////////////////////////////////////////////////////////////////////
#ifndef VJ_INPUT_H
#define VJ_INPUT_H

#include <config.h>

#ifndef WIN32
#include <unistd.h>
#include <termio.h>
#ifdef HAVE_SYS_PRCTL_H
#   include <sys/prctl.h>
#endif		/* ifdef HAVE_SYS_PRCTL_H */
#endif		/* ifndef WIN32 */

#include <limits.h>
#include <iostream.h>
#include <fstream.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <float.h>

#include <SharedMem/vjMemory.h>
#include <Sync/vjMutex.h>
#include <Threads/vjThread.h>
#include <Config/vjConfigChunkDB.h>


typedef unsigned char byte;

/** Abilities List
 *
 *  vjInput devices can have one or more 'Abilities'
 *  The function FDeviceSupport(ability) allows a user
 *  of a vjInput object to query which types it may be
 *  casted up to.
 */
const int DEVICE_POSITION = 1;
const int DEVICE_GESTURE  = 2;
const int DEVICE_DIGITAL  = 4;
const int DEVICE_ANALOG   = 8;
const int DEVICE_GROW1    = 16;
const int DEVICE_GROW2    = 32;

//------------------------------------------------------------------------------------------
//: vjInput is the abstract base class that all input objects derive from.
//
//  vjInput is the base class for all Input Devices, all the devices are therefore
//  forced to implement the pure virtual functions of Sample, StartSampling, StopSampling,
//  and UpdateData. <br> <br>
//
//  Dummy devices can use a default constructor, but physical devices should have a 
//  Constructor which takes a config chunk and calls the vjInput constructor taking a
//  vjConfigChunk. <br> <br>
//
//  All Physical devices will inherit from not c2Input but another abstract class which
//  inherits from vjInput, currently there is support for Positional Devices, Analog Devices,
//  and Digital devices, each has its own added pure virtual functions providing a
//  simple and equal interface to themselves.
//-------------------------------------------------------------------------------------------
class vjInput : public vjMemory
{
public:
   //: vjConfigChunk constructor,
   //  
   //  This baselevel constructor will fill the base datamembers
   //  when found in the vjConfigChunk, such as serial port, instance name
   //  and baud rate.
   vjInput(vjConfigChunk *c);

   //: Default Constructor
   //
   //  The default constructor is intended only for use by the DummyProxies
   //  which do not need to have their serial port and baud rate etc set up.
   vjInput();

   //: vjInput Destructor
   //
   // Free the memory for the Instance Name and Serial Port strings if allocated
   ~vjInput();

   //: Sample the device
   //  
   //  Every input device should have a sample function, after which the device has
   //  been sampled to have new data.  (This new data is not accessable until UpdateData
   //  is called, however)
   virtual int Sample() = 0;

   //: Start a device sampling.
   //
   //  Start the device sampling, normally this will spawn a thread which will
   //  just repeatedly call Sample().
   //  This function should return true when it sucessfully starts, 
   //      false otherwise.
   virtual int StartSampling() = 0;

   //: StopSampling.
   //
   //  Reverse the effects of StartSampling()
   virtual int StopSampling() = 0;

   //: UpdateData()
   //
   //  After this function is called subsequent calls to GetData(d) will return the most
   //  recent data at the time of THIS function call.  Data is guaranteed to be valid and
   //  static until the next call to UpdateData.
   virtual void UpdateData() = 0;

   //: GetDevicename()
   //
   //  Returns the name identifying the TYPE of Input Device
   virtual char* GetDeviceName() { return "vjInputBase";}  

   /** @name Functions to remove (?)
     *
     * It hasn't been resolved as to whether these functions should be removed 
     */
   //@{
   void SetPort(char* serialPort);
   char* GetPort();
   void SetBaudRate(int baud);
   int  GetBaudRate() { return baudRate;}
   //@}

   //: GetInstanceName()
   //
   //  Returns the name identifying the INSTANCE of this InputDevice
   char* GetInstanceName() { 
      if (instName == NULL) return "Undefined";
      return instName; 
   }

   //: FDeviceSupport(ability)
   //
   //  Returns true/false does this input device support the ability passed in
   int  FDeviceSupport(int devAbility);

   //: Is this input device active?.
   int IsActive() { return active;}

protected:
   char *sPort;
   char *instName;
   int port_id;
   vjThreadId* samplePID;
   int active;
   
   //: Index holders
   // Current is the index to the current data being returned by GetData
   // Progress is the index to the data being filled by Sample() at any given time
   // Valid is the index to the data that has already been filled by Sample() and
   // will become the next Current data.  (NOTE: no dirty bit checking is done
   // to make SURE that valid is newer than current on UpdateData switches)
   int current, progress, valid;

   // Mutex for swapping the pointers.. (Issue: move this to the device level so dummys don't have
   //                        an unused datamember?)
   vjMutex lock;
   int baudRate;
   int deviceAbilities;
};

#endif
