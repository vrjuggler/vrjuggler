/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/


#include <FlockAdaptor.h>
#include <string>


// Constructor

//spinner() was here also
namespace gadget_test
{
FlockAdaptor::FlockAdaptor(const std::string& devName = "Flock of Birds")
{
    mName=devName;	//MUST KEEP HERE ALWAYS
    //mFlock = new FlockStandalone();



	//const char* port = "/dev/ttyd3";
	const char* port = "/dev/ttyS0";
	const int baud = 38400;
	const int numBirds = 2;
	const char* calfile = "/home/users/allenb/Source/juggler/Data/calibration.table";
	const BIRD_HEMI hemi = LOWER_HEM;
	const BIRD_FILT filt = AC_NARROW;
	const int sync = 1;
	const int blocking = 0;
	const char report = 'R';
	const int transmitter = 3; 
	mFlock = new FlockStandalone( port, baud, sync, blocking, numBirds, transmitter, 
					   hemi, filt, report, calfile );


}

FlockAdaptor::~FlockAdaptor()
{
   if ( mFlock != NULL )
   {
      delete mFlock;
      mFlock = NULL;
   }
}
bool FlockAdaptor::Start( const std::string& ttyPort, int mBaudRate )
{
	if(mFlock->start() == vpr::ReturnStatus::Fail)
    {
       return(false);
    }
    else
    {
       return(true);
    }
}
bool FlockAdaptor::Start()
{
	std::string ttyPort;
	int mBaudRate;

	std::cout<<"          ====================================================\n"<<std::flush;
	std::cout<<"          What serial port do you want to use? \n"<<std::flush;
	std::cout<<"          ====================================================\n"<<std::flush;

	std::cin >> ttyPort;

	std::cout<<"          ====================================================\n"<<std::flush;
	std::cout<<"          What baud rate do you want to use? \n"<<std::flush;
	std::cout<<"          ====================================================\n"<<std::flush;

	std::cin >> mBaudRate;
	
	return(Start(ttyPort,mBaudRate));
}
bool FlockAdaptor::Stop()
{
	if ( mFlock != NULL )
	{
		mFlock->stop();
		delete mFlock;
		mFlock = NULL;
	}
	return(true);
}
bool FlockAdaptor::Sample( int numSamples )
{
	for(int z = 0; z < 10; z++)
	{
		int i;

		mFlock->sample();
		system("clear");  
		std::cout << "vjFlock------------------------------------------------------------" << std::endl;
		std::cout<< "Data: x:" << std::setw(10);
		for(i=1; i <= mFlock->getNumBirds(); ++i)
		{
			std::cout<< std::setw(10) <<mFlock->xPos(i);
			std::cout<<std::endl;
			std::cout<< "      y:" << std::setw(10);
		}
		for(  i=1; i <= mFlock->getNumBirds(); ++i) 
		{
			std::cout<< std::setw(10) <<mFlock->yPos(i);
			std::cout<<std::endl;
			std::cout<< "      z:" << std::setw(10);
		}
		for(  i=1; i <= mFlock->getNumBirds(); ++i)
		{	
			std::cout<< std::setw(10) <<mFlock->zPos(i);
			std::cout<<std::endl;
			std::cout<< "    azi:" << std::setw(10);
		}
		for(  i=1; i <= mFlock->getNumBirds(); ++i)
		{
			std::cout<< std::setw(10) <<mFlock->zRot(i);
			std::cout<<std::endl;
			std::cout<< "   elev:" << std::setw(10);
		}
		for(  i=1; i <= mFlock->getNumBirds(); ++i) 
		{
			std::cout<< std::setw(10) <<mFlock->yRot(i);
			std::cout<<std::endl;
			std::cout<< "   roll:"  ;
		}
		for(  i=1; i <= mFlock->getNumBirds(); ++i) 
		{
			std::cout<< std::setw(10) <<mFlock->xRot(i);
			std::cout<<std::endl<<std::endl;
		}
		sleep(2);
	}

	return(true);
}
bool FlockAdaptor::Sample()
{
	int numSamples;

	std::cout<<"          ====================================================\n"<<std::flush;
	std::cout<<"          How many samples of data do you want? \n"<<std::flush;
	std::cout<<"          ====================================================\n"<<std::flush;
	std::cin >> numSamples;
	
	return(Sample(numSamples));
}
bool FlockAdaptor::Dump()
{
	return(true);
}
void FlockAdaptor::printCommands()
{
	std::cout << "          ==================================\n" << std::flush;
	std::cout << "          Flock Interactive Test\n" << std::flush;
	std::cout << "          ==================================\n" << std::flush;
	std::cout << "          S = Start\n" << std::flush;
	std::cout << "          X = Stop\n" << std::flush;
	std::cout << "          N = Sample\n" << std::flush;
	std::cout << "          Q = Quit\n" << std::flush;
	std::cout << "          ==================================\n" << std::flush;
}
void FlockAdaptor::Command(const char ch)
{
	if(ch=='S')
	{
		Start();
	}
	else if(ch=='X')
	{
		Stop();
	}
	else if(ch=='N')
	{
		Sample();
	}
}
}


