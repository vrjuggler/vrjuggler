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
import java.util.Vector;
import javax.swing.*;
import javax.swing.border.*;

import VjConfig.*;
import VjControl.Core;
import VjComponents.UI.Widgets.*;
import VjComponents.UI.EditorPanel;
import VjComponents.ConfigEditor.*;
import VjComponents.ConfigEditor.ConfigChunkUI.*;

/** A panel for viewing/editing a single ChunkDesc
 *
 * @author Christopher Just
 * @version $Revision$
 */
public class ChunkDescPanel
    extends JPanel
    implements EditorPanel,
               ActionListener,
               DescDBListener,
               ChildFrameParent { 

    ChunkDesc desc;
    ChunkDescDB descdb;
    Vector proppanels; // property description panels.
    JPanel properties;
    JTextField namefield;
    JTextField tokenfield;
    JTextField helpfield;
    JButton removebutton;
    JButton insertbutton;
    JButton edit_defaults_button;
    JButton clear_defaults_button;
    boolean editable;
    JPanel mainpanel, buttonspanel;
    ConfigChunk new_default_chunk;
    ConfigUIHelper confighelper_module; //needed to edit default values
    GenericEditorFrame default_chunk_frame;

    public ChunkDescPanel (ChunkDesc _desc,
			   ChunkDescDB _descdb,
                           ConfigUIHelper _confighelper_module,
			   boolean _editable) {

	JScrollPane sp;
	PropertyDescPanel t;
	
	editable = _editable;
	desc = _desc;
	descdb = _descdb;
        confighelper_module = _confighelper_module;
	proppanels = new Vector();
        new_default_chunk = desc.getDefaultChunk();
        default_chunk_frame = null;
	
	//setFont(core.ui.windowfont);
	
	/******************* North Panel **********************/
	
        setBorder (BorderFactory.createEmptyBorder (3,3,3,3));

	JLabel l;
	GridBagLayout gbl;
	GridBagConstraints gbc = new GridBagConstraints();
	setLayout (gbl = new GridBagLayout());
	
	gbc.fill = gbc.HORIZONTAL;
	gbc.insets = new Insets (2,4,2,4);
	gbc.gridwidth = 1;
	
	l = new JLabel ("ChunkDesc Name: ", JLabel.RIGHT);
	gbl.setConstraints (l, gbc);
	add (l);
	
	namefield = new StringTextField (desc.name, 15);
	gbc.weightx = 0.5;
	gbl.setConstraints (namefield, gbc);
	add (namefield);

	l = new JLabel ("Token: ", JLabel.RIGHT);
	gbc.weightx = 0;
	gbl.setConstraints (l, gbc);
	add (l);

	tokenfield = new NoSpaceTextField (desc.token, 15);
	gbc.gridwidth = gbc.REMAINDER;
	gbc.weightx = 0.5;
	gbl.setConstraints (tokenfield, gbc);
	add (tokenfield);

	l = new JLabel ("Help String: ", JLabel.RIGHT);
	gbc.gridwidth = 1;
	gbc.weightx = 0;
	gbl.setConstraints (l, gbc);
	add (l);

	helpfield = new StringTextField (desc.help, 50);
	gbc.weightx = 1;
	gbc.gridwidth = gbc.REMAINDER;
	gbl.setConstraints (helpfield, gbc);
	add (helpfield);


	/******************* Center Panel ********************/
	properties = new JPanel ();
	sp = new JScrollPane(properties, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
			     JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        sp.getVerticalScrollBar().setUnitIncrement(40);
        sp.getHorizontalScrollBar().setUnitIncrement(40);

	gbc.weighty = 1;
	gbc.weightx = 1;
	gbc.fill = gbc.BOTH;
	gbc.gridheight = gbc.RELATIVE;
	gbl.setConstraints (sp, gbc);
	add(sp);
	
	properties.setLayout (new BoxLayout (properties, BoxLayout.Y_AXIS));

	// make property description panels
        PropertyDesc[] props = desc.getPropertyDescs();
	for (int i = 0; i < props.length; i++) {
	    t = new PropertyDescPanel (this, 
				       !props[i].name.equalsIgnoreCase("name") && editable,
				       props[i]);
	    proppanels.add(t);
	    properties.add(t);
	}

	/****************** South Panel ***********************/

	if (editable) {
	    buttonspanel = new JPanel();
	    insertbutton = new JButton ("Insert Property");
	    insertbutton.addActionListener (this);
	    buttonspanel.add (insertbutton);
	    removebutton = new JButton ("Remove Property");
	    removebutton.addActionListener (this);
	    buttonspanel.add (removebutton);
            edit_defaults_button = new JButton ("Edit Default Values");
            edit_defaults_button.addActionListener (this);
            buttonspanel.add (edit_defaults_button);
            clear_defaults_button = new JButton ("Clear Default Values");
            clear_defaults_button.addActionListener (this);
            buttonspanel.add (clear_defaults_button);

            gbc.weighty = 0;
            gbc.fill = gbc.HORIZONTAL;
            gbc.gridwidth = 1;
            gbc.gridheight = gbc.REMAINDER;
            gbc.gridwidth = gbc.REMAINDER;
            gbl.setConstraints (buttonspanel, gbc);
            add (buttonspanel);
	}

        descdb.addDescDBListener (this);

    }



    public ChunkDescDB getDescDB() {
	return descdb;
    }



    public String getName () {
	return desc.name;
    }



    public void actionPerformed (ActionEvent e) {
        Object source = e.getSource();
	if (source == removebutton) {
	    for (int i = 0; i < proppanels.size(); ) {
		PropertyDescPanel p = 
		    (PropertyDescPanel)proppanels.elementAt(i);
		if (p.getSelected()) {
		    properties.remove(p);
		    proppanels.removeElementAt(i);
		}
		else
		    i++;
	    }
            // validate & repaint needed to get components to "shift up"
            // after removing one in the middle.
            validate();
	    repaint();
	}
	else if (source == insertbutton) {
	    PropertyDescPanel t = new PropertyDescPanel (this, true);
	    proppanels.addElement(t);
	    properties.add(t); 
	    //setReasonableSize();
	    validate();
	}
        else if (source == clear_defaults_button) {
            new_default_chunk = null;
        }
        else if (source == edit_defaults_button) {
            if (default_chunk_frame == null) {
                ConfigChunkPanel p;
                ChunkDesc new_desc = getNewValue();
//                  if (new_desc.equals (desc))
//                      new_desc = desc;
                if (new_default_chunk == null)
                    new_default_chunk = new ConfigChunk (new_desc);
                else
                    new_default_chunk.applyNewDesc(new_desc);
                if (new_desc.equals(desc)) {
                    p = confighelper_module.configchunkpanel_factory.createConfigChunkPanel (new_default_chunk.getDescToken());
                }
                else {
                    // we're bypassing configuihelper's panel factory cuz it
                    // isn't safe to try to use a specific panel in this 
                    // case... since we're editing 
                    p = new DefaultConfigChunkPanel();
                }
                p.setChunk (new_default_chunk, null);
                default_chunk_frame = new GenericEditorFrame (this, p);
            }
            else
                default_chunk_frame.show();
        }
    }



    public ChunkDesc getOldValue() {
	return desc;
    }



    public ChunkDesc getNewValue() {
	// returns the value currently shown in the panel...
	if (tokenfield.getText().equals("")) {
	    /* this really oughtta bring up a dialog box */
	    System.err.println ("Error: Must fill in ChunkDesc token field");
	    return null;
	}
	    
	ChunkDesc d = new ChunkDesc();
	d.token = tokenfield.getText();
	d.help = helpfield.getText();
	d.name = namefield.getText();
	if (d.name.equals(""))
	    d.name = d.token;

	/* start at one to skip instance name field which is already there */
        int n = proppanels.size();
	for (int i = 1; i < n; i++) {
	    PropertyDesc p = 
		((PropertyDescPanel)proppanels.get(i)).getPropertyDesc();
	    if (p != null)
		d.addPropertyDesc(p);
	}
        if (new_default_chunk != null)
            new_default_chunk.applyNewDesc (d);
        d.setDefaultChunk (new_default_chunk);
	return d;
    }
    


    //----------------------- EditorPanel Stuff ------------------------------

    public void showHelp () {
    }

    public String getComponentName () {
        return desc.getName();
    }

    public boolean addConfig (ConfigChunk ch) {
        return false;
    }


    public boolean removeConfig (String name) {
        return false;
    }

    /* is this safe? */
    public ConfigChunk getConfiguration () {
        return null;
    }


    public boolean configure (ConfigChunk ch) {
        return true;
    }



    //------------------------ ChildFrame Stuff ------------------------------

    public void destroy () {
        if (descdb != null)
            descdb.removeDescDBListener (this);

        int n = proppanels.size();
        for (int i = 0; i < n; i++) {
	    ((PropertyDescPanel)proppanels.elementAt(i)).closeFrames();
	}
        if (default_chunk_frame != null)
            default_chunk_frame.destroy();
    }


    public boolean matches (String cl, Object db, Object o) {
	if (cl != null) {
	    try {
		if (!(Class.forName(cl).isInstance(this)))
		    return false;
	    }
	    catch (Exception e) {
		return false;
	    }
	}
	if (db != descdb)
	    return false;
        if (o == null)
            return true;
        else if (o instanceof ChunkDesc) 
            return ((ChunkDesc)o).getToken().equalsIgnoreCase(desc.getToken());
        else
            return false;
    }


    //------------------- DescDBListener stuff ----------------------
    public void addDesc (DescDBEvent e) {;}
    public void removeDesc (DescDBEvent e) {;}
    public void replaceDesc (DescDBEvent e) {
        // basically we're checking for when an apply comes across, so we
        // can correct exactly which chunk we're supposed to be editing.
        // at some point, we should add the ability to completely update the
        // displayed state; that'll be helpful when we get more "live" 
        // information from juggler.  (it'd probably be helpful at that
        // point if we can query the ConfigModule about whether this is an
        // "active db" or not - if not, then we shouldn't have to do a full
        // update).
        if (e.getOldDesc() == desc)
            desc = e.getNewDesc();
    }
    public void removeAllDescs (DescDBEvent e) {;}


    //------------------ ChildFrameParent stuff ------------------------

    public void closeChild (ChildFrame frame) {
        if (frame == default_chunk_frame) {
            default_chunk_frame.destroy();
            //default_chunk_frame.dispose();
            default_chunk_frame = null;
        }
    }


    public void applyChild (ChildFrame frame) {
        if (frame == default_chunk_frame) {
            ConfigChunkPanel p = (ConfigChunkPanel)default_chunk_frame.getEditorPanel();
            new_default_chunk = p.getNewValue();
        }
    }


}

