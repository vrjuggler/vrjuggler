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
 
