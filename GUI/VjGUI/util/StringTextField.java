package VjGUI.util;

import javax.swing.*;
import java.awt.event.*;

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

