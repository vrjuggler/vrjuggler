#include <vrj/Display/SimViewport.h>

#include <vrj/Kernel/User.h>

#include <vrj/Display/CameraProjection.h>
#include <vrj/Display/Projection.h>
#include <gadget/Type/Position/PositionUnitConversion.h>

#include <vrj/Draw/OGL/GlBasicSimulator.h>

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

   bool has_simulator(false);
   has_simulator = chunk->getProperty<bool>("hasSimPlugin");
   mSimulator = NULL;

   // Create the simulator stuff
   if(has_simulator)
   {
      jccl::ConfigChunkPtr sim_chunk = chunk->getProperty<jccl::ConfigChunkPtr>("simPlugIn");

      // XXX: Use factory to create the correct type of simulator
      mSimulator = new GlBasicSimulator;
      mSimulator->initialize(this);
      mSimulator->config(sim_chunk);
   }

}

void SimViewport::updateProjections(const float positionScale)
{
   if(mSimulator != NULL)
      mSimulator->updateProjectionData(positionScale, mLeftProj, mRightProj);
}

}

