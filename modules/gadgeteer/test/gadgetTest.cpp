/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#include <iostream>
#include <string>
#include <unistd.h> // for usleep

#include "PinchGloveAdaptor.h"
#include "IboxAdaptor.h"
#include "FlockAdaptor.h"

#include <iomanip>
#include <vpr/System.h>
////////////////////////////////////////////////////////////////////////////////
////         Adaptor Includes
////
// #include <gadget/Devices/Ascension/FlockStandalone.h>
// #include <gadget/Devices/Immersion/IboxStandalone.h>
////
////         Adaptor Includes
////////////////////////////////////////////////////////////////////////////////

//namespace gadget_test
//{
int main(int argc, char* argv[])
{
    vector<gadget_test::BaseAdaptor*> deviceList;
	vector<gadget_test::BaseAdaptor*>::iterator deviceIterator;
    
	////////////////////////////////////////////////////////////////////////////////
	////         CREATE DRIVER LIST
	////
	
	deviceList.push_back(new gadget_test::PinchGloveAdaptor("pinch_glove"));
	deviceList.push_back(new gadget_test::IboxAdaptor("ibox"));
	deviceList.push_back(new gadget_test::FlockAdaptor("Flock of Birds"));

	////   ex. deviceList.push_back(new newDeviceDriverAdaptor("newDeviceDriver"));
	////         CREATE DRIVER LIST
	////////////////////////////////////////////////////////////////////////////////
    
	
	////////////////////////////////////////////////////////////////////////////////
	////         CREATE MAIN MENU
	////
    
	std::cout<<"====================================================\n"<<std::flush;
	std::cout<<"        Gadgeteer Device Standalone Test Suite\n"<<std::flush;
	std::cout<<"====================================================\n"<<std::flush;
    int devCount = 0;
    for(deviceIterator = deviceList.begin();deviceIterator != deviceList.end();++deviceIterator) 
	{
        cout <<  ++devCount << ") " << (*deviceIterator)->mName << endl;
	}
    std::cout<<"0) Quit \n"<<std::flush;
	std::cout<<"====================================================\n"<<std::flush;
    int num=0;
	std::cin >> num;
    
	////
	////         CREATE MAIN MENU
	////////////////////////////////////////////////////////////////////////////////

	while(num != 0)
	{
        if(num>deviceList.size() && num<(deviceList.size())*-1)
		{
			cout << endl << endl << "ERROR: Not a valid selection, please try again!" << endl << endl;
		}
		else if(num>0)
		{
			char ch;
			
			(deviceList[num-1])->printCommands();
            
			//=============================================================
			// This is the INTERACTIVE MODE where the writer of the adaptor can write
			// his own additional commands to the required Start(), Stop(), Sample(), and Dump()
			std::cin >> ch;
            while(ch != 'Q')
			{
				(deviceList[num-1])->Command(ch);
				(deviceList[num-1])->printCommands();
				std::cin >> ch;
			}
		}
		else if(num<0)
		{
			//=============================================================
			//This is the AUTOMATIC TESTING MODE where the test suite uses 
			//the required adaptor commands to test the device
			//PinchGloveAdaptor* pinchGlove;
			//pinchGlove = new PinchGloveAdaptor;
			(deviceList[(num * -1)-1])->Start("/dev/ttyS1",9600);
			(deviceList[(num * -1)-1])->Sample(999999);
			(deviceList[(num * -1)-1])->Stop();
		}


		
		///////////////////////////////////////////////////////////////////////////////////////////////
		////         CREATE MAIN MENU
		////
        
		std::cout<<"====================================================\n"<<std::flush;
		std::cout<<"        Gadgeteer Device Standalone Test Suite\n"<<std::flush;
		std::cout<<"====================================================\n"<<std::flush;
        int devCount = 0;
        for(deviceIterator = deviceList.begin();deviceIterator != deviceList.end();++deviceIterator) 
		{
			cout <<  ++devCount << ") " << (*deviceIterator)->mName << endl;
		}
        std::cout<<"0) Quit \n"<<std::flush;
		std::cout<<"====================================================\n"<<std::flush;
        std::cin >> num;
        
		////
		////         CREATE MAIN MENU
		///////////////////////////////////////////////////////////////////////////////////////////////

	}// end while(num!=-1)
	return 0;
}
//}
