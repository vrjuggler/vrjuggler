

#include "vjEnvironmentManager.h"
#include <Kernel/vjKernel.h>
#include <iostream.h>
#include <fstream.h>
#include <strings.h>    // For bzero()
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <Config/vjChunkDescDB.h>
#include <Config/vjConfigChunkDB.h>
#include <Environment/vjTimedUpdate.h>


vjEnvironmentManager::vjEnvironmentManager(): connections() {

    /* I want some hardcoded defaults, yes? */
    Port = 4450;
    listen_thread = NULL;
    listen_socket = -1;
    vector<vjConfigChunk*>* vjNetworkChunks;

    chunkdb = vjKernel::instance()->getChunkDB();
    vjNetworkChunks = chunkdb->getMatching("vjNetwork");
    if (!vjNetworkChunks->empty()) {
	Port = (*vjNetworkChunks)[0]->getProperty("Port");
    }
    delete vjNetworkChunks;
}



vjEnvironmentManager::~vjEnvironmentManager() {
    rejectConnections();
}



bool vjEnvironmentManager::acceptConnections() {
    struct sockaddr_in sockaddress;

    /* here, we open a socket & get ready to read connections */
    listen_socket = socket (AF_INET, SOCK_STREAM, 0);
    bzero(&sockaddress, sizeof (struct sockaddr_in));
    sockaddress.sin_family = PF_INET;
    sockaddress.sin_port = htons(Port);
    
    if (bind ( listen_socket, (sockaddr*)&sockaddress, 
	       sizeof (struct sockaddr_in))) {
	vjDEBUG(1) << "Environment Manager couldn't open socket" 
		   << endl << vjDEBUG_FLUSH;
	return false;
    }

    /* now we ought to spin off a thread to do the listening */
    vjThreadMemberFunctor<vjEnvironmentManager>* memberFunctor = 
	new vjThreadMemberFunctor<vjEnvironmentManager>(this, 
							&vjEnvironmentManager::controlLoop, 
							NULL);
    listen_thread = new vjThread (memberFunctor, 0);
    
    return (listen_thread != NULL);
}



bool vjEnvironmentManager::rejectConnections () {
    for (int i = 0; i < connections.size(); i++) {
	connections[i]->stopProcess();
	delete (connections[i]);
    }
    connections.erase(connections.begin(),connections.end());
    if (listen_thread)
	listen_thread->kill();
    close(listen_socket);
    
    return 1;
}



void vjEnvironmentManager::controlLoop (void* nullParam) {
    /* this is the main loop of the child process spun off by vjNControl */
    struct sockaddr_in servaddr;
    int servsock;
    int len;
    vjNetworkConnect* connection;
    
    vjDEBUG(5) << "Environment Manager network server running." << vjDEBUG_FLUSH;
    
    /* start listening for connections */
    if (listen (listen_socket, 0)) {
	cerr << "vjEnvironmentManager couldn't listen on its socket" << endl;
	return;
    }

    for (;;) {
	len = sizeof (struct sockaddr_in);
	servsock = accept (listen_socket, (sockaddr*)&servaddr, &len);
	vjDEBUG(7) << "vj network server Accepted connection\n" 
		   << vjDEBUG_FLUSH;
	connection = new vjNetworkConnect (servsock);
	connections.push_back( connection );
	connection->startProcess();
    }
}

