/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */




/* a frame that displays a URL w/ active hyperlinks.  Useful for 
 * help info, etc.
 */

package VjGUI;

import java.net.*;
import java.io.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.text.*;
import javax.swing.text.html.*;
import java.util.Enumeration;
import VjGUI.Core;
import VjGUI.util.JFrameParent;
import VjGUI.util.ChildFrame;

public class HTMLFrame extends JFrame 
    implements ChildFrame, HyperlinkListener, WindowListener {

    JScrollPane sp;
    JEditorPane pane;
    JFrameParent parent;

    public HTMLFrame (JFrameParent _parent, String s, URL url) {
	super ();
	parent = _parent;
	pane = new JEditorPane();
	sp = new JScrollPane (pane);
	getContentPane().add(sp);
	pane.setEditable (false);
	
	pane.addHyperlinkListener (this);
	addWindowListener (this);

	setSize (650, 550);
	setURL (url);
    }



    public void setURL (URL url) {
	try {
	    pane.setPage (url);
	    HTMLDocument d = (HTMLDocument)pane.getDocument();
	    String s =  (String)d.getProperty (d.TitleProperty);
	    setTitle (s);
	    /*
	    String s1;
	    d.getStyleSheet().setBaseFontSize (50);
	    Enumeration e = d.getStyleNames();
	    while (e.hasMoreElements()) {
		s1 = (String)e.nextElement();
		System.out.println (s1);
		d.styleChanged(d.getStyle(s1));
	    }
	    validate();
	    */
	}
	catch (IOException ex) {
		Core.consoleErrorMessage ("GUI", "Failed to open URL " + url);
		pane.setText ("<h2>Failed to open URL: " + url + "</h2>");
	}
    }


    public void hyperlinkUpdate (HyperlinkEvent e) {
	if (e.getEventType() == HyperlinkEvent.EventType.ACTIVATED) {
	    setURL (e.getURL());
	}
    }


    /* WindowListener Stuff */
    public void windowActivated(WindowEvent e) {}
    public void windowClosed(WindowEvent e) {
	parent.closedChild (this, true);
    }
    public void windowClosing(WindowEvent e) {}
    public void windowDeactivated(WindowEvent e) {}
    public void windowDeiconified(WindowEvent e) {}
    public void windowIconified(WindowEvent e) {}
    public void windowOpened(WindowEvent e) {}


    /******************* ChildFrame Stuff **********************/
    public void destroy () {
	pane.removeHyperlinkListener (this);
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



