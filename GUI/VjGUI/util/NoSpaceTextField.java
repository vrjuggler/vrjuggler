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

