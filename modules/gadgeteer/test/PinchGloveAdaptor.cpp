/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/


#include <PinchGloveAdaptor.h>
#include <string>

namespace gadget_test
{
// Constructor
PinchGloveAdaptor::PinchGloveAdaptor(const std::string& devName)
{
    mName=devName;
    mPinchGlove = new PinchGloveStandalone();
}

PinchGloveAdaptor::~PinchGloveAdaptor()
{
   if ( mPinchGlove != NULL )
   {
      delete mPinchGlove;
      mPinchGlove = NULL;
   }
}
bool PinchGloveAdaptor::Start( const std::string& ttyPort, int mBaudRate )
{
	bool result = false;
    result = mPinchGlove->connectToHardware( ttyPort.data() ,mBaudRate);
    for(int i=0;(i<5&&result==false);i++)
	{
		vpr::System::usleep(1500);
		result = mPinchGlove->connectToHardware( ttyPort.data() , mBaudRate);
    }
	return(result);
}
bool PinchGloveAdaptor::Start()
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
bool PinchGloveAdaptor::Stop()
{
	if ( mPinchGlove != NULL )
	{
		delete mPinchGlove;
		mPinchGlove = NULL;
	}
	return(true);
}
bool PinchGloveAdaptor::Sample( int numSamples )
{
	std::cout<<"\n"<<std::flush;
	std::cout<<"            Left |Right\n";
	std::string gestureString = "00000|00000";

	for(int i=0;i<numSamples;i++)						//FIX!!!!!!!!!!!!!!!!!!!!!!!!!
	{
		// call updateStringFromHardware to get most current pinch data. 
		mPinchGlove->updateStringFromHardware();

		// get the last sampled string, 
		// NOTE: call reSampleData to get most current pinch data.
		mPinchGlove->getSampledString( gestureString );
		std::cout<<"["<<spiner()<<"] Sampled "<<gestureString<<"\r"<<std::flush;
	}
	return(true);
}
bool PinchGloveAdaptor::Sample()
{
	int numSamples;

	std::cout<<"          ====================================================\n"<<std::flush;
	std::cout<<"          How many samples of data do you want? \n"<<std::flush;
	std::cout<<"          ====================================================\n"<<std::flush;
	std::cin >> numSamples;

	return(Sample(numSamples));
}
bool PinchGloveAdaptor::Dump()
{
	std::string gestureString = "00000|00000";
	mPinchGlove->updateStringFromHardware();
	// get the last sampled string, 
	// NOTE: call reSampleData to get most current pinch data.
	mPinchGlove->getSampledString( gestureString );
	std::cout<<"Sampled "<<gestureString<<"\r"<<std::flush;
	return(true);
}
void PinchGloveAdaptor::printCommands()
{
	std::cout << "          ==================================\n" << std::flush;
	std::cout << "          PinchGlove Interactive Test\n" << std::flush;
	std::cout << "          ==================================\n" << std::flush;
	std::cout << "          S = Start\n" << std::flush;
	std::cout << "          X = Stop\n" << std::flush;
	std::cout << "          N = Sample\n" << std::flush;
	std::cout << "          Q = Quit\n" << std::flush;
	std::cout << "          ==================================\n" << std::flush;
}
void PinchGloveAdaptor::Command(const char ch)
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


