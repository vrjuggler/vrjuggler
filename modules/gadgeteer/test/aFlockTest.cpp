/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 */


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
