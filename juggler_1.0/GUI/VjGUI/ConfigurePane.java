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


package VjGUI;

import java.io.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.tree.*;
import javax.swing.event.*;

import VjConfig.*;
import VjGUI.*;
import VjGUI.util.*;
import VjGUI.configchunk.*;

public class ConfigurePane extends JSplitPane 
    implements PlugPanel, MouseListener {

    ChunkDBPanel leftpanel, rightpanel;


    public ConfigurePane() {
	super(JSplitPane.HORIZONTAL_SPLIT);
	
	//-------------------- BUILD CONFIGURATION PANE ---------------------

	setLeftComponent (leftpanel = new ChunkDBPanel(0));
	setRightComponent (rightpanel = new ChunkDBPanel(1));

	leftpanel.setMinimumSize (new Dimension (0, 50));
	rightpanel.setMinimumSize (new Dimension (0, 50));
	//setDividerLocation (.5);

	leftpanel.setSendAcrossTarget (rightpanel);
	rightpanel.setSendAcrossTarget (leftpanel);
	
	leftpanel.addMouseListener (leftpanel);
	rightpanel.addMouseListener (rightpanel);
	
    }



    public void selectLeftDB (String name) {
	leftpanel.selectDB (name);
    }

    public void selectRightDB (String name) {
	rightpanel.selectDB (name);
    }



    // MouseListener stuff
    public void mouseClicked(MouseEvent e) {
	if (e.getSource() == leftpanel)
	    System.out.println ("left panel click");
	else
	    System.out.println ("right panel click");
    }

    public void mouseEntered(MouseEvent e) {}
    public void mouseExited(MouseEvent e) {}
    public void mousePressed(MouseEvent e) {}
    public void mouseReleased(MouseEvent e) {}



    /********************** PlugPanel Stuff **************************/
    public void destroy () {
	leftpanel.destroy();
	rightpanel.destroy();
    }

}



