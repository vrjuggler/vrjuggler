#ifndef _VJ_CYBER_GLOVE_H_
#define _VJ_CYBER_GLOVE_H_

#include <vjConfig.h>
#include <Input/vjGlove/vjGlove.h>
#include <Input/vjGlove/CyberGloveBasic.h>

class vjCyberGlove : virtual public vjGlove
{
public:
   //: Construct using chunk
   vjCyberGlove(vjConfigChunk *c);

   //: Destroy the glove
   ~vjCyberGlove ();

   virtual char* GetDeviceName() { return "vjCyberGlove";}

   virtual int StartSampling();
   virtual int StopSampling();
   virtual int Sample();
   virtual void UpdateData ();

protected:
	//: The main control loop for the object
   void controlLoop(void* nullParam);

   void copyDataFromGlove();

protected:
   CyberGloveBasic*  mGlove;              // The actual glove
   char*             mCalDir;             // Calibration file directory
};

#endif	/* _VJ_CYBER_GLOVE_H_ */
