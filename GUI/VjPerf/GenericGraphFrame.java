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



package VjPerf;

import javax.swing.*;
import java.awt.Dimension;
import java.awt.event.WindowListener;
import java.awt.event.WindowEvent;
import VjGUI.Core;
import VjGUI.util.*;
import VjPerf.GenericGraphPanel;

public class GenericGraphFrame extends JFrame 
    implements ChildFrame, WindowListener {


    protected JFrameParent parent;
    protected GenericGraphPanel panel;



    public GenericGraphFrame (GenericGraphPanel _panel, String title, JFrameParent _parent) {
	super (title);

	panel = _panel;
	parent = _parent;

	getContentPane().add (panel);
	addWindowListener (this);

	//pack();
	Dimension d = getPreferredSize();
	d.height = Math.min (d.height + 28, Core.screenHeight);
	d.width = Math.min (d.width + 20, Core.screenWidth);
	setSize(d);

    }



    public void refresh() {
	panel.refresh();
    }



    /* WindowListener Stuff */
    public void windowActivated(WindowEvent e) {}
    public void windowClosed(WindowEvent e) {}
    public void windowClosing(WindowEvent e) {
	parent.closedChild (this, false);
    }
    public void windowDeactivated(WindowEvent e) {}
    public void windowDeiconified(WindowEvent e) {}
    public void windowIconified(WindowEvent e) {}
    public void windowOpened(WindowEvent e) {}


    /***************** ChildFrame Stuff ********************/

    public void destroy () {
	dispose();
    }


    public boolean matches (String cl, Object db, Object o) {
	if (cl != null) {
	    try {
		if (!(Class.forName(cl).isInstance(this)))
		    return false;
	    }
	    catch (Exception e) {
		return false;
	    }
	}
	return false;
    }


    public void updateUI () {
	SwingUtilities.updateComponentTreeUI (this);
    }


}
