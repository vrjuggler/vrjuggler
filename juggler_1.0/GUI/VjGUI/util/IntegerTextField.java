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



package VjGUI.util;

import javax.swing.*;
import java.awt.event.*;

public class IntegerTextField extends JTextField {

    public IntegerTextField (int i) {
	super(i);
    }
    public IntegerTextField (String s, int i) {
	super (s, i);
    }

    protected void processKeyEvent (KeyEvent e) {

	if (e.getModifiers() == 0) {
	    int key = e.getKeyCode();
	    if (key == KeyEvent.VK_BACK_SPACE 
		|| key == KeyEvent.VK_DELETE
		|| key == KeyEvent.VK_LEFT
		|| key == KeyEvent.VK_RIGHT)
		super.processKeyEvent (e);

	    int keyChar = e.getKeyChar();
	    if ((keyChar >= '0' && keyChar <= '9') ||
		keyChar == '+' || keyChar == '-' 
		|| keyChar == '\n'
		)
		super.processKeyEvent (e);
	}
    }

}

