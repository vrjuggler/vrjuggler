/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <IboxAdaptor.h>
#include <string>


// Constructor

//spinner() was here also
namespace gadget_test
{
IboxAdaptor::IboxAdaptor(const std::string& devName)
   : button(4), analog(4)
{
    mName=devName;
    mIbox = new IboxStandalone();
}

IboxAdaptor::~IboxAdaptor()
{
   if ( mIbox != NULL )
   {
      delete mIbox;
      mIbox = NULL;
   }
}
bool IboxAdaptor::Start( const std::string& ttyPort, int mBaudRate )
{
	vpr::ReturnStatus result = vpr::ReturnStatus::Fail;
    result = mIbox->connect( ttyPort.data() ,mBaudRate);
    for(int i=0;(i<5&&result==vpr::ReturnStatus::Fail);i++)
	{
		vpr::System::usleep(1500);
		result = mIbox->connect( ttyPort.data() ,mBaudRate);
    }
	return((result == vpr::ReturnStatus::Succeed) ? true : false );
}
bool IboxAdaptor::Start()
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
bool IboxAdaptor::Stop()
{
	if ( mIbox != NULL )
	{
		mIbox->disconnect();
		delete mIbox;
		mIbox = NULL;
	}
	return(true);
}
bool IboxAdaptor::Sample( int numSamples )
{
	for(int i=0;i<numSamples;i++)						//FIX!!!!!!!!!!!!!!!!!!!!!!!!!
	{
		mIbox->std_cmd(0,4,0);
		if (mIbox->check_packet() == vpr::ReturnStatus::Succeed)
		{
			//Get the Digital Data from the IBox
			button[0] = mIbox->buttonFunc(0);
			button[1] = mIbox->buttonFunc(1);
			button[2] = mIbox->buttonFunc(2);
			button[3] = mIbox->buttonFunc(3);

			//Get the Analog Data from the IBox
			analog[0] = mIbox->analogFunc(0);
			analog[1] = mIbox->analogFunc(0);
			analog[2] = mIbox->analogFunc(0);
			analog[3] = mIbox->analogFunc(0);
			//std::cout<<"["<<spiner()<<"] Buttons: " << button[0] << " " << button[1] << " " << button[2] << " " << button[3] << " Analog: " <<  analog[0] << " " << analog[1] << " " << analog[2] << " " << analog[3] << "\r" << std::flush;
			std::cout<<"["<<spiner()<<"] Buttons: " << button[0] << " " << button[1] << " " << button[2] << " " << button[3] << " Analog: " <<  analog[0] << " " << analog[1] << " " << analog[2] << " " << analog[3] << "\r" << std::flush;
			i++;
		}
	}
	return(true);
}
bool IboxAdaptor::Sample()
{
	int numSamples;

	std::cout<<"          ====================================================\n"<<std::flush;
	std::cout<<"          How many samples of data do you want? \n"<<std::flush;
	std::cout<<"          ====================================================\n"<<std::flush;
	std::cin >> numSamples;
	
	return(Sample(numSamples));
}
bool IboxAdaptor::Dump()
{
	mIbox->std_cmd(0,4,0);
	if (mIbox->check_packet() == vpr::ReturnStatus::Succeed)
	{
		//Get the Digital Data from the IBox
		button[0] = mIbox->buttonFunc(0);
		button[1] = mIbox->buttonFunc(1);
		button[2] = mIbox->buttonFunc(2);
		button[3] = mIbox->buttonFunc(3);

		//Get the Analog Data from the IBox
		analog[0] = mIbox->analogFunc(0);
		analog[1] = mIbox->analogFunc(0);
		analog[2] = mIbox->analogFunc(0);
		analog[3] = mIbox->analogFunc(0);
		std::cout<<"Buttons: " << button[0] << " " << button[1] << " " << button[2] << " " << button[3] << " Analog: " <<  analog[0] << " " << analog[1] << " " << analog[2] << " " << analog[3] << "\r" << std::flush;
	}
	return(true);
}
void IboxAdaptor::printCommands()
{
	std::cout << "          ==================================\n" << std::flush;
	std::cout << "          IBox Interactive Test\n" << std::flush;
	std::cout << "          ==================================\n" << std::flush;
	std::cout << "          S = Start\n" << std::flush;
	std::cout << "          X = Stop\n" << std::flush;
	std::cout << "          N = Sample\n" << std::flush;
	std::cout << "          Q = Quit\n" << std::flush;
	std::cout << "          ==================================\n" << std::flush;
}
void IboxAdaptor::Command(const char ch)
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


