
/* A TextField variant that only accepts integer inputs */

package VjGUI;

import java.awt.*;
import java.awt.event.*;

/* based on Graphic Java 1.1, p234 */

public class IntegerTextField extends TextField {

    class FlyValidator extends KeyAdapter {
	public void keyPressed (KeyEvent event) {
	    int value;
	    TextField field = (TextField)event.getSource();
	    String oldstring = ((TextField)event.getSource()).getText();
	    char c = event.getKeyChar();
	    int i = field.getCaretPosition();
	    // special case: leading '-' sign
	    if (oldstring.equals("") &&
		(c == '-'))
		return;
	    // special case: cursor keys, etc.
	    if (Character.getType(c) == Character.CONTROL)
		return;
	    //String newstring = oldstring + c;
	    String newstring = oldstring.substring(0, i) + c
		+ oldstring.substring(i);
	    //System.out.println ("testing string '" + 
	    //                     newstring + "'");
	    try {
		value = Integer.parseInt (newstring);
	    }
	    catch (NumberFormatException e) {
		event.consume();
	    }
	}
    }

    public IntegerTextField () {
        super();
	addKeyListener(new FlyValidator());
    }
    public IntegerTextField (int i) {
        super(i);
	addKeyListener(new FlyValidator());
    }
    public IntegerTextField (String s) {
        super(s);
	addKeyListener(new FlyValidator());
    }
    public IntegerTextField (String s, int i) {
        super(s,i);
	addKeyListener(new FlyValidator());
    }
}
