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
 * Event type for modifications to a descdb : insert, remove, and
 * replace descs...
 */

package VjConfig;

import java.util.EventObject;
import VjConfig.ChunkDesc;

public class DescDBEvent extends EventObject {
    private ChunkDesc new_desc;
    private ChunkDesc old_desc;

    int action;

    public final static int INSERT = 1;
    public final static int REMOVE = 2;
    public final static int REPLACE = 3;
    public final static int REMOVEALL = 4;

    public DescDBEvent (Object _source, int _action, ChunkDesc _olddesc, ChunkDesc _newdesc) {
	super (_source);
	action = _action;
	new_desc = _newdesc;
	old_desc = _olddesc;
    }


    public int getAction() {
	return action;
    }

    public ChunkDesc getOldDesc () {
	return old_desc;
    }

    public ChunkDesc getNewDesc () {
	return new_desc;
    }

}



