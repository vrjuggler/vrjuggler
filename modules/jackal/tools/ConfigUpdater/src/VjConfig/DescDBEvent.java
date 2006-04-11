/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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



