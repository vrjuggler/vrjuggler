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

public class ConfigContextEditor
   extends JPanel
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
               // Remove all old custom editors.
               if (mCustomEditors != null)
               {
                  for(Iterator itr = mCustomEditors.iterator() ; itr.hasNext() ; )
                  {
                     CustomEditor editor = (CustomEditor)itr.next(); 
                     mTabPane.remove(mTabPane.indexOfTab(editor.getTitle()));
                  }
                  mCustomEditors = null;
               }

               // Get the selected node.
               DefaultMutableTreeNode node =
                  (DefaultMutableTreeNode)mElementTree.getLastSelectedPathComponent();
               if(null == node)
               {
                  // Empty the property sheet.
                  mElementPropSheet.removeAll();
                  mElementPropSheet.refresh();
                  return;
               }

               // Get the value of the UserObject attached to the node.
               Object value = node.getUserObject();
               if (value == null || !(value instanceof ConfigElement))
               {
                  System.out.println("Selecting a node that is not a ConfigElement");
                  return;
               }

               
               Color start_color = new Color(160, 160, 180);
      
               Object color = UIManager.get( "desktop" );
               if(null != color && color instanceof Color)
               {
                  start_color = (Color)color;
               }
               else
               {
                  System.out.println("Could not get the desktop color from the  UIManager.");
               }
               
               // Create a new PropertySheet for the given ConfigElement.
               ConfigElement elt = (ConfigElement)value;
               
               mElementPropSheet =
                  PropertySheetFactory.instance().makeSheet(elt, start_color);
               mElementPropSheetScrollPane.getViewport().removeAll();
               mElementPropSheetScrollPane.getViewport().add(mElementPropSheet, null);


               // Load all supported custom editors.
               mCustomEditors = CustomEditorRegistry.findEditors(elt.getDefinition().getToken());
    
               if (mCustomEditors != null)
               {


                  for(Iterator itr = mCustomEditors.iterator() ; itr.hasNext() ; )
                  {
                     CustomEditor editor = (CustomEditor)itr.next(); 
                     editor.setConfigElement(elt);
                     mTabPane.add(editor.getPanel(), editor.getTitle());
                  }
               }  
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
      mContextModel.setContext(context);
      java.util.List elts = getBroker().getElements(context);
      if (elts.size() > 0)
      {
         ConfigElement elt = (ConfigElement)elts.get(0);

         // Create a PropertySheet for the default selected ConfigElement.
         Color start_color = new Color(160, 160, 180);

         Object color = UIManager.get( "desktop" );
         if(null != color && color instanceof Color)
         {
            start_color = (Color)color;
         }
         else
         {
            System.out.println("Could not get the desktop color from the  UIManager.");
         }
         
         // Create a new PropertySheet for the given ConfigElement.
         mElementPropSheet =
            PropertySheetFactory.instance().makeSheet(elt, start_color);
               
         selectConfigElement(elt);
      }

      mElementTree.expandPath(new TreePath(mContextModel.getRoot()));
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

      propsSplitPane.setOrientation(JSplitPane.VERTICAL_SPLIT);
      propsSplitPane.setOneTouchExpandable(true);
     
      helpScrollPane.setViewportView(helpPane);
      propsSplitPane.add(mTabPane, JSplitPane.TOP);
      propsSplitPane.add(helpScrollPane, JSplitPane.BOTTOM);
      
      mTabPane.add(mElementPropSheetScrollPane, "DefaultEditor");
      mElementPropSheetScrollPane.getViewport().add(mElementPropSheet, null);
      mElementTreeScrollPane.getViewport().add(mElementTree, null);
      
      treeToolbar.setFloatable(false);

      addBtn.setText("Add");
      //addBtn.setEnabled(false);
      removeBtn.setText("Remove");
      //removeBtn.setEnabled(false);

      helpScrollPane.setMinimumSize(new Dimension(0, 0));
      helpPane.setBackground(new Color(255, 253, 181));
      helpPane.setBorder(null);
      helpPane.setEditable(false);
      helpPane.setMinimumSize(new Dimension(0, 0));

      helpScrollPane.setViewportView(helpPane);
      
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
      TreePath path = mElementTree.getLeadSelectionPath();
      DefaultMutableTreeNode node = (DefaultMutableTreeNode)path.getLastPathComponent();
      Object temp = node.getUserObject();
      if(temp instanceof ConfigElement)
      {
         // Try to remove the element.
         if (!getBroker().remove(mContextModel.getContext(), (ConfigElement)temp))
         {
            System.out.println("Failed...");
         }
      }
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

      // Ask the user to choose a base ConfigDefinition.
      ConfigDefinitionChooser chooser = new ConfigDefinitionChooser();
      chooser.setDefinitions(defs);
      int result = chooser.showDialog(this);

      // If the user did not cancel their choice, make a new ConfigChunk for
      // the chosen ConfigDefinition.
      if (result == ConfigDefinitionChooser.APPROVE_OPTION)
      {
         ConfigElementFactory temp_factory = new ConfigElementFactory(defs);
         
         // TODO: Compute a unique name
         // -We used to use the following to get a unique name, but it is gone now :(
         //       chunk.setName(configChunkDB.getNewName(chunk.getDesc().getName()));

         // -The following method can lead to problems since we do not want
         // elements that are named the same.
         //       chunk.setName(chunk.getDesc().getName()); 
         
         // -Our temporary Solution is to try to make it obvious to the user that they 
         // need to change it.
         ConfigElement element = temp_factory.create("CHANGEME", chooser.getSelectedDefinition());
         

         // Make sure this add goes through successfully
         if (! getBroker().add(getContext(), element))
         {
            JOptionPane.showMessageDialog(this,
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
    
   private BorderLayout mBaseLayout = new BorderLayout();
   private JSplitPane mBaseSplitPane = new JSplitPane();
   private JTabbedPane mTabPane = new JTabbedPane();
   private JScrollPane mElementTreeScrollPane = new JScrollPane();
   private ElementTree mElementTree = new ElementTree();
   private JScrollPane mElementPropSheetScrollPane = new JScrollPane();

   // CustomEditor stuff
   private JScrollPane mCustomConfigElementEditorScrollPane = new JScrollPane();
   private java.util.List mCustomEditors = null;
   
   private PropertySheet mElementPropSheet = new PropertySheet();

   private JSplitPane propsSplitPane = new JSplitPane();
   private JScrollPane helpScrollPane = new JScrollPane();
   private JEditorPane helpPane = new JEditorPane("text/html", "")
   {
      public boolean getScrollableTracksViewportWidth() { return true; }
      public boolean getScrollableTracksViewportHeight() { return true; }
   };
   
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
