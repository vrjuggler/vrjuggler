#include <iostream.h>
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
		
		cout<<"====================================================\n"<<flush;
		cout<<"\n"<<flush;
		cout<<"Test driver for pinch glove on port: "<<port.data()<<"\n"<<flush;
		cout<<"Hit Ctrl-C to Exit\n"<<flush;
		cout<<"\n"<<flush;
		cout<<"====================================================\n"<<flush;
		cout<<"\n\n\n"<<flush;
		
		// Connect to the pinch glove hardware, keep trying
		bool result = false;
		while (result == false)
		{
			result = pinchGlove.connectToHardware( port.data() );
			//usleep( 50000 );
		}
		
		cout<<"\n"<<flush;
		cout<<"            Left |Right\n";
		std::string gestureString = "00000|00000";
		while (1)
		{
			// call updateStringFromHardware to get most current pinch data. 
			pinchGlove.updateStringFromHardware();

			// get the last sampled string, 
			// NOTE: call reSampleData to get most current pinch data.
			pinchGlove.getSampledString( gestureString );
			cout<<"["<<spin()<<"] Sampled "<<gestureString<<"\r"<<flush;
		}

		return;
	} 
	
	else 
	{	
		cout<<"\n"<<flush;
		cout<<argv[0]<<" by Kevin Meinert - kevn@vrjuggler.org\n"<<flush;
		cout<<"Usage:   "<<argv[0]<<"   <port>\n"<<flush;
		cout<<"Example (UNIx): "<<argv[0]<<" /dev/ttyd2\n"<<flush;
		cout<<"Example (WIN):  "<<argv[0]<<" COM1\n"<<flush;
		cout<<"\n"<<flush;
	}
}
