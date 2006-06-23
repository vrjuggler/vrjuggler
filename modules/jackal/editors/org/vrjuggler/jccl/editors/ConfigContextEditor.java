/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/
package org.vrjuggler.jccl.editors;

import java.awt.*;
import java.awt.datatransfer.*;
import java.awt.dnd.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.tree.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.ConfigContextEvent;
import org.vrjuggler.jccl.config.event.ConfigContextListener;
import org.vrjuggler.jccl.editors.net.TinyBrowser;


public class ConfigContextEditor
   extends JPanel
   implements ConfigContextListener
{
   public ConfigContextEditor()
   {
      // Init the GUI
      try
      {
         jbInit();
      }
      catch (Exception e)
      {
         e.printStackTrace();
      }

      helpPane.addHyperlinkListener(new DocLinkListener());
      helpBrowserFrame.setContentPane(helpBrowser);
      helpBrowserFrame.setSize(new Dimension(640, 480));
      helpBrowserFrame.setTitle("Config Element Help Browser");
      helpBrowserFrame.validate();

      // Hack around JSplitPane bug where it can't handle setting the divider
      // location until it's visible. JDC Bug #4182558.
      propsSplitPane.addComponentListener(new ComponentAdapter()
      {
         public void componentResized(ComponentEvent evt)
         {
            propsSplitPane.setDividerLocation(0.80);
            propsSplitPane.removeComponentListener(this);
         }
      });

      // Set up the tree
      mContextModel = new ConfigContextModel();
      mElementTree.setModel(mContextModel);
      mElementTree.setCellRenderer(new ConfigContextCellRenderer());
      mElementTree.getSelectionModel().setSelectionMode(TreeSelectionModel.SINGLE_TREE_SELECTION);

      // Setup a listener for the tree so that when an element is selected, the
      // appropriate element is displayed in the property sheet.
      mElementTree.addTreeSelectionListener(new TreeSelectionListener()
         {
            public void valueChanged(TreeSelectionEvent evt)
            {
               // Get the selected node.
               DefaultMutableTreeNode node =
                  (DefaultMutableTreeNode)mElementTree.getLastSelectedPathComponent();
               if(null == node)
               {
                  removeBtn.setEnabled(false);
                  emptyPropertySheet();
                  return;
               }

               // Get the value of the UserObject attached to the node.
               Object value = node.getUserObject();
               if (value == null || !(value instanceof ConfigElement))
               {
                  System.out.println("Selecting a node that is not a ConfigElement");
                  removeBtn.setEnabled(false);
                  emptyPropertySheet();
                  // Empty the help information
                  getHelpPane().setText("");

                  return;
               }

               removeBtn.setEnabled(! ((ConfigElement) value).isReadOnly());
               Color start_color = getStartColor();

               // Create a new PropertySheet for the given ConfigElement.
               ConfigElement elt = (ConfigElement)value;
               
               if (null != mElementPropSheet)
               {
                  mElementPropSheet.finalize();
               }

               ConfigDefinition def = elt.getDefinition();
               mElementTypeLabel.setText(def.getName() + " <" +
                                         def.getToken() + ">");
               mElementPropSheet =
                  PropertySheetFactory.instance().makeSheet(getContext(), elt,
                                                            start_color);
               mElementPropSheetScrollPane.getViewport().removeAll();
               mElementPropSheetScrollPane.getViewport().add(mElementPropSheet,
                                                             null);
               
               // Display the help information about the selected configuration element.
               getHelpPane().setText(elt.getDefinition().getHelp());
            }
         });
   }

   /**
    * Sets the configuration context this editor should be editing.
    *
    * @param context      the context to edit
    */
   public void setContext(ConfigContext context)
   {
      if (null != getContext())
      {
         getContext().removeConfigContextListener(this);
      }
      
      mContextModel.setContext(context);
      getContext().addConfigContextListener(this);
      
      // Make sure that we can only add when we have a writable data source.
      addBtn.setEnabled(false);
      removeBtn.setEnabled(false);
      mElementTree.setContextEditable(false);
      
      for (Iterator itr = context.getResources().iterator() ; itr.hasNext() ; )
      {
         if ( !getBroker().get((String)itr.next()).isReadOnly() )
         {
            addBtn.setEnabled(true);
            removeBtn.setEnabled(true);
            mElementTree.setContextEditable(true);
            break;
         }
      }
      
      java.util.List elts = getBroker().getElements(context);
      if (elts.size() > 0)
      {
         ConfigElement elt = (ConfigElement)elts.get(0);

         // Create a PropertySheet for the default selected ConfigElement.
         Color start_color = getStartColor();

         // Create a new PropertySheet for the given ConfigElement.
         if (null != mElementPropSheet)
         {
            mElementPropSheet.finalize();
         }
         mElementPropSheet =
            PropertySheetFactory.instance().makeSheet(getContext(), elt,
                                                      start_color);

         selectConfigElement(elt);
      }

      mElementTree.expandPath(new TreePath(mContextModel.getRoot()));
   }

   /**
    * Gives notification that a resource was added to the context.
    *
    * @param evt     the context event
    */
   public void resourceAdded(ConfigContextEvent evt)
   {
      // Make sure that we can only add when we have a writable data source.
      addBtn.setEnabled(false);
      removeBtn.setEnabled(false);
      mElementTree.setContextEditable(false);
      
      for (Iterator itr = getContext().getResources().iterator() ; itr.hasNext() ; )
      {
         if ( !getBroker().get((String)itr.next()).isReadOnly() )
         {
            addBtn.setEnabled(true);
            removeBtn.setEnabled(true);
            mElementTree.setContextEditable(true);
            return;
         }
      }
   }

   /**
    * Gives notification that a resource was removed from the context.
    *
    * @param evt     the context event
    */
   public void resourceRemoved(ConfigContextEvent evt)
   {
      // Make sure that we can only add when we have a writable data source.
      addBtn.setEnabled(false);
      removeBtn.setEnabled(false);
      mElementTree.setContextEditable(false);
      
      for (Iterator itr = getContext().getResources().iterator() ; itr.hasNext() ; )
      {
         if ( !getBroker().get((String)itr.next()).isReadOnly() )
         {
            addBtn.setEnabled(true);
            removeBtn.setEnabled(true);
            mElementTree.setContextEditable(true);
            return;
         }
      }
   }

   /**
    * Selects the path to the given ConfigElement in the ElementTree.
    */
   public void selectConfigElement(ConfigElement elm)
   {
      java.util.List element_nodes = mContextModel.getNodesFor(elm);
      if (element_nodes.size() > 0)
      {
         TreeNode element_node = (TreeNode)element_nodes.get(0);
         TreePath path = new TreePath(mContextModel.getPathToRoot(element_node));
         mElementTree.setSelectionPath(path);
      } 
   }

   /**
    * Gets the configuration context this editor is editing.
    *
    * @return  the context being edited
    */
   public ConfigContext getContext()
   {
      return mContextModel.getContext();
   }

   /**
    * Gets a handle to the configuration broker.
    */
   protected ConfigBroker getBroker()
   {
      if (mBroker == null)
      {
         synchronized (this)
         {
            if (mBroker == null)
            {
               mBroker = new ConfigBrokerProxy();
            }
         }
      }
      return mBroker;
   }

   /**
    * Gets a handle to the ElementTree that is being used.
    */
   public ElementTree getElementTree()
   {
      return mElementTree;
   }

   /**
    * Initializes the GUI. Auto-generated by JBuilder.
    */
   private void jbInit()
      throws Exception
   {
      JPanel temp = new JPanel();
      temp.setLayout(new BorderLayout());
      treePane.setLayout(treeLayout);
      
      this.setLayout(mBaseLayout);
      mElementTreeScrollPane.setMinimumSize(new Dimension(0, 0));
      mElementTree.setRootVisible(false);
      mElementTree.setShowsRootHandles(true);
      
      mBaseSplitPane.setOneTouchExpandable(true);
      this.add(mBaseSplitPane, BorderLayout.CENTER);
      mBaseSplitPane.add(treePane, JSplitPane.LEFT);
      mBaseSplitPane.add(propsSplitPane, JSplitPane.RIGHT);

      mElementTypeLabel.setBackground(UIManager.getColor("textHighlight"));
      mElementTypeLabel.setFont(new java.awt.Font("SansSerif",
                                                  java.awt.Font.BOLD, 14));
      mElementTypeLabel.setForeground(Color.black);
      mElementTypeLabel.setBorder(BorderFactory.createRaisedBevelBorder());
      mElementTypeLabel.setHorizontalAlignment(SwingConstants.LEFT);

      mElementPropSheetPanel.setLayout(mElementPropSheetLayout);
      mElementPropSheetPanel.add(mElementTypeLabel, BorderLayout.NORTH);
      mElementPropSheetPanel.add(mElementPropSheetScrollPane,
                                 BorderLayout.CENTER);

      propsSplitPane.setOrientation(JSplitPane.VERTICAL_SPLIT);
      propsSplitPane.setOneTouchExpandable(true);
      propsSplitPane.setDividerSize(5);

      helpScrollPane.setViewportView(helpPane);
      propsSplitPane.add(mElementPropSheetPanel, JSplitPane.TOP);
      propsSplitPane.add(helpScrollPane, JSplitPane.BOTTOM);
      propsSplitPane.setResizeWeight(1.0);
      
      mElementPropSheetScrollPane.getViewport().add(mElementPropSheet, null);
      mElementTreeScrollPane.getViewport().add(mElementTree, null);
      
      treeToolbar.setFloatable(false);

      addBtn.setText("Add");
      addBtn.setEnabled(false);
      removeBtn.setText("Remove");
      removeBtn.setEnabled(false);
      removeBtn.setActionCommand("delete");
      mElementTree.setContextEditable(false);

      helpScrollPane.setMinimumSize(new Dimension(0, 0));
      helpScrollPane.setPreferredSize(new Dimension(150, 40));
      helpPane.setBackground(new Color(255, 253, 181));
      helpPane.setBorder(null);
      helpPane.setEditable(false);
      helpPane.setMinimumSize(new Dimension(0, 0));

      addBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            addAction(evt);
         }
      });
      removeBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            removeAction(evt);
         }
      });
      
      treePane.add(treeToolbar, BorderLayout.NORTH);
      treePane.add(mElementTreeScrollPane, BorderLayout.CENTER);
      treeToolbar.add(addBtn);
      treeToolbar.add(removeBtn);
   }

   /**
    * Removes a config element from the current context using the ConfigBroker.
    */
   protected void removeAction(ActionEvent evt)
   {
      mElementTree.actionPerformed(evt);
   }

   /**
    * Adds a new config element to the current context using the ConfigBroker.
    */
   protected void addAction(ActionEvent evt)
   {
      DefaultMutableTreeNode node = (DefaultMutableTreeNode)
                     mElementTree.getLastSelectedPathComponent();

      // Get a list of all known ConfigDefinitions.
      java.util.List defs = getBroker().getRepository().getAllLatest();
      java.util.List non_abstract_defs = new ArrayList();

      for ( Iterator i = defs.iterator(); i.hasNext(); )
      {
         ConfigDefinition d = (ConfigDefinition) i.next();
         if ( ! d.isAbstract() )
         {
            non_abstract_defs.add(d);
         }
      }

      // Ask the user to choose a base ConfigDefinition.
      ConfigDefinitionChooser chooser = new ConfigDefinitionChooser();
      chooser.setDefinitions(non_abstract_defs);
      int result = chooser.showDialog(this);

      // If the user did not cancel their choice, make a new ConfigElement
      // for the chosen ConfigDefinition.
      if (result == ConfigDefinitionChooser.APPROVE_OPTION)
      {
         ConfigElementFactory temp_factory = new ConfigElementFactory(defs);
         
         // Create a unique name
         ConfigElement element = temp_factory.createUnique(chooser.getSelectedDefinition(), getContext());

         // Make sure this add goes through successfully
         if (! getBroker().add(getContext(), element))
         {
            JOptionPane.showMessageDialog(SwingUtilities.getAncestorOfClass(Frame.class, this),
                                          "There are no configuration files active.",
                                          "Error",
                                          JOptionPane.ERROR_MESSAGE);
            return;
         }

         // Make sure the new node gets selected
         selectConfigElement(element);
      }
   }
   
   public JEditorPane getHelpPane()
   {
      return helpPane;
   }

   private void emptyPropertySheet()
   {
      mElementTypeLabel.setText("");
      mElementPropSheet.finalize();
      mElementPropSheet.revalidate();
      mElementPropSheet.repaint();
   }

   private Color getStartColor()
   {
      Color color = UIManager.getColor("Tree.selectionBackground");

      if ( null == color )
      {
         color = new Color(160, 160, 180);
         System.out.println("Could not get the color " +
                            "'Tree.selectionBackground' from the " +
                            "UIManager.");
      }
      float[] hsb = Color.RGBtoHSB(color.getRed(), color.getGreen(),
                                   color.getBlue(), null);

      System.out.println("HSB before: " + hsb[0] + " " + hsb[1] + " " + hsb[2]);

      // Make sure that the color is bright enough to allow good contrast
      // between the background color and the text (foreground) color.
      // XXX: There is probably a better way to handle this... -PH 12/24/2004
      if ( hsb[2] < 0.8 )
      {
         color = Color.getHSBColor(hsb[0], hsb[1], 0.9f);
      }

      return color;
   }

   private class DocLinkListener
      implements HyperlinkListener
   {
      public void hyperlinkUpdate(HyperlinkEvent e)
      {
         if (e.getEventType() == HyperlinkEvent.EventType.ACTIVATED)
         {
            helpBrowser.setPage(e.getURL());
            helpBrowserFrame.setVisible(true);
         }
      }
   }

   private BorderLayout mBaseLayout = new BorderLayout();
   private JSplitPane mBaseSplitPane = new JSplitPane();
   private JScrollPane mElementTreeScrollPane = new JScrollPane();
   private ElementTree mElementTree = new ElementTree();
   private JLabel mElementTypeLabel = new JLabel();
   private JPanel mElementPropSheetPanel = new JPanel();
   private BorderLayout mElementPropSheetLayout = new BorderLayout();
   private JScrollPane mElementPropSheetScrollPane = new JScrollPane();

   // CustomEditor stuff
   private JScrollPane mCustomConfigElementEditorScrollPane = new JScrollPane();
   private java.util.List mCustomEditors = null;
   
   private PropertySheet mElementPropSheet = new PropertySheet();

   private JSplitPane propsSplitPane = new JSplitPane();
   private JScrollPane helpScrollPane = new JScrollPane();
   private JEditorPane helpPane = new JEditorPane("text/html", "");
   private JFrame helpBrowserFrame = new JFrame();
   private TinyBrowser helpBrowser = new TinyBrowser();

   private BorderLayout treeLayout = new BorderLayout();
   private JPanel treePane = new JPanel();
   private JToolBar treeToolbar = new JToolBar();
   private JButton addBtn = new JButton();
   private JButton removeBtn = new JButton();

   /** The data model used to represent the context. */
   private ConfigContextModel mContextModel;

   /** The handle to the configuration broker. */
   private ConfigBroker mBroker;
}

