
/*
 * vjTimedUpdate.C
 * Implementation of TimedUpdate and its children,
 *   TrackerTimedUpdate
 *
 * 6/9/98
 *
 * author: Christopher Just
 */


#include <vjConfig.h>
#include <Input/InputManager/vjInputManager.h>
#include <Environment/vjConnect.h>
#include <Environment/vjTimedUpdate.h>
#include <Kernel/vjKernel.h>
#include <Config/vjChunkFactory.h>

void vjTrackerTimedUpdate::controlLoop(void* nullParam) {
    std::vector<vjConfigChunk*> results;
    vjConnect* t;
    vjConfigChunk *p;
    vjMatrix pos;
    float x, y, z, rx, ry, rz;
    char buf[128];
    vjChunkDesc *desc;
    vjInputManager* im;
    int i;

    vjDEBUG(2) << "EM TimedUpdate:: started controlLoop\n" 
	       << vjDEBUG_FLUSH;
    // get desc
    desc = vjKernel::instance()->getChunkDB()->getChunkDescDB()->
	getChunkDesc("position_timed_update");
    if (!desc) {
	vjDEBUG(1) << "EM TimedUpdate:: couldn't find chunktype"
		   << "\"position_timed_update\"\n" 
		   << vjDEBUG_FLUSH;
	return;
    }
  
    im = vjKernel::instance()->getInputManager();
    if (!im) {
	vjDEBUG(1) << "EM TimedUpdate:: couldn't find Input Manager\n" 
		   << vjDEBUG_FLUSH;
	return;
    }

    for (;;) {
	t = target;  // just in case target changes. may not be safe
	if (t) {
	    for (i = 0; i < 1; i++) {
		p = vjChunkFactory::createChunk (desc);
		if (!p)
		    cerr << "foo - couldn't make chunk" << endl;
		//    im->GetPosData (pos, i);
		// BUG!!!
		pos.getTrans(x, y, z);
		pos.getXYZEuler(rx, ry, rz);
		sprintf (buf, "Position Update Proxy %d", i);
		p->setProperty("Name", buf);
		p->setProperty("Position", x, 0);
		p->setProperty("Position", y, 1);
		p->setProperty("Position", z, 2);
		p->setProperty("Rotation", rx, 0);
		p->setProperty("Rotation", ry, 1);
		p->setProperty("Rotation", rz, 2);
		results.push_back(p);
	    }
	    // gather data
	    vjDEBUG(2) << "EM TimedUpdate:: in controlLoop main loop" 
		       << vjDEBUG_FLUSH;
	    // xmit
	    // lock output
	    //	    t->transmitData ("chunks\n");
	    //t->output.lock();
	    t->output << "chunks\n";
	    for (i = 0; i < results.size(); i++) {
		//t->transmitData (results[i]);
		t->output << *results[i];
		delete(results[i]);
	    }
	    //t->transmitData ("End\n");
	    t->output << "End\n";
	    //t->output.unlock();
	    results.erase (results.begin(), results.end());
	    // unlock parent output
	}

	sginap(refresh_time);
    }
}


