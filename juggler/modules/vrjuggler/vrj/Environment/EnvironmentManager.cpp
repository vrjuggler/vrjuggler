// vjEnvironmentManager.C
// implementation of Environment Manager
//
// author: Christopher Just


// needed for network cruft
#include <strings.h>    // For bzero()
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <Environment/vjEnvironmentManager.h>
#include <Environment/vjConnect.h>
#include <Performance/vjPerfDataBuffer.h>
#include <Config/vjChunkDescDB.h>
#include <Config/vjConfigChunkDB.h>
#include <Environment/vjTimedUpdate.h>

vjEnvironmentManager::vjEnvironmentManager():
                          connections(),
                          //updaters(),
                          perf_buffers() {

    /* I want some hardcoded defaults, yes? */
    Port = 4450;
    listen_thread = NULL;
    listen_socket = -1;
    configured_to_accept = false;
    perf_refresh_time = 500;

    perf_target_name = "";
    perf_target = NULL;
    current_perf_config = NULL;
}



vjEnvironmentManager::~vjEnvironmentManager() {
    rejectConnections();
    killConnections();
}



bool vjEnvironmentManager::isAccepting() {
    return (listen_thread != NULL);
}



void vjEnvironmentManager::addPerfDataBuffer (vjPerfDataBuffer *v) {
    vjDEBUG (vjDBG_ENV_MGR, 3) << "EM adding perf data buffer " << v->getName() << "\n"
		<< vjDEBUG_FLUSH;
    perf_buffers.push_back(v);
    activatePerfBuffers();
}




void vjEnvironmentManager::removePerfDataBuffer (vjPerfDataBuffer *b) {
    std::vector<vjPerfDataBuffer*>::iterator it;

    b->deactivate();
    if (perf_target)
	perf_target->removeTimedUpdate (b);
    // this is one of those things I really hate:
    for (it = perf_buffers.begin(); it != perf_buffers.end(); it++) {
        if (*it == b) {
            perf_buffers.erase(it);
            break;
        }
    }

}



void vjEnvironmentManager::sendRefresh() {
    for (int i = 0; i < connections.size(); i++) {
	connections[i]->sendRefresh();
    }

}



//: ConfigChunkHandler stuff
//! PRE: configCanHandle(chunk) == true
//! RETURNS: success
bool vjEnvironmentManager::configAdd(vjConfigChunk* chunk) {
    bool networkingchanged = false;
    int newport;

    //cout << "EM configAdd recv'd chunk:\n" << *chunk << endl;
    std::string s = chunk->getType();
    if (!vjstrcasecmp (s, "EnvironmentManager")) {
	configured_to_accept = chunk->getProperty ("AcceptConnections");
	newport = chunk->getProperty("Port");

	if (newport == 0)
	    newport = Port;
	if ((newport != Port) || (configured_to_accept != isAccepting()))
	    networkingchanged = true;
	perf_target_name = (std::string)chunk->getProperty ("PerformanceTarget");
	vjConnect* new_perf_target = getConnect(perf_target_name);
	if (new_perf_target != perf_target)
	    setPerformanceTarget (NULL);

	if (networkingchanged) {
	    Port = newport;
	    if (isAccepting())
		rejectConnections();
	    if (configured_to_accept)
		acceptConnections();
	    else
		killConnections();
	}
	setPerformanceTarget(getConnect(s));
	
	return true;
    }
    else if (!vjstrcasecmp (s, "PerfMeasure")) {
	current_perf_config = new vjConfigChunk (*chunk);
	activatePerfBuffers();
	return true;
    }
    else if (!vjstrcasecmp (s, "FileConnect")) {
	// I wanted to just look if the fileconnect had been added yet.
	// however I seem to have a chicken/egg problem.
	// so the kludge we'll do now is to not directly add a chunk that's
	// of type VJC_INTERACTIVE. sigh.
	// Unfortunately, this means that for other cases (such as attaching
	// to a named pipe) we're still broken
	if ((int)chunk->getProperty("Mode") != VJC_INTERACTIVE) {
	    vjDEBUG (vjDBG_ENV_MGR, 1) << "adding fileconnect\n"
				       << vjDEBUG_FLUSH;
	    // it's new to us
	    vjConnect* vn = new vjConnect (chunk);
	    connections.push_back (vn);
	    vn->startProcess();
	    if (!vjstrcasecmp (vn->getName(), perf_target_name))
		setPerformanceTarget (vn);
	}
	return true;
    }
    // shouldn't ever happen
    vjDEBUG(vjDBG_ENV_MGR,3) << "EnvironmentManager::configAdd - Unrecognized Chunk " + s << endl
	       << vjDEBUG_FLUSH;
    return false;
}



//: Remove the chunk from the current configuration
//! PRE: configCanHandle(chunk) == true
//!RETURNS: success
bool vjEnvironmentManager::configRemove(vjConfigChunk* chunk) {

    std::string s = chunk->getType();
    if (!vjstrcasecmp (s, "EnvironmentManager")) {
	// this could be trouble if the chunk being removed isn't the chunk
	// we were configured with...
	rejectConnections();
	Port = 4450;
	configured_to_accept = false;
	return true;
    }
    else if (!vjstrcasecmp (s, "PerfMeasure")) {
	if (current_perf_config) {
	    if (!vjstrcasecmp (current_perf_config->getProperty ("Name"),
			       chunk->getProperty ("Name"))) {
		delete (current_perf_config);
		current_perf_config = NULL;
		deactivatePerfBuffers ();
	    }
	}
	return true;
    }
    else if (!vjstrcasecmp (s, "FileConnect")) {
	cout << "removing fileconnect named " << flush
	     << chunk->getProperty ("Name") << endl;
 	vjConnect* c = getConnect (chunk->getProperty ("Name"));
 	if (c) {
 	    removeConnect (c);
 	}
	cout << "done fileconnect remove" << endl;
	return true;
    }

    return false;
}



