#include <iostream.h>
#include <iomanip.h>
#include <SharedMem/vjMemPool.h>
//#include <SharedMem/vjSharedType.h>
#include <Input/vjPosition/logiclass.h>
//#include <Input/ibox/ibox.h>

int main()
{
  vjMemPool* anSgiPool = new vjSharedPool(1024*1024);
  
  vjThreeDMouse* t1 = new(anSgiPool) vjThreeDMouse;

  vjPOS_DATA *data;

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
              << "Data: x:" << setw(10) << data->pos.vec[0] << endl
              << "      y:" << setw(10) << data->pos.vec[1] << endl
              << "      z:" << setw(10) << data->pos.vec[2] << endl
              << "    azi:" << setw(10) << data->orient.vec[0]  << endl //lr
              << "   elev:" << setw(10) << data->orient.vec[1]  << endl //ud
              << "   roll:" << setw(10) <<data->orient.vec[2] << endl
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
