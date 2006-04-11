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
package org.vrjuggler.jccl.perfmonitor;

import java.awt.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.tree.*;

public class PanelTree extends JComponent implements TreeModelListener {

    TreeModel mModel;
    //PanelTreeFolder mRootFolder;
    GridBagLayout gbl;
    GridBagConstraints gbc;

    public PanelTree (TreeModel model) {
	super();

	mModel = model;

	//setLayout (new BoxLayout (this, BoxLayout.Y_AXIS));
	//setLayout (new GridLayout (1,1));
        gbl = new GridBagLayout();
        setLayout(gbl);

        gbc = new GridBagConstraints();
        gbc.anchor = GridBagConstraints.NORTHWEST;
        gbc.fill = GridBagConstraints.REMAINDER;

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


    //-------------------- TreeModelListener Stuff ------------------------

    public void treeNodesChanged(TreeModelEvent e) {
	//System.out.println ("got a treenodeschanged event");
	repaint();
//  	removeAll();
//  	doInitialLayout();
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
 
