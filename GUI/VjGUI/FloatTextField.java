
/* A TextField variant that only accepts floating point numbers
 */

/* known bugs: doesn't accept "-.xxx" use "-0.xxx" instead
 */

package VjGUI;

import java.awt.*;
import java.awt.event.*;

/* based on Graphic Java 1.1, p234 */

public class FloatTextField extends TextField {

    class FlyValidator extends KeyAdapter {
	public void keyPressed (KeyEvent event) {
	    Float value;
	    TextField field = (TextField)event.getSource();
	    String oldstring = ((TextField)event.getSource()).getText();
	    char c = event.getKeyChar();
	    int i = field.getCaretPosition();
	    // special case: leading '-' sign
	    if (oldstring.equals("") &&
		((c == '-') || (c == '.')))
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
		value = Float.valueOf (newstring);
	    }
	    catch (NumberFormatException e) {
		event.consume();
	    }
	}
    }

    public FloatTextField () {
        super();
	addKeyListener(new FlyValidator());
    }
    public FloatTextField (int i) {
        super(i);
	addKeyListener(new FlyValidator());
    }
    public FloatTextField (String s) {
        super(s);
	addKeyListener(new FlyValidator());
    }
    public FloatTextField (String s, int i) {
        super(s,i);
	addKeyListener(new FlyValidator());
    }
}
