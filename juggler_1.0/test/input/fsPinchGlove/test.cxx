#include <iostream>
#include <string>
#include <unistd.h> // for usleep

#include "fsPinchGlove.h"

char spin()
{
	static float spinCounter = 0;
	static std::string spinner = "\\|/-";
	spinCounter += .0005;
	return spinner[ static_cast<int>(spinCounter) % spinner.size() ];	
}

void main(int argc, char* argv[])
{
	if (argc == 2)
	{
		fsPinchGlove pinchGlove;
		
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
			result = pinchGlove.connectToHardware( port.data() );
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

		return;
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
}
