/////////////////////////////////////////////////////////////////////////
// vjAnalog.h
//
// positional tracker base class
//
////////////////////////////////////////////////////////////////////////
#ifndef _VJ_ANALOG_H_
#define _VJ_ANALOG_H_

#include <vjConfig.h>
#include <Input/vjInput/vjInput.h>

//-----------------------------------------------------------------------------
//: vjAnalog is the abstract base class that devices with digital data derive
//+ from.
//
//  vjAnalog is the base class that digital devices must derive from.
//  vjAnalog inherits from vjInput, so it has pure virtual function
//  constraints from vjInput in the following functions: StartSampling,
//  StopSampling, Sample, and UpdateData. <br> <br>
//
//  vjAnalog adds one new pure virtual function, GetAnalogData for retreiving
//  the digital data, similar to the addition for vjPosition and vjDigital.
//!PUBLIC_API:
//-----------------------------------------------------------------------------
class vjAnalog : virtual public vjInput
{
public:

	//: Constructor
   //! POST: Set device abilities
   //! NOTE: Must be called from all derived classes
   vjAnalog () {  deviceAbilities = deviceAbilities | DEVICE_ANALOG; }
	~vjAnalog() {}
	
   // Just call base class config
   //! NOTE: Let constructor set device abilities
   virtual bool config(vjConfigChunk* c)
   { return vjInput::config(c); }

	/* vjInput pure virtual functions */
	virtual int startSampling() = 0;
	virtual int stopSampling() = 0;
	virtual int sample() = 0;
	virtual void updateData() = 0;
	
	//: Get the device name
	char* getDeviceName() { return "vjAnalog"; }
	
	/* New pure virtual functions */
	//: Return analog data
	virtual int getAnalogData(int devNum = 0) = 0;
};


#endif	/* _VJ_ANALOG_H_ */
