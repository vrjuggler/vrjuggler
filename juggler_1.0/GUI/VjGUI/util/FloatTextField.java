/*
 *  File:	    $Name$
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

public class FloatTextField extends JTextField {

    public FloatTextField (int i) {
	super(i);
    }
    public FloatTextField (String s, int i) {
	super (s, i);
    }

    protected void processKeyEvent (KeyEvent e) {
	int key = e.getKeyCode();
	if (key == KeyEvent.VK_BACK_SPACE 
	    || key == KeyEvent.VK_DELETE
	    || key == KeyEvent.VK_LEFT
	    || key == KeyEvent.VK_RIGHT)
	    super.processKeyEvent (e);

	if (e.getModifiers() == 0) {
	    int keyChar = e.getKeyChar();
	    if ((keyChar >= '0' && keyChar <= '9') ||
		keyChar == '+' || keyChar == '-' || keyChar == '.'
		|| keyChar == '\n')
		super.processKeyEvent (e);
	}
    }

}

