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

      // -- Get default pos data -- //
   cout << "Get the position data for proxy 0:" << endl;
   data = input_manager->GetPosData(pos_data, 0);
   cout  << "  x:" << data->x 
         << "  y:" << data->y 
         << "  z:" << data->z << endl;

      // --- Add dummy to input group -- //
   cout << "\nAdding myDummy to InputGroup" << endl;
   int devNum = input_manager->FAddDevice(myDummy);
   cout << "Device added as devNum: " << devNum << endl;
   cout << "\nSet proxy 0 to devNum, subnumber 0" << endl;
   input_manager->SetPosProxy(0,devNum,0);

      // --- Now get Dummy's data -- //
   cout << "Now get the positional data from posproxy0:" << endl;
   data = input_manager->GetPosData(0);
   cout  << "  x:" << data->x 
         << "  y:" << data->y 
         << "  z:" << data->z << endl;


   cout << "\nGetDevice(devNum):" << (input_manager->GetDevice(devNum)) << endl;
   (input_manager->GetDevice(devNum))->UpdateData();
   cout << "\nGet the devicename: " << (input_manager->GetDevice(devNum))->GetDeviceName() << endl;

   cout << "Doing UpdateAllData:" << flush;
   input_manager->UpdateAllData();
   cout << "..Done" << endl;

//  cout << "get an invalid device: ";
//  cout << (input_manager->GetDevice(3)) << endl;

   cout << "\ntype something: " << flush ;
   cin.get();

#ifdef TRACKERS
   vjFlock* aFlock = new (shared_pool) vjFlock;
   aFlock->StartSampling();
   devNum = input_manager->FAddDevice(aFlock);
   input_manager->SetPosProxy(1,devNum,0);
   input_manager->SetPosProxy(2,devNum,1);

   cout << "Trackers set up.. " << endl;
   //sleep(5);

   vjPOS_DATA *data1,*data2,*data0;

   for (int i = 0; i < 5; i++)
   {
      input_manager->UpdateAllData();
      data0 = input_manager->GetPosData(0);
      data1 = input_manager->GetPosData(1);
      data2 = input_manager->GetPosData(2);
      cout << endl;
      cout << "X: " << data0->x << "  " << data1->x << "  " << data2->x << endl
      << "Y: " << data0->y << "  " << data1->y << "  " << data2->y << endl
      << "Z: " << data0->z << "  " << data1->z << "  " << data2->z << endl;

      sleep(1);
   }

   aFlock->StopSampling();
#endif
   
   vjIBox *myibox = new (shared_pool) vjIBox;
   myibox->StartSampling();
   cout << "ibox is: " << myibox->GetDeviceName();
   C2Dinput_manageritalProxy dp1(myibox,0);
   devNum = input_manager->FAddDevice(myibox);
   input_manager->SetDinput_managerProxy(0,devNum,0);
   input_manager->SetDinput_managerProxy(1,devNum,1);
   input_manager->SetDinput_managerProxy(2,devNum,2);

   for (int i = 0; i < 20; i++)
   {
      input_manager->UpdateAllData();
      cout << "dinput_managerital0: " << input_manager->GetDinput_managerData(0);
      cout << " dinput_managerital1: " << input_manager->GetDinput_managerData(1);
      cout << " dinput_managerital2: " << input_manager->GetDinput_managerData(2) << endl;
      sleep(1);
   }


   myibox->StopSampling();
   delete myibox;
   // delete aFlock;
   delete myDummy;

   delete input_manager;
   delete shared_pool;

   return 0;
} 
