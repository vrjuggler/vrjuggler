/*
  @file    tools/plugin/vrjuggler/MotionNode.cpp
  @author  Luke Tokheim, luke@motionnode.com
  @version 1.0

  (C) Copyright GLI Interactive LLC 2007. All rights reserved.

  The coded instructions, statements, computer programs, and/or related
  material (collectively the "Data") in these files contain unpublished
  information proprietary to GLI Interactive LLC, which is protected by
  US federal copyright law and by international treaties.

  The Data may not be disclosed or distributed to third parties, in whole
  or in part, without the prior written consent of GLI Interactive LLC.

  The Data is provided "as is" without express or implied warranty, and
  with no claim as to its suitability for any purpose.
*/
#include "MotionNode.h"

#include <gadget/gadgetParam.h>
#include <gadget/InputManager.h>
#include <gadget/Type/DeviceConstructor.h>
#include <gadget/Util/Debug.h>

#include <gmtl/Generate.h>
#include <gmtl/Quat.h>


extern "C"
{

GADGET_DRIVER_EXPORT vpr::Uint32 getGadgeteerVersion()
{
  return __GADGET_version;
}

GADGET_DRIVER_EXPORT void initDevice(gadget::InputManager * inputMgr)
{
  new gadget::DeviceConstructor<gadget::MotionNode>(inputMgr);
}

} // extern "C"


namespace gadget {

// Static Manager instance, on demand.
::MotionNode::SDK::Device::Manager<MotionNode::sampler_type> & StaticManager()
{
  static ::MotionNode::SDK::Device::Manager<MotionNode::sampler_type> manager;
  return manager;
}


MotionNode::MotionNode()
  : mSampler("", 0)
{
  vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
    << "*** MotionNode::MotionNode() ***\n" << vprDEBUG_FLUSH;
}

MotionNode::~MotionNode()
{
  vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
    << "*** MotionNode::~MotionNode() ***\n" << vprDEBUG_FLUSH;

  // Always call the stop method to
  // delete the thread object.
  //stopSampling();
  try {
    // This is the equivalent of stopSampling(),
    // except do not print any warnings or even check
    // for error conditions.
    StaticManager().detach(mSampler);
  } catch (std::exception &) {
  }

}

bool MotionNode::config(jccl::ConfigElementPtr e)
{
  vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
    << "*** MotionNode::config() ***\n"
    << vprDEBUG_FLUSH;

  // Configure all our base classes first.  If any of those fail,
  // we cannot finish configuring ourself.
  if (!gadget::Input::config(e) && !gadget::Position::config(e))
  {
    return false;
  }

  // Read configuration parameters. Fall back on default hard
  // coded values.
  std::string address = "";
  std::size_t port = 32079;
  try {
    address = e->getProperty<std::string>("address");
    port = e->getProperty<std::size_t>("port");
  } catch (std::exception &) {
    vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
      << "MotionNode driver failed to parse input parameters\n"
      << vprDEBUG_FLUSH;
  }


  mSampler = sampler_type(address, port, boost::bind(&MotionNode::sample, this));

  return true;
}

bool MotionNode::startSampling()
{
  bool result = false;

  try {
    if (StaticManager().attach(mSampler))
    {
      // Attempt to read a single sample from
      // the stream. Use a 1 second blocking call.
      sampler_type::data_type data;
      if (mSampler.get_data_block(data, 1)) {
        result = true;
      } else {
        vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
          << "MotionNode driver failed to start sampling, device not currently reading\n"
          << vprDEBUG_FLUSH;
      }
    }
  } catch (std::exception & e) {
    vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
      << e.what()
      << "\n"
      << vprDEBUG_FLUSH;
  }

  return result;
}

bool MotionNode::stopSampling()
{
  bool result = false;

  try {
    if (StaticManager().detach(mSampler))
    {
      result = true;
    }
  } catch (std::exception & e) {
    vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
      << e.what()
      << "\n"
      << vprDEBUG_FLUSH;
  }

  return result;
}

bool MotionNode::sample()
{
  bool result = false;

  sampler_type::Data data;
  if (mSampler.get_data(data) && !data.empty())
  {
    std::vector<PositionData> sample(data.size());

    // Copy the MotionNode SDK preview data map into an
    // array of VRJ PositionData.
    {
      std::size_t index = 0;
      for (sampler_type::Data::iterator itr=data.begin(); itr!=data.end(); ++itr)
      {
        std::vector<float> q = itr->second.getQuaternion(false);

        gmtl::Quatf quatValue(q[1], q[2], q[3], q[0]);
        gmtl::set(sample[index].editValue(), quatValue);

        index++;
      }
    }

    addPositionSample(sample);
  }

  return result;
}

void MotionNode::updateData()
{
  if (mSampler.is_reading()) {
    swapPositionBuffers();
  }
}

void MotionNode::operator delete(void* p)
{
  ::operator delete(p);
}

std::string MotionNode::getElementType()
{
  return "motionnode";
}

void MotionNode::destroy()
{
  delete this;
}

} // namespace gadget
