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


package VjComponents.ConfigEditor.ConfigChunkUI;

import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import javax.swing.*;
import javax.swing.border.*;

import VjConfig.*;
import VjComponents.ConfigEditor.*;
import VjComponents.UI.Widgets.*;
import VjControl.*;


/** Default implementation of ConfigChunkPanel.
 *  DO NOT INSTANTIATE THIS YOURSELF.  ConfigChunkPanels should be allocated
 *  via ConfigUIHelperModule (and its ConfigChunkPanelFactory).
 */
public class DefaultConfigChunkPanel extends JPanel 
    implements ConfigChunkPanel, ChunkDBListener { 

    ConfigChunk   chunk;
    ConfigChunkDB chunkdb;
    Vector proppanels; // property description panels.
    Box properties_panel;
    JButton cancelbutton;
    JButton okbutton;
    JButton applybutton;
    JButton helpbutton;
    JTextField namef;
    JTextField helpfield;
    JScrollPane sp;

    GridBagLayout playout;
    GridBagConstraints pconstraints;

    protected ConfigChunk component_chunk;
    protected String component_name;

    ConfigUIHelper uihelper_module;

    public DefaultConfigChunkPanel () {
        super();

        component_name = "Unconfigured DefaultConfigChunkPanel";
        component_chunk = null;
        uihelper_module = null;

	JPanel northpanel, centerpanel;

	proppanels = new Vector();

	chunk = null;
	chunkdb = null;
	//setBorder (new EmptyBorder (5, 5, 0, 5));
	setLayout (new BorderLayout (5,5));

	JPanel p1;
	northpanel = new JPanel();
	northpanel.setLayout (new BoxLayout (northpanel, BoxLayout.Y_AXIS));
	p1 = new JPanel();
	p1.setLayout (new BoxLayout (p1, BoxLayout.X_AXIS));
	p1.add (new JLabel ("Instance Name: "));
	p1.add (namef = new StringTextField ("", 25));
	northpanel.add (p1);

        helpfield = new JTextField ("");
        helpfield.setEditable (false);
        northpanel.add (helpfield);

	add(northpanel, "North");
	
	properties_panel = Box.createVerticalBox();

	sp = new JScrollPane(properties_panel, 
                             JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
			     JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
	sp.getVerticalScrollBar().setUnitIncrement(40);
	sp.getHorizontalScrollBar().setUnitIncrement(40);

	add(sp,"Center");	

    }



    /* note: _chunkdb is specifically allowed to be null; this is often
     * the case w/ embedded chunks.  if chunkdb is non-null we'll listen
     * to it for changes to _chunk.  otherwise, whoever called setChunk
     * on us is responsible for keeping us up-to-date.
     */
    public void setChunk (ConfigChunk _chunk, ConfigChunkDB _chunkdb) {
        if (chunk == _chunk)
            return;

	chunk = _chunk;
	chunkdb = _chunkdb;

        //System.out.println ("editing chunk: " + _chunk.toString());

        // double check this to make sure i'm not leaving anything
        // dangling.
        proppanels.clear();
        properties_panel.removeAll();

        if (chunk != null) {
            namef.setText (chunk.getLastNameComponent());
            helpfield.setText (chunk.getDescHelp());

            // make property panels
            PropertyPanel t;
            Property p;
            int i, n = chunk.getPropertiesSize();
            for (i = 0; i < n; i++) {
                p = chunk.getProperty (i);
                if (Core.user_profile.accepts (p.getDesc().getUserLevel())) {
                    t = new PropertyPanel (chunk.getProperty(i), 
                                           this, uihelper_module);
                    proppanels.add(t);
                    properties_panel.add(t);
                }
            }
        }

        if (chunkdb != null)
            chunkdb.addChunkDBListener (this);
    }


    public String getName () {
	return chunk.getName();
    }


    public String getComponentName () {
        return chunk.getName();
    }


    public void setComponentName (String _name) {
        ; // determined by our chunk...
    }


//      public void setConfigUIHelper (ConfigUIHelper h) {
//          uihelper_module = h;
//      }

    public void setConfiguration (ConfigChunk ch) throws VjComponentException {
        component_chunk = ch;
        component_name = ch.getName();

        // get pointers to the modules we need.
        Property p = ch.getPropertyFromToken ("Dependencies");
        if (p != null) {
            int i;
            int n = p.getNum();
            String s;
            VjComponent c;
            for (i = 0; i < n; i++) {
                s = p.getValue(i).toString();
                c = Core.getComponentFromRegistry(s);
                if (c != null) {
                    if (c instanceof ConfigUIHelper)
                        uihelper_module = (ConfigUIHelper)c;
                }
            }
        }
    }

     
    public void initialize () throws VjComponentException {
        if (uihelper_module == null)
            throw new VjComponentException (component_name + ": Initialized with unmet dependencies.");
    }


    public ConfigChunk getConfiguration () {
        return component_chunk;
    }


    public boolean addConfig (ConfigChunk ch) {
        return false;
    }


    public boolean removeConfig (String name) {
        return false;
    }


    public void destroy () {
        if (chunkdb != null)
            chunkdb.removeChunkDBListener (this);
    }


    public ConfigChunkDB getChunkDB () {
	return chunkdb;
    }


    public ConfigChunk getOldValue() {
	return chunk;
    }



    public ConfigChunk getNewValue() {
	/* returns a configchunk based on the values current 
	 * in this window */
        String name = chunk.getName();
        name = ConfigChunk.setLastNameComponent (name, namef.getText());
	ConfigChunk c = new ConfigChunk (chunk);
        c.setFullName (name);
        int i, n = proppanels.size();
        Property p;
        for (i = 0; i < n; i++) {
            p = ((PropertyPanel)proppanels.get(i)).getValue();
            c.setProperty (p);
        }
        c.validateEmbeddedChunkNames();
	return c;
    }



    public boolean matches (String cl, Object db, Object o) {
        try {
            if (cl != null) {
                if (!(Class.forName(cl).isInstance(this)))
                    return false;
            }
            if (chunkdb != db)
                return false;
            ConfigChunk ch = (ConfigChunk)o;
            return (ch == null) || (ch.getName().equals(chunk.getName()));
        }
        catch (Exception e) {
            return false;
        }
    }


    public void showHelp () {
        uihelper_module.loadDescHelp (chunk.getDescToken());
    }


    //------------------- ChunkDBListener stuff ----------------------
    public void addChunk (ChunkDBEvent e) {;}
    public void removeChunk (ChunkDBEvent e) {;}
    public void replaceChunk (ChunkDBEvent e) {
        // basically we're checking for when an apply comes across, so we
        // can correct exactly which chunk we're supposed to be editing.
        // at some point, we should add the ability to completely update the
        // displayed state; that'll be helpful when we get more "live" 
        // information from juggler.  (it'd probably be helpful at that
        // point if we can query the ConfigModule about whether this is an
        // "active db" or not - if not, then we shouldn't have to do a full
        // update).
        if (e.getOldChunk() == chunk)
            chunk = e.getNewChunk();
    }
    public void removeAllChunks (ChunkDBEvent e) {;}


}

