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
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


package VjGUI;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.text.*;

public class ConsolePane extends JPanel {

    JTextArea textarea;
    JScrollPane sp;

    public ConsolePane () {
	setLayout (new BorderLayout (5, 5));
	textarea = new JTextArea ("");
	textarea.setEditable (false);
	add (sp = new JScrollPane (textarea), "Center");
    }

    public void addInfoMessage (String source, String content) {
	textarea.append ("(" + source + "): " + content + "\n");
    }
    public void addErrorMessage (String source, String content) {
	textarea.append ("ERROR (" + source + "): " + content + "\n");
    }

}
