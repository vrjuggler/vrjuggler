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


package VjGUI;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.text.*;
import VjGUI.LogMessageListener;
import VjGUI.LogMessageEvent;
import VjGUI.Core;

public class ConsolePane extends JPanel implements LogMessageListener {

    JTextArea textarea;
    JScrollPane sp;

    public ConsolePane () {
	setLayout (new BorderLayout (5, 5));
	textarea = new JTextArea ("");
	textarea.setEditable (false);
	add (sp = new JScrollPane (textarea), "Center");

	Core.addLogMessageListener (this);
    }



    public void logMessage (LogMessageEvent e) {
	switch (e.getStyle()) {
	case LogMessageEvent.TEMPORARY_MESSAGE:
	    break;
	case LogMessageEvent.PERMANENT_MESSAGE:
	    textarea.append ("(" + e.getSourceName() + "): " + e.getMessage() + "\n");
	    break;
	case LogMessageEvent.PERMANENT_ERROR:
	    textarea.append ("ERROR (" + e.getSourceName() + "): " + e.getMessage() + "\n");
	    break;
	}
    }


}
