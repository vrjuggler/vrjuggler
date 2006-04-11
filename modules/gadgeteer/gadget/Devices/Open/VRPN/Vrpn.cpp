/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

//=====================================================
// Vrpn
//
// Purpose:
//	VR Juggler VRPN
//
// Author:
//	Jason Jerald   8-26-02
// Revisions:
//      Ported to to 1.1 (DR2) and sgi platform Anthony Steed, 10-4-02. 
//      Simplified to one class to deal with threading issues.
//
// ToDo:
//   Should we fetch the room->tracker_base and tracker->hand transforms
//    from the VRPN config system, or always set them in VRJuggler. Or just 
//    set room->juggler in VRJuggler?
//
//   Analog and other VRPN devices. 
//
//   Tidy up variables, there is some redundancy
//
//   Consider doing pre-rotate in posiiton proxy tracker offset. Though 
//    note there are no handedness changes in position proxy.
//======================================================

#include <gadget/Devices/DriverConfig.h>

#include <boost/concept_check.hpp>

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/EulerAngle.h>
#include <gmtl/Quat.h>
#include <gmtl/AxisAngle.h>

#include <vpr/vpr.h>
#include <vpr/Util/Debug.h>
#include <jccl/Config/ConfigChunk.h>
#include <gadget/InputManager.h>
#include <gadget/Type/DeviceConstructor.h>

#include <gadget/Devices/Open/VRPN/Vrpn.h>

#define VRPN_DEBUG 0
				// 1 == reporting
				// 2 == misc


void initDevice(gadget::InputManager* inputMgr)
{
   new gadget::DeviceConstructor<gadget::Vrpn>(inputMgr);
}

namespace gadget
{
    
  void staticHandleTracker(void *userdata, vrpn_TRACKERCB t)
  {
    
#if (VRPN_DEBUG&1)
    std::cout << "HandleTracker" << std::endl;
#endif

    gadget::Vrpn *this_ptr = static_cast<gadget::Vrpn*>(userdata);
    this_ptr->handleTracker(t);
  }
  
  void staticHandleButton(void *userdata, vrpn_BUTTONCB b)
  {
#if (VRPN_DEBUG&1)
    std::cout << "HandleButton" << std::endl;
#endif
    gadget::Vrpn *this_ptr = static_cast<gadget::Vrpn*>(userdata);
    this_ptr->handleButton(b);
  }
  

  bool Vrpn::config(jccl::ConfigChunkPtr c)
  {

    if(! (Input::config(c) && Position::config(c)))
      return false;
    
    std::cerr << "***************** config ******* " << std::endl;

    // tracking
    mTrackerServer = c->getProperty<std::string>("trackerServer");
    if(mTrackerServer == std::string(""))
      {
	std::cerr << "vrpn tracker server name not set!!!!!!" << std::endl;
      }
    else
      {
	std::cerr << "vrpn tracker server name set to " << mTrackerServer << std::endl;
      }

    mTrackerNumber = c->getProperty<int>("numTrackers");

    // button
    mButtonServer = c->getProperty<std::string >("buttonServer");
    if(mButtonServer == std::string(""))
      {
	std::cerr << "vrpn button server name not set!!!!!!" << std::endl;
      }
    else
      {
	std::cerr << "vrpn button server name set to " << mButtonServer << std::endl;
      }
    
    mButtonNumber = c->getProperty<int>("numButtons");

    mPositions.resize(mTrackerNumber);
    mQuats.resize(mTrackerNumber);
    mButtons.resize(mButtonNumber);

    mCurPositions.resize(mTrackerNumber);
    mCurButtons.resize(mButtonNumber);

    return true;
  }

  Vrpn::~Vrpn()
  {
    stopSampling();
  }


