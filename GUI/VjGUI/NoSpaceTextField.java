
package VjGUI;

import java.awt.*;
import java.awt.event.*;

/* based on Graphic Java 1.1, p234 */

public class NoSpaceTextField extends TextField {

    class FlyValidator extends KeyAdapter {
	public void keyPressed (KeyEvent event) {
	    if (event.getKeyChar() == ' ') 
		event.consume();
	}
    }

    public NoSpaceTextField () {
        super();
	addKeyListener(new FlyValidator());
    }
    public NoSpaceTextField (int i) {
        super(i);
	addKeyListener(new FlyValidator());
    }
    public NoSpaceTextField (String s) {
        super(s);
	addKeyListener(new FlyValidator());
    }
    public NoSpaceTextField (String s, int i) {
        super(s,i);
	addKeyListener(new FlyValidator());
    }
}
