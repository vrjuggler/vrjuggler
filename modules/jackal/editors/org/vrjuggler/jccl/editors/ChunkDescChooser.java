/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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
package org.vrjuggler.jccl.editors;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Container;
import java.awt.Frame;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.border.Border;
import org.vrjuggler.jccl.config.*;

/**
 * <code>ChunkDescChooser</code> provides a simple mechanism for the user to
 * choose a ChunkDesc.
 */
public class ChunkDescChooser
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
    * Constructs a new <code>ChunkDescChooser</code>.
    */
   public ChunkDescChooser()
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
      setDialogTitle("Choose a Chunk Desc");
      setApproveButtonText("OK");

      // Setup the browse list
      browseList.setModel(browseListModel);
      browseList.setCellRenderer(new ChunkDescCellRenderer());
   }

   public int showDialog(Component parent)
   {
      return showDialog(parent, null);
   }

   /**
    * Pops up a custom ChunkDesc chooser dialog.
    *
    * @param parent        the parent component of the dialog; can be null
    * @param approveText   the text of the approval button
    *
    * @return  the return state of the chooser on popdown:
    *          <ul>
    *           <li>ChunkDescChooser.APPROVE_OPTION</li>
    *           <li>ChunkDescChooser.CANCEL_OPTION</li>
    *           <li>ChunkDescChooser.ERROR_OPTION</li>
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
      dialog.show();

      return returnValue;
   }

   /**
    * Gets the ChunkDescDB that this chooser is using to display the list of
    * allowable ChunkDescs.
    */
   public ChunkDescDB getChunkDescDB()
   {
      return chunkDescDB;
   }

   /**
    * Sets the ChunkDescDB that this chooser should use to display the list of
    * allowable ChunkDescs from.
    */
   public void setChunkDescDB(ChunkDescDB descDB)
   {
      ChunkDescDB old = chunkDescDB;
      chunkDescDB = descDB;
      browseListModel.setChunkDescDB(descDB);
      firePropertyChange("chunkDescDB", old, descDB);
   }

   /**
    * Gets the selected ChunkDesc. This can be set either by the programmer
    * via <code>setSelectedChunkDesc</code> or by a user action, such as
    * either typing the filename into the UI or selecting the file from a list
    * in the UI.
    */
   public ChunkDesc getSelectedChunkDesc()
   {
      return selectedChunkDesc;
   }

   /**
    * Sets the selected ChunkDes to the given desc.
    *
    * @param desc    the selected ChunkDesc
    */
   public void setSelectedChunkDesc(ChunkDesc desc)
   {
      ChunkDesc old = selectedChunkDesc;
      selectedChunkDesc = desc;
      firePropertyChange("selectedChunkDesc", old, desc);
   }

   /**
    * Gets the string that goes in the <code>ChunkDescChooser</code> window's
    * title bar.
    */
   public String getDialogTitle()
   {
      return dialogTitle;
   }

   /**
    * Sets the string that goes in the <code>ChunkDescChooser</code> window's
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
      browseSearchBox = Box.createHorizontalBox();
      browseSearchBoxSpacer = Box.createHorizontalStrut(8);
      buttonBoxGlue = Box.createGlue();
      buttonBoxSpacer = Box.createHorizontalStrut(8);
      this.setLayout(baseLayout);
      browseTab.setLayout(browseTabLayout);
      browseSearchLbl.setText("Chunk Desc Name:");
      approveBtn.setText("OK");
      cancelBtn.setText("Cancel");
      browseTab.setBorder(BorderFactory.createEmptyBorder(5,5,5,5));
      browseList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
      this.add(tabPane,  BorderLayout.CENTER);
      tabPane.add(browseTab,  "Browse");
      tabPane.add(searchTab,  "Search");
      browseTab.add(browseSearchBox,  BorderLayout.NORTH);
      browseSearchBox.add(browseSearchLbl, null);
      browseSearchBox.add(browseSearchBoxSpacer, null);
      browseSearchBox.add(browseSearchTextField, null);
      browseList.addListSelectionListener(new ListSelectionListener()
      {
         public void valueChanged(ListSelectionEvent evt)
         {
            if (! evt.getValueIsAdjusting())
            {
               JList list = (JList)evt.getSource();
               ChunkDesc desc = (ChunkDesc)list.getSelectedValue();
               if (desc != null)
               {
                  setSelectedChunkDesc(desc);
               }
            }
         }
      });
      browseList.addMouseListener(new MouseAdapter()
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
      this.add(buttonBox, BorderLayout.SOUTH);
      buttonBox.add(buttonBoxGlue, null);
      buttonBox.add(approveBtn, null);
      buttonBox.add(buttonBoxSpacer, null);
      buttonBox.add(cancelBtn, null);
      browseTab.add(browseListScrollPane,  BorderLayout.CENTER);
      browseListScrollPane.setViewportView(browseList);
  }

   // JBuilder UI variables
   private JTabbedPane tabPane = new JTabbedPane();
   private BorderLayout baseLayout = new BorderLayout();
   private Box buttonBox;
   private JPanel browseTab = new JPanel();
   private JPanel searchTab = new JPanel();
   private BorderLayout browseTabLayout = new BorderLayout();
   private Box browseSearchBox;
   private JLabel browseSearchLbl = new JLabel();
   private JTextField browseSearchTextField = new JTextField();
   private Component browseSearchBoxSpacer;
   private JButton approveBtn = new JButton();
   private Component buttonBoxGlue;
   private Component buttonBoxSpacer;
   private JButton cancelBtn = new JButton();
   private JScrollPane browseListScrollPane = new JScrollPane();
   private JList browseList = new JList();

   /**
    * The ChunkDescDB containing the allowable ChunkDescs.
    */
   private ChunkDescDB chunkDescDB;

   /**
    * The last ChunkDesc that was selected in the dialog.
    */
   private ChunkDesc selectedChunkDesc;

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
   private ChunkDescListModel browseListModel = new ChunkDescListModel();

   /**
    * A specialized list model for the ChunkDesc list.
    */
   class ChunkDescListModel
      extends AbstractListModel
   {
      /**
       * Creates a new list model with an empty ChunkDescDB.
       */
      public ChunkDescListModel()
      {
         chunkDescDB = new ChunkDescDB();
      }

      public void setChunkDescDB(ChunkDescDB descDB)
      {
         int index1 = chunkDescDB.size() - 1;
         chunkDescDB = descDB;

         // Notify listeners that everything was removed
         if (index1 >= 0)
         {
            fireIntervalRemoved(this, 0, index1);
         }
         // Notify listeners of the new chunk descs
         if (chunkDescDB.size() > 0)
         {
            fireIntervalAdded(this, 0, chunkDescDB.size()-1);
         }
      }

      public ChunkDescDB getChunkDescDB()
      {
         return chunkDescDB;
      }

      public Object getElementAt(int idx)
      {
         return chunkDescDB.get(idx);
      }

      public int getSize()
      {
         return chunkDescDB.size();
      }

      private ChunkDescDB chunkDescDB;
   }

   /**
    * A specialized list cell renderer for ChunkDescs.
    */
   class ChunkDescCellRenderer
      extends JLabel
      implements ListCellRenderer
   {
      public ChunkDescCellRenderer()
      {
         setOpaque(true);
      }

      public Component getListCellRendererComponent(JList list,
                                                    Object value,
                                                    int index,
                                                    boolean selected,
                                                    boolean focused)
      {
         if (selected)
         {
            setBackground(list.getSelectionBackground());
            setForeground(list.getSelectionForeground());
         }
         else
         {
            setBackground(list.getBackground());
            setForeground(list.getForeground());
         }

         if (value instanceof ChunkDesc)
         {
            setText(((ChunkDesc)value).getName());
         }
         else
         {
            setText(value.toString());
         }
         return this;
      }
   }
}
