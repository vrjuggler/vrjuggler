/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#include <Kernel/vjProjection.h>

float vjProjection::mNearDist = 0.1f;
float vjProjection::mFarDist = 10000.0f;


//: Helper to the frustum apex and corners in model coordinates
//!NOTE: The normal frustum is in camera (clip) coordinates
//+      and the model is in model (eye) coordinates.
//+      The matrix viewMat transforms from eye to clip
void vjProjection::getFrustumApexAndCorners(vjVec3& apex, vjVec3& ur, vjVec3& lr, vjVec3& ul, vjVec3& ll)
{
   vjMatrix viewMatInv;
   viewMatInv.invert(viewMat);   // Get the inverse matrix

   //float near_dist = mFocusPlaneDist;
   // User like triangles to get the params for the focus surface
   float mult_factor = mFocusPlaneDist/frustum[vjFrustum::VJ_NEAR];
   float bot = frustum[vjFrustum::VJ_BOTTOM]*mult_factor;
   float left = frustum[vjFrustum::VJ_LEFT]*mult_factor;
   float top = frustum[vjFrustum::VJ_TOP]*mult_factor;
   float right = frustum[vjFrustum::VJ_RIGHT]*mult_factor;

   // Create points in clip space
   vjVec3 apexClip(0.0f, 0.0f, 0.0f);
   vjVec3 urClip(right, top, -mFocusPlaneDist);
   vjVec3 lrClip(right, bot, -mFocusPlaneDist);
   vjVec3 ulClip(left, top, -mFocusPlaneDist);
   vjVec3 llClip(left, bot, -mFocusPlaneDist);

   apex.xformFull(viewMatInv, apexClip);
   ur.xformFull(viewMatInv, urClip);
   lr.xformFull(viewMatInv, lrClip);
   ul.xformFull(viewMatInv, ulClip);
   ll.xformFull(viewMatInv, llClip);
}


ostream& vjProjection::outStream(ostream& out)
{
   out << "eye: ";
   switch(mEye)
   {
   case vjProjection::LEFT:
      out << "Left";
      break;
   case vjProjection::RIGHT:
      out << "Right";
      break;
   }
   out << "\nFrustum: " << frustum << endl;
   return out;
}

ostream& operator<<(ostream& out, vjProjection& proj)
{
   return proj.outStream(out);
}
