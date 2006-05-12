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
package org.vrjuggler.tweek.wizard.builder;

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.*;
import java.util.jar.*;
import javax.swing.*;
import org.vrjuggler.tweek.beans.loader.BeanJarClassLoader;
import org.vrjuggler.tweek.wizard.*;

/**
 * Dialog that lets the user choose a WizardStep.
 */
public class WizardStepChooserDialog
   extends JDialog
{
   JPanel basePnl = new JPanel();
   BorderLayout baseLayout = new BorderLayout();
   JPanel msgPanel = new JPanel();
   BorderLayout msgPanelLayout = new BorderLayout();
   JTextArea msgText = new JTextArea();
   JTabbedPane tabPane = new JTabbedPane();
   JScrollPane paneScrollPane = new JScrollPane();
   JList panesList = new JList();
   JScrollPane sequenceScrollPane = new JScrollPane();
   JList sequencesList = new JList();
   JPanel buttonsPanel = new JPanel();
   BoxLayout buttonsLayout = new BoxLayout(buttonsPanel, BoxLayout.X_AXIS);
   Component buttonsGlue;
   Component buttonsStrut;
   JButton newStepBtn = new JButton();
   JButton okBtn = new JButton();
   JButton cancelBtn = new JButton();

   public WizardStepChooserDialog()
   {
      this(null);
   }

   public WizardStepChooserDialog(Frame owner)
   {
      this(owner, "Wizard Step Chooser", true);
   }

   public WizardStepChooserDialog(Frame owner, String title, boolean modal)
   {
      super(owner, title, modal);

      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      // Set the help text
      msgText.setText("Please choose a Wizard Step to add to your Wizard.");

      // Setup our lists
      panesModel = new DefaultListModel();
      panesList.setModel(panesModel);
      panesList.setCellRenderer(new WizardStepListCellRenderer());
      panesList.addMouseListener(new MouseAdapter()
      {
         public void mouseClicked(MouseEvent evt)
         {
            // Double-click on a list item, approves that item
            if (evt.getClickCount() == 2)
            {
               JList list = (JList)evt.getSource();
               int idx = list.locationToIndex(evt.getPoint());
               if (idx != -1)
               {
                  okPressed();
               }
            }
         }
      });
      sequencesModel = new DefaultListModel();
      sequencesList.setModel(sequencesModel);
      sequencesList.addMouseListener(new MouseAdapter()
      {
         public void mouseClicked(MouseEvent evt)
         {
            // Double-click on a list item, approves that item
            if (evt.getClickCount() == 2)
            {
               JList list = (JList)evt.getSource();
               int idx = list.locationToIndex(evt.getPoint());
               if (idx != -1)
               {
                  okPressed();
               }
            }
         }
      });
      sequencesList.setCellRenderer(new WizardStepListCellRenderer());

      refreshLists();
   }

   private void jbInit() throws Exception
   {
      buttonsGlue = Box.createGlue();
      buttonsStrut = Box.createHorizontalStrut(8);
      setSize(400, 300);
      basePnl.setLayout(baseLayout);
      msgPanel.setLayout(msgPanelLayout);
      msgText.setBackground(UIManager.getColor("Label.background"));
      msgText.setFont(new java.awt.Font("Dialog", 1, 12));
      msgText.setForeground(UIManager.getColor("Label.foreground"));
      msgText.setBorder(null);
      msgText.setOpaque(false);
      msgText.setEditable(false);
      msgText.setLineWrap(true);
      buttonsPanel.setLayout(buttonsLayout);
      newStepBtn.setText("Add New");
      newStepBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            addNewStep();
         }
      });
      okBtn.setText("OK");
      okBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            okPressed();
         }
      });
      cancelBtn.setText("Cancel");
      cancelBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            cancelPressed();
         }
      });
      buttonsPanel.setBorder(BorderFactory.createEtchedBorder());
      msgPanel.setBorder(BorderFactory.createEtchedBorder());
      buttonsPanel.add(newStepBtn, null);
      buttonsPanel.add(buttonsGlue, null);
      buttonsPanel.add(cancelBtn, null);
      buttonsPanel.add(buttonsStrut, null);
      this.getContentPane().add(basePnl, BorderLayout.CENTER);
      basePnl.add(msgPanel, BorderLayout.NORTH);
      msgPanel.add(msgText, BorderLayout.NORTH);
      basePnl.add(tabPane,  BorderLayout.CENTER);
      tabPane.add(paneScrollPane,    "Panes");
      paneScrollPane.getViewport().add(panesList, null);
      tabPane.add(sequenceScrollPane,  "Sequences");
      sequenceScrollPane.getViewport().add(sequencesList, null);
      basePnl.add(buttonsPanel,  BorderLayout.SOUTH);
      buttonsPanel.add(okBtn, null);
      tabPane.setSelectedIndex(0);
   }

   public WizardStep getSelectedItem()
   {
      return selectedItem;
   }

   /**
    * Handle cancel action.
    */
   void cancelPressed()
   {
      selectedItem = null;
      this.setVisible(false);
   }

   /**
    * Handle ok action.
    */
   void okPressed()
   {
      int tabIdx = tabPane.getSelectedIndex();
      // get the selected item
      if (tabIdx == 0)
      {
         selectedItem = (WizardStep)panesList.getSelectedValue();
      }
      else
      {
         selectedItem = (WizardStep)sequencesList.getSelectedValue();
      }

      this.setVisible(false);
   }

   /**
    * Adds new wizard steps to the builder by asking the user to specify JAR
    * files where the steps can be found.
    */
   void addNewStep()
   {
      try
      {
         // Ask the user for the location of the JAR to add to the search path.
         JFileChooser chooser = new JFileChooser();
         int result = chooser.showOpenDialog(this);
         if (result == JFileChooser.APPROVE_OPTION)
         {
            File file = chooser.getSelectedFile();
            JarFile jar = new JarFile(file);
            WizardStepFinder finder = new WizardStepFinder();
            java.util.List steps = finder.find(jar);
            if (steps.size() == 0)
            {
               JOptionPane.showMessageDialog(this,
                                             "There are no wizard steps in " + file.getName(),
                                             "No Wizard Steps",
                                             JOptionPane.INFORMATION_MESSAGE);
            }
            else
            {
               JOptionPane.showMessageDialog(this,
                                             "Found " + steps.size() + " Wizard Steps.",
                                             "Success",
                                             JOptionPane.INFORMATION_MESSAGE);
               // Add the jar file to the classpath.
               BeanJarClassLoader loader = BeanJarClassLoader.instance();
               loader.addJarFile(jar);

               // Add the wizard steps to the factory.
               for (Iterator itr = steps.iterator(); itr.hasNext(); )
               {
                  String classname = (String) itr.next();
                  System.out.println("Found: " + classname);
                  try
                  {
                     Class cls = loader.loadClass(classname);
                     WizardStepFactory.registerWizardStep(cls);
                  }
                  catch (ClassNotFoundException cnfe)
                  {
                     cnfe.printStackTrace();
                     JOptionPane.showMessageDialog(this,
                                                   "Error: Could not load Wizard Step. "+cnfe.getMessage(),
                                                   "Error",
                                                   JOptionPane.ERROR_MESSAGE);
                  }
               }
               refreshLists();
            }
         }
      }
      catch (IOException ioe)
      {
         ioe.printStackTrace();
         JOptionPane.showMessageDialog(this,
                                       "Error: "+ioe.getMessage(),
                                       "Error",
                                       JOptionPane.ERROR_MESSAGE);
      }
   }

   /**
    * Refreshes the contents of the wizard step lists.
    */
   private void refreshLists()
   {
      panesModel.clear();

      // Add in the all the registered panes
      for (int i=0; i<WizardStepFactory.getNumWizardSteps(); ++i)
      {
         panesModel.addElement(WizardStepFactory.getWizardStep(i));
      }
   }

   /**
    * The step that was chosen through this dialog.
    */
   private WizardStep selectedItem;

   private DefaultListModel panesModel, sequencesModel;
}

class WizardStepListCellRenderer
   extends DefaultListCellRenderer
{
   public Component getListCellRendererComponent(JList list, Object value,
                                                 int index, boolean sel,
                                                 boolean focus)
   {
      super.getListCellRendererComponent(list, value, index, sel, focus);
//      String cn = (String)value;
//      String cn = step.getName();
//      setText(cn.substring(cn.lastIndexOf('.')+1));

      WizardStep step = (WizardStep)value;
      setText(step.getName());
      return this;
   }
}
