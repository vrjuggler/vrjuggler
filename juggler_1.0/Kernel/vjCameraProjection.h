#ifndef _VJ_CAMERA_PROJECTION_H_
#define _VJ_CAMERA_PROJECTION_H_

#include <config.h>
#include <Kernel/vjProjection.h>
class vjMatrix;

//: Projection class that simply takes a matrix for the camera position
//
class vjCameraProjection : public vjProjection
{
public:
   virtual void config(vjConfigChunk* chunk)
   {
      vjProjection::config(chunk);
   }

   virtual void calcViewMatrix(vjMatrix& cameraPos)
   {
      viewMat = cameraPos;
      frustum.set(-0.6, 0.6, -0.6, 0.6, 0.1, 1000);
   }

protected:
   virtual void surfaceSet()
   {
      ;
   }
};

#endif
