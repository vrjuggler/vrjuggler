/////////////////////////////////////////////////////////////////////////
// vjAnalog.h
//
// positional tracker base class
//
////////////////////////////////////////////////////////////////////////
#ifndef _VJ_ANALOG_H_
#define _VJ_ANALOG_H_

#include <config.h>
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
//-----------------------------------------------------------------------------
class vjAnalog : virtual public vjInput {
    public:
    
	/** @name Construction/Destruction
	 */
	//@{
	vjAnalog(vjConfigChunk *c) : vjInput(c) {
	    deviceAbilities = deviceAbilities | DEVICE_ANALOG; }
	vjAnalog () {  deviceAbilities = deviceAbilities | DEVICE_ANALOG; }
	~vjAnalog() {}
	//@}

	/* vjInput pure virtual functions */
	virtual int StartSampling() = 0;
	virtual int StopSampling() = 0;
	virtual int Sample() = 0;
	virtual void UpdateData() = 0;
	
	/* vjInput virtual functions */
	char* GetDeviceName() { return "vjAnalog"; }
	
	/* New pure virtual functions */
	//: Return analog data
	virtual int GetAnalogData(int devNum = 0) = 0;
};

#endif	/* _VJ_ANALOG_H_ */
