

/* a frame that displays a URL w/ active hyperlinks.  Useful for 
 * help info, etc.
 */

package VjGUI;

import java.net.*;
import java.io.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.text.*;
import javax.swing.text.html.*;
import java.util.Enumeration;
import VjGUI.Core;

public class HTMLFrame extends JFrame implements HyperlinkListener {

    JScrollPane sp;
    JEditorPane pane;

    public HTMLFrame (String s, URL url) {
	super ();
	pane = new JEditorPane();
	sp = new JScrollPane (pane);
	getContentPane().add(sp);
	pane.setEditable (false);
	
	pane.addHyperlinkListener (this);

	setSize (450, 350);
	setURL (url);
    }



    public void setURL (URL url) {
	try {
	    pane.setPage (url);
	    HTMLDocument d = (HTMLDocument)pane.getDocument();
	    String s =  (String)d.getProperty (d.TitleProperty);
	    //System.out.println ("doc title is '" + s + "'");
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

}
