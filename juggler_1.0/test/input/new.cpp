#include <iostream.h>
#include <iomanip.h>
#include <SharedMem/MemPool.h>
#include <SharedMem/SharedType.h>
#include <Input/C2Position/C2Flock.h>
#include <Input/ibox/ibox.h>
#include <Input/C2Glove/CyberGlove.h>

int main()
{
  MemPool* anSgiPool = new MemPoolSGI(1024*1024);
  
  C2Flock* flock = new(anSgiPool) C2Flock;
  IBox* anIbox = new(anSgiPool) IBox;
  CyberGlove* aGlove = new(anSgiPool) 
          CyberGlove("/home/vr/CAVE/glove","/dev/ttyd45",38400);

  C2POS_DATA *data,*data2;
 
  C2IBOX_DATA *iboxData;

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
     case 'u':case 'U':  break;
     case 's':case 'S': flock->StartSampling(); 
			anIbox->StartSampling();
			aGlove->StartSampling();
			break;
     case 'x':case 'X': flock->StopSampling(); 
			anIbox->StopSampling();	
			aGlove->StopSampling();
			break;
     case 'o':case 'O': 
     for(int z = 0; z < 10; z++){
      C2GLOVE_DATA *gdata;
         flock->UpdateData();
         flock->GetData(data);
	 flock->GetData(data2);
         anIbox->UpdateData();
	 anIbox->GetData(iboxData);
	// aGlove->UpdateData();
//	 aGlove->GetData(gdata);
       system("clear");  
     cout << "C2Flock------------------------------------------------------------" << endl 
          << "Data: x:" << setw(10) << data->x << " Data2x:" << setw(10) << data2->x << endl
          << "      y:" << setw(10) << data->y << "      y:" << setw(10) << data2->y << endl
          << "      z:" << setw(10) << data->z << "      z:" << setw(10) << data2->z << endl
          << "    azi:" << setw(10) << data->azi << "    azi:" << setw(10) << data2->azi << endl  
          << "   elev:" << setw(10) << data->elev << "   elev:" << setw(10) << data2->elev << endl 
          << "   roll:" << setw(10) <<data->roll  << "   roll:" << setw(10) <<data2->roll << endl
              << endl;
     cout << "Ibox---------------------------------------------------------------" << endl
	  << " button1: " << iboxData->button[0] << endl
	  << " button2: " << iboxData->button[1] << endl 
	  << " button3: " << iboxData->button[2] << endl 
	  << " button4: " << iboxData->button[3] << endl << endl;
/*     cout << "Glove--------------------------------------------------------------" << endl
	  << " first finger: " << gdata->joints[0][0] << " " << gdata->joints[0][1] << " " << gdata->joints[0][2] << " " <<
		gdata->joints[0][3] << endl
	  << "second finger: " << gdata->joints[1][0] << " " << gdata->joints[1][1] << " " << gdata->joints[1][2] << " " <<
		gdata->joints[1][3] << endl
	  << " third finger: " << gdata->joints[2][0] << " " << gdata->joints[2][1] << " " << gdata->joints[2][2] << " " <<
		gdata->joints[2][3] << endl
	  << " fourth finger: " << gdata->joints[3][0] << " " << gdata->joints[3][1] << " " << gdata->joints[3][2] << " " <<
		gdata->joints[3][3] << endl;
*/
         sleep(2);
    }
     break;
   }
   cout << achar;
  } while ((achar != 'q') && (achar != 'Q'));
  
  flock->StopSampling();
  anIbox->StopSampling();
  aGlove->StopSampling();
  
  delete flock;
  delete anIbox;
  delete aGlove;
  delete anSgiPool;
  return 0;
}
