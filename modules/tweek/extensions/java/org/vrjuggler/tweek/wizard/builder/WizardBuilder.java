/***************** <Tweek heading BEGIN do not edit this line> ****************
 * Tweek
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 ***************** <Tweek heading END do not edit this line> *****************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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
package org.vrjuggler.tweek.wizard.builder;

import java.io.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.tree.*;

import org.vrjuggler.tweek.wizard.*;
import org.vrjuggler.tweek.wizard.panes.*;

/**
 * Tweak panel bean that allows for the construction of Wizards.
 */
public class WizardBuilder
   extends JPanel
   implements Serializable
{
   /**
    * The model for our wizard tree.
    */
   private WizardBuilderTreeModel model = null;

   /**
    * Flag for whether the current wizard has been modified.
    */
   private boolean modified = false;

   /**
    * The name of the file the current wizard is associated with.
    */
   private String filename = null;

   /**
    * The file chooser for this builder. We reuse it in order to save memory
    * and help keep path history for the user.
    */
   private JFileChooser chooser;

   BorderLayout baseLayout = new BorderLayout();
   PropertySheet propSheet = new PropertySheet();
   JPanel westPanel = new JPanel();
   BorderLayout westLayout = new BorderLayout();
   JTree wizardTree = new JTree();
   JScrollPane treeScrollPane = new JScrollPane();
   JToolBar mainToolbar = new JToolBar();
   JButton addBtn = new JButton();
   JButton removeBtn = new JButton();
   JButton moveUpBtn = new JButton();
   JButton moveDownBtn = new JButton();
   JToolBar treeToolbar = new JToolBar();
   JButton newBtn = new JButton();
   JButton openBtn = new JButton();
   JButton saveBtn = new JButton();
   JButton saveAsBtn = new JButton();
   JButton previewBtn = new JButton();
   JSplitPane splitPane = new JSplitPane();

   /**
    * Creates a new WizardBuilder with a blank wizard.
    */
   public WizardBuilder()
   {
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      // Temporary hack to setup the step chooser dialog
      try
      {
         WizardStepFactory.registerWizardStep(
               Class.forName("org.vrjuggler.vrjconfig.wizard.simulator.displays.SimDisplayPane",
                             true,
                             this.getClass().getClassLoader()));
      }
      catch (ClassNotFoundException cnfe)
      {
         cnfe.printStackTrace();
      }

      // ClusterWizard's Temporary hack to setup the step chooser dialog
      try
      {
        WizardStepFactory.registerWizardStep(
               Class.forName("org.vrjuggler.vrjconfig.wizard.cluster.NewClusterFileStep",
                             true,
                             this.getClass().getClassLoader()));
        WizardStepFactory.registerWizardStep(
               Class.forName("org.vrjuggler.vrjconfig.wizard.cluster.SimClusterSequence",
                             true,
                             this.getClass().getClassLoader()));
      }
      catch (ClassNotFoundException cnfe)
      {
         cnfe.printStackTrace();
      }

      // ClusterWizard's Temporary hack to setup the step chooser dialog
      try
      {
        WizardStepFactory.registerWizardStep(
               Class.forName("org.vrjuggler.vrjconfig.wizard.devices.PinchGloveStep",
                             true,
                             this.getClass().getClassLoader()));
      }
      catch (ClassNotFoundException cnfe)
      {
         cnfe.printStackTrace();
      }


      // setup the main toolbar
      ClassLoader loader = this.getClass().getClassLoader();
      newBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/tweek/wizard/builder/images/new24.gif")));
      openBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/tweek/wizard/builder/images/open24.gif")));
      saveBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/tweek/wizard/builder/images/save24.gif")));
      saveAsBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/tweek/wizard/builder/images/saveas24.gif")));
      previewBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/tweek/wizard/builder/images/preview24.gif")));

      // setup the tree toolbar
      addBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/tweek/wizard/builder/images/add16.gif")));
      removeBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/tweek/wizard/builder/images/remove16.gif")));
      moveUpBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/tweek/wizard/builder/images/up16.gif")));
      moveDownBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/tweek/wizard/builder/images/down16.gif")));

      // setup the tree
      model = new WizardBuilderTreeModel(new Wizard());
      wizardTree.setModel(model);
      wizardTree.getSelectionModel().setSelectionMode(
               TreeSelectionModel.SINGLE_TREE_SELECTION);
      wizardTree.setShowsRootHandles(true);
      wizardTree.setCellRenderer(new WizardStepTreeCellRenderer());

      // select the root sequence in the tree by default
      wizardTree.setSelectionPath(new TreePath(model.getPathToRoot(
                                    (WizardStep)model.getRoot())));

      model.add(new WelcomePane(), (WizardSequence)model.getRoot());

      // init modified to true
      modified = false;
   }

   private void jbInit() throws Exception
   {
      splitPane.setBottomComponent(null);
      splitPane.setLeftComponent(westPanel);
      splitPane.setRightComponent(propSheet);
      splitPane.setTopComponent(null);
      addBtn.setEnabled(false);
      addBtn.setToolTipText("Add New Step");
      addBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            addBtn_actionPerformed(e);
         }
      });
      removeBtn.setEnabled(false);
      removeBtn.setToolTipText("Remove Step");
      removeBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            removeStep(wizardTree.getSelectionPath());
         }
      });
      moveUpBtn.setEnabled(false);
      moveUpBtn.setToolTipText("Move Step Up");
      moveUpBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            moveUpBtn_actionPerformed(e);
         }
      });
      moveDownBtn.setEnabled(false);
      moveDownBtn.setToolTipText("Move Step Down");
      moveDownBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            moveDownBtn_actionPerformed(e);
         }
      });
      wizardTree.addTreeSelectionListener(new javax.swing.event.TreeSelectionListener()
      {
         public void valueChanged(TreeSelectionEvent e)
         {
            selectionChanged(e);
         }
      });
      treeToolbar.setFloatable(false);
      westPanel.setBorder(BorderFactory.createRaisedBevelBorder());
      westPanel.setLayout(westLayout);
      this.setLayout(baseLayout);
      mainToolbar.setBorder(BorderFactory.createRaisedBevelBorder());
      mainToolbar.setFloatable(false);
      newBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            newWizard();
         }
      });
      openBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            open();
         }
      });
      saveBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            save();
         }
      });
      saveAsBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            saveAs();
         }
      });
      newBtn.setToolTipText("New Wizard");
      openBtn.setToolTipText("Open Wizard");
      saveBtn.setToolTipText("Save");
      saveAsBtn.setToolTipText("Save As");
      previewBtn.setToolTipText("Preview Wizard");
      previewBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            preview();
         }
      });
      this.add(mainToolbar,  BorderLayout.NORTH);
      mainToolbar.add(newBtn, null);
      mainToolbar.add(openBtn, null);
      mainToolbar.add(saveBtn, null);
      mainToolbar.add(saveAsBtn, null);
      mainToolbar.addSeparator();
      mainToolbar.add(previewBtn, null);
      this.add(splitPane,  BorderLayout.CENTER);
      splitPane.add(propSheet, JSplitPane.RIGHT);
      splitPane.add(westPanel, JSplitPane.LEFT);
      westPanel.add(treeScrollPane, BorderLayout.CENTER);
      westPanel.add(treeToolbar, BorderLayout.NORTH);
      treeScrollPane.getViewport().add(wizardTree, null);
      treeToolbar.add(addBtn, null);
      treeToolbar.add(removeBtn, null);
      treeToolbar.add(moveUpBtn, null);
      treeToolbar.add(moveDownBtn, null);
      treeToolbar.addSeparator();
   }

   void addBtn_actionPerformed(ActionEvent e)
   {
      // Ask the user to choose a step
      WizardStepChooserDialog d = new WizardStepChooserDialog();
      d.setModal(true);
      d.setVisible(true);

      // Get the currently selected item
      WizardStep step = d.getSelectedItem();
      if (step != null)
      {
         addStep(step);
      }
//      if (classname == null)
//      {
//         return;
//      }
//
//      // Try to create the step and add it to the wizard
//      try
//      {
//         Class clazz = Class.forName(classname);
//         WizardStep child = (WizardStep)clazz.newInstance();
//         addStep(child);
//      }
//      catch (Exception ex)
//      {
//         ex.printStackTrace();
//      }
   }

   /**
    * Handles addition of a wizard step.
    */
   void addStep(WizardStep child)
   {
      // Get the selected parent node
      TreePath selPath = wizardTree.getSelectionPath();
      if (selPath == null)
      {
         // no selection to add, ignore the event
         return;
      }
      WizardStep selStep = (WizardStep)selPath.getLastPathComponent();
      if (selStep instanceof WizardSequence)
      {
         // append to end
         model.add(child, (WizardSequence)selStep);
      }
      else
      {
         // insert after selected node in the parent sequence
         WizardSequence parent = (WizardSequence)selStep.getParent();
         int idx = parent.getIndexOf(selStep);
         model.insertInto(child, parent, idx+1);
      }

      // Make sure we can see the new step
      wizardTree.scrollPathToVisible(new TreePath(model.getPathToRoot(child)));
      wizardTree.setSelectionPath(new TreePath(model.getPathToRoot(child)));
   }

   /**
    * Handles removal of a wizard step.
    */
   void removeStep(TreePath path)
   {
      if (path == null)
      {
         // no selection to remove, ignore the event
         return;
      }
      WizardStep child = (WizardStep)path.getLastPathComponent();
      WizardSequence parent = (WizardSequence)child.getParent();
      int idx = parent.getIndexOf(child);
      model.removeFrom(child, parent);

      if (parent.size() > idx)
      {
         // try to select the next child
         wizardTree.setSelectionPath(new TreePath(model.getPathToRoot(
                                                  parent.get(idx))));
      }
      else if (parent.size() > 0)
      {
         // try to select the previous child next
         wizardTree.setSelectionPath(new TreePath(model.getPathToRoot(
                                                  parent.get(idx-1))));
      }
      else
      {
         // finally, select the parent
         wizardTree.setSelectionPath(new TreePath(model.getPathToRoot(parent)));
      }
   }


   /**
    * Handles moving a step up.
    */
   void moveUpBtn_actionPerformed(ActionEvent e)
   {
      TreePath path = wizardTree.getSelectionPath();
      if (path == null)
      {
         // no selection, ignore event
         return;
      }
      WizardStep child = (WizardStep)path.getLastPathComponent();
      WizardSequence parent = (WizardSequence)child.getParent();

      int idx = parent.getIndexOf(child);
      WizardStep prevChild = parent.get(idx-1);
      model.removeFrom(child, parent);
      model.removeFrom(prevChild, parent);

      model.insertInto(child, parent, idx-1);
      model.insertInto(prevChild, parent, idx);

      // keep the selection on the original child
      wizardTree.setSelectionPath(new TreePath(model.getPathToRoot(child)));
   }

   /**
    * Handles moving a step down.
    */
   void moveDownBtn_actionPerformed(ActionEvent e)
   {
      TreePath path = wizardTree.getSelectionPath();
      if (path == null)
      {
         // no selection, ignore event
         return;
      }
      WizardStep child = (WizardStep)path.getLastPathComponent();
      WizardSequence parent = (WizardSequence)child.getParent();

      int idx = parent.getIndexOf(child);
      WizardStep nextChild = parent.get(idx+1);
      model.removeFrom(child, parent);
      model.removeFrom(nextChild, parent);

      model.insertInto(nextChild, parent, idx);
      model.insertInto(child, parent, idx+1);

      // keep the selection on the original child
      wizardTree.setSelectionPath(new TreePath(model.getPathToRoot(child)));
   }

   /**
    * Handles change of selection in the tree.
    */
   void selectionChanged(TreeSelectionEvent e)
   {
      WizardStep step = (WizardStep)wizardTree.getLastSelectedPathComponent();

      //--- Toolbar button handling
      if (step == null)
      {
         // nothing selected
         addBtn.setEnabled(false);
         removeBtn.setEnabled(false);
         moveUpBtn.setEnabled(false);
         moveDownBtn.setEnabled(false);
         return;
      }
      removeBtn.setEnabled(true);
      moveUpBtn.setEnabled(true);
      moveDownBtn.setEnabled(true);
      // sequence selected, we can add
      if (step instanceof WizardSequence)
      {
         addBtn.setEnabled(true);
         // we don't allow you to remove the root
         removeBtn.setEnabled(step != model.getRoot());
      }

      // the root can't move
      if (step == model.getRoot())
      {
         moveUpBtn.setEnabled(false);
         moveDownBtn.setEnabled(false);
      }
      else
      {
         // first step in sequence can't be moved up
         WizardSequence parent = (WizardSequence)step.getParent();
         if (parent.getIndexOf(step) == 0)
         {
            moveUpBtn.setEnabled(false);
         }
         // last step in a sequence can't be moved
         if (parent.getIndexOf(step) == parent.size()-1)
         {
            moveDownBtn.setEnabled(false);
         }
      }

      //--- Property sheet updating
      propSheet.setBean(step);
   }

   /**
    * Handles request for a new wizard.
    */
   void newWizard()
   {
      if (checkForModifiedAndSave())
      {
         model.setWizard(new Wizard());
      }
   }

   /**
    * Handles request to open a wizard. Returns true if the open was successful,
    * false if it failed or the user cancelled it.
    */
   boolean open()
   {
      if (! checkForModifiedAndSave())
      {
         return false;
      }

      JFileChooser chooser = getFileChooser();
      int result = chooser.showOpenDialog(this);
      if (result == JFileChooser.CANCEL_OPTION)
      {
         return false;
      }
      File file = chooser.getSelectedFile();
      String newFilename = file.getAbsolutePath();
      try
      {
         WizardInputStream in = new WizardInputStream(newFilename);
         Wizard wizard = in.readWizard();
         in.close();
         model.setWizard(wizard);
      }
      catch (IOException ioe)
      {
         ioe.printStackTrace();
         JOptionPane.showMessageDialog(this,
                        "Open failed: "+ioe.getMessage(),
                        "Error", JOptionPane.ERROR_MESSAGE);
         return false;
      }
      modified = false;
      filename = newFilename;
      return true;
   }

   /**
    * Handles request to save the wizard. Returns true if the save was
    * successful, false if it failed or the user cancelled it.
    */
   boolean save()
   {
      // Check if we need to ask the user for a filename
      if (filename == null)
      {
         return saveAs();
      }
      Wizard wizard = model.getWizard();

      try
      {
         WizardOutputStream out = new WizardOutputStream(
                                    new BufferedOutputStream(
                                       new FileOutputStream(filename)));
         out.writeWizard(wizard);
         out.close();
      }
      catch (IOException ioe)
      {
         ioe.printStackTrace();
         JOptionPane.showMessageDialog(this,
                        "Save failed: "+ioe.getMessage(),
                        "Error", JOptionPane.ERROR_MESSAGE);
         return false;
      }
      return true;
   }

   /**
    * Asks the user for the filename to save the wizard out to and then saves
    * it out to that file.
    */
   boolean saveAs()
   {
      JFileChooser chooser = getFileChooser();
      int result = chooser.showSaveDialog(this);
      if (result == JFileChooser.CANCEL_OPTION)
      {
         return false;
      }
      File file = chooser.getSelectedFile();
      if (file.exists())
      {
         int overwrite = JOptionPane.showConfirmDialog(this,
                        "File '"+file.getName()+"' already exists. Overwrite?",
                        "Overwrite?", JOptionPane.YES_NO_OPTION,
                        JOptionPane.QUESTION_MESSAGE);
         if (overwrite == JOptionPane.NO_OPTION)
         {
            return false;
         }
      }
      filename = file.getAbsolutePath();
      return save();
   }

   /**
    * Handles a preview action. This will make a copy of the current wizard and
    * let the user walk through it.
    */
   void preview()
   {
      final JDialog dialog = new JDialog();
      dialog.setSize(590, 430);
      dialog.setModal(true);
      dialog.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
      dialog.getContentPane().setLayout(new BorderLayout());

      WizardViewerBean viewer = new WizardViewerBean();
      viewer.setWizard(model.getWizard());
      viewer.setSidebarImage(new ImageIcon(this.getClass().getClassLoader().getResource("org/vrjuggler/tweek/wizard/images/juggler_sidebar.png")));
      viewer.addWizardViewListener(new WizardViewListener()
      {
         public void wizardStarted(WizardViewEvent evt) {}

         public void wizardCancelled(WizardViewEvent evt)
         {
            dialog.setVisible(false);
            dialog.dispose();
         }

         public void wizardFinished(WizardViewEvent evt)
         {
            dialog.setVisible(false);
            dialog.dispose();
         }
      });
      dialog.getContentPane().add(viewer, BorderLayout.CENTER);
      dialog.setVisible(true);
   }

   /**
    * Checks to see if the wizard has been modified and asks the user to save
    * if it is. If the wizard is not modified this returns true. If the wizard
    * is modified and the user cancels or the save failed this returns fails.
    * Otherwise, this returns true.
    */
   boolean checkForModifiedAndSave()
   {
      // Ask the user if they wish to save their changes if things have changed
      if (modified)
      {
         int result = JOptionPane.showConfirmDialog(this,
            "You have unsaved changes. Would your like to save your project?",
            "Save Changes?", JOptionPane.YES_NO_CANCEL_OPTION);

         if (result == JOptionPane.CANCEL_OPTION)
         {
            return false;
         }
         else if (result == JOptionPane.YES_OPTION)
         {
            if (! save())
            {
               return false;
            }
         }
      }
      return true;
   }

   /**
    * Gets the file chooser dialog for this component.
    */
   private JFileChooser getFileChooser()
   {
      if (chooser == null)
      {
         chooser = new JFileChooser();
      }
      return chooser;
   }
}

/**
 * Specialized tree node renderer for wizard steps so that we render only
 * their names.
 */
class WizardStepTreeCellRenderer
   extends DefaultTreeCellRenderer
{
   public Component getTreeCellRendererComponent(
                        JTree tree, Object value, boolean sel,
                        boolean expanded, boolean leaf,
                        int row, boolean hasFocus)
   {
      super.getTreeCellRendererComponent(tree, value, sel, expanded,
                                         leaf, row, hasFocus);
      // Customize the text based on the name
      if (value instanceof WizardStep)
      {
         setText(((WizardStep)value).getName());
      }
      return this;
   }
}