  int Vrpn::startSampling()
  {
    int status(0);
    
    
    std::cout << "Going to create fine\n";
    if ( NULL == mReadThread ) {
      vpr::ThreadMemberFunctor<Vrpn>* read_func =
	new vpr::ThreadMemberFunctor<Vrpn>(this,
					   &Vrpn::readLoop,
					   NULL);
      mReadThread = new vpr::Thread(read_func); 
      if ( mReadThread->valid() ) {
	status = 1; // thread creation succeded
	std::cout << "Created fine\n";
      }
      else {
	std::cout << "Not fine\n";
      }
    } 
    else {
	std::cout << "Not null\n";
    }

    return status;
  }


  void Vrpn::readLoop(void *nullParam)
  {
    boost::ignore_unused_variable_warning(nullParam);

    vrpn_Tracker_Remote *tracker;
    vrpn_Button_Remote *button;
    
    tracker = new vrpn_Tracker_Remote(mTrackerServer.c_str());
    button = new vrpn_Button_Remote(mButtonServer.c_str());
    tracker->register_change_handler((void *) this, staticHandleTracker);
    button->register_change_handler((void *) this, staticHandleButton);
    
    // loop through  and keep sampling
    while(1)
      {
	tracker->mainloop();
	button->mainloop();
	sample();
      }
  }

  void Vrpn::handleTracker(vrpn_TRACKERCB t)
  {	
    if (t.sensor > mTrackerNumber) {
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
	<< "Vrpn: tracker " << t.sensor 
	<< " out of declared range ("<<mPositions.size()<<")"<<std::endl
	<< vprDEBUG_FLUSH;
      mPositions.resize(t.sensor);    
      mQuats.resize(t.sensor);    
    }
  
#if (VRPN_DEBUG&1)
    std::cout << "Tracker #"<<t.sensor<< " quat " <<
      mQuats[t.sensor][0] << " " <<
      mQuats[t.sensor][1] << " " <<
      mQuats[t.sensor][2] << " " <<
      mQuats[t.sensor][3] << " " << std::endl;
  
#endif
  
    mQuats[t.sensor][0] = t.quat[0];
    mQuats[t.sensor][1] = t.quat[1];
    mQuats[t.sensor][2] = t.quat[2];
    mQuats[t.sensor][3] = t.quat[3];
  
    mPositions[t.sensor][0] = t.pos[0];
    mPositions[t.sensor][1] = t.pos[1];
    mPositions[t.sensor][2] = t.pos[2];
  }
  
  void Vrpn::handleButton(vrpn_BUTTONCB b)
  {
    if (b.button > mButtonNumber) {
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
	<< "Vrpn: button " << b.button 
	<< " out of declared range ("<<mButtons.size()<<")"<<std::endl
	<< vprDEBUG_FLUSH;
      mButtons.resize(b.button);    
    }
#if (VRPN_DEBUG&1)
    std::cout << "Button #"<<b.button<< " state " <<
      b.state << " " << std::endl;
  
#endif
    mButtons[b.button] = b.state;
  }


  int Vrpn::sample()
  {
      for(int i=0;i<mTrackerNumber;i++)
      {
         mCurPositions[i].setPosition(getSensorPos(i));
         mCurPositions[i].setTime();
      }
    
    for (int i=0;i<mButtonNumber;i++)
      {
	mCurButtons[i] = getDigitalData(i);
	mCurButtons[i].setTime();
      }
    
    // Update the data buffer
    addPositionSample(mCurPositions);
    addDigitalSample(mCurButtons);

    return 1;
  }


  int Vrpn::stopSampling()
  {
    return 1;
  }
  
  void Vrpn::updateData()
  {
    // Swap it
    swapPositionBuffers();
    swapDigitalBuffers();

  }

  gmtl::Matrix44f Vrpn::getSensorPos(int d)
  {
    gmtl::Matrix44f ret_val;
    gmtl::setRot( ret_val, mQuats[d]);
    gmtl::setTrans( ret_val, mPositions[d]);
    return ret_val;
  }
  
  gadget::DigitalData Vrpn::getDigitalData(int d)
  {
    return mButtons[d];
  }

} // End of gadget namespace
