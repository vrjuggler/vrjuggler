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

/**
 * A frame for displaying a URL with active hyperlinks, eg for help info.
 *
 * @author Christopher Just
 * @version $Revision$
 */
public class HTMLFrame extends JFrame 
    implements ChildFrame, HyperlinkListener, WindowListener {

    JScrollPane sp;
    JEditorPane pane;
    JFrameParent parent;

    /**
     * Constructor
     *
     * @param _parent The "owner" of this frame, who receives close
     *                messages.
     * @param s       Title for the new frame
     * @param url     A URL to display.  May be null.
     */
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



    /**
     * Builds (and verifies) a file URL - utility function for setURL
     *
     * @param fname Name of the file to attempt to open
     * @return A valid URL if possible, or null if the file cannot be
     *         found or opened.
     */
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



    /**
     * sets the URL to be displayed in the frame.
     *
     * @param name A full or partial URL in string form.
     *       If name is not a complete URL, setURL will attempt to search
     *       by appending name to a series of standardized locations.
     *       This is done to ensure that URLs like "DescHelp/foo.html"
     *       work in VjControl.
     */
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



    /**
     * sets the URL to be displayed in the frame.
     *
     * @param url A URL to be displayed.
     *       If the URL contains the string "DescHelp", setURL will
     *       mangle it in an attempt to search the various places that
     *       DescHelp html files are stored (e.g. the VjControl .jar file,
     *       $VJ_BASE_DIR/Data, and ~/.vjconfig).
     */
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



