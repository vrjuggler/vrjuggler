/* DataDisplayFrame is a frame displaying all of the chunks using a 
 * specified ChunkDesc in a read-only format, suitable for viewing
 * performance data, current input device readings, etc.
 */

package VjGUI;

import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import VjConfig.ChunkDesc;
import VjConfig.ConfigChunk;
import VjConfig.Property;
import VjConfig.DescEnum;
import VjGUI.MainWindowPanel;
import VjGUI.AppCore;



public class DataDisplayFrame extends Frame 
    implements ActionListener, WindowListener {

    ClientGlobals core;
    ConfigChunk chunk;
    ChunkDesc desc;
    TextArea dataarea;

    public DataDisplayFrame (ClientGlobals _core, ChunkDesc _desc) {
	super(_desc.name);
	Property  p;
	Vector    v;
	int       i, j, k;

	core = _core;
	desc = _desc;

	setFont (core.ui.windowfont);

	Panel mainp = new Panel();
	mainp.setLayout (new BorderLayout());  // wish I had verticalflowlayout
	add (mainp);


	mainp.add (new Label (desc.help, Label.LEFT), "North");

	dataarea = new TextArea(25, 50);
	mainp.add (dataarea, "Center");

	update();
	pack();
	addWindowListener(this);
    }



    public String getName () {
	/* Returns the type name of chunks that this Frame displays */
	return desc.name;
    }



    public boolean update () {
	int i, j, k;
	Vector v;
	ConfigChunk chunk;
	Property p;

	System.out.println ("called DataDisplayFrame.update");
	dataarea.setText("");
	// get all chunks of this type:
	v = core.chunks.getOfDescToken (desc.token);
	for (k = 0; k < v.size(); k++) {
	    chunk = (ConfigChunk)v.elementAt(k);

	    dataarea.append(chunk.getName() + "\n");

	    // now for each property in chunk...
	    for (i = 0; i < chunk.props.size(); i++) {
		p = (Property)chunk.props.elementAt(i);
		if (p.desc.valtype.equals("chunk")) {
		}
		else {
		    // add it to dataarea
		    dataarea.append("  " + p.desc.name + "\n");
		    for (j = 0; j < p.vals.size(); j++) {
			if (p.desc.valuelabels.size() > j)
			    dataarea.append ("    " + ((DescEnum)p.desc.valuelabels.elementAt(j)).str + "  ");
			else 
			    dataarea.append ("    ");
			dataarea.append( p.vals.elementAt(j) + "\n");
		    }
		    dataarea.append ("\n");
		}
	    }
	}
	return true;
    }



    public void actionPerformed (ActionEvent e) {
	System.out.println ("panel event called");
	int i;
    }



    /* WindowListener Stuff */
    public void windowActivated(WindowEvent e) {}
    public void windowClosed(WindowEvent e) {}
    public void windowClosing(WindowEvent e) {
	hide();
    }
    public void windowDeactivated(WindowEvent e) {}
    public void windowDeiconified(WindowEvent e) {}
    public void windowIconified(WindowEvent e) {}
    public void windowOpened(WindowEvent e) {}


}








