
#include <vjConfig.h>
#include <Environment/vjTuPerfBufReader.h>
#include <Environment/vjConnect.h>

vjTuPerfBufReader::vjTuPerfBufReader( vjConnect *_target, 
				      float _refresh_time ):
            vjTimedUpdate(_target, _refresh_time),
            buffers(), bufferslock() {

    control_thread = NULL;
    name = strdup ("unnamed_perf_buffer_reader");
}



vjTuPerfBufReader::~vjTuPerfBufReader () {
    stopProcess();
}



void vjTuPerfBufReader::addBuffer (vjPerfDataBuffer *b) {
    bufferslock.acquire();
    buffers.push_back (b);
    bufferslock.release();
    if (control_thread)
	b->activate();
    else
	b->deactivate();
}


void vjTuPerfBufReader::removeBuffer (vjPerfDataBuffer *b) {
    std::vector<vjPerfDataBuffer*>::iterator it;

    b->deactivate();
    bufferslock.acquire();
    // this is one of those things I really hate:
    for (it = buffers.begin(); it != buffers.end(); it++) {
	if (*it == b) {
	    buffers.erase(it);
	    break;
	}
    }
    bufferslock.release();
}



bool vjTuPerfBufReader::startProcess() {
    // Create a new thread to handle the control
    int i;

    if (control_thread != NULL) 
	return true;

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
    bool target_was_null = true;
    for (;;) {
	sginap (50);
	if (target == NULL) {
	    target_was_null = true;
	    //cout << "perf buf reader: target is null" << endl;
	    continue;
	}
	if (target_was_null) {
	    target_was_null = false;
	    //target->reopenFile();
	}
	//cout << "emoutput is fnamed '" << target->filename << "'" << endl;
	bufferslock.acquire();
	target->output.lock();
	for (p = buffers.begin(); p != buffers.end(); p++) {
	    //cout << "writing a buffer named " << (*p)->getName() << endl;
	    (*p)->write (target->output);
	}
	target->output << flush;
	target->output.unlock();
	bufferslock.release();
    }
}

