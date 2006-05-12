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
package org.vrjuggler.jccl.config;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Container;
import java.awt.Frame;
import java.awt.event.*;
import java.util.ArrayList;
import java.util.List;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.border.Border;

/**
 * <code>ResourceChooser</code> provides a simple mechanism for the user to
 * choose a resource.
 *
 * @see ConfigBroker
 */
public class ResourceChooser
   extends JPanel
{
   /**
    * Return value if approve is chosen.
    */
   public static final int APPROVE_OPTION = 0;

   /**
    * Return value if cancel is chosen.
    */
   public static final int CANCEL_OPTION = 1;

   /**
    * Return value if an error occured.
    */
   public static final int ERROR_OPTION = -1;

   /**
    * Constructs a new <code>ResourceChooser</code>.
    */
   public ResourceChooser()
   {
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }


      // Setup the default UI texts
      setDialogTitle("Choose a Resource");
      setApproveButtonText("OK");

      // Setup the browse list
      resourceList.setModel(resourceListModel);
   }

   public int showDialog(Component parent)
   {
      return showDialog(parent, null);
   }

   /**
    * Pops up a custom resource chooser dialog.
    *
    * @param parent        the parent component of the dialog; can be null
    * @param approveText   the text of the approval button
    *
    * @return  the return state of the chooser on popdown:
    *          <ul>
    *           <li>ResourceChooser.APPROVE_OPTION</li>
    *           <li>ResourceChooser.CANCEL_OPTION</li>
    *           <li>ResourceChooser.ERROR_OPTION</li>
    *          </ul>
    */
   public int showDialog(Component parent, String approveText)
   {
      if (approveText != null)
      {
         setApproveButtonText(approveText);
      }

      Frame frame = parent instanceof Frame ?
                  (Frame)parent :
                  (Frame)SwingUtilities.getAncestorOfClass(Frame.class, parent);

      // Create a dialog to hold this component
      dialog = new JDialog(frame, getDialogTitle(), true);
      dialog.addWindowListener(new WindowAdapter()
      {
         public void windowClosing(WindowEvent evt)
         {
            cancelSelection();
         }
      });
      Container contentPane = dialog.getContentPane();
      contentPane.setLayout(new BorderLayout());
      contentPane.add(this, BorderLayout.CENTER);

      // Size and position the dialog
      dialog.pack();
      dialog.setLocationRelativeTo(parent);
      dialog.setVisible(true);

      return returnValue;
   }

   /**
    * Gets the resource list that this chooser is displaying to the user.
    */
   public List getResources()
   {
      return resourceListModel.getResources();
   }

   /**
    * Sets the resource list that this chooser should display to the user.
    */
   public void setResources(List resources)
   {
      List old = getResources();
      resourceListModel.setResources(resources);
      firePropertyChange("resources", old, resources);
   }

   /**
    * Gets the selected resource. This can be set either by the programmer
    * via <code>setSelectedResource</code> or by a user action, such as
    * selecting it from a list in the UI.
    */
   public String getSelectedResource()
   {
      return selectedResource;
   }

   /**
    * Sets the selected resource to the given resource.
    *
    * @param resource      the selected resource
    */
   public void setSelectedResource(String resource)
   {
      String old = selectedResource;
      selectedResource = resource;
      approveBtn.setEnabled(selectedResource != null);
      firePropertyChange("selectedResource", old, selectedResource);
   }

   /**
    * Gets the string that goes in the <code>ResourceChooser</code> window's
    * title bar.
    */
   public String getDialogTitle()
   {
      return dialogTitle;
   }

   /**
    * Sets the string that goes in the <code>ResourceChooser</code> window's
    * title bar.
    */
   public void setDialogTitle(String title)
   {
      String old = dialogTitle;
      dialogTitle = title;
      firePropertyChange("dialogTitle", old, title);
   }

   /**
    * Gets the text used in the approval button.
    */
   public String getApproveButtonText()
   {
      return approveButtonText;
   }

   /**
    * Sets the text used in the approval button.
    */
   private void setApproveButtonText(String approveText)
   {
      String old = approveButtonText;
      approveButtonText = approveText;
      approveBtn.setText(approveButtonText);
      firePropertyChange("approveButtonText", old, approveText);
   }

   /**
    * Called when the approve button has been depressed. This can also be
    * called by the programmer.
    */
   public void approveSelection()
   {
      returnValue = APPROVE_OPTION;
      if (dialog != null)
      {
         dialog.setVisible(false);
      }
   }

   /**
    * Called when the cancel button has been depressed. This can also be
    * called by the programmer.
    */
   public void cancelSelection()
   {
      returnValue = CANCEL_OPTION;
      if (dialog != null)
      {
         dialog.setVisible(false);
      }
   }

   /**
    * JBuilder GUI initialization.
    */
   private void jbInit()
       throws Exception
   {
      buttonBox = Box.createHorizontalBox();
      buttonBoxGlue = Box.createGlue();
      buttonBoxSpacer = Box.createHorizontalStrut(8);
      this.setLayout(baseLayout);
      approveBtn.setText("OK");
      approveBtn.setEnabled(false);
      cancelBtn.setText("Cancel");
      resourceList.addListSelectionListener(new ListSelectionListener()
      {
         public void valueChanged(ListSelectionEvent evt)
         {
            if (! evt.getValueIsAdjusting())
            {
               JList list = (JList)evt.getSource();
               String resource = (String)list.getSelectedValue();
               setSelectedResource(resource);
            }
         }
      });
      resourceList.addMouseListener(new MouseAdapter()
      {
         public void mouseClicked(MouseEvent evt)
         {
            // Double-click on a list item and that item is approved
            if (evt.getClickCount() == 2)
            {
               JList list = (JList)evt.getSource();
               int idx = list.locationToIndex(evt.getPoint());
               if (idx != -1)
               {
                  approveSelection();
               }
            }
         }
      });
      approveBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            approveSelection();
         }
      });
      cancelBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            cancelSelection();
         }
      });
      this.add(resourceListScrollPane, BorderLayout.CENTER);
      resourceListScrollPane.setViewportView(resourceList);
      this.add(buttonBox, BorderLayout.SOUTH);
      buttonBox.add(buttonBoxGlue, null);
      buttonBox.add(approveBtn, null);
      buttonBox.add(buttonBoxSpacer, null);
      buttonBox.add(cancelBtn, null);
  }

   // JBuilder UI variables
   private BorderLayout baseLayout = new BorderLayout();
   private Box buttonBox;
   private JButton approveBtn = new JButton();
   private Component buttonBoxGlue;
   private Component buttonBoxSpacer;
   private JButton cancelBtn = new JButton();
   private JScrollPane resourceListScrollPane = new JScrollPane();
   private JList resourceList = new JList();

   /**
    * The last resource that was selected in the dialog.
    */
   private String selectedResource;

   /**
    * The return value from the last time the dialog was shown.
    */
   private int returnValue = ERROR_OPTION;

   /**
    * The title of the dialog.
    */
   private String dialogTitle;

   /**
    * The text of the approve button.
    */
   private String approveButtonText;

   /**
    * The dialog that is currently showing this component.
    */
   private transient JDialog dialog;

   /**
    * The list model for the browse list.
    */
   private ResourceListModel resourceListModel = new ResourceListModel();

   /**
    * A specialized list model for the resource list.
    */
   class ResourceListModel
      extends AbstractListModel
   {
      /**
       * Creates a new list model with an empty resource list.
       */
      public ResourceListModel()
      {
         resources = new ArrayList();
      }

      public void setResources(List resources)
      {
         int index1 = this.resources.size() - 1;
         this.resources = resources;

         // Notify listeners that everything was removed
         if (index1 >= 0)
         {
            fireIntervalRemoved(this, 0, index1);
         }
         // Notify listeners of the new config definitions
         if (this.resources.size() > 0)
         {
            fireIntervalAdded(this, 0, this.resources.size()-1);
         }
      }

      public List getResources()
      {
         return resources;
      }

      public Object getElementAt(int idx)
      {
         return resources.get(idx);
      }

      public int getSize()
      {
         return resources.size();
      }

      private List resources;
   }
}
