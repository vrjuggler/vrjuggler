#include <iostream.h>
#include <iomanip.h>

#include <SharedMem/MemPool.h>
#include <SharedMem/SharedType.h>
//#include <Input/C2Position/Birdy.h>
#include <Input/ibox/ibox.h>

int main()
{
  MemPool* anSgiPool = new C2SharedPool(1024*1024);
  
  IBox* t1 = new(anSgiPool) IBox;
 
  C2IBOX_DATA *data;

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
         
	
	cout << t1->GetDigitalData(0);
	cout << t1->GetDigitalData(1);
	cout << t1->GetDigitalData(2);
	cout << t1->GetDigitalData(3);        
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
