/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <iostream>
#include <fstream>
#include <Config/vjConfigChunkDB.h>
#include <Config/vjParseUtil.h>
#include <Config/vjChunkFactory.h>
#include <Utils/vjDebug.h>
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

    std::cout << "loading chunk descriptions..." << std::flush;
    desc.load("chunks");
    std::cout << "OK." << std::endl;
    std::cout << "Read " << desc.size() << " descriptions." << std::endl;
    std::cout << desc << "\n--------------------------------\n";
    std::cout << "Getting chunkdesc 'flockobirds'..." << std::flush;
    d = desc.getChunkDesc("flockobirds");
    if (d) {
	std::cout << "OK." << std::endl;
    }
    else {
	std::cout << "ERROR: Couldn't find chunkdesc!" << std::endl;
	err = 1;
    }


    // Checking enumerations
    std::cout << "Getting chunkdesc 'enumtest'..." << std::flush;
    d = desc.getChunkDesc("enumtest");
    if (d) {
	std::cout << "OK." << std::endl;
	std::cout << *d;
    }
    else {
	std::cout << "ERROR: Couldn't find chunkdesc!" << std::endl;
	err = 1;
    }



    std::cout << "Getting chunkdesc 'ptrtest'..." << std::flush;
    d = desc.getChunkDesc("ptrtest");
    if (d) {
	std::cout << "OK.  Read:" << std::endl;
	std::cout << *d << std::endl;
    }
    else {
	std::cout << "ERROR: Couldn't find chunkdesc ptrtest!" << std::endl;
	err = 1;
    }


    std::cout << "\n-------DONE-TESTING-CHUNKDESCSDB--------" << std::endl;
    if (err == 1) {
	std::cout << "Failed ChunkDescDB test - aborting" << std::endl;
	//    exit(1);
    }
    
    /**************************************************************
     *                TESTING CONFIGCHUNKDB                       *
     **************************************************************/

    std::cout << "\n\nCreating ConfigChunkDB..." << std::flush;
    vjConfigChunkDB chunk(&desc);
    std::cout << "OK." << std::endl;  // at least we didn't segv.
    
    std::cout << "loading config chunks..." << std::flush;
    if (!chunk.load("config"))
	err = 1;
    if (err) 
	std::cout << "Failed." << std::endl;
    else
	std::cout << "OK." << std::endl;
    //std::cout << "Read " << desc.size() << " descriptions." << std::endl;
    
    std::cout << "Printing Chunk DB:" << std::endl;
    std::cout << chunk;
    std::cout << "endochunks" << std::endl;

    std::cout << "Getting ConfigChunk 'chunk in chunk test 1'..." <<std::flush;
    c = chunk.getChunk ("chunk in chunk test 1");
    if (c == NULL) {
	err = 1;
	std::cout << "Failed!" << std::endl;
    }
    else 
	std::cout << "OK.\nPrinting:\n" << *c << std::endl;
    
    std::cout << "Getting dependencies for 'chunk in chunk test 1'..."
              << std::flush;
    std::vector<std::string> deps = c->getDependencies();
    std::cout << "OK\nPrinting:" << std::endl;
    for (int k = 0; k < deps.size(); k++) {
	std::cout << "    " << deps[k] << std::endl;
    }
    std::cout << "Finished." << std::endl;

    std::cout << "Getting its 2nd display prop..." << std::flush;
    c2 = c->getProperty ("bigchunk", 1);
    if (c2 == NULL) {
	err = 1;
	std::cout << "Failed!" << std::endl;
    }
    else
	std::cout << "OK.\nPrinting:\n" << *c2 << std::endl;
    
    c3 = chunk.getChunk("Right");
    if (c3) {
	c->setProperty ("bigchunk", c3, 1);
	std::cout << "replaced c's 2nd bigchunk value w/ 'right':\n"
	          << *c3 << "\nResulting chunk is:\n" << *c;
    }
    
    std::cout << "Getting ConfigChunk 'ptrtest 1'..." << std::flush;
    c = chunk.getChunk("ptrtest 1");
    if (c == NULL) {
	err = 1;
	std::cout << "Failed!" << std::endl;
    }
    else {
	std::cout << "OK.";
    
	/* query "ptrtest 1" chunk */
    
	std::cout << "Querying 'ptrtest 1'" << std::endl;
	std::string s = c->getProperty ("ptr");

	std::cout << "  'ptr' property value is '" << s << "'" << std::endl;

	std::cout << "Getting property '" << s << "'..." << std::flush;
	c2 = chunk.getChunk(s);
	if (c2) {
	    std::cout << "OK.  Read:\n" << *c2 << std::endl;
	}
	else {
	    err = 1;
	    std::cout << "Failed!" << std::endl;
	}

    }


    /****************** TESTING VARVALUES *************************/
std::cout << "Getting ConfigChunk 'varvaltest 1'..." << std::flush;
    c = chunk.getChunk("varvaltest 1");
    if (c == NULL) {
	err = 1;
	std::cout << "Failed!" << std::endl;
    }
    else
	std::cout << "OK." << std::endl;
    
    if (c) {
	/* query "varvaltest 1" chunk */
	std::cout << "  Checking booleans...";
	
	bool b = c->getProperty ("boolprop");
	if (b)
	    std::cout << "  Error: initial value of boolprop is wrong."
	              << std::endl;
	bool v1 = 1;
	bool v2;
	c->setProperty ("boolprop", v1);
	v2 = c->getProperty ("boolprop");
	if (v1 != v2) {
	    std::cout << "Error in boolean set-get test 1" << std::endl;
	    err = 1;
	}
	v1 = 0;
	c->setProperty ("boolprop", v1);
	v2 = c->getProperty ("boolprop");
	if (v1 != v2) {
	    std::cout << "Error in boolean set-get test 2" << std::endl;
	    err = 1;
	}
	if (err) {
	    std::cout << "Error: boolean get or set is broken!" << std::endl;
	    std::cout << "Chunk prints as:\n" << *c << std::endl;
	}
	else
	    std::cout << "OK." << std::endl;
    }

    std::cout << "\n-------DONE-TESTING-CONFIGCHUNKDB--------" << std::endl;
    if (err == 1) {
	std::cout << "Failed ConfigChunkDB test - aborting" << std::endl;
	exit(1);
    }

}
