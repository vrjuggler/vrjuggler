/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
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
 * The ConfigDefinitionChooser provides a simple mechanism for the user to
 * choose a configuration definition from a list.
 */
public class ConfigDefinitionChooser
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
    * Constructs a new ConfigDefinitionChooser.
    */
   public ConfigDefinitionChooser()
   {
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      // Make sure the focus and selected definition get updated appropriately
      // for the currently visible tab when the tab selection changes.
      browseTab.addComponentListener(new ComponentAdapter()
      {
         public void componentShown(ComponentEvent evt)
         {
            browseSearchTextField.requestFocus();
            setSelectedDefinition((ConfigDefinition)browseList.getSelectedValue());
         }
      });
      searchTab.addComponentListener(new ComponentAdapter()
      {
         public void componentShown(ComponentEvent evt)
         {
            searchSearchTextField.requestFocus();
            setSelectedDefinition((ConfigDefinition)searchList.getSelectedValue());
         }
      });

      // Setup the default UI texts
      setDialogTitle("Choose a Type");
      setApproveButtonText("OK");

      // Setup the browse list
      browseList.setModel(mBrowseListModel);
      browseList.setCellRenderer(new ConfigDefinitionCellRenderer());

      // Setup the search results list
      searchList.setModel(mSearchListModel);
      searchList.setCellRenderer(new ConfigDefinitionCellRenderer());
   }

   /**
    * Pops up a custom ConfigDefinition chooser dialog.
    *
    * @param parent        the parent component of the dialog; can be null
    *
    * @see #showDialog(Component,String)
    */
   public int showDialog(Component parent)
   {
      return showDialog(parent, null);
   }

   /**
    * Pops up a custom ConfigDefinition chooser dialog.
    *
    * @param parent        the parent component of the dialog; can be null
    * @param approveText   the text of the approval button
    *
    * @return  the return state of the chooser on popdown:
    *          <ul>
    *           <li>ConfigDefinitionChooser.APPROVE_OPTION</li>
    *           <li>ConfigDefinitionChooser.CANCEL_OPTION</li>
    *           <li>ConfigDefinitionChooser.ERROR_OPTION</li>
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
      mDialog = new JDialog(frame, getDialogTitle(), true);
      mDialog.addWindowListener(new WindowAdapter()
      {
         public void windowClosing(WindowEvent evt)
         {
            cancelSelection();
         }
      });

      mDialog.getRootPane().setDefaultButton(approveBtn);
         
      Container contentPane = mDialog.getContentPane();
      contentPane.setLayout(new BorderLayout());
      contentPane.add(this, BorderLayout.CENTER);

      // Size and position the dialog
      mDialog.pack();
      mDialog.setLocationRelativeTo(parent);
      mDialog.setVisible(true);

      return mReturnValue;
   }

   /**
    * Gets the list of configuration definitions that this chooser allows the
    * user to choose from.
    */
   public List getDefinitions()
   {
      return mBrowseListModel.getDefinitions();
   }

   /**
    * Sets the list of configuration definitions that this chooser allows the
    * user to choose from.
    */
   public void setDefinitions(List definitions)
   {
      List old = mBrowseListModel.getDefinitions();
      mBrowseListModel.setDefinitions(definitions);
      firePropertyChange("definitions", old, definitions);
   }

   /**
    * Gets the selected ConfigDefinition. This can be set either by the
    * programmer via setSelectedDefinition or by a user action, such as either
    * typing the filename into the UI or selecting the file from a list in the
    * UI.
    */
   public ConfigDefinition getSelectedDefinition()
   {
      return mSelectedDefinition;
   }

   /**
    * Sets the selected configuration definition to the given definition.
    *
    * @param def     the selected definition
    */
   public void setSelectedDefinition(ConfigDefinition def)
   {
      ConfigDefinition old = mSelectedDefinition;
      mSelectedDefinition = def;
      approveBtn.setEnabled(mSelectedDefinition != null);
      firePropertyChange("selectedDefinition", old, def);
   }

   /**
    * Gets the string that goes in theConfigDefinitionChooser window's
    * title bar.
    */
   public String getDialogTitle()
   {
      return mDialogTitle;
   }

   /**
    * Sets the string that goes in the ConfigDefinitionChooser window's
    * title bar.
    */
   public void setDialogTitle(String title)
   {
      String old = mDialogTitle;
      mDialogTitle = title;
      firePropertyChange("dialogTitle", old, title);
   }

   /**
    * Gets the text used in the approval button.
    */
   public String getApproveButtonText()
   {
      return mApproveButtonText;
   }

   /**
    * Sets the text used in the approval button.
    */
   private void setApproveButtonText(String approveText)
   {
      String old = mApproveButtonText;
      mApproveButtonText = approveText;
      approveBtn.setText(mApproveButtonText);
      firePropertyChange("approveButtonText", old, approveText);
   }

   /**
    * Called when the approve button has been depressed. This can also be
    * called by the programmer.
    */
   public void approveSelection()
   {
      mReturnValue = APPROVE_OPTION;
      if (mDialog != null)
      {
         mDialog.setVisible(false);
      }
   }

   /**
    * Called when the cancel button has been depressed. This can also be
    * called by the programmer.
    */
   public void cancelSelection()
   {
      mReturnValue = CANCEL_OPTION;
      if (mDialog != null)
      {
         mDialog.setVisible(false);
      }
   }

   /**
    * This method takes the text in the browse search text field and tries to
    * select the first definition in the browseList whose name begins with that
    * string.
    */
   private void searchBrowseList()
   {
      String search_str = browseSearchTextField.getText();
      for (int i=0; i<mBrowseListModel.getSize(); ++i)
      {
         ConfigDefinition def = (ConfigDefinition)mBrowseListModel.getElementAt(i);
         if (def.getName().toLowerCase().startsWith(search_str.toLowerCase()))
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
    * simple pattern match of all the definitions available to see if any of
    * their names have the search string. Those that do match are added to the
    * search results list.
    */
   private void updateSearchResults()
   {
      List results = new ArrayList();

      String search_str = searchSearchTextField.getText();
      for (int i=0; i<mBrowseListModel.getSize(); ++i)
      {
         // Yes, I do want to get the definition from the browse list model
         ConfigDefinition def = (ConfigDefinition)mBrowseListModel.getElementAt(i);
         if (def.getName().toLowerCase().indexOf(search_str.toLowerCase()) != -1)
         {
            results.add(def);
         }
      }

      Collections.sort(results, new Comparator()
      {
         public int compare(Object o1, Object o2)
         {
            ConfigDefinition cd1 = (ConfigDefinition)o1;
            ConfigDefinition cd2 = (ConfigDefinition)o2;

            return cd1.getName().compareTo(cd2.getName());
         }
      });

      mSearchListModel.setResults(results);

      // Make sure the first result is selected
      if (mSearchListModel.getSize() > 0)
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
      browseSearchLbl.setText("Config Element Type:");
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
      browseTab.addMouseWheelListener(mBrowseMouseListener);
      browseList.addMouseWheelListener(mBrowseMouseListener);
      browseSearchTextField.addKeyListener(mBrowseKeyAdapter);

      browseList.addListSelectionListener(new ListSelectionListener()
      {
         public void valueChanged(ListSelectionEvent evt)
         {
            if (! evt.getValueIsAdjusting())
            {
               JList list = (JList)evt.getSource();
               ConfigDefinition def = (ConfigDefinition)list.getSelectedValue();
               setSelectedDefinition(def);
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
               ConfigDefinition def = (ConfigDefinition)list.getSelectedValue();
               setSelectedDefinition(def);
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
      searchTab.addMouseWheelListener(mSearchMouseListener);
      searchList.addMouseWheelListener(mSearchMouseListener);
      searchSearchTextField.addKeyListener(mSearchKeyAdapter);
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
      browseListScrollPane.setWheelScrollingEnabled(false);
      searchTab.add(searchSearchBox, BorderLayout.NORTH);
      searchSearchBox.add(searchSearchLbl, null);
      searchSearchBox.add(searchSearchBoxSpacer, null);
      searchSearchBox.add(searchSearchTextField, null);
      searchTab.add(searchListScrollPane,  BorderLayout.CENTER);
      searchListScrollPane.setViewportView(searchList);
      searchListScrollPane.setWheelScrollingEnabled(false);
  }

   private KeyAdapter mBrowseKeyAdapter = new KeyAdapter()
      {
         public void keyPressed(KeyEvent e)
         {
            int new_index = -1;
            int list_size = browseList.getModel().getSize();
            if (KeyEvent.VK_DOWN == e.getKeyCode())
            {
               new_index = browseList.getSelectedIndex() + 1;
            }
            else if(KeyEvent.VK_UP == e.getKeyCode())
            {
               new_index = browseList.getSelectedIndex() - 1;
            }
            else if(KeyEvent.VK_PAGE_DOWN == e.getKeyCode())
            {
               int offset = browseList.getLastVisibleIndex() - browseList.getFirstVisibleIndex();
               new_index = browseList.getSelectedIndex() + offset;
               if (new_index >= list_size)
               {
                  new_index = list_size - 1;
               }
            }
            else if(KeyEvent.VK_PAGE_UP == e.getKeyCode())
            {
               int offset = browseList.getLastVisibleIndex() - browseList.getFirstVisibleIndex();
               new_index = browseList.getSelectedIndex() - offset;
               if (new_index < 0)
               {
                  new_index = 0;
               }
            }
            
            if (new_index < list_size && new_index >= 0)
            {
               browseList.setSelectedIndex(new_index);
               browseList.ensureIndexIsVisible(new_index);
            }
         }
      };
   
   private MouseWheelListener mBrowseMouseListener = new MouseWheelListener()
      {
         public void mouseWheelMoved(MouseWheelEvent e) 
         {
            int new_index = browseList.getSelectedIndex() + e.getWheelRotation();
            int list_size = browseList.getModel().getSize();
            if (new_index < list_size && new_index >= 0)
            {
               browseList.setSelectedIndex(new_index);
               browseList.ensureIndexIsVisible(new_index);
            }
         }
      };
   
   private KeyAdapter mSearchKeyAdapter = new KeyAdapter()
      {
         public void keyPressed(KeyEvent e)
         {
            int new_index = -1;
            int list_size = searchList.getModel().getSize();
            
            if (KeyEvent.VK_DOWN == e.getKeyCode())
            {
               new_index = searchList.getSelectedIndex() + 1;
            }
            else if(KeyEvent.VK_UP == e.getKeyCode())
            {
               new_index = searchList.getSelectedIndex() - 1;
            }
            else if(KeyEvent.VK_PAGE_DOWN == e.getKeyCode())
            {
               int offset = searchList.getLastVisibleIndex() - searchList.getFirstVisibleIndex();
               new_index = searchList.getSelectedIndex() + offset;
               if (new_index >= list_size)
               {
                  new_index = list_size - 1;
               }
            }
            else if(KeyEvent.VK_PAGE_UP == e.getKeyCode())
            {
               int offset = searchList.getLastVisibleIndex() - searchList.getFirstVisibleIndex();
               new_index = searchList.getSelectedIndex() - offset;
               if (new_index < 0)
               {
                  new_index = 0;
               }
            }
            
            if (new_index < list_size && new_index >= 0)
            {
               searchList.setSelectedIndex(new_index);
               searchList.ensureIndexIsVisible(new_index);
            }
         }
      };

   private MouseWheelListener mSearchMouseListener = new MouseWheelListener()
      {
         public void mouseWheelMoved(MouseWheelEvent e) 
         {
            int new_index = searchList.getSelectedIndex() + e.getWheelRotation();
            int list_size = searchList.getModel().getSize();
            if (new_index < list_size && new_index >= 0)
            {
               searchList.setSelectedIndex(new_index);
               searchList.ensureIndexIsVisible(new_index);
            }
         }
      };

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
    * The last ConfigDefinition that was selected in the dialog.
    */
   private ConfigDefinition mSelectedDefinition;

   /**
    * The return value from the last time the dialog was shown.
    */
   private int mReturnValue = ERROR_OPTION;

   /**
    * The title of the dialog.
    */
   private String mDialogTitle;

   /**
    * The text of the approve button.
    */
   private String mApproveButtonText;

   /**
    * The dialog that is currently showing this component.
    */
   private transient JDialog mDialog;

   /**
    * The list model for the browse list.
    */
   private ConfigDefinitionListModel mBrowseListModel = new ConfigDefinitionListModel();

   /**
    * The list model for the search results list.
    */
   private SearchResultsListModel mSearchListModel = new SearchResultsListModel();

   /**
    * A specialized list model for the ConfigDefinition list.
    */
   class ConfigDefinitionListModel
      extends AbstractListModel
   {
      /**
       * Creates a new list model with an empty list of definitions.
       */
      public ConfigDefinitionListModel()
      {
         mDefinitions = new ArrayList();
      }

      public void setDefinitions(List defs)
      {
         int index1 = mDefinitions.size() - 1;
         mDefinitions = defs;

         // Notify listeners that everything was removed
         if (index1 >= 0)
         {
            fireIntervalRemoved(this, 0, index1);
         }
         // Notify listeners of the new definitions
         if (mDefinitions.size() > 0)
         {
            fireIntervalAdded(this, 0, mDefinitions.size()-1);
         }
      }

      public List getDefinitions()
      {
         return mDefinitions;
      }

      public Object getElementAt(int idx)
      {
         return mDefinitions.get(idx);
      }

      public int getSize()
      {
         return mDefinitions.size();
      }

      private List mDefinitions;
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
         mResults = new ArrayList();
      }

      public void setResults(List results)
      {
         int index1 = this.mResults.size() - 1;
         this.mResults = results;

         // Notify listeners that everything was removed
         if (index1 >= 0)
         {
            fireIntervalRemoved(this, 0, index1);
         }
         // Notify listeners of the new results
         if (this.mResults.size() > 0)
         {
            fireIntervalAdded(this, 0, this.mResults.size()-1);
         }
      }

      public List getResults()
      {
         return mResults;
      }

      public Object getElementAt(int idx)
      {
         return mResults.get(idx);
      }

      public int getSize()
      {
         return mResults.size();
      }

      private List mResults;
   }

   /**
    * A specialized list cell renderer for definitions.
    */
   class ConfigDefinitionCellRenderer
      extends JLabel
      implements ListCellRenderer
   {
      public ConfigDefinitionCellRenderer()
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

         if (value instanceof ConfigDefinition)
         {
            setText(((ConfigDefinition)value).getName());
         }
         else
         {
            setText(value.toString());
         }
         return this;
      }
   }
}
