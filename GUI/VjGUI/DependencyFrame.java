
// frame to display a vector of dependency information

package VjGUI;

import javax.swing.*;
import java.awt.*;
import java.util.Vector;
import VjConfig.ChunkDependEntry;

public class DependencyFrame extends JFrame {

    public Vector deps;
    JTextArea textarea;
    JPanel mainpanel;

    public DependencyFrame (Vector v) {
	super();

	deps = v;

	mainpanel = new JPanel();
	getContentPane().add(mainpanel);
	mainpanel.setLayout (new BorderLayout());
	textarea = new JTextArea();
	mainpanel.add (textarea, "Center");

	resetText();
    }



    private void resetText() {
	int i;
	ChunkDependEntry e;

	if (deps.size() == 0)
	    textarea.setText ("No Dependencies");
	else {
	    textarea.setText ("Dependencies for foo\n");
	    for (i = 0; i < deps.size(); i++) {
		e = (ChunkDependEntry)deps.elementAt(i);
		textarea.append (e.toString());
	    }
	}
    }

}
