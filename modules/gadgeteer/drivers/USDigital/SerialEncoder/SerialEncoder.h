#ifndef SERIAL_ENCODER_H
#define SERIAL_ENCODER_H

#include <vpr/Thread/Thread.h>
#include <vpr/Sync/Guard.h>

#include <gadget/Type/Input.h>
#include <gadget/Type/Position.h>
#include <gadget/Devices/DriverConfig.h>
#include <gadget/Type/DeviceConstructor.h>
#include <jccl/Config/ConfigElementPtr.h>

#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/InputMixer.h>

#include "SEIBus.h"

namespace gadget
{
   class InputManager;
}

extern "C" GADGET_DRIVER_API(void) initDevice(gadget::InputManager* inputMgr);

class SerialEncoder:public gadget::InputMixer<gadget::Input,gadget::Position>{
public:
   SerialEncoder();
   virtual ~SerialEncoder();

   static std::string getElementType(){
      return std::string("serial_encoder");
   }

	bool config(jccl::ConfigElementPtr e);

	bool startSampling();
	bool sample();
	bool stopSampling();
  
	void updateData(){
		swapPositionBuffers();
	}
    
	void threadedSampleFunction(void* classPointer);

	void operator delete(void* p){
		::operator delete(p);
	}

protected:
	virtual void destroy(){
		delete this;
	}

private:
	vpr::Thread* mSampleThread;
   bool  mExitFlag;
	std::string mPortStr;
	long mBaudRate;

	SEIBus mBus;
};

#endif

