/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
package org.vrjuggler.jccl.perfmonitor;

import java.awt.BorderLayout;
import java.awt.event.*;
import javax.swing.*;

public class PanelTreeFolder extends JPanel implements ActionListener {

    JPanel mNorth;
    JComponent mCenter;
    JButton mExpandButton;
    boolean mExpanded;

    public PanelTreeFolder (JComponent top_component) {
        super();

        setLayout (new BorderLayout());
        add (Box.createHorizontalStrut(40), BorderLayout.WEST);

        mNorth = new JPanel();
	mNorth.setLayout (new BoxLayout(mNorth, BoxLayout.X_AXIS));
        add (mNorth, BorderLayout.NORTH);

        mExpandButton = new JButton ("Hide");
        mNorth.add (mExpandButton);
        mNorth.add (top_component);

        mCenter = new JPanel();
        mCenter.setLayout (new BoxLayout (mCenter, BoxLayout.Y_AXIS));
        add (mCenter, BorderLayout.CENTER);

	mExpanded = true;
	mExpandButton.addActionListener (this);
    }

    public int getSubpanelCount() {
        return mCenter.getComponentCount();
    }

    public JComponent getSubpanel(int i) {
        return (JComponent)mCenter.getComponent(i);
    }

    public void addSubpanel(JComponent p) {
	mCenter.add(p);
    }

    public void actionPerformed (ActionEvent e) {
	if (e.getSource() == mExpandButton) {
	    if (mExpanded) {
                mExpandButton.setText ("Show");
		mExpanded = false;
		remove (mCenter);
                revalidate();
	    }
	    else {
                mExpandButton.setText ("Hide");
		mExpanded = true;
		add (mCenter, BorderLayout.CENTER);
	    }
	    mCenter.revalidate();
	    repaint();
	}
    }


}



        
