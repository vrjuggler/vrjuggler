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
    
    std::string words[4] = {"Kevin", "is", "really", "cool"};
    socketUtil sUtil;
    
    sUtil.setRole( socketUtil::SERVER );
    sUtil.setRemote(address, port);
    

	
	sUtil.connect();
	sleep(1);
	for (int i = 0; i < 50000; ++i)
	for (int x = 0; x < 4; ++x)
	{
	    socketUtil::Message message;
	    message.messageType = socketUtil::STRING;
	    message.sizeOfData = 69;
	    message.data = words[x];
	    
	    sUtil.enqueue( message );
	}
	
	
	sUtil.sendQueue();
	
	sleep(5);
	
	sUtil.disconnect();

}
