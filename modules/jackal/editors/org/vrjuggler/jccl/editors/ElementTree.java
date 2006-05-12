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


/**
 * Wraps a ConfigElement to allow us to transfer it from on context to another
 * either using copy/past or drag and drop methods.
 */
class ConfigElementSelection
   extends TransferableConfigElement
   implements ClipboardOwner
{
   /**
    * Constructor that takes the given ConfigElement and creates a Transferable
    * wrapper around it to allow us to move the ConfigElement from one context
    * to another.
    */
   public ConfigElementSelection(ConfigElement elm)
   {
      super(elm);
   }

   /**
    * Informs the ConfigContextEditor that it has lost the clipboard.
    */
   public void lostOwnership(Clipboard clip, Transferable obj)
   {
      ;
   }
}

/**
 * Returns a component used to edit the name of the ConfigElement in the JTree.
 */
class ElementNameEditor implements TreeCellEditor
{
   /**
    * Constructor that takes a reference to the JTree that we will be editing.
    * This is required so that we can determine if the selected node is
    * editable.
    */
   public ElementNameEditor(JTree tree)
   {
      mTree = tree;
   }

   /**
    * Returns a component that will be used to edit the name of the
    * ConfigElement.
    */
   public Component getTreeCellEditorComponent(JTree tree,
                                               Object value,
                                               boolean isSelected,
                                               boolean expanded,
                                               boolean leaf,
                                               int row)
   {
      // Get a reference to the selected TreeNode.
      DefaultMutableTreeNode node = (DefaultMutableTreeNode)value;

      if(!(node.getUserObject() instanceof ConfigElement))
      {
         throw new IllegalArgumentException("Error: "
               + "The selected node contains an object with type:"
               + node.getUserObject().getClass());
      }

      // Get a reference to the selected ConfigElement.
      mElement = (ConfigElement)node.getUserObject();
      mTextField = new JTextField(mElement.getName());
      mTree = tree;

      // Specify what should happen when done editing.
      mTextField.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            // Force the focus to be lost.
            //mTextField.transferFocusUpCycle();
            // Force the focus to be transfered to the next component.
            //mTextField.transferFocus();

            // This is no longer needed since the above line will force a
            // focusLostEvent. But I have choosen to leave this line here in
            // case it might become useful later.
            //stopCellEditing();
            mTree.clearSelection();
         }
      });

      mTextField.addFocusListener(new FocusAdapter()
      {
         public void focusLost(FocusEvent evt)
         {
            stopCellEditing();
         }
      });

      return mTextField;
   }

   public Object getCellEditorValue()
   {
      return mElement;
   }

   public boolean isCellEditable(EventObject evt)
   {
      TreePath current_selection = mTree.getSelectionPath();
      if (current_selection != null)
      {
         DefaultMutableTreeNode current_node = (DefaultMutableTreeNode)
            (current_selection.getLastPathComponent());
         if (current_node != null)
         {
            if(current_node.getUserObject() instanceof ConfigElement)
            {
               return true;
            }
         }
      }
      return false;
   }

   public boolean shouldSelectCell(EventObject evt)
   {
      return true;
   }

   public boolean stopCellEditing()
   {
      // Detect when the user does not actually change the name.
      if (mTextField.getText().equals(mElement.getName()))
      {
         return false;
      }

      ConfigContext ctx = ((ConfigContextModel)mTree.getModel()).getContext();
      if (ctx.containsElementNamed(mTextField.getText()))
      {
         JOptionPane.showMessageDialog(null, "A ConfigElement named \""
               + mTextField.getText() + "\" already exists.",
               "Invalid Name", JOptionPane.ERROR_MESSAGE);
         return false;
      }

      mElement.setName(mTextField.getText(), ctx);

      return true;
   }

   public void cancelCellEditing()
   {
      ;
   }

   public void addCellEditorListener(CellEditorListener l)
   {
      ;
   }

   public void removeCellEditorListener(CellEditorListener l)
   {
      ;
   }

   private JTree           mTree       = null;
   private JTextField      mTextField  = null;
   private ConfigElement   mElement    = null;
}

/**
 * A JTree that allows us to transfer ConfigElements to/from other ElementTrees
 * in other contexts.
 */
