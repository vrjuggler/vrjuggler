

#include <iostream.h>
#include <fstream.h>
#include <Config/vjConfigChunkDB.h>
#include <Config/vjParseUtil.h>
#include <Config/vjChunkFactory.h>
#include <Kernel/vjDebug.h>
#include <Kernel/vjSGISystemFactory.h>


main () {

    vjChunkDescDB desc;
    vjChunkFactory::setChunkDescDB (&desc);
    vjChunkDesc *d;
    vjConfigChunk *c,*c2,*c3;
    int err = 0;


    /**************************************************************
     *                   TESTING CHUNKDESCDB                      *
     **************************************************************/

    cout << "loading chunk descriptions..." << flush;
    desc.load("chunks");
    cout << "OK." << endl;
    cout << "Read " << desc.size() << " descriptions." << endl;
    cout << desc << "\n--------------------------------\n";
    cout << "Getting chunkdesc 'flockobirds'..." << flush;
    d = desc.getChunkDesc("flockobirds");
    if (d) {
	cout << "OK." << endl;
    }
    else {
	cout << "ERROR: Couldn't find chunkdesc!" << endl;
	err = 1;
    }


    // Checking enumerations
    cout << "Getting chunkdesc 'enumtest'..." << flush;
    d = desc.getChunkDesc("enumtest");
    if (d) {
	cout << "OK." << endl;
	cout << *d;
    }
    else {
	cout << "ERROR: Couldn't find chunkdesc!" << endl;
	err = 1;
    }



    cout << "Getting chunkdesc 'ptrtest'..." << flush;
    d = desc.getChunkDesc("ptrtest");
    if (d) {
	cout << "OK.  Read:" << endl;
	cout << *d << endl;
    }
    else {
	cout << "ERROR: Couldn't find chunkdesc ptrtest!" << endl;
	err = 1;
    }


    cout << "\n-------DONE-TESTING-CHUNKDESCSDB--------" << endl;
    if (err == 1) {
	cout << "Failed ChunkDescDB test - aborting" << endl;
	//    exit(1);
    }
    
    /**************************************************************
     *                TESTING CONFIGCHUNKDB                       *
     **************************************************************/

    cout << "\n\nCreating ConfigChunkDB..." << flush;
    vjConfigChunkDB chunk(&desc);
    cout << "OK." << endl;  // at least we didn't segv.
    
    cout << "loading config chunks..." << flush;
    if (!chunk.load("config"))
	err = 1;
    if (err) 
	cout << "Failed." << endl;
    else
	cout << "OK." << endl;
    //cout << "Read " << desc.size() << " descriptions." << endl;
    
    //cout << "Printing Chunk DB:" << endl;
    //cout << chunk;
    //cout << "endochunks" << endl;

    cout << "Getting ConfigChunk 'chunk in chunk test 1'..." <<flush;
    c = chunk.getChunk ("chunk in chunk test 1");
    if (c == NULL) {
	err = 1;
	cout << "Failed!" << endl;
    }
    else
	cout << "OK.\nPrinting:\n" << *c << endl;
    
    cout << "Getting its 2nd display prop..." << flush;
    c2 = c->getProperty ("bigchunk", 1);
    if (c2 == NULL) {
	err = 1;
	cout << "Failed!" << endl;
    }
    else
	cout << "OK.\nPrinting:\n" << *c2 << endl;
    
    c3 = chunk.getChunk("Right");
    if (c3) {
	c->setProperty ("bigchunk", c3, 1);
	cout << "replaced c's 2nd bigchunk value w/ 'right':\n"
	     << *c3 << "\nResulting chunk is:\n"
	     << *c;
    }
    
    cout << "Getting ConfigChunk 'ptrtest 1'..." << flush;
    c = chunk.getChunk("ptrtest 1");
    if (c == NULL) {
	err = 1;
	cout << "Failed!" << endl;
    }
    else
	cout << "OK.";
    
    if (c) {
	/* query "ptrtest 1" chunk */
    
	cout << "Querying 'ptrtest 1'" << endl;
	char *s = c->getProperty ("ptr");
	if (s == NULL) {
	    err = 1;
	    cout << "  Error: GetProperty 'ptr' returned NULL." << endl;
	}
	else {
	    cout << "  'ptr' property value is '" << s << "'" << endl;
	}
	if (s) {
	    cout << "Getting property '" << s << "'..." << flush;
	    c2 = chunk.getChunk(s);
	    if (c2) {
		cout << "OK.  Read:\n" << *c2 << endl;
	    }
	    else {
		err = 1;
		cout << "Failed!" << endl;
	    }

	    
	}
    }


    /****************** TESTING VARVALUES *************************/
    cout << "Getting ConfigChunk 'varvaltest 1'..." << flush;
    c = chunk.getChunk("varvaltest 1");
    if (c == NULL) {
	err = 1;
	cout << "Failed!" << endl;
    }
    else
	cout << "OK." << endl;
    
    if (c) {
	/* query "varvaltest 1" chunk */
	cout << "  Checking booleans...";
	
	bool b = c->getProperty ("boolprop");
	if (b)
	    cout << "  Error: initial value of boolprop is wrong." << endl;
	bool v1 = 1;
	bool v2;
	c->setProperty ("boolprop", v1);
	v2 = c->getProperty ("boolprop");
	if (v1 != v2) {
	    cout << "Error in boolean set-get test 1" << endl;
	    err = 1;
	}
	v1 = 0;
	c->setProperty ("boolprop", v1);
	v2 = c->getProperty ("boolprop");
	if (v1 != v2) {
	    cout << "Error in boolean set-get test 2" << endl;
	    err = 1;
	}
	if (err) {
	    cout << "Error: boolean get or set is broken!" << endl;
	    cout << "Chunk prints as:\n" << *c << endl;
	}
	else
	    cout << "OK." << endl;
    }

    cout << "\n-------DONE-TESTING-CONFIGCHUNKDB--------" << endl;
    if (err == 1) {
	cout << "Failed ConfigChunkDB test - aborting" << endl;
	exit(1);
    }

}



