/*
 *  File:          $RCSfile$
 *  Date modified: $Date$
 *  Version:       $Revision$
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
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _VJ_PROJECTION_
#define _VJ_PROJECTION_

#include <vjConfig.h>
#include <Math/vjMatrix.h>
#include <Kernel/vjFrustum.h>
#include <Kernel/vjDebug.h>
#include <Math/vjVec3.h>
#include <Input/vjPosition/vjPosition.h>

//------------------------------------------------------------------
//: Pure virtual base class for viewport definitions.
//
// Responsible for storing and computing projection
//  information based upon an eye positions
// This class is an abstract base class for other classes
//  that actually compute the projections.
//
// @author Allen Bierbaum
//  Date: 9-8-97
//------------------------------------------------------------------
class vjProjection
{
public:
   // Eye and type
   enum
   {LEFT = 1, RIGHT = 2, SURFACE, SIM};

public:
   vjProjection()
   {
      mType = -1;
      mEye = 0;
      mFocusPlaneDist = 1.0f;
   }

   virtual void config(vjConfigChunk* chunk)
   {;}

   void setEye(int _eye)
   { mEye = _eye; }

   int getEye()
   { return mEye;}

   virtual void calcViewMatrix(vjMatrix& eyePos) = 0;

   //: Helper to the frustum apex and corners in model coordinates
   //!NOTE: This function is meant for debugging purposes
   //!POST: The given vars contain the values of the frustums
   //+ corners in model space.
   void getFrustumApexAndCorners(vjVec3& apex, vjVec3& ur, vjVec3& lr, vjVec3& ul, vjVec3& ll);

   //: Virtual output oporators.
   // Every class derived from us shoudl just define this, and
   // the opertetor<< will "just work"
   virtual ostream& outStream(ostream& out);

   friend ostream& operator<<(ostream& out, vjProjection& proj);


public:
   vjMatrix    mViewMat;
   vjFrustum   mFrustum;

protected:
   int mEye;
   int mType;

   float       mFocusPlaneDist;     // Basically the distance to the surface.  Needed for drawing.

protected:     // Statics
   static float mNearDist;
   static float mFarDist;    // Near far distances

public:
   static void setNearFar(float near_val, float far_val)
   {
      mNearDist = near_val;
      mFarDist = far_val;
   }
};



#endif
