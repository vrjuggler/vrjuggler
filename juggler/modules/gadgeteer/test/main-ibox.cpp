#include <iostream.h>
#include <iomanip.h>
#include <unistd.h>

#include <SharedMem/vjMemPool.h>
//#include <SharedMem/vjSharedType.h>
#include <Input/vjInput/vjIbox.h>
#include <Input/ibox/hci.h>
#include <Input/ibox/ibox.h>

int main()
{
  vjMemPool* anSgiPool = new vjSharedPool(1024*1024);
  
  vjIBox* t1 = new(anSgiPool) vjIBox;
 
  vjIBOX_DATA *data;

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
     case 'u':case 'U': t1->updateData(); break;
     case 's':case 'S': t1->startSampling(); break;
     case 'x':case 'X': t1->stopSampling(); break;
     case 'o':case 'O': 
     for(int z = 0; z < 10; z++){
         t1->updateData();
         
	
	cout << t1->getDigitalData(0);
	cout << t1->getDigitalData(1);
	cout << t1->getDigitalData(2);
	cout << t1->getDigitalData(3);        
         cout << endl; 
         sleep(2);
    }
     break;
   }
   cout << achar;
  } while ((achar != 'q') && (achar != 'Q'));
  t1->stopSampling();
  delete t1;
  delete anSgiPool;
  return 0;
}
