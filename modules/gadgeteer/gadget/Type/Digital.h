/////////////////////////////////////////////////////////////////////////
// vjDigital.h
//
// positional tracker base class
//
////////////////////////////////////////////////////////////////////////
#ifndef _VJ_DIGITAL_H_
#define _VJ_DIGITAL_H_

#include <vjConfig.h>
#include <Input/vjInput/vjInput.h>

//-----------------------------------------------------------------------------
//: vjDigital is the abstract base class that devices with digital data derive
//+ from.
//
//  vjDigital is the base class that digital devices must derive from.
//  vjDigital inherits from vjInput, so it has pure virtual function
//  constraints from vjInput in the following functions: StartSampling,
//  StopSampling, Sample, and UpdateData. <br>
//  vjDigital adds one new pure virtual function, GetDigitalData for
//  retreiving the digital data, similar to the addition for vjPosition and
//  vjAnalog.
//
// See also: vjInput
//!PUBLIC_API:
//-----------------------------------------------------------------------------
class vjDigital : virtual public vjInput
{
public:
   //: Enum for the state of the digital buttons
   // Used in vjDigitalProxy
   enum {
      OFF=0, ON=1, TOGGLE_ON=2, TOGGLE_OFF=3
   };

public:
   /* Constructor/Destructors */
   vjDigital() { deviceAbilities = deviceAbilities | DEVICE_DIGITAL;}
   ~vjDigital() {}

   virtual bool config(vjConfigChunk* c)
   { return vjInput::config(c); }

   /* Pure virtual functions required from vjInput */
   virtual int StartSampling() = 0;
   virtual int StopSampling() = 0;
   virtual int Sample() = 0;
   virtual void UpdateData() = 0;

   //: Get the name of the digital device
   char* GetDeviceName() { return "vjDigital";}

   //: Get the digital data for the given devNum
   virtual int GetDigitalData(int devNum = 0) = 0;
};

#endif	/* _VJ_DIGITAL_H_ */
