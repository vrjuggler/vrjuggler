/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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

#include <iostream>
#include <vrj/Input/InputManager.h>
#include <vrj/Input/Devices/Immersion/hci.h>
#include <vrj/Input/Devices/Immersion/iboxStandalone.h>
#include <vrj/Input/Devices/Ascension/Flock.h>
#include <vrj/Input/Devices/Immersion/Ibox.h>
#include <vpr/System.h>

int main()
{
   vrj::Matrix pos_data;
   vrj::InputManager* input_manager = new vrj::InputManager ;

      // --- Create Dummy -- //
   std::cout << "\nCreating myDummy: DummyPosition w/X=5.0" << std::endl;
   vrj::DummyPosition* myDummy = new vrj::DummyPosition;
   myDummy->SetX(5.0);

      // -- get default pos data -- //
   std::cout << "get the position data for proxy 0:" << std::endl;
   data = input_manager->getPosData(pos_data, 0);
   std::cout  << "  x:" << data->x 
              << "  y:" << data->y 
              << "  z:" << data->z << std::endl;

      // --- Add dummy to input group -- //
   std::cout << "\nAdding myDummy to InputGroup" << std::endl;
   int devNum = input_manager->fAddDevice(myDummy);
   std::cout << "Device added as devNum: " << devNum << std::endl;
   std::cout << "\nSet proxy 0 to devNum, subnumber 0" << std::endl;
   input_manager->setPosProxy(0,devNum,0);

      // --- Now get Dummy's data -- //
   std::cout << "Now get the positional data from posproxy0:" << std::endl;
   data = input_manager->getPosData(0);
   std::cout  << "  x:" << data->x 
              << "  y:" << data->y 
              << "  z:" << data->z << std::endl;


   std::cout << "\ngetDevice(devNum):" << (input_manager->getDevice(devNum))
             << std::endl;
   (input_manager->getDevice(devNum))->UpdateData();
   std::cout << "\nget the devicename: "
             << (input_manager->getDevice(devNum))->getDeviceName() 
             << std::endl;

   std::cout << "Doing UpdateAllData:" << std::flush;
   input_manager->updateAllData();
   std::cout << "..Done" << std::endl;

//  std::cout << "get an invalid device: ";
//  std::cout << (input_manager->getDevice(3)) << std::endl;

   std::cout << "\ntype something: " << std::flush;
   std::cin.get();

#ifdef TRACKERS
   vrj::Flock* aFlock = new vrj::Flock;
   aFlock->startSampling();
   devNum = input_manager->fAddDevice(aFlock);
   input_manager->SetPosProxy(1,devNum,0);
   input_manager->SetPosProxy(2,devNum,1);

   std::cout << "Trackers set up.. " << std::endl;
   //vjSystem::sleep(5);

   POS_DATA *data1,*data2,*data0;

   for (int i = 0; i < 5; i++)
   {
      input_manager->UpdateAllData();
      data0 = input_manager->getPosData(0);
      data1 = input_manager->getPosData(1);
      data2 = input_manager->getPosData(2);
      std::cout << std::endl;
      std::cout << "X: " << data0->x << "  " << data1->x << "  " << data2->x
                << std::endl
      << "Y: " << data0->y << "  " << data1->y << "  " << data2->y << endl
      << "Z: " << data0->z << "  " << data1->z << "  " << data2->z << endl;

      vpr::System::sleep(1);
   }

   aFlock->stopSampling();
#endif
   
   vrj::IBox *myibox = new vrj::IBox;
   myibox->startSampling();
   std::cout << "ibox is: " << myibox->getDeviceName();
   C2Dinput_manageritalProxy dp1(myibox,0);
   devNum = input_manager->FAddDevice(myibox);
   input_manager->SetDinput_managerProxy(0,devNum,0);
   input_manager->SetDinput_managerProxy(1,devNum,1);
   input_manager->SetDinput_managerProxy(2,devNum,2);

   for (int i = 0; i < 20; i++)
   {
      input_manager->UpdateAllData();
      std::cout << "dinput_managerital0: "
                << input_manager->getDinput_managerData(0);
      std::cout << " dinput_managerital1: "
                << input_manager->getDinput_managerData(1);
      std::cout << " dinput_managerital2: "
                << input_manager->getDinput_managerData(2) << std::endl;
      vpr::System::sleep(1);
   }


   myibox->stopSampling();
   delete myibox;
   // delete aFlock;
   delete myDummy;

   delete input_manager;

   return 0;
} 
