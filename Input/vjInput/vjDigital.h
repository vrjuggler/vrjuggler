/////////////////////////////////////////////////////////////////////////
// vjDigital.h
//
// positional tracker base class
//
////////////////////////////////////////////////////////////////////////
#ifndef _VJ_DIGITAL_H_
#define _VJ_DIGITAL_H_

#include <config.h>
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
//----------------------------------------------------------------------------- 
class vjDigital : virtual public vjInput
{
public:

   /* Constructor/Destructors */
   vjDigital(vjConfigChunk *c):vjInput(c) {
      deviceAbilities = deviceAbilities | DEVICE_DIGITAL;}
   vjDigital() { deviceAbilities = deviceAbilities | DEVICE_DIGITAL;}
   ~vjDigital() {}

   /* Pure virtual functions required from vjInput */
   virtual int StartSampling() = 0;
   virtual int StopSampling() = 0;
   virtual int Sample() = 0;
   virtual void UpdateData() = 0;

   /* vjInput virtual functions
    *
    *  virtual functions that inherited members should
    *  override but are not required to
    */
   char* GetDeviceName() { return "vjDigital";}

   /* New pure virtual functions
    *
    *  pure virtual functions introduced by vjDigital
    */
   //: Get the digital data for the given devNum
   virtual int GetDigitalData(int devNum = 0) = 0;
};

#endif	/* _VJ_DIGITAL_H_ */
