
package VjGUI;

import VjGUI.ChunkDBTreeModel;
import VjConfig.ConfigChunkDB;
import java.util.Vector;
import VjGUI.Core;

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
	dbt = new ChunkDBTreeModel (db, Core.chunkorgtree);
	dbt.inuse = true;
	models.addElement (dbt);
	return dbt;
    }

    public void releaseTreeModel (ChunkDBTreeModel dbt) {
	dbt.inuse = false;
    }

}
