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
        if (url != null)
            setURL (url);
    }

    /* Utility for setURL */
    private URL getFileURL (String fname) {
	URL url = null;
	try {
	    url = new URL ("file", "localhost", -1, fname);
	    // make sure it's really there.
	    if (url.openConnection().getContentLength() == -1)
		url = null;
	}
	catch (Exception e) {
	    url = null;
	}
	return url;
    }

    public void setURL (String name) {

        URL url;
        try {
            url = new URL (name);
	    if (url.openConnection().getContentLength() == -1)
		url = null;
        }
        catch (Exception e) {
            url = null;
        }

        // if url is null, then the string wasn't a complete url, so
        // we'll try appending it to a variety of search paths...
        
        url = getFileURL (System.getProperty ("user.dir") + "/.vjconfig/" + name);
        if (url == null)
            url = getFileURL (System.getProperty ("VJ_BASE_DIR") + "/Data/" + name);
        if (url == null)
            url = getFileURL ("Data/" + name);
        if (url == null)
            url = ClassLoader.getSystemResource ("VjFiles/" + name);
        if (url == null)
            url = ClassLoader.getSystemResource ("VjFiles/NoHelpAvailable.html");
        
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

    public void setURL (URL url) {

        /* a kludge to provide full search paths for DescHelp entries...
         * I'm not really keen on making HTMLFrame aware of this stuff,
         * but I need to do it to handle URLs in the documentation correctly.
         * I should come up with a more generalized way of doing an
         * "HTML search path" for the HTMLFrame.
         */
        System.out.println ("setURL: '" + url + "'");
        int i = url.toString().indexOf("DescHelp/");
        if (i != -1) {
            String name = url.toString().substring(i);
            url = getFileURL (System.getProperty ("user.dir") + "/.vjconfig/" + name);
            if (url == null)
                url = getFileURL (System.getProperty ("VJ_BASE_DIR") + "/Data/" + name);
            if (url == null)
                url = getFileURL ("Data/" + name);
            if (url == null)
                url = ClassLoader.getSystemResource ("VjFiles/" + name);
            if (url == null)
                url = ClassLoader.getSystemResource ("VjFiles/NoHelpAvailable.html");
        }

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



