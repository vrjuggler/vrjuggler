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
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */
/*
 * Event type for modifications to a chunkdb : insert, remove, and
 * replace chunks...
 */

package VjConfig;

import java.util.EventObject;
import VjConfig.ConfigChunk;

public class ChunkDBEvent extends EventObject {
    private ConfigChunk newchunk;
    private ConfigChunk oldchunk;

    int action;

    public final static int INSERT = 1;
    public final static int REMOVE = 2;
    public final static int REPLACE = 3;
    public final static int REMOVEALL = 4;

    public ChunkDBEvent (Object _source, int _action, ConfigChunk _oldchunk, ConfigChunk _newchunk) {
	super (_source);
	action = _action;
	newchunk = _newchunk;
	oldchunk = _oldchunk;
    }


    public int getAction() {
	return action;
    }

    public ConfigChunk getOldChunk () {
	return oldchunk;
    }

    public ConfigChunk getNewChunk () {
	return newchunk;
    }

}



