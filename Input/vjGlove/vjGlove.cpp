#include <Input/vjGlove/vjGlove.h>

/////////////////////////////////////////////////////////////////////////
//                vjGloveData                                          //
/////////////////////////////////////////////////////////////////////////

/*
 * Implementation of the vjGlove
 * abstract device class
 *
 * This class is the base class for all gloves.
 * We will implment all the necessary data operations
 * on the glove data. (Get finger pos, get vectors, get matrices, etc.)
 */

vjGloveData::vjGloveData()
{
   // Zero out the angles
   for(int i=0;i<NUM_COMPONENTS;i++)
      for(int j=0;j<NUM_JOINTS;j++)
         angles[i][j] = 0.0f;

   // Matrices are already identities
}

//: Calulate all the xform matrices
// This is calculated based upon the angles in the data structure
// This code will be very complex, so I will HACK it for now.
// XXX: This hack should be fixed soon though
//
// I am just rotating around single axis for fingers, not taking abduct into account
// The thumb is a complete fudge.
// Wrist is not being done at all
vjGloveData::calcXforms()
{
   vjVec3 xAxis(1.0f, 0.0f, 0.0f);
   vjVec3 yAxis(0.0f, 1.0f, 0.0f);
   const float oneIn(1/12.0f);

   dims[INDEX][DIJ] = yAxis * (oneIn * 1.0f);
   dims[INDEX][PIJ] = yAxis * (oneIn * 1.5f);
   dims[INDEX][MPJ] = (yAxis * (oneIn * 2.5f)) + (xAxis * -0.75f);

   dims[MIDDLE][DIJ] = yAxis * (oneIn * 1.0f);
   dims[MIDDLE][PIJ] = yAxis * (oneIn * 1.5f);
   dims[MIDDLE][MPJ] = (yAxis * (oneIn * 2.5f)) + (xAxis * 0.0f);

   dims[RING][DIJ] = yAxis * (oneIn * 1.0f);
   dims[RING][PIJ] = yAxis * (oneIn * 1.5f);
   dims[RING][MPJ] = (yAxis * (oneIn * 2.5f)) + (xAxis * 0.3f);

   dims[PINKY][DIJ] = yAxis * (oneIn * 1.0f);
   dims[PINKY][PIJ] = yAxis * (oneIn * 1.5f);
   dims[PINKY][MPJ] = (yAxis * (oneIn * 2.5f)) + (xAxis * 0.4f);


   // ----------------------- //
   // ----- XFORMS ---------- //
   // ----------------------- //

   // THUMB
   // INDEX
   xforms[INDEX][DIJ].makeRot(angles[INDEX][DIJ],xAxis);
   xforms[INDEX][DIJ].preTrans(dims[INDEX][DIJ],xforms[INDEX][DIJ]);
   xforms[INDEX][PIJ].makeRot(angles[INDEX][PIJ],xAxis);
   xforms[INDEX][PIJ].preTrans(dims[INDEX][PIJ],xforms[INDEX][PIJ]);
   xforms[INDEX][MPJ].makeRot(angles[INDEX][MPJ],xAxis);
   xforms[INDEX][MPJ].preTrans(dims[INDEX][MPJ],xforms[INDEX][MPJ]);

   // MIDDLE
   xforms[MIDDLE][DIJ].makeRot(angles[MIDDLE][DIJ],xAxis);
   xforms[MIDDLE][DIJ].preTrans(dims[MIDDLE][DIJ],xforms[MIDDLE][DIJ]);
   xforms[MIDDLE][PIJ].makeRot(angles[MIDDLE][PIJ],xAxis);
   xforms[MIDDLE][PIJ].preTrans(dims[MIDDLE][PIJ],xforms[MIDDLE][PIJ]);
   xforms[MIDDLE][MPJ].makeRot(angles[MIDDLE][MPJ],xAxis);
   xforms[MIDDLE][MPJ].preTrans(dims[MIDDLE][MPJ],xforms[MIDDLE][MPJ]);

   // RING
   xforms[RING][DIJ].makeRot(angles[RING][DIJ],xAxis);
   xforms[RING][DIJ].preTrans(dims[RING][DIJ],xforms[RING][DIJ]);
   xforms[RING][PIJ].makeRot(angles[RING][PIJ],xAxis);
   xforms[RING][PIJ].preTrans(dims[RING][PIJ],xforms[RING][PIJ]);
   xforms[RING][MPJ].makeRot(angles[RING][MPJ],xAxis);
   xforms[RING][MPJ].preTrans(dims[RING][MPJ],xforms[RING][MPJ]);

   // PINKY
   xforms[PINKY][DIJ].makeRot(angles[PINKY][DIJ],xAxis);
   xforms[PINKY][DIJ].preTrans(dims[PINKY][DIJ],xforms[PINKY][DIJ]);
   xforms[PINKY][PIJ].makeRot(angles[PINKY][PIJ],xAxis);
   xforms[PINKY][PIJ].preTrans(dims[PINKY][PIJ],xforms[PINKY][PIJ]);
   xforms[PINKY][MPJ].makeRot(angles[PINKY][MPJ],xAxis);
   xforms[PINKY][MPJ].preTrans(dims[PINKY][MPJ],xforms[PINKY][MPJ]);

   // WRIST

   return 1;
}


////////////////////////////////////////////////////////////////////////
//                          vjGlove                                   //
////////////////////////////////////////////////////////////////////////
vjGlove::vjGlove()
{ deviceAbilities = deviceAbilities | DEVICE_GLOVE; }

vjGlove::vjGlove(vjConfigChunk* chunk)
   : vjInput(chunk)
{ deviceAbilities = deviceAbilities | DEVICE_GLOVE; }


// Just get the data from the current vector entry
float vjGlove::getGloveAngle(vjGloveData::vjGloveComponent component,
                             vjGloveData::vjGloveJoint joint, int devNum)
{
   return mTheData[devNum][current].angles[component][joint];
}

//: This returns a vector ponting "out" of the component
// Can be used for selection, etc.
vjVec3 vjGlove::getGloveVector(vjGloveData::vjGloveComponent component, int devNum)
{
   return vjVec3(1.0,0.0,0.0);
}

// For now we will not return anything valid
vjVec3 vjGlove::getGlovePos(vjGloveData::vjGloveComponent component, int devNum)
{
   return vjVec3(1.0, 0.0, 0.0);
}

// Grab a copy of the most current glove data
vjGloveData vjGlove::getGloveData(int devNum)
{
   return mTheData[devNum][current];
}

// Create a list of angles
// Return it to the user
vector<float> vjGlove::getGloveAngles(int devNum)
{
   vector<float> angles(vjGloveData::NUM_COMPONENTS*vjGloveData::NUM_JOINTS);

   // Copy the angles
   for(int i=0;i<vjGloveData::NUM_COMPONENTS;i++)
      for(int j=0;j<vjGloveData::NUM_JOINTS;j++)
         angles.push_back(mTheData[devNum][current].angles[i][j]);

   return angles;
}

