/////////////////////////////////////////////////////////////////////////
// vjPosition.h
//
// positional tracker base class
//
////////////////////////////////////////////////////////////////////////
#ifndef _VJ_POSITION_H_
#define _VJ_POSITION_H_

#include <vjConfig.h>
#include <Input/vjInput/vjInput.h>
#include <Math/vjVec3.h>
#include <Math/vjMatrix.h>

typedef struct {
  vjVec3 pos, orient;   // orient - EulerZYX , 0-Z, 1-Y, 2-X ???
  //float x,y,z,azi,elev,roll;
  } vjPOS_DATA;

//-----------------------------------------------------------------------------------
//: vjPosition is the abstract base class that devices with digital data derive from.
//
//  vjPosition is the base class that digital devices must derive from.  vjPosition
//  inherits from vjInput, so it has pure virtual function constraints from
//  vjInput in the following functions: StartSampling,StopSampling,Sample,
//  and UpdateData.
//
//  vjPosition objects have the ability to convert from the tracker's coord system
//  to the Juggler coordinate system.
//
//  vjPosition adds one new pure virtual function, GetPosData for retreiving
//  the positional data, similar to the addition for vjAnalog and vjDigital.
//-----------------------------------------------------------------------------------
//!PUBLIC_API:
class vjPosition : virtual public vjInput {
    public:
	/** @name Construction/Destruction
	 */
	//@{
	vjPosition();
	~vjPosition();
	//@}

   virtual bool config(vjConfigChunk *c);


	/** @name vjInput pure virtual functions
	 *
	 *  pure virtual functions required from vjInput
	 */
	//@{
	virtual int StartSampling() = 0;
	virtual int StopSampling() = 0;
	virtual int Sample() = 0;
	virtual void UpdateData() = 0;
	//@}

	//: Get the device name
	char* GetDeviceName() { return "vjPosition"; }

	/* New pure virtual functions */
	//: Get Position data
	virtual vjMatrix* GetPosData(int devNum = 0) = 0;

public:
   /* XXX: Some of this stuff should be removed */
   /* XXX: theData is a ptr because flock needs an infinite number */
   /* XXX: We should change this so that theData is defined in each class and not here */
	vjMatrix* theData;   // Ptr to matrix that holds the actually position data
	
   vjMatrix xformMat;   // The total xform matrix.  T*R  NOTE: Used to move from trk coord system to Juggler coord system
   vjMatrix rotMat;     // Only the rotation matrix
};

#endif

	
