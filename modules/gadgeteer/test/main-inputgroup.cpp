/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */

#include <iostream.h>
#include <Input/InputManager/vjInputManager.h>
#include <Input/ibox/hci.h>
#include <Input/ibox/ibox.h>
#include <Input/vjPosition/vjFlock.h>
#include <Input/vjInput/vjIbox.h>
#include <SharedMem/vjMemPool.h>

int main()
{
   vjMemPool* shared_pool = new vjSharedPool(1024*1024);
   vjMatrix pos_data;
   vjInputManager* input_manager = new(shared_pool) vjInputManager ;

      // --- Create Dummy -- //
   cout << "\nCreating myDummy: vjDummyPosition w/X=5.0" << endl;
   vjDummyPosition* myDummy = new(shared_pool) vjDummyPosition;
   myDummy->SetX(5.0);

      // -- get default pos data -- //
   cout << "get the position data for proxy 0:" << endl;
   data = input_manager->getPosData(pos_data, 0);
   cout  << "  x:" << data->x 
         << "  y:" << data->y 
         << "  z:" << data->z << endl;

      // --- Add dummy to input group -- //
   cout << "\nAdding myDummy to InputGroup" << endl;
   int devNum = input_manager->fAddDevice(myDummy);
   cout << "Device added as devNum: " << devNum << endl;
   cout << "\nSet proxy 0 to devNum, subnumber 0" << endl;
   input_manager->setPosProxy(0,devNum,0);

      // --- Now get Dummy's data -- //
   cout << "Now get the positional data from posproxy0:" << endl;
   data = input_manager->getPosData(0);
   cout  << "  x:" << data->x 
         << "  y:" << data->y 
         << "  z:" << data->z << endl;


   cout << "\ngetDevice(devNum):" << (input_manager->getDevice(devNum)) << endl;
   (input_manager->getDevice(devNum))->UpdateData();
   cout << "\nget the devicename: " << (input_manager->getDevice(devNum))->getDeviceName() << endl;

   cout << "Doing UpdateAllData:" << flush;
   input_manager->updateAllData();
   cout << "..Done" << endl;

//  cout << "get an invalid device: ";
//  cout << (input_manager->getDevice(3)) << endl;

   cout << "\ntype something: " << flush ;
   cin.get();

#ifdef TRACKERS
   vjFlock* aFlock = new (shared_pool) vjFlock;
   aFlock->startSampling();
   devNum = input_manager->fAddDevice(aFlock);
   input_manager->SetPosProxy(1,devNum,0);
   input_manager->SetPosProxy(2,devNum,1);

   cout << "Trackers set up.. " << endl;
   //sleep(5);

   vjPOS_DATA *data1,*data2,*data0;

   for (int i = 0; i < 5; i++)
   {
      input_manager->UpdateAllData();
      data0 = input_manager->getPosData(0);
      data1 = input_manager->getPosData(1);
      data2 = input_manager->getPosData(2);
      cout << endl;
      cout << "X: " << data0->x << "  " << data1->x << "  " << data2->x << endl
      << "Y: " << data0->y << "  " << data1->y << "  " << data2->y << endl
      << "Z: " << data0->z << "  " << data1->z << "  " << data2->z << endl;

      sleep(1);
   }

   aFlock->stopSampling();
#endif
   
   vjIBox *myibox = new (shared_pool) vjIBox;
   myibox->StartSampling();
   cout << "ibox is: " << myibox->getDeviceName();
   C2Dinput_manageritalProxy dp1(myibox,0);
   devNum = input_manager->FAddDevice(myibox);
   input_manager->SetDinput_managerProxy(0,devNum,0);
   input_manager->SetDinput_managerProxy(1,devNum,1);
   input_manager->SetDinput_managerProxy(2,devNum,2);

   for (int i = 0; i < 20; i++)
   {
      input_manager->UpdateAllData();
      cout << "dinput_managerital0: " << input_manager->getDinput_managerData(0);
      cout << " dinput_managerital1: " << input_manager->getDinput_managerData(1);
      cout << " dinput_managerital2: " << input_manager->getDinput_managerData(2) << endl;
      sleep(1);
   }


   myibox->stopSampling();
   delete myibox;
   // delete aFlock;
   delete myDummy;

   delete input_manager;
   delete shared_pool;

   return 0;
} 
