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


package VjGUI.util;

import javax.swing.*;
import java.awt.event.*;

public class NoSpaceTextField extends JTextField {

    public NoSpaceTextField (int i) {
	super(i);
    }
    public NoSpaceTextField (String s, int i) {
	super (s, i);
    }

    protected void processKeyEvent (KeyEvent e) {

	if (e.getModifiers() == 0) {
	    int keyChar = e.getKeyChar();
	    if (keyChar != '"' && keyChar != ' ')
		super.processKeyEvent (e);
	}
    }

}

