
package VjComponents.PerfMonitor;

import javax.swing.*;
import javax.swing.tree.*;
import javax.swing.event.*;

public class PanelTree extends JComponent implements TreeModelListener {

    TreeModel mModel;

    public PanelTree (TreeModel model) {
	super();

	mModel = model;

	setLayout (new BoxLayout (this, BoxLayout.Y_AXIS));

	doInitialLayout((DefaultMutableTreeNode)mModel.getRoot());

	mModel.addTreeModelListener (this);
    }

    public void doInitialLayout (DefaultMutableTreeNode node) {
	PerfTreeNodeInfo ni = (PerfTreeNodeInfo)node.getUserObject();
	if (ni != null) {
	    add (ni.getComponent());
	}
	for (int i = 0; i < node.getChildCount(); i++) {
	    doInitialLayout ((DefaultMutableTreeNode)node.getChildAt(i));
	}
    }

    public void treeNodesChanged(TreeModelEvent e) {
    }
    public void treeNodesInserted(TreeModelEvent e) {
	// crappy implementation
	removeAll();
	doInitialLayout((DefaultMutableTreeNode)mModel.getRoot());
    }
    public void treeNodesRemoved(TreeModelEvent e) {
    }
    public void treeStructureChanged(TreeModelEvent e) {
    }

}
 
