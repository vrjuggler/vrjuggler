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


package VjGUI.util;

import javax.swing.*;
import java.awt.event.*;


// A text field that only accepts valid vjcontrol strings
// (ie no spaces).
public class StringTextField extends JTextField {

    public StringTextField (int i) {
	super(i);
    }
    public StringTextField (String s, int i) {
	super (s,i);
    }

    protected void processKeyEvent (KeyEvent e) {

	if (e.getModifiers() == 0) {
	    int keyChar = e.getKeyChar();
	    if (keyChar != '"')
		super.processKeyEvent (e);
	}
    }

}

