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

#include <iostream>
#include <string>
#include <unistd.h> // for usleep

#include <gadget/Devices/Fakespace/PinchGloveStandalone.h>

char spin()
{
	static float spinCounter = 0;
	static std::string spinner = "\\|/-";
	spinCounter += .0005;
	return spinner[ static_cast<int>(spinCounter) % spinner.size() ];	
}

int main(int argc, char* argv[])
{
	if (argc == 2)
	{
		PinchGloveStandalone pinchGlove;
		
		std::string port;
		port = argv[1];
		
		std::cout<<"====================================================\n"<<std::flush;
		std::cout<<"\n"<<std::flush;
		std::cout<<"Test driver for pinch glove on port: "<<port.data()<<"\n"<<std::flush;
		std::cout<<"Hit Ctrl-C to Exit\n"<<std::flush;
		std::cout<<"\n"<<std::flush;
		std::cout<<"====================================================\n"<<std::flush;
		std::cout<<"\n\n\n"<<std::flush;
		
		// Connect to the pinch glove hardware, keep trying
		bool result = false;
		while (result == false)
		{
			result = pinchGlove.connectToHardware( port.data() , 9600);
			//usleep( 50000 );
		}
		
		std::cout<<"\n"<<std::flush;
		std::cout<<"            Left |Right\n";
		std::string gestureString = "00000|00000";
		while (1)
		{
			// call updateStringFromHardware to get most current pinch data. 
			pinchGlove.updateStringFromHardware();

			// get the last sampled string, 
			// NOTE: call reSampleData to get most current pinch data.
			pinchGlove.getSampledString( gestureString );
			std::cout<<"["<<spin()<<"] Sampled "<<gestureString<<"\r"<<std::flush;
		}

		return 1;
	} 
	
	else 
	{	
		std::cout<<"\n"<<std::flush;
		std::cout<<argv[0]<<" by Kevin Meinert - kevn@vrjuggler.org\n"<<std::flush;
		std::cout<<"Usage:   "<<argv[0]<<"   <port>\n"<<std::flush;
		std::cout<<"Example (UNIx): "<<argv[0]<<" /dev/ttyd2\n"<<std::flush;
		std::cout<<"Example (WIN):  "<<argv[0]<<" COM1\n"<<std::flush;
		std::cout<<"\n"<<std::flush;
	}

	return 0;
}