class ElementTree
   extends JTree
   implements DragGestureListener
            , DragSourceListener
            , DropTargetListener
            , ActionListener
{
   JPopupMenu mPopup;

   Clipboard clipboard = getToolkit().getSystemClipboard();

   /**
    * Create a JTree that supports the dragging and dropping of ConfigElements.
    */
   public ElementTree()
   {
      super();

      // Allow the user to change the name of a leaf node(an element)
      setEditable(true);

//      setCellEditor(new DefaultTreeCellEditor(this,
//                                              new DefaultTreeCellRenderer()));
      setCellEditor(new DefaultTreeCellEditor(this,
                                              new DefaultTreeCellRenderer(),
                                              new ElementNameEditor(this)));

      DragSource dragSource = DragSource.getDefaultDragSource();

      // Create a new drop target. We do not need to keep a reference to this
      // since internally it associates itself with the componenets that it
      // needs.
      new DropTarget(
         this,                             // Component to associate with.
         DnDConstants.ACTION_COPY_OR_MOVE, // Default acceptable actions.
         this                              // Drop target listener.
      );

      // Create a DragGestureRecognizer to watch for drags. Once again we do
      // not need to keep the reference that is returned from this method.
      dragSource.createDefaultDragGestureRecognizer(
         this,                             // component where drag originates
         DnDConstants.ACTION_COPY_OR_MOVE, // actions
         this                              // drag gesture recognizer
      );

      // Load the icons for the popup menu.
      ClassLoader loader = getClass().getClassLoader();
      ImageIcon cut_icon = null;
      ImageIcon copy_icon = null;
      ImageIcon paste_icon = null;
      ImageIcon remove_icon = null;

      try
      {
         String img_root = "org/vrjuggler/jccl/editors/images";
         cut_icon    = new ImageIcon(loader.getResource(img_root +
                                                        "/Cut16.gif"));
         copy_icon   = new ImageIcon(loader.getResource(img_root +
                                                        "/Copy16.gif"));
         paste_icon  = new ImageIcon(loader.getResource(img_root +
                                                        "/Paste16.gif"));
         remove_icon = new ImageIcon(loader.getResource(img_root +
                                                        "/Delete16.gif"));
      }
      catch(Exception ex)
      {
         ex.printStackTrace();
      }

      // Set up the pop up menu
      mPopup = new JPopupMenu();
      mDeleteMenuItem = new JMenuItem("Delete", remove_icon);
      mDeleteMenuItem.addActionListener(this);
      mDeleteMenuItem.setActionCommand("delete");
      mDeleteMenuItem.setAccelerator(
         KeyStroke.getKeyStroke(KeyEvent.VK_DELETE, 0)
      );
      mPopup.add(mDeleteMenuItem);

      mRenameMenuItem = new JMenuItem("Rename");
      mRenameMenuItem.addActionListener(this);
      mRenameMenuItem.setActionCommand("rename");
      mPopup.add(mRenameMenuItem);

      mPopup.addSeparator();

      int shortcut_mask = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();

      mCutMenuItem = new JMenuItem("Cut", cut_icon);
      mCutMenuItem.addActionListener(this);
      mCutMenuItem.setActionCommand("cut");
      mCutMenuItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_X,
                                                         shortcut_mask));
      mPopup.add(mCutMenuItem);

      mCopyMenuItem = new JMenuItem("Copy", copy_icon);
      mCopyMenuItem.addActionListener(this);
      mCopyMenuItem.setActionCommand("copy");
      mCopyMenuItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_C,
                                                          shortcut_mask));
      mPopup.add(mCopyMenuItem);

      mPasteMenuItem = new JMenuItem("Paste", paste_icon);
      mPasteMenuItem.addActionListener(this);
      mPasteMenuItem.setActionCommand("paste");
      mPasteMenuItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_V,
                                                           shortcut_mask));
      mPopup.add(mPasteMenuItem);

      mPopup.setOpaque(true);
      mPopup.setLightWeightPopupEnabled(true);

      // Add mouse listener to get the popup menu events.
      addMouseListener(
         new MouseAdapter()
         {
            // NOTE: XWindows and Windows sense popup trigger events at
            //       different times. On windows it occurs when the mouse is
            //       released, while on XWindows is occurs when the mouse
            //       button is pressed.

            // Check for a popup trigger on Windows.
            public void mouseReleased( MouseEvent e )
            {
               // Get the currently selected ConfigElement.
               TreePath path = getLeadSelectionPath();
               if (path != null)
               {
                  DefaultMutableTreeNode node =
                     (DefaultMutableTreeNode)path.getLastPathComponent();
                  Object temp = node.getUserObject();

                  if (temp instanceof ConfigElement)
                  {
                     mDeleteMenuItem.setEnabled(true);
                     mRenameMenuItem.setEnabled(true);
                     mCutMenuItem.setEnabled(true);
                     mCopyMenuItem.setEnabled(true);
                     mPasteMenuItem.setEnabled(true);

                     // Ensure that the ConfigElement is writable.
                     if ( ((ConfigElement)temp).isReadOnly() )
                     {
                        mDeleteMenuItem.setEnabled(false);
                        mRenameMenuItem.setEnabled(false);
                        mCutMenuItem.setEnabled(false);
                        mPasteMenuItem.setEnabled(false);
                     }

                     if ( e.isPopupTrigger())
                     {
                        mPopup.show((JComponent) e.getSource(), e.getX(),
                                    e.getY());
                     }
                  }
               }
            }
            // Check for a popup trigger on XWindows.
            public void mousePressed(MouseEvent e)
            {
               mouseReleased(e);
            }
         });

      // Add key listener to catch the shortcuts for cut/copy/paste/delete
      addKeyListener(
         new KeyAdapter()
         {
            public void keyPressed(KeyEvent e)
            {
               int shortcut_mask =
                  Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();
               if(e.getKeyCode() == KeyEvent.VK_C &&
                  e.getModifiers() == shortcut_mask)
               {
                  System.out.println("We have a copy.");
                  actionPerformed(new ActionEvent(this, 0, "copy"));
               }
               if(e.getKeyCode() == KeyEvent.VK_V &&
                  e.getModifiers() == shortcut_mask)
               {
                  System.out.println("We have a paste.");
                  actionPerformed(new ActionEvent(this, 0, "paste"));
               }
               if(e.getKeyCode() == KeyEvent.VK_X &&
                  e.getModifiers() == shortcut_mask)
               {
                  System.out.println("We have a cut.");
                  actionPerformed(new ActionEvent(this, 0, "cut"));
               }
               if(e.getKeyCode() == KeyEvent.VK_DELETE && e.getModifiers() == 0)
               {
                  System.out.println("We have a delete.");
                  actionPerformed(new ActionEvent(this, 0, "delete"));
               }
            }
         }
      );
   }

   public void setContextEditable(boolean val)
   {
      mContextEditable = val;
   }

   /**
    * Catch all events for the pop-up menu events.
    */
   public void actionPerformed(ActionEvent ae)
   {
      // Get the currently selected ConfigElement.
      TreePath path = this.getLeadSelectionPath();

      // Ensure that we have a valid selection path.
      if (null == path)
      {
         return;
      }

      DefaultMutableTreeNode node =
         (DefaultMutableTreeNode)path.getLastPathComponent();
      Object temp = node.getUserObject();

      // Make sure that we have a ConfigElement selected.
      if ( (temp instanceof ConfigElement) )
      {
         ConfigElement elm = (ConfigElement)temp;

         // If the user selected the copy action.
         if( ae.getActionCommand().equals("copy") )
         {
            // Place ConfigElement in the system clipboard.
            ConfigElementSelection selection = new ConfigElementSelection(elm);
            // TODO: Change owner of clipboard.
            clipboard.setContents(selection, selection);
         }

         if ( !elm.isReadOnly() ) // Ensure that the ConfigElement is writable.
         {
            // If the user selcted the remove menu item.
            if( ae.getActionCommand().equals("delete") )
            {
               TreeModel model = this.getModel();

               // Remove the ConfigElement from the context.
               if( model instanceof ConfigContextModel )
               {
                  Object[] nodes = getLeadSelectionPath().getPath();

                  DefaultMutableTreeNode cur_node;

                  // The last node in the path is the config element that the
                  // user wants to remove.  We do not have to test the type
                  // before the cast because the user interface only allows
                  // removal of ConfigElement objects.
                  cur_node = (DefaultMutableTreeNode) nodes[nodes.length - 1];
                  ConfigElement elt_to_remove =
                     (ConfigElement) cur_node.getUserObject();

                  // Now, we need to see if the parent of the selected config
                  // element is a PropertyDefinition object.
                  cur_node = (DefaultMutableTreeNode) nodes[nodes.length - 2];
                  Object obj = cur_node.getUserObject();

                  ConfigContext context =
                     ((ConfigContextModel) model).getContext();

                  // If the parent of the config element to remove is a
                  // PropertyDefinition object, then the user wants to remove
                  // an embedded element.  This must be done by editing the
                  // appropriate property in the config element containing the
                  // one to be removed.
                  if ( obj instanceof PropertyDefinition )
                  {
                     PropertyDefinition prop_def = (PropertyDefinition) obj;

                     cur_node =
                        (DefaultMutableTreeNode) nodes[nodes.length - 3];
                     ConfigElement owner =
                        (ConfigElement) cur_node.getUserObject();
                     owner.removeProperty(prop_def.getToken(), elt_to_remove,
                                          context);
                  }
                  // If the parent of the config element to remove is not a
                  // PropertyDefinition object, then we have a "top-level"
                  // config element.  We have to remove it using the Config
                  // Broker.
                  else
                  {
                     ConfigBroker broker = new ConfigBrokerProxy();
                     boolean removed = broker.remove(context, elt_to_remove);

                     if ( ! removed )
                     {
                        Container parent =
                           SwingUtilities.getAncestorOfClass(Container.class,
                                                             this);
                        JOptionPane.showMessageDialog(
                           parent,
                           "Failed to remove config element '" +
                              elt_to_remove.getName() + "'",
                           "Config Element Removal Failed",
                           JOptionPane.ERROR_MESSAGE
                        );
                     }
                  }
               }
            }

            // Start editing the currently selected ConfigElement if we want
            // to rename it.
            if(ae.getActionCommand().equals("rename"))
            {
               // Get the currently selected ConfigElement.
               startEditingAtPath(path);
            }

            // If the user selects the cut operation we should first copy it
            // into the clipboard and then delete it from the active context.
            if(ae.getActionCommand().equals("cut"))
            {
               ActionEvent evt;
               evt = new ActionEvent(this, -1, "copy");
               actionPerformed(evt);
               evt = new ActionEvent(this, -1, "delete");
               actionPerformed(evt);
            }
         }
      }

      // If the user selected the paste action.
      if(ae.getActionCommand().equals("paste") && mContextEditable)
      {
         try
         {
            // Get the context model for this JTree.
            TreeModel model = this.getModel();
            ConfigContextModel config_model = (ConfigContextModel)model;

            // Get the ConfigElement to paste out of the clipboard.
            DataFlavor my_flavor =
               DataFlavorRepository.getConfigElementFlavor();
            Transferable tr = clipboard.getContents(this);

            // If this JTree has a valid JTreeModel and the incoming paste
            // supports the DataFlavor that we are trying to use.
            if ( model instanceof ConfigContextModel &&
                 tr.isDataFlavorSupported(my_flavor) )
            {
               // Get the ConfigElement that we are transfering and make a deep
               // copy of it.
               ConfigElement old_elm =
                  (ConfigElement) tr.getTransferData(my_flavor);
               ConfigElement new_elm = new ConfigElement(old_elm);

               // Make sure that we have a meaningful unique name.
               ConfigContext ctx = config_model.getContext();
               String base_name = new_elm.getName();
               int num = 1;
               while(ctx.containsElement(new_elm))
               {
                  String new_name = base_name + "Copy" + Integer.toString(num);
                  new_elm.setName(new_name);
                  ++num;
               }

               // Make sure that we are not trying to add an element to
               // ourselves.
               ConfigBroker broker = new ConfigBrokerProxy();

               // Make sure this add goes through successfully
               if (!broker.add(config_model.getContext(), new_elm))
               {
                  throw new Exception("Could not paste ConfigElement into " +
                                      "context.");
               }
               System.out.println("Paste completed...");
            }
         }
         catch(Exception ex)
         {
            System.out.println(ex);
            ex.printStackTrace();
         }
      }
   }

   /**
    * When the user attempts to drag an object out of the JTree we need to
    * prepare the associated transfer object.
    */
   public void dragGestureRecognized(DragGestureEvent e)
   {
      if(e.getDragAction() == DnDConstants.ACTION_COPY)
      {
         System.out.println("Copy...");
      }
      else if(e.getDragAction() == DnDConstants.ACTION_MOVE)
      {
         System.out.println("Move...");
      }
      else
      {
         System.out.println("Something else: " + e.getDragAction());
      }

      // Get the selected node the JTree.
      TreePath path = getLeadSelectionPath();
      DefaultMutableTreeNode node =
         (DefaultMutableTreeNode) path.getLastPathComponent();

      // Get the object associated with it, and if it is a ConfigElement
      // prepare it to be transfered.
      Object temp = node.getUserObject();
      if(null != temp && temp instanceof ConfigElement)
      {
         // Start the dragging action with the given icon, transferable object
         // and listener to listen for the dropEnd event that will only occur
         // in the case of a successful drag and drop.
         e.startDrag(DragSource.DefaultCopyDrop, // cursor
                     new ConfigElementSelection((ConfigElement)temp), // transferable
                     this);  // drag source listener
      }
   }

   /**
    * Take appropriate actions after a drop has ended.
    */
   public void dragDropEnd(DragSourceDropEvent e)
   {
      if(e.getDropAction() == DnDConstants.ACTION_COPY)
      {
         System.out.println("Copy has occured...");
      }
      else if(e.getDropAction() == DnDConstants.ACTION_MOVE)
      {
         System.out.println("Move has occured...");
      }
      else
      {
         System.out.println("Something else: " + e.getDropAction() +
                            " has occured.");
      }

      // If the drag was successful and it was not a copy action, then default
      // to a move action and remove the ConfigElement from our context.
      if(e.getDropSuccess() && (e.getDropAction() != DnDConstants.ACTION_COPY))
      {
         TreeModel model = this.getModel();
         if(model instanceof ConfigContextModel)
         {
            DataFlavor my_flavor =
               DataFlavorRepository.getConfigElementFlavor();

            try
            {
               // Get the ConfigElement that we are trying to transfer.
               // NOTE: We could get the selected ConfigElement from the JTree,
               // but this might be more error prone than getting it this way.
               ConfigElement elm =
                  (ConfigElement) e.getDragSourceContext().getTransferable().getTransferData(my_flavor);

               ConfigContextModel config_model = (ConfigContextModel)model;
               ConfigBroker broker = new ConfigBrokerProxy();

               // Make sure this add goes through successfully
               if (!broker.remove(config_model.getContext(), elm))
               {
                  throw new Exception("Failed to remove ConfigElement from " +
                                      "context after a copy action.");
               }
            }
            catch(Exception ex)
            {
               System.out.println(ex);
               ex.printStackTrace();
            }
         }
      }
   }

   public void dragEnter(DragSourceDragEvent e)
   {
      ;
   }

   public void dragExit(DragSourceEvent e)
   {
      ;
   }

   public void dragOver(DragSourceDragEvent e)
   {
      ;
   }

   public void dropActionChanged(DragSourceDragEvent e)
   {
      ;
   }

   /**
    * Attept to handle the dropping of a ConfigElement into this JTree.
    */
   public void drop(DropTargetDropEvent e)
   {
      if(e.getDropAction() == DnDConstants.ACTION_COPY)
      {
         System.out.println("Dropping a Copy...");
      }
      else if(e.getDropAction() == DnDConstants.ACTION_MOVE)
      {
         System.out.println("Dropping a Move...");
      }
      else
      {
         System.out.println("Dropping Something else: " + e.getDropAction());
      }

      try
      {
         TreeModel model = this.getModel();
         ConfigContextModel config_model = (ConfigContextModel)model;
         DataFlavor my_flavor = DataFlavorRepository.getConfigElementFlavor();
         Transferable tr = e.getTransferable();

         // If this JTree has a valid JTreeModel and the incoming drop supports
         // the DataFlavor that we are trying to use.
         if ( model instanceof ConfigContextModel &&
              e.isDataFlavorSupported(my_flavor) )
         {
            // Get the ConfigElement that we are transfering.
            ConfigElement elm = (ConfigElement)tr.getTransferData(my_flavor);

            // Make sure that we are not trying to add an element to ourselves.
            ConfigBroker broker = new ConfigBrokerProxy();
            java.util.List data_sources =
               config_model.getContext().getResources();

            for(Iterator itr = data_sources.iterator() ; itr.hasNext() ; )
            {
               java.util.List elm_list =
                  broker.getElementsIn((String)itr.next());

               if(elm_list.contains(elm))
               {
                  System.out.println("You can not drag an Element into a "
                                   + "context in which it already exists.");
                  e.rejectDrop();
                  return;
               }
            }

            // Accept whatever action was sent to us.
            e.acceptDrop(e.getDropAction());

            // Make sure this add goes through successfully
            if (!broker.add(config_model.getContext(), elm))
            {
               e.rejectDrop();
               throw new Exception("Could not add dropped ConfigElement " +
                                   "into context.");
            }
            System.out.println("Drop completed...");
            e.dropComplete(true);
         }
         else
         {
            e.rejectDrop();
         }
      }
      catch(Exception ex)
      {
         System.out.println(ex);
         ex.printStackTrace();
      }
   }

   public void dragEnter(DropTargetDragEvent e)
   {
      ;
   }

   public void dragExit(DropTargetEvent e)
   {
      ;
   }

   public void dragOver(DropTargetDragEvent e)
   {
      ;
   }

   public void dropActionChanged(DropTargetDragEvent e)
   {
      ;
   }

   private JMenuItem mDeleteMenuItem = null;
   private JMenuItem mRenameMenuItem = null;
   private JMenuItem mCutMenuItem = null;
   private JMenuItem mCopyMenuItem = null;
   private JMenuItem mPasteMenuItem = null;
   private boolean mContextEditable = false;
}
