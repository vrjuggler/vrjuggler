/*
  @file    tools/plugin/vrjuggler/MotionNode.h
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
#ifndef _GADGET_MOTIONNODE_H_
#define _GADGET_MOTIONNODE_H_

#include <gadget/Devices/DriverConfig.h>

#include <gadget/Type/InputBaseTypes.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/Position.h>

/**
  If you prefer to link to the DLL versions
  of the Boost library.
*/
#define BOOST_DYN_LINK 1

/**
  From the MotionNode SDK, available at
  http://www.motionnode.com/sdk.html.
*/
#include <plugin/Device.hpp>


namespace gadget {

/**
  Gadgeteer driver for all MotionNode devices. Depends
  on the MotionNode Software Development Kit (SDK).
*/
class MotionNode : public input_position_t {
public:
  typedef ::MotionNode::SDK::Device::Sampler<> sampler_type;

  MotionNode();
  virtual ~MotionNode();

  bool config(jccl::ConfigElementPtr e);

  /** Begins sampling. */
  bool startSampling();

  /** Stops sampling. */
  bool stopSampling();

  /** Samples data. */
  bool sample();

  /** Updates to the sampled data. */
  void updateData();

  /**
  * Invokes the global scope delete operator.  This is required for proper
  * releasing of memory in DLLs on Win32.
  */
  void operator delete(void* p);

  /** Returns what element type is associated with this class. */
  static std::string getElementType();
protected:
  /**
  * Deletes this object.  This is an implementation of the pure virtual
  * gadget::Input::destroy() method.
  */
  virtual void destroy();

private:
  /** Interface to attach to a MotionNode data stream. */
  sampler_type mSampler;
}; // class MotionNode

} // namespace gadget

#endif // _GADGET_MOTIONNODE_H_
