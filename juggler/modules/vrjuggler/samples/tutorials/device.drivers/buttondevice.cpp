#include <Input/vjInput/vjDigital.h>
#include <Input/InputManager/vjDeviceFactory.h>
#include <vpr/System.h>

class MyButtonDevice : public vjDigital
{
public:
   MyButtonDevice() : mSampleThread( NULL ) {}
   virtual ~MyButtonDevice() { this->stopSampling(); }
   virtual void  getData();
public:
   virtual void  startSampling();
   virtual void  sample();
   virtual void  stopSampling();
   static std::string getChunkType();
private:
   static void   threadedSampleFunction( void* classPointer );
   int           mDigitalData;
   vpr::Thread*     mSampleThread;
   
   // configuration data set by config()
   int           mPortId, mBaud;
};

vjDeviceConstructor<MyButtonDevice>* this_ptr_not_used = new vjDeviceConstructor<MyButtonDevice>;

//: What is the name of this device?
//  This function returns a string that should match this device's 
//  configchunk name.
static std::string  MyButtonDevice::getChunkType() 
{ 
   return std::string("MyButtonDevice");
}

// spawn a sample thread, 
// which calls MyButtonDevice::sample() repeatedly
void MyButtonDevice::startSampling()
{
   mSampleThread = new vpr::Thread( threadedSampleFunction, (void*)this, 0 );
   if (!mSampleThread->valid())
      return 0; // thread creation failed
   else 
      return 1; // thread creation success
}
   
//: Record (or sample) the current data
// this is called repeatedly by the sample thread created by startSampling()
void MyButtonDevice::sample()
{
   // here you would add your code to 
   // sample the hardware for a button press:
   mDigitalData[progress] = 1;//rand_number_0_or_1();
}

// kill sample thread
int MyButtonDevice::stopSampling()
{
   if (mSampleThread != NULL)
   {
      mSampleThread->kill();
      delete mSampleThread;
      mSampleThread = NULL;
   }
   return 1;
}

//: function for users to get the digital data.
//  here we overload vjDigital::getDigitalData
int MyButtonDevice::getDigitalData(int d)
{
   // only one button, so we ignore "d"
   return mDigitalData[current];
}

// Our threaded sample function
// This function is declared as a <b>static</b> member of MyButtonDevice
// just spins... calling sample() over and over.
void MyButtonDevice::threadedSampleFunction( void* classPointer )
{
   MyButtonDevice* this_ptr = static_cast<MyButtonDevice*>( classPointer );

   // spin until someone kills "mSampleThread"
   while (1)   
   {
     this_ptr->sample();
     vpr::System::sleep(1); //specify some time here, so you don't waste CPU cycles
   }
}

//: When the system detects a configuration change for your driver, it will
//  pass the new vjConfigChunk into this function.  See the documentation
//  on config chunks, for information on how to access them.
bool MyButtonDevice::config( vjConfigChunk *c )
{
  if (!vjDigital::config(c))
     return false;

  mPortId = c->getProperty( "port" );
  mBaud = c->getProperty( "baud" );

  return true;
}
