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


package VjComponents.UI;

import javax.swing.*;
import javax.swing.tree.*;
import java.util.Vector;
import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Graphics;
import java.awt.Color;
import java.awt.event.MouseListener;
import java.awt.event.MouseEvent;
import java.awt.Dimension;

import VjControl.*;
import VjComponents.UI.PlugPanel;
import VjComponents.UI.PlugPanelContainer;
import VjConfig.*;

public class TreeSelectContainer
    extends JSplitPane 
    implements PlugPanelContainer,
               MouseListener {

    /** UserData object for the tree widget. */
    public class tscUserObject {
        public ImageIcon icon;
        public String name;
        public Object object; // eg a plugpanel
        public int style;

        // values for style
        static final int FOLDER = 0;
        static final int LEAF = 1;

        public tscUserObject (String _name, ImageIcon _icon, Object _object,
                              int _style) {
            name = _name;
            icon = _icon;
            object = _object;
            style = _style;
        }

        public String toString () {
            return name;
        }
    }


    /** Cell Renderer for the tree widget. */
    public class tscTreeCellRenderer 
        extends JLabel 
        implements TreeCellRenderer {

        private Icon mOpenIcon;
        private Icon mClosedIcon;

        public tscTreeCellRenderer (Icon open, Icon close) {
            mOpenIcon = open;
            mClosedIcon = close;
        }


        public Component getTreeCellRendererComponent (JTree tree,
                                                       Object value,
                                                       boolean selected,
                                                       boolean expanded,
                                                       boolean leaf,
                                                       int row,
                                                       boolean hasFocus) {
            DefaultMutableTreeNode node = (DefaultMutableTreeNode)value;
            boolean disabled;
            try {
                tscUserObject o = (tscUserObject)node.getUserObject();

                setText (o.name);
                if (o.style == tscUserObject.FOLDER) {
                    disabled = node.getChildCount() == 0;
                    setIcon (expanded ? mOpenIcon : mClosedIcon);
                }
                else {
                    disabled = false;
                    setIcon (o.icon);
                }
                    
            }
            catch (ClassCastException e) {
                setText ("foo");
                disabled = false;
            }
	    
            if (disabled) {
                setForeground (Color.lightGray);
                setBackground (UIManager.getColor ("Tree.textBackground"));
            }
            else if (selected) {
                setForeground (UIManager.getColor ("Tree.selectionForeground"));
                setBackground (UIManager.getColor ("Tree.selectionBackground"));
            }
            else {
                setForeground (UIManager.getColor("Tree.textForeground"));
                setBackground (UIManager.getColor("Tree.textBackground"));
            }

            return this;
        }


        public void paint (Graphics g) {
            int textstart, textwidth;
            Icon icon = getIcon();
            if (icon == null)
                textstart = 0;
            else
                textstart = icon.getIconWidth() + getIconTextGap();
            textwidth = getSize().width - textstart;
            g.setColor (getBackground() );
            g.fillRect (textstart, 0, textwidth, getSize().height);
            if (icon != null)
                icon.paintIcon (this, g, 0, 0);
            g.setColor (getForeground());
            g.drawString (getText(), textstart, g.getFontMetrics().getAscent() );
        }

    } // tscTreeCellRenderer




    protected String component_name;
    protected ConfigChunk component_chunk;
    protected JPanel container_panel;
    protected Vector child_panels;
    protected PlugPanel active_panel;
    protected JTree nav_tree;
    protected DefaultTreeModel nav_tree_model;
    protected DefaultMutableTreeNode nav_tree_root;
    protected JPanel dummypanel;

    protected String defaultpanel_name;

JScrollPane sp;
    public TreeSelectContainer () {
        super();
        component_name = "Unconfigured TreeSelectContainer";
        component_chunk = null;
        defaultpanel_name = "";
//         setLayout (new BorderLayout(5,5));
//         container_panel = new JPanel();
        //add (container_panel, "Center");
        child_panels = new Vector();
        active_panel = null;
        
        dummypanel = new JPanel ();
        setRightComponent (dummypanel);

        // set up the left-hand tree view
        nav_tree_root = new DefaultMutableTreeNode (new tscUserObject ("Select a Panel", null, null, tscUserObject.FOLDER), true);
        nav_tree_model = new DefaultTreeModel(nav_tree_root);
        nav_tree = new JTree(nav_tree_model);
        nav_tree.putClientProperty("JTree.lineStyle", "Angled");
        //nav_tree.setRootVisible(false);

        DefaultTreeCellRenderer r = (DefaultTreeCellRenderer)nav_tree.getCellRenderer();
        nav_tree.setCellRenderer (new tscTreeCellRenderer (r.getOpenIcon(),
                                                           r.getClosedIcon()));

        sp = new JScrollPane (JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
				       JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        sp.setViewportView (nav_tree);
        setLeftComponent (sp);

    }



    /** Update look-n-feel.
     *  We need to override this ourselves so that child panels which aren't 
     *  being displayed (and therefore aren't in the normal awt component 
     *  hierarcy) get updated appropriately.
     *  This still doesn't seem to be propogating correctly to the plugpanel's 
     *  children JComponents.
     */
    public void updateUI () {
        super.updateUI();
        int i;
        JComponent p;
        if (child_panels != null) { 
            int n = child_panels.size();
            for (i = 0; i < n; i++) {
                p = (JComponent)child_panels.get(i);
                if (p != (JComponent)active_panel)
                    p.updateUI();
            }
        }
    }



    //---------------------- PlugPanelContainer Stuff ------------------------

    public void insertPanel (PlugPanel p) {
        try {
            int index = 0;
            child_panels.add (p);

            // add to the tree
            DefaultMutableTreeNode tn = new DefaultMutableTreeNode (new tscUserObject (p.getComponentName(), p.getComponentIcon(), p, tscUserObject.LEAF), false);
            String s = "";
            ConfigChunk ch = p.getConfiguration();
            if (ch != null) {
                VarValue v = ch.getValueFromToken ("Category", 0);
                if (v != null)
                    s = v.getString();
            }
            if (s.equals ("")) {
                nav_tree_root.add (tn);
                nav_tree_model.reload (nav_tree_root);
                //sp.setMinimumSize (sp.getPreferredSize());
            }
            else {
                DefaultMutableTreeNode t;
                int i;
                int n = nav_tree_root.getChildCount();
                for (i = 0; i < n; i++) {
                    t = (DefaultMutableTreeNode)nav_tree_root.getChildAt(i);
                    if (t.toString().equalsIgnoreCase(s)) {
                        t.add (tn);
                        nav_tree_model.reload(t);
                        break;
                    }
                }
                if (i == n) {
                    t = new DefaultMutableTreeNode (new tscUserObject (s, null, null, tscUserObject.FOLDER), true);
                    t.add (tn);
                    nav_tree_root.add(t);
                    nav_tree_model.reload (nav_tree_root);
                }
            }
           


            // if it's the "default" panel, display it
            if (p.getComponentName().equals (defaultpanel_name)) {
                System.out.println ("setting tree active component to '" + defaultpanel_name + "'");
                setActivePanel (p);
            }

            // since we mucked with the tree, we may have messed up the width
            // of the scroll panel (especially at first).
            nav_tree.validate();
            Dimension d = nav_tree.getPreferredSize();
            d.setSize (d.getWidth()+50, d.getHeight());
            sp.setSize (d);
            if (active_panel != null)
                ((JComponent)active_panel).setPreferredSize (new Dimension (0,0));

            resetToPreferredSizes();
        }
        catch (ClassCastException e) {
            Core.consoleErrorMessage ("GUI", "PlugPanel '" + 
                                      p.getComponentName() +
                                      "' isn't a JComponent!");
        }
    }



    public void removePanel (PlugPanel p) {
        try {
            if (active_panel == p) {
                setActivePanel (null);
            }
            child_panels.removeElement (p);
        }
        catch (ClassCastException e) {
            Core.consoleErrorMessage ("GUI", "PlugPanel '" + 
                                      p.getComponentName() +
                                      "' isn't a JComponent!");
        }
    }



    public void setActivePanel (PlugPanel p) {
        // all the messing around with sp's size is so that the split in
        // the splitpane won't jump around when we switch from one panel
        // to another.  it still might jump slightly immediately after
        // expanding the tree (because the tree might get wider).  but it's
        // pretty good now.

        if (p != active_panel) {

            Dimension d1 = sp.getMinimumSize();
            Dimension d2 = sp.getMaximumSize();
            Dimension d3 = sp.getSize();
            sp.setMinimumSize (d3);
            sp.setMaximumSize (d3);

            if (active_panel != null)
                remove (active_panel.getUIComponent());
            //container_panel.removeAll();
            active_panel = p;
            if (p != null) {
                //container_panel.add ((JComponent)p);
                //add ((JComponent)p, "Center");
                p.initUIComponent();
                setRightComponent (p.getUIComponent());
                validate();
                repaint();
            }

            sp.setMinimumSize (d1);
            sp.setMaximumSize (d2);
        }
    }



    public void rebuildDisplays () {
        if (active_panel != null)
            active_panel.rebuildDisplay();
    }

    //----------------------- VjComponent Stuff ------------------------------


    public String getComponentName () {
        return component_name;
    }


    public void setComponentName (String _name) {
        component_name = _name;
    }


    public void setConfiguration (ConfigChunk ch) throws VjComponentException {
        component_chunk = ch;
        component_name = ch.getName();

        defaultpanel_name = ch.getValueFromToken("default_panel",0).getString();
    }

    
    public ConfigChunk getConfiguration () {
        return component_chunk;
    }


    public void initialize () throws VjComponentException {
        // should add the rest of UI initialization code here...

        nav_tree.addMouseListener (this);

        setOneTouchExpandable (true);
        setDividerLocation (0.16d);
    }


    public VjComponent addConfig (ConfigChunk ch) throws VjComponentException {
        throw new VjComponentException (component_name + " does not support child component: " + ch.getName());
    }


    public boolean removeConfig (String name) {
        return false;
    }


    public void destroy () {
        ;
    }


    //----------------------- Mouse Event Stuff ------------------------------

    public void mouseEntered(MouseEvent e) {}
    public void mouseReleased(MouseEvent e) {}
    public void mouseExited(MouseEvent e) {}
    public void mousePressed (MouseEvent e) {}

    public void mouseClicked(MouseEvent e) {

	int selRow = nav_tree.getRowForLocation(e.getX(), e.getY());
	if (selRow == -1)
	    return;
	TreePath treeitem_menu_path = nav_tree.getPathForLocation(e.getX(), e.getY());
	tscUserObject ud = ((tscUserObject)((DefaultMutableTreeNode)treeitem_menu_path.getLastPathComponent()).getUserObject());


	int mod = e.getModifiers();
	if (e.getClickCount() == 1) {
            if (ud.style == tscUserObject.LEAF) {
                setActivePanel ((PlugPanel)ud.object);
            }
        }
    }


}
