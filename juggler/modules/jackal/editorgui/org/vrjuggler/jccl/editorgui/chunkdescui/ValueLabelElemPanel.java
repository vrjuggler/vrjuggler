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



package VjComponents.ConfigEditor.ChunkDescUI;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.*;

import VjControl.Core;
import VjConfig.*;
import VjComponents.ConfigEditor.*;
import VjComponents.UI.Widgets.*;


/** A small bordered panel for editing a single PropertyDesc ValueLabel.
 *
 *  @author Christopher Just
 *  @version $Revision$
 */
public class ValueLabelElemPanel extends JPanel implements MouseListener {

    static AbstractBorder select_border=null;
    static AbstractBorder unselect_border=null;

    JTextField namef;
    boolean selected;


    public ValueLabelElemPanel (String _name) {
	super();
	if (select_border == null) {
	    select_border = new CompoundBorder ( new BevelBorder (BevelBorder.LOWERED),
						 new EmptyBorder (5,5,5,5));
	}
	if (unselect_border == null) {
	    unselect_border = new CompoundBorder ( new BevelBorder (BevelBorder.RAISED),
						   new EmptyBorder (5,5,5,5));
	}
    
	selected = false;
	setBorder (unselect_border);

	int i, j;

	addMouseListener (this);

	JLabel l = new JLabel ("Label Name:");
	l.addMouseListener(this);
	add (l);
        namef = new JTextField (_name, 20);
        add (namef);

	Dimension d3 = getPreferredSize();
	Dimension d4 = getMaximumSize();
	d4.height = d3.height;
	setMaximumSize (d4);

    }


    /* I'm going to let the parent frame deal with building the actual desc
     * enum & catch any possible numberformat exceptions.  we'll just return
     * bits and pieces.
     */
    public String getName() {
        return namef.getText();
    }


    private void toggleSelected() {
	selected = !selected;
        super.setBorder(selected?select_border:unselect_border);
        repaint();
    }

    public boolean getSelected() {
	return selected;
    }


    /* MouseListener stuff */
    public void mouseClicked (MouseEvent e) {
	toggleSelected();
    }
    public void mouseEntered (MouseEvent e) {
    }
    public void mouseExited (MouseEvent e) {
    }
    public void mousePressed (MouseEvent e) {
    }
    public void mouseReleased (MouseEvent e) {
    }


}
