#ifndef _VJ_GLOVE_H_
#define _VJ_GLOVE_H_
#pragma once

#include <vjConfig.h>
#include <Input/vjInput/vjInput.h>
#include <Input/InputManager/vjPosInterface.h>
#include <Math/vjMatrix.h>

#define VJ_MAX_GLOVE_DEVS 2          // The maximum number of gloves per device

//: This is the data stored about a glove
//
// Desc:
//  The angles are the joint angles for all fingers and the wrist
//  DIJ = Distal Interphalangeal Joint  --- finger tip
//  PIJ = Proximal "              "     --- Middle joint
//  MPJ = Metacarpo "             "     --- closest to hand
//  ABDUCT = spread of fingers
//
// YAW and PITCH apply only to WRIST
//
// xforms transfer you from one coord system to the other
// if the xforms are tied together, then they can return
// complete transformations
//
// NOTE: More docs needed here
//
class vjGloveData
{
public:
   enum { NUM_JOINTS = 4, NUM_COMPONENTS = 6 };
   enum vjGloveJoint
      { MPJ = 0, PIJ = 1, DIJ = 2, ABDUCT = 3, YAW = 0, PITCH = 1};
   enum vjGloveComponent
      { THUMB = 0, INDEX = 1, MIDDLE = 2, RING = 3, PINKY = 4, WRIST = 5};

public:
   //: Constructor
   //! POST: Initialized with zero values and identities
   vjGloveData();

   //: From the given angle data, calculate the xforms
   calcXforms();

public:  // --- I/O Functions --- //
   //: Output the angles
   ostream& outputAngles(ostream& out) const;
   istream& inputAngles(istream& in);

public:
   float     angles[NUM_COMPONENTS][NUM_JOINTS];

   //: These are the xforms from TO the coord system of the given joint
   // Ex: xforms[0] ==> <br>
   //     base<b>T</b>mpj mpj<b>T</b>pij pij<b>T</b>dij
   vjMatrix  xforms[NUM_COMPONENTS][(NUM_JOINTS-1)];

   // Finger params
   //  XXX: Should put better info about hand dimensions in here
   // For now this is the translations FROM the previous joint to
   // the SPECIFIED joint.  In case of (DIJ+1), length to tip of finger
   vjVec3    dims[NUM_COMPONENTS][NUM_JOINTS];
};


//------------------------------------------------------------------------
//: This is the abstract base glove class. Derived from vjInput.
// It specifies the interface to all glove objects in the system.
// VR Juggler will deal only with gloves using this interface.
//
// Description:
//
//-------------------------------------------------------------------------
class vjGlove : virtual public vjInput
{
public:
   vjGlove();

   vjGlove(vjConfigChunk* chunk);

   char* GetDeviceName() { return "vjGlove"; }

public:  // ---- GLOVE INTERFACE ---- //
   //: Return the angle of the given joint.
   // joint is one of a predefined enum type (jointType)
   float getGloveAngle(vjGloveData::vjGloveComponent component,
                       vjGloveData::vjGloveJoint joint, int devNum);

   //: This returns a vector ponting "out" of the component
   // Can be used for selection, etc.
   vjVec3 getGloveVector(vjGloveData::vjGloveComponent component, int devNum);

   //: This returns the position of given components.
   // Defaults to returning the palm position.
   // Can also get finger tips.
   vjMatrix getGlovePos(vjGloveData::vjGloveComponent component = vjGloveData::WRIST, int devNum = 0);

   //: This returns a copy of the glove data struct
   vjGloveData getGloveData(int devNum);

protected:
   vjGloveData mTheData[VJ_MAX_GLOVE_DEVS][3];

  //: This is the positional proxy of the glove.  It defines the location of the
  // "center" of the glove. "center" could be different for each glove type.
  vjPosProxy*  mGlovePos[VJ_MAX_GLOVE_DEVS];

};

#endif
