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
//	Responsible for storing and computing projection
//  information.
//
// @author Allen Bierbaum
//  Date: 9-8-97
//------------------------------------------------------------------
class vjProjection
{
public:
   enum
   {LEFT = 1, RIGHT = 2, FRONT, FLOOR, BACK, CEILING, USER};

public:
   vjProjection()
   {
      surface = -1;
   }

   virtual void config(vjConfigChunk* chunk)
   {
      char* proj  = chunk->getProperty("projectiontype");
      vjASSERT(proj != NULL);    // Check for a seg fault error
      setSurface(proj);
   }

   void setSurface(char* projStr)
   {
      vjDEBUG(2) << "vjProjection::setSurface: Setting surface:" << projStr << endl << vjDEBUG_FLUSH;
      if (strcasecmp(projStr, "left") == 0)
         setSurface(vjProjection::LEFT);
      else if (strcasecmp(projStr, "right") == 0)
         setSurface(vjProjection::RIGHT);
      else if (strcasecmp(projStr, "front") == 0)
         setSurface(vjProjection::FRONT);
      else if (strcasecmp(projStr, "floor") == 0)
         setSurface(vjProjection::FLOOR);
      else if (strcasecmp(projStr, "back") == 0)
         setSurface(vjProjection::BACK);
      else if (strcasecmp(projStr, "ceiling") == 0)
         setSurface(vjProjection::CEILING);
      else if (strcasecmp(projStr, "user") == 0)
         setSurface(vjProjection::USER);
   }

   virtual void setSurface(int _surface)
   {
      surface = _surface;
      this->surfaceSet();
   }

   int   getSurface()
   { return surface;}

   void setEye(int _eye)
   {
      eye = _eye;
   }

   int getEye()
   { return eye;}

   virtual void calcViewMatrix(vjMatrix& eyePos) = 0;

   friend ostream& operator<<(ostream& out, vjProjection& proj);

protected:
   /**  Must be Called whenever surface is changed by class.
    *	Used by sub-classes to do any updates that need to happen.
    */
   virtual void surfaceSet() = 0;

public:
   vjMatrix    viewMat;
   vjFrustum   frustum;

protected:
   int surface;
   int eye;
};


#endif
