/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/



package VjComponents.UI.Widgets;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.*;

import VjControl.Core;
import VjComponents.UI.*;



/** Generic frame for editing something (ie it has some panel, and it
 *  has OK, Apply, Cancel, and Help buttons).
 * 
 *  @author Christopher Just
 *  @version $Revision$
 */
public class GenericEditorFrame 
    extends JFrame 
    implements ChildFrame, 
               ActionListener, 
               WindowListener {

    EditorPanel editor_panel;

    JButton cancelbutton;
    JButton okbutton;
    JButton applybutton;
    JButton helpbutton;
    ChildFrameParent parent;


    public GenericEditorFrame (ChildFrameParent par, 
			     EditorPanel _editor_panel) {
	super("Edit: " + _editor_panel.getComponentName());

	Core.consoleTempMessage ("Opening Editor Frame: " + _editor_panel.getComponentName());
	JPanel southpanel;

	parent = par;
	JPanel mainp = new JPanel();
	mainp.setBorder (new EmptyBorder (5, 5, 0, 5));
	mainp.setLayout (new BorderLayout (5,5));
	getContentPane().add (mainp);

        editor_panel = _editor_panel;
        mainp.add ((JComponent)editor_panel, "Center");

	southpanel = new JPanel();
	okbutton = new JButton ("  OK  ");
	okbutton.addActionListener (this);
	southpanel.add (okbutton);
	applybutton = new JButton ("Apply");
	applybutton.addActionListener (this);
	southpanel.add (applybutton);
	cancelbutton = new JButton ("Cancel");
	cancelbutton.addActionListener (this);
	southpanel.add (cancelbutton);
	helpbutton = new JButton ("Help");
	helpbutton.addActionListener (this);
	southpanel.add (helpbutton);
	mainp.add(southpanel, "South");

	addWindowListener(this);

	Dimension d = getPreferredSize();
	d.height = Math.min (d.height + 28, Core.screenHeight);
	d.width = Math.min (d.width + 20, Core.screenWidth);
	setSize(d);

	show();
	//setVisible(true);

// no good - you just can't tell java to relocate a window...
// 	if (Core.window_pos_kludge == true) {
// 	    Point p = getLocation();
// 	    Dimension d2 = getSize();
// 	    int y = Core.screenHeight - d2.height;
// 	    y = (y < 0)?0:y;
// 	    int x = Core.screenWidth - d2.width;
// 	    x = (x < 0)?0:x;
// 	    if (y < p.y || x < p.x) {
// 		setLocation (x,y);
// 	    }
// 	    System.out.println ("screenWidth is " + Core.screenWidth + " so I'm opening at " + x);
// 	    p = getLocation();
// 	    System.out.println ("new location is " + p);
// 	}

	Core.consoleTempMessage ("");
    }



    public final String getName () {
        return editor_panel.getComponentName();
    }


    
    public final EditorPanel getEditorPanel () {
        return editor_panel;
    }



    public void actionPerformed (ActionEvent e) {
	if (e.getSource() == cancelbutton) {
	    parent.closeChild (this);
	}
	else if (e.getSource() == okbutton) {
            parent.applyChild (this);
	    parent.closeChild (this);
	}
        else if (e.getSource() == applybutton) {
            parent.applyChild (this);
        }
	else if (e.getSource() == helpbutton) {
            editor_panel.showHelp();
	}
    }



    /* WindowListener Stuff */
    public void windowActivated(WindowEvent e) {}
    public void windowClosed(WindowEvent e) {}
    public void windowClosing(WindowEvent e) {
	parent.closeChild (this);
    }
    public void windowDeactivated(WindowEvent e) {}
    public void windowDeiconified(WindowEvent e) {}
    public void windowIconified(WindowEvent e) {}
    public void windowOpened(WindowEvent e) {}



    //------------------------ ChildFrame Stuff ----------------------------
    public void destroy () {
        editor_panel.destroy();
	dispose();
    }


    public boolean matches (String cl, Object db, Object o) {
        return editor_panel.matches (cl, db, o);
    }

    
    public void updateUI () {
	SwingUtilities.updateComponentTreeUI (this);
    }

}

