
package VjComponents.PerfMonitor;

import javax.swing.*;
import javax.swing.tree.*;
import javax.swing.event.*;

public class PanelTree extends JComponent implements TreeModelListener {

    TreeModel mModel;
    //PanelTreeFolder mRootFolder;

    public PanelTree (TreeModel model) {
	super();

	mModel = model;

	setLayout (new BoxLayout (this, BoxLayout.Y_AXIS));
	//setLayout (new GridLayout (1,1));

	doInitialLayout();

	mModel.addTreeModelListener (this);
    }

    public void doInitialLayout () {
	DefaultMutableTreeNode root = (DefaultMutableTreeNode)mModel.getRoot();
	for (int i = 0; i <root.getChildCount(); i++) {
	    DefaultMutableTreeNode node = (DefaultMutableTreeNode)root.getChildAt(i);
	    PerfTreeNodeInfo ni = (PerfTreeNodeInfo)node.getUserObject();
	    PanelTreeFolder ptf = new PanelTreeFolder(ni.getComponent());
	    add (ptf);
	    doInitialLayoutHelper (node, ptf);
	}
    }

    public void doInitialLayoutHelper (DefaultMutableTreeNode root, PanelTreeFolder ptf) {
	for (int i = 0; i < root.getChildCount(); i++) {
	    DefaultMutableTreeNode node = (DefaultMutableTreeNode)root.getChildAt(i);
	    PerfTreeNodeInfo ni = (PerfTreeNodeInfo)node.getUserObject();
	    if (ni.getIndexInfo() == null) {
		// it's a subfolder
		PanelTreeFolder ptf2 = new PanelTreeFolder (ni.getComponent());
		ptf.addSubpanel(ptf2);
		doInitialLayoutHelper (node, ptf2);
	    }
	    else {
		// it's just a data line
		ptf.addSubpanel (ni.getComponent());
	    }
	}
    }

//      public void doInitialLayout (DefaultMutableTreeNode node) {
//  	PerfTreeNodeInfo ni = (PerfTreeNodeInfo)node.getUserObject();
//  	if (ni != null) {
//  	    add (ni.getComponent());
//  	}
//  	for (int i = 0; i < node.getChildCount(); i++) {
//  	    doInitialLayout ((DefaultMutableTreeNode)node.getChildAt(i));
//  	}
//      }

    public void treeNodesChanged(TreeModelEvent e) {
	removeAll();
	doInitialLayout();
    }
    public void treeNodesInserted(TreeModelEvent e) {
	// crappy implementation
	removeAll();
	doInitialLayout();
    }
    public void treeNodesRemoved(TreeModelEvent e) {
	removeAll();
	doInitialLayout();
    }
    public void treeStructureChanged(TreeModelEvent e) {
	removeAll();
	doInitialLayout();
    }

}
 
