// vjEnvironmentManager.C
// implementation of Environment Manager
//
// author: Christopher Just


#include <vjConfig.h>
#include <iostream.h>
#include <fstream.h>
#include <strings.h>    // For bzero()
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <Environment/vjEnvironmentManager.h>
#include <Kernel/vjKernel.h>
#include <Config/vjChunkDescDB.h>
#include <Config/vjConfigChunkDB.h>
#include <Environment/vjTimedUpdate.h>


vjEnvironmentManager::vjEnvironmentManager(): 
                          connections(), 
                          updaters() {

    /* I want some hardcoded defaults, yes? */
    Port = 4450;
    listen_thread = NULL;
    listen_socket = -1;
    activated = false;
    configured_to_accept = false;

    perf_buffer_reader = new vjTuPerfBufReader(NULL, 500);
    updaters.push_back (perf_buffer_reader);

    reconfigure();
}



vjEnvironmentManager::~vjEnvironmentManager() {
    deactivate();
}



void vjEnvironmentManager::activate() {
    reconfigure();

    /* start updaters */
    for (int i = 0; i < updaters.size(); i++) {
	updaters[i]->startProcess();
    }

    activated = true;
    if (configured_to_accept) {
	acceptConnections();
    }
    else
	vjDEBUG(0) << "NOTE: EM not configured to accept connections!!!\n" << vjDEBUG_FLUSH;
}



void vjEnvironmentManager::deactivate() {
    activated = false;
    rejectConnections();
    killConnections();

    /* kill updaters - this needs work */
    for (int i = 0; i < updaters.size(); i++) {
	updaters[i]->stopProcess();
    }
}



bool vjEnvironmentManager::acceptConnections() {
    struct sockaddr_in sockaddress;

    if (!activated)
	return false;
    if (listen_thread != NULL)
	return true;
    
    /* here, we open a socket & get ready to read connections */
    listen_socket = socket (AF_INET, SOCK_STREAM, 0);
    bzero(&sockaddress, sizeof (struct sockaddr_in));
    sockaddress.sin_family = PF_INET;
    sockaddress.sin_port = htons(Port);
    
    if (bind ( listen_socket, (sockaddr*)&sockaddress, 
	       sizeof (struct sockaddr_in))) {
	vjDEBUG(0) << "vjEnvironmentManager couldn't open socket\n" 
		   << vjDEBUG_FLUSH;
	return false;
    }
    else
	vjDEBUG(0) << "vjEnvironmentManager accepting connections on port "
		   << Port << '\n' << vjDEBUG_FLUSH;

    /* now we ought to spin off a thread to do the listening */
    vjThreadMemberFunctor<vjEnvironmentManager>* memberFunctor = 
	new vjThreadMemberFunctor<vjEnvironmentManager>(
				this, 
				&vjEnvironmentManager::controlLoop, 
	       			NULL);
    listen_thread = new vjThread (memberFunctor, 0);
    
    return (listen_thread != NULL);
}



bool vjEnvironmentManager::rejectConnections () {
    if (listen_thread) {
	listen_thread->kill();
	listen_thread = NULL;
	close(listen_socket);
    }
    
    return 1;
}



void vjEnvironmentManager::killConnections() {
    int i;

    for (i = 0; i < connections.size(); i++) {
	connections[i]->stopProcess();
	delete (connections[i]);
    }
    connections.erase (connections.begin(), connections.end());
}



bool vjEnvironmentManager::isAccepting() {
    return (listen_thread != NULL);
}



void vjEnvironmentManager::addPerfDataBuffer (vjPerfDataBuffer *v) {
    perf_buffer_reader->addBuffer(v);
}



vjConnect* vjEnvironmentManager::getConnect (char* s) {
    for (int i = 0; i < connections.size(); i++)
	if (!strcasecmp (s, connections[i]->name))
	    return connections[i];
    return NULL;
}



void vjEnvironmentManager::controlLoop (void* nullParam) {
    // Child process used to listen for new network connections
    struct sockaddr_in servaddr;
    int servsock;
    int len;
    vjConnect* connection;

    vjDEBUG(3) << "vjEnvironmentManager network server running.\n" 
	       << vjDEBUG_FLUSH;
    
    /* start listening for connections */
    if (listen (listen_socket, 0)) {
	vjDEBUG(1) << "ERROR: vjEnvironmentManager socket listen "
		   << "failed\n" << vjDEBUG_FLUSH;
	return;
    }

    for (;;) {
	len = sizeof (struct sockaddr_in);
	servsock = accept (listen_socket, 
			   (sockaddr*)&servaddr, &len);
	connection = new vjConnect (servsock);
	connections.push_back( connection );
	connection->startProcess();
    }
}



void vjEnvironmentManager::reconfigure () {
    int i;
    vjConfigChunk *c;
    vector<vjConfigChunk*>* v;
    vjConnect* vn;
    char* s;
    bool networkingchanged;
    int newport;

    chunkdb = vjKernel::instance()->getChunkDB();
    v = chunkdb->getMatching ("EnvironmentManager");

    if (v->size() == 0) {
	vjDEBUG(1) << "ERROR: vjEnvironmentManager::reconfigure()"
		   << " - can't find ConfigChunk. Using defaults.\n"
		   << vjDEBUG_FLUSH;
	return;
    }
    if (v->size() > 1) {
	vjDEBUG(2) << "vjEnvironmentManager::reconfigure()"
		   << " - multiple EM ConfigChunks found. Using "
		   << "first one.\n" << vjDEBUG_FLUSH;
    }
    
    c = (*v)[0];
    delete v;
    
    configured_to_accept = c->getProperty ("AcceptConnections");
    newport = c->getProperty("Port");

    if (newport == 0)
	newport = Port;
    if (newport != Port || configured_to_accept != isAccepting())
	networkingchanged = 1;
    
    /* BUG: we need to 
     *    a) delete current connects that aren't in v
     *    b) not add connects for ones that are in v
     * otherwise we won't reconfigure correctly
     */
    v = chunkdb->getMatching ("FileConnect");
    for (i = 0; i < v->size(); i++) {
	vn = new vjConnect ((*v)[i]);
	connections.push_back (vn);
	vn->startProcess();
    }
    delete v;
    
    s = c->getProperty ("PerformanceTarget");
    perf_buffer_reader->setTarget(getConnect(s));
    delete s;
    
    if (networkingchanged) {
	Port = newport;
	if (isAccepting())
	    rejectConnections();
	if (configured_to_accept)
	    acceptConnections();
	else
	    killConnections();
    }

    /* repeat above w/ various tu's */
    
}
