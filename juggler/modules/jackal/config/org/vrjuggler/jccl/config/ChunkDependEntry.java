/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */



package VjConfig;

import VjConfig.ConfigChunk;
import VjConfig.Property;
import java.util.Vector;

public class ChunkDependEntry {

    public ConfigChunk chunk;
    public Vector propdepends; // vector of PropDependEntry

    public ChunkDependEntry() {
	propdepends = new Vector();
    }

    public String toString() {
	int i;
	PropDependEntry pd;
	String s = "Chunk \"" + chunk.getName() + "\"\n";
	for (i = 0; i < propdepends.size(); i++)
	    s += ((PropDependEntry)propdepends.elementAt(i)).toString();
	return s;
    }

}
