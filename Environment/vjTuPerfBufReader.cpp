
#include <vjConfig.h>
#include <Environment/vjTuPerfBufReader.h>
#include <Environment/vjConnect.h>

vjTuPerfBufReader::vjTuPerfBufReader( vjConnect *_target, 
				      float _refresh_time ):
            vjTimedUpdate(_target, _refresh_time),
            buffers() {

    control_thread = NULL;
}



vjTuPerfBufReader::~vjTuPerfBufReader () {
    stopProcess();
}



void vjTuPerfBufReader::addBuffer (vjPerfDataBuffer *b) {
    buffers.push_back (b);
    if (control_thread)
	b->activate();
    else
	b->deactivate();
}



bool vjTuPerfBufReader::startProcess() {
    // Create a new thread to handle the control
    int i;

    if (control_thread != NULL) 
	return false;

    vjThreadMemberFunctor<vjTimedUpdate> *memberFunctor = 
	new vjThreadMemberFunctor<vjTimedUpdate>(this, &vjTimedUpdate::controlLoop, NULL);
    
    control_thread = new vjThread(memberFunctor, 0);

    if (control_thread) {
	for (i = 0; i < buffers.size(); i++)
	    buffers[i]->activate();
	return true;
    }
    else
	return false;
}
    


bool vjTuPerfBufReader::stopProcess() {
    int i;
    if (control_thread) {
	for (i = 0; i < buffers.size(); i++)
	    buffers[i]->deactivate();
	control_thread->kill();
	delete control_thread;
    }
    control_thread = NULL;
    return 1;
}



void vjTuPerfBufReader::controlLoop(void* nullParam) {
    std::vector<vjPerfDataBuffer*>::iterator p;

    for (;;) {
	sginap (50);

	if (target == NULL)
	    continue;
	target->output.lock();
	for (p = buffers.begin(); p != buffers.end(); p++) {
	    (*p)->write (target->output);
	}
	target->output.unlock();
    }
}

