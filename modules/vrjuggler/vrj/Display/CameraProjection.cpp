#include <vrj/Display/CameraProjection.h>

#include <jccl/Config/ConfigChunk.h>

#include <gmtl/Output.h>
#include <gmtl/Generate.h>

#include <vrj/Display/Display.h>


namespace vrj
{

void CameraProjection::config(jccl::ConfigChunkPtr chunk)
{
   Projection::config(chunk);

   vprASSERT( (chunk->getDescToken() == std::string("simDisplay")) ||
              (chunk->getDescToken() == std::string("simViewport")) );

   // Set fov and make sure that it is not 0.0
   mVertFOV = chunk->getProperty<float>("vert_fov");
   if(mVertFOV == 0.0f)
      mVertFOV = 60.0f;
}

/** Calculate the viewmatrix and frustum for the camera
* Calculates a perspective transform for the given settings.
* Auto-calculates aspect ratio from the current size of the window and viewport
*/
void CameraProjection::calcViewMatrix(gmtl::Matrix44f& cameraPos)
{
   mViewMat = cameraPos;

   gmtl::Vec3f camera_trans = gmtl::makeTrans<gmtl::Vec3f>(cameraPos);
   vprDEBUG(vprDBG_ALL,6) << "calcView: Cam pos:" << camera_trans << std::endl << vprDEBUG_FLUSH;

   int win_xo, win_yo, win_xs, win_ys;  // origin and size of display window
   float vp_xo, vp_yo, vp_xs, vp_ys;  // origin and size of viewport
   float width, height;
   float aspect_ratio;

   getViewport()->getDisplay()->getOriginAndSize(win_xo,win_yo, win_xs, win_ys);
   getViewport()->getOriginAndSize( vp_xo, vp_yo, vp_xs, vp_ys );
   width = float(win_xs) * vp_xs;
   height = float(win_ys) * vp_ys;

   aspect_ratio = (width/height);

   // Calculate frustum
   float top, right;

   top = gmtl::Math::tan( gmtl::Math::deg2Rad(mVertFOV/2.0f) ) * mNearDist;     // Calculate the top based on fovy
   right = aspect_ratio * top;

   // XXX: The frustum is not used
   mFrustum.set(-right, right, -top, top, mNearDist, mFarDist);
}

}
