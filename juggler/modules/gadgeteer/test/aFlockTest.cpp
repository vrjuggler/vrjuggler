
#include <iostream.h> 	// for cout
#include <iomanip.h> 	// for setw

#include <stdlib.h> 	// for system
#include <unistd.h> 	// for sleep

#include <Input/vjPosition/aFlock.h>

void main()
{
    
    const char* port = "/dev/ttyd3";
    const int baud = 38400;
    const int numBirds = 2;
    const char* calfile = "/home/users/allenb/Source/juggler/Data/calibration.table";
    const BIRD_HEMI hemi = LOWER_HEM;
    const BIRD_FILT filt = AC_NARROW;
    const int sync = 1;
    const int blocking = 0;
    const char report = 'R';
    const int transmitter = 3; 
    aFlock flock(   port, 
		    baud, 
		    sync, 
		    blocking, 
		    numBirds, 
		    transmitter, 
		    hemi, 
		    filt, 
		    report, 
		    calfile );
    
    // these are in the vjInput base class...
    flock.setBaudRate( baud );
    flock.setPort( port );
    
    char achar;
    
    do 
    {
	cout << "U - Update\n"
       << "S - Start\n"
       << "X - Stop\n"
       << "Q - Quit\n"
       << "O - Output\n";
       
	cin >> achar;
	
	cout << flush;
	switch (achar) 
	{
	case 'u':case 'U':  break;
	case 's':case 'S': flock.start(); 
			break;
	case 'x':case 'X': flock.stop(); 
			break;
	case 'o':case 'O': 
	    for(int z = 0; z < 10; z++)
	    {
		flock.sample();
		system("clear");  
		cout << "vjFlock------------------------------------------------------------" << endl ;
		    cout<< "Data: x:" << setw(10) ;
		    for(int i=1; i <= flock.getNumBirds(); ++i) 
		    	cout<< setw(10) <<flock.xPos(i);
		    cout<<endl;
		    cout<< "      y:" << setw(10) ;
		    for(  i=1; i <= flock.getNumBirds(); ++i) 
		    	cout<< setw(10) <<flock.yPos(i);
		    cout<<endl;
		    cout<< "      z:" << setw(10) ;
		    for(  i=1; i <= flock.getNumBirds(); ++i) 
		    	cout<< setw(10) <<flock.zPos(i);
		    cout<<endl;
		    cout<< "    azi:" << setw(10) ;
		    for(  i=1; i <= flock.getNumBirds(); ++i) 
		    	cout<< setw(10) <<flock.zRot(i);
		    cout<<endl;
		    cout<< "   elev:" << setw(10) ;
		    for(  i=1; i <= flock.getNumBirds(); ++i) 
		    	cout<< setw(10) <<flock.yRot(i);
		    cout<<endl;
		    cout<< "   roll:"  ;
		    for(  i=1; i <= flock.getNumBirds(); ++i) 
		    	cout<< setw(10) <<flock.xRot(i);
		    cout<<endl<<endl;
		sleep(2);
	    }
	    break;
	}
	
       
	cout << achar;
    } while ((achar != 'q') && (achar != 'Q'));
    
    flock.stop();
}
