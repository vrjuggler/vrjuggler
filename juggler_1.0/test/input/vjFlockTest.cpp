#include <iostream.h>
#include <iomanip.h>
#include <SharedMem/vjMemPool.h>
//#include <SharedMem/vjSharedType.h>
#include <Input/vjPosition/vjFlock.h>
#include <Math/vjCoord.h>

int main()
{
  vjMemPool* anSgiPool = new vjMemPoolSGI(1024*1024);
 	
  
  const int numBirds = 2;
  const  char* calfile = "/home/users/allenb/Source/juggler/Data/calibration.table";
  const BIRD_HEMI hemi = LOWER_HEM;
  const BIRD_FILT filt = AC_NARROW;
  const int sync = 1;
  const int blocking = 0;
  const char report = 'R';
  const int transmitter = 3; 
  const  char* port = "/dev/ttyd3";
  const int baud = 38400;
  vjFlock* flock = new(anSgiPool) vjFlock
  		(sync, blocking, numBirds, transmitter,
		hemi, filt, report, calfile);
  
  // these are in the vjInput base class...
  flock->SetBaudRate( baud );
  flock->SetPort( port );

  char achar;
  cout << "U - Update\n"
       << "S - Start\n"
       << "X - Stop\n"
       << "Q - Quit\n"
       << "O - Output\n";
  do {
   cin >> achar;

   cout << flush;
   switch (achar) 
   {
     case 'u':case 'U':  break;
     case 's':case 'S': flock->StartSampling(); 
			break;
     case 'x':case 'X': flock->StopSampling(); 
			break;
     case 'o':case 'O': 
     for(int z = 0; z < 10; z++){
         flock->UpdateData();
         vjVec3 pos0 = vjCoord( *flock->GetPosData() ).pos;
	 vjVec3 ori0 = vjCoord( *flock->GetPosData() ).orient;
       system("clear");  
     cout << "vjFlock------------------------------------------------------------" << endl 
          << "Data: x:" << setw(10) << pos0[0] << endl
          << "      y:" << setw(10) << pos0[1] << endl
          << "      z:" << setw(10) << pos0[2] << endl
          << "    azi:" << setw(10) << ori0[0] << endl  
          << "   elev:" << setw(10) << ori0[1] << endl 
          << "   roll:" << setw(10) << ori0[2] << endl << endl;
         sleep(2);
    }
     break;
   }
   cout << achar;
  } while ((achar != 'q') && (achar != 'Q'));
  
  flock->StopSampling();
  
  delete flock;
  delete anSgiPool;
  return 0;
}