//: Can the handler handle the given chunk?
//! RETURNS: true - Can handle it
//+          false - Can't handle it
bool vjEnvironmentManager::configCanHandle(vjConfigChunk* chunk) {
    std::string s = chunk->getType();
    return (!vjstrcasecmp (s, "EnvironmentManager") ||
	    !vjstrcasecmp (s, "PerfMeasure") ||
	    !vjstrcasecmp (s, "FileConnect"));
}



//-------------------- PRIVATE MEMBER FUNCTIONS -------------------------

void vjEnvironmentManager::removeConnect (vjConnect* con) {
    if (!con)
	return;
    if (con == perf_target)
	setPerformanceTarget (NULL);
    std::vector<vjConnect*>::iterator i;
    for (i = connections.begin(); i != connections.end(); i++)
	if (con == *i) {
	    connections.erase (i);
	    delete con;
	    break;
	}
}



void vjEnvironmentManager::setPerformanceTarget (vjConnect* con) {
    if (con == perf_target)
	return;
    deactivatePerfBuffers();
    perf_target = con;
    activatePerfBuffers();
}



vjConnect* vjEnvironmentManager::getConnect (const std::string& s) {
    for (int i = 0; i < connections.size(); i++)
	if (s == connections[i]->getName())
	    return connections[i];
    return NULL;
}



void vjEnvironmentManager::controlLoop (void* nullParam) {
    // Child process used to listen for new network connections
    struct sockaddr_in servaddr;
    int servsock;
    int len;
    vjConnect* connection;

    vjDEBUG(vjDBG_ENV_MGR,3) << "vjEnvironmentManager started control loop.\n"
	       << vjDEBUG_FLUSH;

    /* start listening for connections */
    if (listen (listen_socket, 0)) {
	vjDEBUG(vjDBG_ALL,0) << "ERROR: vjEnvironmentManager socket listen "
		   << "failed\n" << vjDEBUG_FLUSH;
	return;
    }

    for (;;) {
	len = sizeof (struct sockaddr_in);
	servsock = accept (listen_socket,
			   (sockaddr*)&servaddr, &len);
	char name[128];
	sprintf (name, "Network Connect %d", servsock);
	connection = new vjConnect (servsock, (std::string)name);
	connections.push_back( connection );
	connection->startProcess();
    }
}



void vjEnvironmentManager::deactivatePerfBuffers () {
    std::vector<vjPerfDataBuffer*>::iterator i;
    for (i = perf_buffers.begin(); i != perf_buffers.end(); i++) {
	(*i)->deactivate();
	if (perf_target)
	    perf_target->removeTimedUpdate (*i);
    }
}


void vjEnvironmentManager::activatePerfBuffers () {
    // activates all perf buffers configured to do so
    // this is still a bit on the big and bulky side.

    if (perf_target == NULL || current_perf_config == NULL) {
	deactivatePerfBuffers();
	return;
    }

    if (perf_buffers.empty())
	return;

    std::vector<vjVarValue*> v = current_perf_config->getAllProperties ("TimingTests");
    std::vector<vjPerfDataBuffer*>::const_iterator b;
    std::vector<vjVarValue*>::const_iterator val;
    bool found;
    vjConfigChunk* ch;

    for (b = perf_buffers.begin(); b != perf_buffers.end(); b++) {
	found = false;
	for (val = v.begin(); val != v.end(); val++) {
	    ch = *(*val); // this line demonstrates a subtle danger
	    if (ch == NULL) cout << "somethings's fucked up" << endl;
	    if ((bool)ch->getProperty ("Enabled")) {
		if (!vjstrncasecmp(ch->getProperty("Prefix"), (*b)->getName()))
		    found = true;
	    }
	}
	if (found) {
	    (*b)->activate();
	    perf_target->addTimedUpdate ((*b), perf_refresh_time);
	}
	else if ((*b)->isActive()) {
	    (*b)->deactivate();
	    perf_target->removeTimedUpdate (*b);
	}
    }
    for (val = v.begin(); val != v.end(); val++) {
	delete (*val);
    }

}



bool vjEnvironmentManager::acceptConnections() {
    struct sockaddr_in sockaddress;

    if (listen_thread != NULL)
	return true;

    /* here, we open a socket & get ready to read connections */
    listen_socket = socket (AF_INET, SOCK_STREAM, 0);
    bzero(&sockaddress, sizeof (struct sockaddr_in));
    sockaddress.sin_family = PF_INET;
    sockaddress.sin_port = htons(Port);

    if (bind ( listen_socket, (sockaddr*)&sockaddress,
	       sizeof (struct sockaddr_in))) {
	vjDEBUG(vjDBG_ALL,0) << "ERROR: vjEnvironmentManager couldn't open socket\n"
		   << vjDEBUG_FLUSH;
	return false;
    }
    else
	vjDEBUG(vjDBG_ALL,0) << "vjEnvironmentManager accepting connections on port "
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



