#include "socketUtil.h"

void main(int argc, char** argv)
{
    std::string address = "129.186.232.64";
    int port = 4550;
    
    if (argc == 2)
	address = argv[1];
    
    if (argc == 3)
	port = atoi(argv[2]);
    //-------------------------------------------
    
    socketUtil sUtil;
    socketUtil::Message message;
    
    
    sUtil.setRole( socketUtil::CLIENT );
    sUtil.setRemote(address, port);
    
    sUtil.connect();
    
    for (int y = 0; y < 500000; ++y)
    {
	cout<<"."<<flush;
	sUtil.collectIncomingData();
	
	for (int x = 0; sUtil.queueSize(); ++x)
	{
	    sUtil.getCurrentMessage( message );
	    sUtil.dequeue();
	    switch(message.messageType)
	    {
	    case socketUtil::STRING:
		cout<< "STRING: "<<message.data<<"\n"<<flush;
		break;
	    case socketUtil::COMMAND:
		cout<< "COMMAND: "<<message.data<<"\n"<<flush;
		break;
	    }
	}
    }

    sUtil.disconnect();
}
