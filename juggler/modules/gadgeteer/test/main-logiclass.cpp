#include <iostream.h>
#include <iomanip.h>
#include <SharedMem/MemPool.h>
#include <SharedMem/SharedType.h>
#include <Input/C2Position/logiclass.h>
//#include <Input/ibox/ibox.h>

int main()
{
  MemPool* anSgiPool = new MemPoolSGI(1024*1024);
  
  ThreeDMouse* t1 = new(anSgiPool) ThreeDMouse;

  C2POS_DATA *data;

  char achar;
  cout << "U - Update\n"
       << "S - Start\n"
       << "X - Stop\n"
       << "Q - Quit\n"
       << "O - Output\n";
  do {
   cin >> achar;

   cout << flush;
   switch(achar) {
     case 'u':case 'U': t1->UpdateData(); break;
     case 's':case 'S': t1->StartSampling(); break;
     case 'x':case 'X': t1->StopSampling(); break;
     case 'o':case 'O': 
     for(int z = 0; z < 10; z++){
         t1->UpdateData();
         t1->GetData(data);
         
         cout << endl 
              << "Data: x:" << setw(10) << data->x << endl
              << "      y:" << setw(10) << data->y << endl
              << "      z:" << setw(10) << data->z << endl
              << "    azi:" << setw(10) << data->azi  << endl //lr
              << "   elev:" << setw(10) << data->elev  << endl //ud
              << "   roll:" << setw(10) <<data->roll << endl
              << endl;
         cout << endl; 
         sleep(2);
    }
     break;
   }
   cout << achar;
  } while ((achar != 'q') && (achar != 'Q'));
  
  t1->StopSampling();
  
  delete t1;
  delete anSgiPool;
  return 0;
}
