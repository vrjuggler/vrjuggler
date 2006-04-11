/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
import java.util.*;
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

      // Make sure the focus and selected chunk desc get updated appropriately
      // for the currently visible tab when the tab selection changes.
      browseTab.addComponentListener(new ComponentAdapter()
      {
         public void componentShown(ComponentEvent evt)
         {
            browseSearchTextField.requestFocus();
            setSelectedChunkDesc((ChunkDesc)browseList.getSelectedValue());
         }
      });
      searchTab.addComponentListener(new ComponentAdapter()
      {
         public void componentShown(ComponentEvent evt)
         {
            searchSearchTextField.requestFocus();
            setSelectedChunkDesc((ChunkDesc)searchList.getSelectedValue());
         }
      });

      // Setup the default UI texts
      setDialogTitle("Choose a Chunk Desc");
      setApproveButtonText("OK");

      // Setup the browse list
      browseList.setModel(browseListModel);
      browseList.setCellRenderer(new ChunkDescCellRenderer());

      // Setup the search results list
      searchList.setModel(searchListModel);
      searchList.setCellRenderer(new ChunkDescCellRenderer());
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
    * Gets the list of ChunkDescs that this chooser allows the user to choose
    * from.
    */
   public List getDescs()
   {
      return browseListModel.getDescs();
   }

   /**
    * Sets the list of ChunkDescs that this chooser allows the user to choose
    * from.
    */
   public void setDescs(List descs)
   {
      List old = browseListModel.getDescs();
      browseListModel.setDescs(descs);
      firePropertyChange("descs", old, descs);
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
      approveBtn.setEnabled(selectedChunkDesc != null);
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
    * This method takes the text in the browse search text field and tries to
    * select the first ChunkDesc in the browseList whose name begins with that
    * string.
    */
   private void searchBrowseList()
   {
      String search_str = browseSearchTextField.getText();
      for (int i=0; i<browseListModel.getSize(); ++i)
      {
         ChunkDesc desc = (ChunkDesc)browseListModel.getElementAt(i);
         if (desc.getName().toLowerCase().startsWith(search_str.toLowerCase()))
         {
            browseList.setSelectedIndex(i);
            browseList.ensureIndexIsVisible(i);
            return;
         }
      }

      // Looks like we didn't find a match, clear the selection
      browseList.getSelectionModel().clearSelection();
      browseList.ensureIndexIsVisible(0);
   }

   /**
    * This method takes the text in the text field in the search tab and does a
    * simple pattern match of all the ChunkDescs available to see if any of
    * their names have the search string. Those that do match are added to the
    * search results list.
    */
   private void updateSearchResults()
   {
      List results = new ArrayList();

      final String search_str = searchSearchTextField.getText();
      for (int i=0; i<browseListModel.getSize(); ++i)
      {
         // Yes, I do want to get the desc from the browse list model
         ChunkDesc desc = (ChunkDesc)browseListModel.getElementAt(i);
         if (desc.getName().toLowerCase().indexOf(search_str.toLowerCase()) != -1)
         {
            results.add(desc);
         }
      }

      Collections.sort(results, new Comparator()
      {
         public int compare(Object o1, Object o2)
         {
            ChunkDesc cd1 = (ChunkDesc)o1;
            ChunkDesc cd2 = (ChunkDesc)o2;

            return cd1.getName().compareTo(cd2.getName());
         }
      });

      searchListModel.setResults(results);

      // Make sure the first result is selected
      if (searchListModel.getSize() > 0)
      {
         searchList.setSelectedIndex(0);
         searchList.ensureIndexIsVisible(0);
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
      searchSearchBox = Box.createHorizontalBox();
      searchSearchBoxSpacer = Box.createHorizontalStrut(8);
      this.setLayout(baseLayout);
      browseTab.setLayout(browseTabLayout);
      browseSearchLbl.setText("Chunk Desc Name:");
      approveBtn.setText("OK");
      approveBtn.setEnabled(false);
      cancelBtn.setText("Cancel");
      browseTab.setBorder(BorderFactory.createEmptyBorder(5,5,5,5));
      browseList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
      searchTab.setBorder(BorderFactory.createEmptyBorder(5,5,5,5));
      searchSearchLbl.setText("Search for: ");
      searchList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
      browseSearchTextField.getDocument().addDocumentListener(new DocumentListener()
      {
         public void changedUpdate(DocumentEvent evt)
         {
            searchBrowseList();
         }

         public void insertUpdate(DocumentEvent evt)
         {
            searchBrowseList();
         }

         public void removeUpdate(DocumentEvent evt)
         {
            searchBrowseList();
         }
      });
      browseSearchTextField.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            // If an item in the browse list is selected, approve it
            if (browseList.getSelectedIndex() != -1)
            {
               approveSelection();
            }
         }
      });
      browseList.addListSelectionListener(new ListSelectionListener()
      {
         public void valueChanged(ListSelectionEvent evt)
         {
            if (! evt.getValueIsAdjusting())
            {
               JList list = (JList)evt.getSource();
               ChunkDesc desc = (ChunkDesc)list.getSelectedValue();
               setSelectedChunkDesc(desc);
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
      searchSearchTextField.getDocument().addDocumentListener(new DocumentListener()
      {
         public void changedUpdate(DocumentEvent evt)
         {
            updateSearchResults();
         }

         public void insertUpdate(DocumentEvent evt)
         {
            updateSearchResults();
         }

         public void removeUpdate(DocumentEvent evt)
         {
            updateSearchResults();
         }
      });
      searchSearchTextField.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            // If an item in the browse list is selected, approve it
            if (searchList.getSelectedIndex() != -1)
            {
               approveSelection();
            }
         }
      });
      searchList.addListSelectionListener(new ListSelectionListener()
      {
         public void valueChanged(ListSelectionEvent evt)
         {
            if (! evt.getValueIsAdjusting())
            {
               JList list = (JList)evt.getSource();
               ChunkDesc desc = (ChunkDesc)list.getSelectedValue();
               setSelectedChunkDesc(desc);
            }
         }
      });
      searchList.addMouseListener(new MouseAdapter()
      {
         public void mouseClicked(MouseEvent evt)
         {
            // Double-clicking a list item approves it
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
      searchTab.setLayout(searchTabLayout);
      this.add(tabPane,  BorderLayout.CENTER);
      tabPane.add(browseTab,  "Browse");
      tabPane.add(searchTab,  "Search");
      browseTab.add(browseSearchBox,  BorderLayout.NORTH);
      browseSearchBox.add(browseSearchLbl, null);
      browseSearchBox.add(browseSearchBoxSpacer, null);
      browseSearchBox.add(browseSearchTextField, null);
      this.add(buttonBox, BorderLayout.SOUTH);
      buttonBox.add(buttonBoxGlue, null);
      buttonBox.add(approveBtn, null);
      buttonBox.add(buttonBoxSpacer, null);
      buttonBox.add(cancelBtn, null);
      browseTab.add(browseListScrollPane,  BorderLayout.CENTER);
      browseListScrollPane.setViewportView(browseList);
      searchTab.add(searchSearchBox, BorderLayout.NORTH);
      searchSearchBox.add(searchSearchLbl, null);
      searchSearchBox.add(searchSearchBoxSpacer, null);
      searchSearchBox.add(searchSearchTextField, null);
      searchTab.add(searchListScrollPane,  BorderLayout.CENTER);
      searchListScrollPane.setViewportView(searchList);
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
   private BorderLayout searchTabLayout = new BorderLayout();
   private Box searchSearchBox;
   private JLabel searchSearchLbl = new JLabel();
   private Component searchSearchBoxSpacer;
   private JTextField searchSearchTextField = new JTextField();
   private JScrollPane searchListScrollPane = new JScrollPane();
   private JList searchList = new JList();

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
    * The list model for the search results list.
    */
   private SearchResultsListModel searchListModel = new SearchResultsListModel();

   /**
    * A specialized list model for the ChunkDesc list.
    */
   class ChunkDescListModel
      extends AbstractListModel
   {
      /**
       * Creates a new list model with an empty list of ChunkDescs.
       */
      public ChunkDescListModel()
      {
         descs = new ArrayList();
      }

      public void setDescs(List descs)
      {
         int index1 = this.descs.size() - 1;
         this.descs = descs;

         // Notify listeners that everything was removed
         if (index1 >= 0)
         {
            fireIntervalRemoved(this, 0, index1);
         }
         // Notify listeners of the new chunk descs
         if (this.descs.size() > 0)
         {
            fireIntervalAdded(this, 0, this.descs.size()-1);
         }
      }

      public List getDescs()
      {
         return descs;
      }

      public Object getElementAt(int idx)
      {
         return descs.get(idx);
      }

      public int getSize()
      {
         return descs.size();
      }

      private List descs;
   }

   /**
    * A specialized list model for the search results list.
    */
   class SearchResultsListModel
      extends AbstractListModel
   {
      /**
       * Creates a new list model with an empty results list.
       */
      public SearchResultsListModel()
      {
         results = new ArrayList();
      }

      public void setResults(List results)
      {
         int index1 = this.results.size() - 1;
         this.results = results;

         // Notify listeners that everything was removed
         if (index1 >= 0)
         {
            fireIntervalRemoved(this, 0, index1);
         }
         // Notify listeners of the new results
         if (this.results.size() > 0)
         {
            fireIntervalAdded(this, 0, this.results.size()-1);
         }
      }

      public List getResults()
      {
         return results;
      }

      public Object getElementAt(int idx)
      {
         return results.get(idx);
      }

      public int getSize()
      {
         return results.size();
      }

      private List results;
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
