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


package VjComponents.ConfigEditor;

import java.util.Vector;

import VjComponents.ConfigEditor.ChunkDBTreeModel;
import VjConfig.ConfigChunkDB;
import VjControl.Core;
import VjComponents.ConfigEditor.ChunkOrgTreeModule;


/** Creates trees for the ChunkDBPanel.
 * 
 *  @author Christopher Just
 *  @version $Revision$
 */
public class ChunkDBTreeModelFactory {

    Vector models;

    public ChunkDBTreeModelFactory () {
	models = new Vector();
    }

    public ChunkDBTreeModel getTreeModel (ConfigChunkDB db) {
	ChunkDBTreeModel dbt;
	for (int i = 0; i < models.size(); i++) {
	    dbt = (ChunkDBTreeModel)models.elementAt(i);
	    if ((dbt.chunkdb == db) && (!dbt.inuse)) {
		dbt.inuse = true;
		return dbt;
	    }
	}
        ChunkOrgTreeModule orgtree_module= 
            (ChunkOrgTreeModule)Core.getModule ("ChunkOrgTree Module");
        if (orgtree_module == null)
            Core.consoleErrorMessage ("UI", "Expected ChunkOrgTree Module to exist");

	dbt = new ChunkDBTreeModel (db, orgtree_module.getOrgTree());
	dbt.inuse = true;
	models.addElement (dbt);
	return dbt;
    }

    public void releaseTreeModel (ChunkDBTreeModel dbt) {
	dbt.inuse = false;
    }

    public void removeAllElements () {
        models.removeAllElements();
    }
}
