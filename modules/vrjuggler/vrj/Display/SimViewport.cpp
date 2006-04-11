#include <vrj/Display/SimViewport.h>

#include <gmtl/Matrix.h>
#include <gmtl/Generate.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>
#include <gmtl/Xforms.h>
#include <gmtl/Output.h>

#include <vrj/Kernel/User.h>

#include <vrj/Display/CameraProjection.h>
#include <vrj/Display/Projection.h>
#include <gadget/Type/Position/PositionUnitConversion.h>

namespace vrj
{

   /**  Configure the simulator */
void SimViewport::config(jccl::ConfigChunkPtr chunk)
{
   vprASSERT(chunk.get() != NULL);
   vprASSERT(chunk->getDescToken() == std::string("simViewport"));

   Viewport::config(chunk);

   mType = SIM;

   mLeftProj = new CameraProjection;
   mLeftProj->config(chunk);
   mLeftProj->setEye(Projection::LEFT);
   mLeftProj->setViewport(this);

   mRightProj = new CameraProjection;
   mRightProj->config(chunk);
   mRightProj->setEye(Projection::RIGHT);
   mRightProj->setViewport(this);

   std::string camera_proxy_str = chunk->getProperty<std::string>("cameraPos");
   std::string wand_proxy_str = chunk->getProperty<std::string>("wandPos");

   mCamera.init(camera_proxy_str);
   mWand.init(wand_proxy_str);      // Configure the wand to use

   if(!mCamera.isConnected())
   {
      vprDEBUG(vprDBG_ERROR,0)
         << clrOutNORM(clrRED,"ERROR:")
         << "vjSimViewport:: Fatal Error: Camera not found named: "
         << camera_proxy_str.c_str() << vprDEBUG_FLUSH;
      vprASSERT(false);
   }

   // Get drawing parameters
   mDrawProjections = chunk->getProperty<bool>("drawProjections");
   mSurfaceColor[0] = chunk->getProperty<float>("surfaceColor", 0);
   mSurfaceColor[1] = chunk->getProperty<float>("surfaceColor", 1);
   mSurfaceColor[2] = chunk->getProperty<float>("surfaceColor", 2);
}

void SimViewport::updateProjections(const float positionScale)
{
   updateInternalData(positionScale);
   gmtl::Matrix44f camera_pos = getCameraPos();
   gmtl::Vec3f camera_trans = gmtl::makeTrans<gmtl::Vec3f>(camera_pos);

   //mCameraProj->calcViewMatrix(camera_pos);

   gmtl::Matrix44f left_eye_pos, right_eye_pos;     // NOTE: Eye coord system is -z forward, x-right, y-up

   // -- Calculate camera (eye) Positions -- //
   vprDEBUG(vprDBG_ALL,7)
      << "vjDisplay::updateProjections: Getting cam position" << std::endl
      << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL,7) << "CamPos:" << camera_trans << std::endl << vprDEBUG_FLUSH;

   // Compute location of left and right eyes
   float interocular_dist = mUser->getInterocularDistance();
   interocular_dist *= positionScale;               // Scale into correct units
   float eye_offset = interocular_dist / 2.0f;      // Distance to move eye

   left_eye_pos = camera_pos * gmtl::makeTrans<gmtl::Matrix44f>( gmtl::Vec3f(-eye_offset, 0.0f, 0.0f) );
   right_eye_pos = camera_pos * gmtl::makeTrans<gmtl::Matrix44f>( gmtl::Vec3f(eye_offset, 0.0f, 0.0f) );

   mLeftProj->calcViewMatrix(left_eye_pos, positionScale);
   mRightProj->calcViewMatrix(right_eye_pos, positionScale);
}

/**  Update internal simulator data */
void SimViewport::updateInternalData(float positionScale)
{
   mHeadPos = mUser->getHeadPosProxy()->getData(positionScale);
   mWandPos = mWand->getData(positionScale);

   mCameraPos = mCamera->getData(positionScale);
   gmtl::invert(mCameraPos);
}

}