class ConfigContextCellRenderer
   extends DefaultTreeCellRenderer
{
   public ConfigContextCellRenderer()
   {
      ClassLoader loader = getClass().getClassLoader();
      try
      {
         // Get all the icons we'll ever need now so we can reuse them later
         mElementIcon = new ImageIcon(loader.getResource(
            "org/vrjuggler/jccl/editors/images/config_element.gif"));
         mPropertyIcon = new ImageIcon(loader.getResource(
            "org/vrjuggler/jccl/editors/images/property.gif"));
         mCategoryIcon = new ImageIcon(loader.getResource(
            "org/vrjuggler/jccl/editors/images/category.gif"));
      }
      catch (NullPointerException npe)
      {
         // Images aren't available ... oh well
      }
   }

   public Component getTreeCellRendererComponent(
                        JTree tree, Object node, boolean selected,
                        boolean expanded, boolean leaf, int row,
                        boolean focused)
   {
      // Get the default settings from the UI LAF
      super.getTreeCellRendererComponent(tree, node, selected,
                                         expanded, leaf, row, focused);

      Object value = ((DefaultMutableTreeNode)node).getUserObject();

      // Display the name of configuration elements
      if (value instanceof ConfigElement)
      {
         setText(((ConfigElement)value).getName());
         setFont(null);
         setIcon(mElementIcon);
      }
      // Display the name of properties
      else if (value instanceof PropertyDefinition)
      {
         setText(((PropertyDefinition)value).getName());
         setFont(tree.getFont().deriveFont(Font.ITALIC));
         setIcon(mPropertyIcon);
      }
      // Display the name of categories
      else if (value instanceof Category)
      {
         setText(((Category)value).getName());
         setFont(tree.getFont().deriveFont(Font.BOLD));
         setIcon(null);
      }

      return this;
   }

   private Icon mElementIcon;
   private Icon mPropertyIcon;
   private Icon mCategoryIcon;
}
