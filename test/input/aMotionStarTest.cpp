#include <iostream.h>
#include <iomanip.h>
#include <SharedMem/vjMemPool.h>
//#include <SharedMem/vjSharedType.h>
#include <Input/vjPosition/aMotionStar.h>
#include <Math/vjCoord.h>

int main()
{
  vjMemPool* aMemPool = new vjSharedPool(1024*1024);

  // Get configuration information \
   
  cout << "-------- Bird Test program -------\n";
/*       << "First we need some config info:\n"
       << "NOTE: Sample values are in ()'s\n" << flush;

  cout << "\nEnter port (/dev/ttyd3): " << flush;
  cin >> port;
  cout << "\nEnter baud rate (38400): " << flush;
  cin >> baud;
*/  
//  cout << "\nHow many birds (3): " << flush;
//  cin >> birdsRequired;
/*  
  cout << "\nNot using calibration file." << flush;
  cout << "\nUsing LOWER_HEM" << flush;
  cout << "\nUsing AC_NARROW filter" << flush;
  cout << "\nEnter Sync (0 or 1): " << flush;
  cin >> sync;
  cout << "\nBlocking port? (0 or 1): " << flush;
  cin >> blocking;
  cout << "\nWhat report rate (Q-every cycle, R-every other, S-every 8, T-every 32): " << flush;
  cin >> report;
  cout << "\nWhat is the transmitter id (1-based numbering): " << flush;
  cin >> transmitter;
*/
  int hemisphere = 3;
  unsigned int birdFormat = 4;
  double birdRate = 90.0;
  unsigned int birdsRequired = 3;
  int runMode = 1;
  

    aMotionStar* motionstar = new(aMemPool) aMotionStar
    		("129.186.232.39", hemisphere,
             birdFormat,
             birdsRequired,
             runMode,
             birdRate, 1
             );

  char achar;
  bool birds_running(false);      // Are the birds currently running

   do
   {
      cout  << "\n----- Bird Test Program ----\n"
            << "----- Main Menu ----\n"
            << "Cur State:\n"
            << "   running:" << birds_running
            << "\n\nCommands:\n"
            << "   U - Update\n"
            << "   S - Start\n"
            << "   X - Stop\n"
            << "   Q - Quit\n"
            << "   O - Output\n"
            << "\nCommand:" << flush;
      cin >> achar;        // Get next command

      cout << "\nGot command: " << achar << endl << flush;
      switch (achar)
      {
      case 'u':
      case 'U':
         cout << "---- Updating (not implemented) ---\n" << flush;
         break;
      case 's':
      case 'S':
         cout << "---- Starting MotionStar ---\n" << flush;
         motionstar->start();
         birds_running = true;
         break;
      case 'x':
      case 'X':
         cout << "---- Stoping motionstar ---\n" << flush;
         motionstar->stop();
         birds_running = false;
         break;
      case 'o':
      case 'O':
         int num_samples;
         cout << "Number of samples?";
         cin >> num_samples;

         for (int z = 0; z < num_samples; z++)
         {
            motionstar->sample();
            system("clear");
            cout << "-------- aMotionStar: Sampling (" << z << " of " << num_samples << ") -------" << endl;
            for(int bird=0;bird<birdsRequired;bird++)
            {
            //   vjVec3 pos0 = vjCoord( *motionstar->getPosData(bird) ).pos;
            //   vjVec3 ori0 = vjCoord( *motionstar->getPosData(bird) ).orient;
               cout << "Bird " << bird << ":\n"
                 << setiosflags(ios::right | ios::fixed | ios::showpoint)
                 << "Data: x:" << setprecision(2) << setw(10) << motionstar->xPos(bird) << endl
                 << "      y:" << setprecision(2) << setw(10) << motionstar->yPos(bird) << endl
                 << "      z:" << setprecision(2) << setw(10) << motionstar->zPos(bird) << endl
                 << "    azi:" << setprecision(2) << setw(10) << motionstar->xRot(bird) << endl
                 << "   elev:" << setprecision(2) << setw(10) << motionstar->yRot(bird) << endl
                 << "   roll:" << setprecision(2) << setw(10) << motionstar->zRot(bird) << endl << endl;
            }
            sleep(2);
         }
         break;
      }
   } while ((achar != 'q') && (achar != 'Q'));

   cout << "---- Quiting -----\n"
        << "Stoping birds.....\n" << flush;

  motionstar->stop();

  delete motionstar;
  delete aMemPool;
  return 0;
}
