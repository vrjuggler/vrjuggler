/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vrj/Input/Type/Digital.h>
#include <vrj/Input/Type/DeviceFactory.h>
#include <vpr/System.h>

using namespace vrj;

class MyButtonDevice : public Digital
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

DeviceConstructor<MyButtonDevice>* this_ptr_not_used = new DeviceConstructor<MyButtonDevice>;

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
   mSampleThread = new vpr::Thread(threadedSampleFunction, (void*) this, 0);

   if ( ! mSampleThread->valid() )
   {
      return 0; // thread creation failed
   }
   else 
   {
      return 1; // thread creation success
   }
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
//  here we overload Digital::getDigitalData
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
//  pass the new ConfigChunk into this function.  See the documentation
//  on config chunks, for information on how to access them.
bool MyButtonDevice::config( ConfigChunk *c )
{
  if (!Digital::config(c))
     return false;

  mPortId = c->getProperty<int>( "port" );
  mBaud = c->getProperty<int>( "baud" );

  return true;
}
