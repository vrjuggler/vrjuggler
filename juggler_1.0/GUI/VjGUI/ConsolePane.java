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
