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

  cerr << "-------- Bird Test program -------\n";
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
  int hemisphere = 0;      // "Front=0" "Aft=1" "Upper=2" "Lower=3" "Left=4" "Right=5" }
  unsigned int birdFormat = 4;   // "No Data=0" "Position=1" "Angles=2" "Matrix=3" "Position And Angles=4" "Position And Matrix=5" "Quaternions=6" "Position And Quaternions=7"
  double birdRate = 100.0;
  unsigned int birdsRequired = 2;
  int runMode = 1;      // "Run Continuous=0" "Sample Individually=1" }

  unsigned short port = 6000;


    aMotionStar* motionstar = new(aMemPool) aMotionStar
         ("129.186.232.39",
             port,
             hemisphere,
             birdFormat,
             birdsRequired,
             runMode,
             birdRate,
          1
             );

  char achar;
  bool birds_running(false);      // Are the birds currently running

   do
   {
      cerr  << "\n----- Bird Test Program ----\n"
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

      cerr << "\nGot command: " << achar << endl << flush;
      switch (achar)
      {
      case 'u':
      case 'U':
         cerr << "---- Updating (not implemented) ---\n" << flush;
         break;
      case 's':
      case 'S':
         cerr << "---- Starting MotionStar ---\n" << flush;
         motionstar->start();
         birds_running = true;
         break;
      case 'x':
      case 'X':
         cerr << "---- Stoping motionstar ---\n" << flush;
         motionstar->stop();
         birds_running = false;
         break;
      case 'o':
      case 'O':
         long num_samples;
         cerr << "Number of samples?";
         cin >> num_samples;

         for (long z = 0; z < num_samples; z++)
         {
            motionstar->sample();
            //system("clear");
            //cout << "-------- aMotionStar: Sampling (" << z << " of " << num_samples << ") -------" << endl;
            cout << z << ": ";
            for(int bird=0;bird<birdsRequired;bird++)
            {
            //   vjVec3 pos0 = vjCoord( *motionstar->getPosData(bird) ).pos;
            //   vjVec3 ori0 = vjCoord( *motionstar->getPosData(bird) ).orient;
               /*
               cout << "Bird " << bird << ":\n"
                 << setiosflags(ios::right | ios::fixed | ios::showpoint)
                 << "Data: x:" << setprecision(2) << setw(10) << motionstar->xPos(bird) << endl
                 << "      y:" << setprecision(2) << setw(10) << motionstar->yPos(bird) << endl
                 << "      z:" << setprecision(2) << setw(10) << motionstar->zPos(bird) << endl
                 << "    azi:" << setprecision(2) << setw(10) << motionstar->xRot(bird) << endl
                 << "   elev:" << setprecision(2) << setw(10) << motionstar->yRot(bird) << endl
                 << "   roll:" << setprecision(2) << setw(10) << motionstar->zRot(bird) << endl << endl;
                 */
                  //cout << " Bird: " << bird;
                  cout << " " << setprecision(8)  << motionstar->xPos(bird)
                       << " " << setprecision(8)  << motionstar->yPos(bird)
                       << " " << setprecision(8)  << motionstar->zPos(bird)
                       << " " << setprecision(8)  << motionstar->xRot(bird)
                       << " " << setprecision(8) << motionstar->yRot(bird)
                       << " " << setprecision(8) << motionstar->zRot(bird);
            }
            cout << endl;
            //sleep(2);
         }
         break;
      }
   } while ((achar != 'q') && (achar != 'Q'));

   cerr << "---- Quiting -----\n"
        << "Stoping birds.....\n" << flush;

  motionstar->stop();

  delete motionstar;
  delete aMemPool;
  return 0;
}
