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


package VjComponents.UI;

import java.net.*;
import java.io.*;
import java.awt.event.*;
import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.Container;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.text.*;
import javax.swing.text.html.*;
import javax.swing.border.EmptyBorder;
import java.util.*;

import VjControl.Core;
import VjComponents.UI.Widgets.ChildFrame;


/**
 * A frame for displaying a URL with active hyperlinks, eg for help info.
 *
 *  An HTMLFrame is a source of ActionEvents with the action command "Close".
 *
 * @author Christopher Just
 * @version $Revision$
 */
public class HTMLFrame extends JFrame 
    implements ChildFrame, HyperlinkListener, WindowListener, ActionListener {


    URL contents_url;
    URL current_url;
    Vector back_buffer;
    Vector forward_buffer;

    JScrollPane sp;
    JEditorPane pane;

    protected JButton contents_button;
    protected JButton forward_button;
    protected JButton back_button;
    protected JButton close_button;

    private java.util.List action_listeners;


    /**
     * Constructor
     *
     * @param s       Title for the new frame
     * @param url     A URL to display.  May be null.
     */
    public HTMLFrame (String s, URL url) {
	super (s);

        action_listeners = new ArrayList();

        contents_url = null;
        back_buffer = new Vector();
        forward_buffer = new Vector();

        JPanel main_panel = new JPanel();
	main_panel.setBorder (new EmptyBorder (5, 5, 5, 5));
        main_panel.setLayout (new BorderLayout (5,5));
        getContentPane().add(main_panel);

	pane = new JEditorPane();
	sp = new JScrollPane (pane);
	main_panel.add(sp, "Center");
	pane.setEditable (false);
	
	pane.addHyperlinkListener (this);

        JPanel buttons_panel = new JPanel();
        buttons_panel.setLayout (new GridLayout (1, 4, 5, 0));
        main_panel.add (buttons_panel, "South");

        contents_button = new JButton ("Contents");
        buttons_panel.add (contents_button);
        contents_button.setEnabled (false);
        contents_button.addActionListener (this);

        back_button = new JButton ("Back");
        buttons_panel.add (back_button);
        back_button.setEnabled (false);
        back_button.addActionListener (this);

        forward_button = new JButton ("Forward");
        buttons_panel.add (forward_button);
        forward_button.setEnabled (false);
        forward_button.addActionListener (this);

        close_button = new JButton ("Close");
        buttons_panel.add (close_button);
        close_button.addActionListener (this);

	addWindowListener (this);

	setSize (650, 550);

        current_url = url;
        if (url != null)
            setURL (url);
    }



    /**
     * Sets the URL to use for the "Contents" button
     */
    public void setContentsURL (URL _contents_url) {
        contents_url = _contents_url;
        contents_button.setEnabled (contents_url != null);
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
            url = getFileURL (System.getProperty ("VJ_SHARE_DIR") + "/Data/" + name);
        if (url == null)
            url = getFileURL ("share/Data/" + name);
        if (url == null)
            url = ClassLoader.getSystemResource ("VjFiles/" + name);
        if (url == null)
            url = ClassLoader.getSystemResource ("VjFiles/NoHelpAvailable.html");
     
        if (current_url != null) {
            back_buffer.addElement (current_url);
            back_button.setEnabled (true);
        }
        forward_buffer.removeAllElements ();

        displayURL (url);

    }


    /**
     * sets the URL to be displayed in the frame.
     *
     * @param url A URL to be displayed.
     *       If the URL contains the string "DescHelp", setURL will
     *       mangle it in an attempt to search the various places that
     *       DescHelp html files are stored (e.g. the VjControl .jar file,
     *       $VJ_SHARE_DIR/Data, and ~/.vjconfig).
     */
    public void setURL (URL url) {

        /* a kludge to provide full search paths for DescHelp entries...
         * I'm not really keen on making HTMLFrame aware of this stuff,
         * but I need to do it to handle URLs in the documentation correctly.
         * I should come up with a more generalized way of doing an
         * "HTML search path" for the HTMLFrame.
         */
        //System.out.println ("setURL: '" + url + "'");
        int i = url.toString().indexOf("DescHelp/");
        if (i != -1) {
            String name = url.toString().substring(i);
            url = getFileURL (System.getProperty ("user.dir") + "/.vjconfig/" + name);
            if (url == null)
                url = getFileURL (System.getProperty ("VJ_SHARE_DIR") + "/Data/" + name);
            if (url == null)
                url = getFileURL ("share/Data/" + name);
            if (url == null)
                url = ClassLoader.getSystemResource ("VjFiles/" + name);
            if (url == null)
                url = ClassLoader.getSystemResource ("VjFiles/NoHelpAvailable.html");
        }

        if (current_url != null) {
            back_buffer.addElement (current_url);
            back_button.setEnabled (true);
        }
        forward_buffer.removeAllElements ();

        displayURL (url);
    }



    /**
     * Display the given URL.
     *
     * This is a utility function used by setURL and actionPerformed. Outside
     * code should use setURL only.
     * <p>
     * Any code calling displayURL should be careful to maintain the
     * forward_buffer and back_buffer for navigation.
     *
     * @param url A URL to be displayed.  url can be null, but that will
     *            cause an error message to be displayed in the HTMLFrame.
     */
    protected void displayURL (URL url) {
        try {
            current_url = url;
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
		pane.setText ("<h2>Failed to open URL: <br>" + url + "</h2>");
	}
    }


    /********************** HyperLinkListener Stuff ***************/

    public void hyperlinkUpdate (HyperlinkEvent e) {
	if (e.getEventType() == HyperlinkEvent.EventType.ACTIVATED) {
	    setURL (e.getURL());
	}
    }


    /********************* ActionListener Stuff *******************/

    public void actionPerformed (ActionEvent e) {
        if (e.getSource() == contents_button) {
            setURL (contents_url);
        }
        else if (e.getSource() == close_button) {
            notifyActionListenersClose();
        }
        else if (e.getSource() == back_button) {
            URL new_url = (URL)back_buffer.lastElement();
            back_buffer.removeElementAt (back_buffer.size()-1);
            back_button.setEnabled (back_buffer.size() != 0);
            forward_buffer.addElement (current_url);
            forward_button.setEnabled (true);
            displayURL (new_url);
        }
        else if (e.getSource() == forward_button) {
            URL new_url = (URL)forward_buffer.lastElement();
            forward_buffer.removeElementAt (forward_buffer.size()-1);
            forward_button.setEnabled (forward_buffer.size() != 0);
            back_buffer.addElement (current_url);
            back_button.setEnabled (true);
            displayURL (new_url);
        }
    }


    /********************* WindowListener Stuff *******************/

    public void windowActivated(WindowEvent e) {}
    public void windowClosed(WindowEvent e) {
        notifyActionListenersClose();
    }
    public void windowClosing(WindowEvent e) {}
    public void windowDeactivated(WindowEvent e) {}
    public void windowDeiconified(WindowEvent e) {}
    public void windowIconified(WindowEvent e) {}
    public void windowOpened(WindowEvent e) {}


    //--------------------- ActionEvent Stuff ------------------------

    public void addActionListener (ActionListener l) {
	synchronized (action_listeners) {
	    action_listeners.add (l);
	}
    }

    public void removeActionListener (ActionListener l) {
	synchronized (action_listeners) {
	    action_listeners.remove (l);
	}
    }


    private void notifyActionListenersClose () {
        ActionEvent e = new ActionEvent (this, ActionEvent.ACTION_PERFORMED,
                                         "Close");
        notifyActionListeners (e);
    }

    private void notifyActionListeners (ActionEvent e) {
        ActionListener l;
        int i, n;
        synchronized (action_listeners) {
            n = action_listeners.size();
            for (i = 0; i < n; i++) {
                l = (ActionListener)action_listeners.get(i);
                l.actionPerformed (e);
            }
        }
    }


    /********************** ChildFrame Stuff **********************/

    public void destroy () {
	pane.removeHyperlinkListener (this);
        removeWindowListener (this);
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



