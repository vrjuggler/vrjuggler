/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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
import java.awt.event.*;
import java.beans.*;
import java.util.*;
import javax.swing.*;
import javax.swing.border.*;
import javax.swing.event.*;
import javax.swing.table.*;

import info.clearthought.layout.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.*;
import org.vrjuggler.jccl.config.event.ConfigDefinitionEvent;


/**
 * A specialized panel that knows how to edit a single configuration
 * definition.
 */
public class ConfigDefinitionEditor
   extends JPanel
{
   public ConfigDefinitionEditor()
   {
      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      // Try to get icons for the toolbar buttons
      try
      {
         ClassLoader loader = getClass().getClassLoader();
         String img_root = "org/vrjuggler/jccl/editors/images";
         ImageIcon new_icon =
            new ImageIcon(loader.getResource(img_root + "/newcategory.gif"));
         ImageIcon remove_icon =
            new ImageIcon(loader.getResource(img_root + "/removecategory.gif"));
         mParentsListNewBtn.setIcon(new_icon);
         mParentsListRemoveBtn.setIcon(remove_icon);
         mCategoriesListNewBtn.setIcon(new_icon);
         mCategoriesListRemoveBtn.setIcon(remove_icon);
      }
      catch (Exception e)
      {
         // Ack! No icons. Use text labels instead
         mParentsListNewBtn.setText("New");
         mParentsListRemoveBtn.setText("Remove");
         mCategoriesListNewBtn.setText("New");
         mCategoriesListRemoveBtn.setText("Remove");

         mParentsListNewBtn.setIcon(null);
         mParentsListRemoveBtn.setIcon(null);
         mCategoriesListNewBtn.setIcon(null);
         mCategoriesListRemoveBtn.setIcon(null);
      }

      // Setup the parents and categories lists
      mParentsList.setModel(mParentsModel);
      mCategoriesList.setModel(mCategoriesModel);
   }

   /**
    * Returns the ConfigDefinition that is currently being edited.
    */
   public ConfigDefinition getDefinition()
   {
      return mDefinition;
   }
   
   /**
    * Sets the configuration definition that this panel is editing.
    *
    * @param definition   the configuration definition to edit
    */
   public void setDefinition(ConfigDefinition definition)
   {
      // Setup the listeners
      if (mDefinition != null)
      {
         mDefinition.removeConfigDefinitionListener(mDefinitionListener);
      }
      mDefinition = definition;
      if (mDefinition != null)
      {
         mDefinition.addConfigDefinitionListener(mDefinitionListener);
      }

      String name = (mDefinition != null) ? mDefinition.getName() : "";
      String token = (mDefinition != null) ? mDefinition.getToken() : "";

      // Set the new data in the GUI
      mNameTxt.setText(name);
      mTokenTxt.setText(token);

      if ( mDefinition != null )
      {
         mAbstractBox.setSelected(mDefinition.isAbstract());
      }

      // Reset the parents and categories lists.
      mParentsModel.clear();
      mCategoriesModel.clear();

      if (mDefinition != null)
      {
         ConfigBroker broker = new ConfigBrokerProxy();
         ConfigDefinitionRepository repos = broker.getRepository();

         for ( Iterator itr = mDefinition.getParents().iterator();
               itr.hasNext(); )
         {
            mParentsModel.addElement(repos.get((String) itr.next()));
         }

         for ( Iterator itr = mDefinition.getCategories().iterator();
               itr.hasNext(); )
         {
            mCategoriesModel.add(itr.next());
         }
      }
   }

   private void parentSelectionChanged(ListSelectionEvent e)
   {
      if ( mParentsList.getSelectedIndex() == -1 )
      {
         mParentsListRemoveBtn.setEnabled(false);
      }
      else
      {
         mParentsListRemoveBtn.setEnabled(true);
      }
   }

   private void addParent()
   {
      ConfigBroker broker = new ConfigBrokerProxy();
      ConfigDefinitionRepository repos = broker.getRepository();
      java.util.List all_defs = repos.getAllLatest();
      filterDisallowedParents(repos, mDefinition.getParents(), all_defs);
      ConfigDefinitionChooser chooser = new ConfigDefinitionChooser();
      chooser.setDefinitions(all_defs);
      Frame parent = (Frame) SwingUtilities.getAncestorOfClass(Frame.class,
                                                               this);

      if ( chooser.showDialog(parent) == ConfigDefinitionChooser.APPROVE_OPTION )
      {
         ConfigDefinition new_parent = chooser.getSelectedDefinition();
         if ( new_parent != null )
         {
            mDefinition.addParent(new_parent.getToken());
         }
      }
   }

   private void filterDisallowedParents(ConfigDefinitionRepository repos,
                                        java.util.List parentTokens,
                                        java.util.List allDefs)
   {
      for ( Iterator i = parentTokens.iterator(); i.hasNext(); )
      {
         String parent_name = (String) i.next();

         if ( ! parent_name.equals("") )
         {
            ConfigDefinition parent_def = repos.get(parent_name);

            if ( parent_def.getParents() != null &&
                 ! parent_def.getParents().isEmpty() )
            {
               filterDisallowedParents(repos, parent_def.getParents(),
                                       allDefs);
            }

            allDefs.remove(parent_def);
         }
      }
   }

   private void removeParent(ConfigDefinition parentDef)
   {
      mDefinition.removeParent(parentDef.getToken());
   }

   private void addCategory()
   {
      mDefinition.addCategory(new Category("/"));
   }

   private void removeCategory(int idx)
   {
      if ( idx > -1 )
      {
         mDefinition.removeCategory((Category)mCategoriesModel.getValueAt(idx,
                                                                          0));
      }
   }

   /**
    * JBuilder auto-generated GUI code.
    */
   private void jbInit()
      throws Exception
   {
      double[][] sizes =
         {
            {TableLayout.FILL},
            {0.5, 0.5}
         };
      mParentCategoryPanel.setLayout(new TableLayout(sizes));
      this.setLayout(mBaseLayout);
      mBasicPanel.setBorder(new TitledBorder(new EtchedBorder(EtchedBorder.RAISED,Color.white,new Color(142, 142, 142)),"Basic"));
      mBasicPanel.setLayout(mBasicPanelLayout);
      mNameLbl.setText("Name:");
      mNamePnl.setLayout(mNamePnlLayout);
      mTokenLbl.setText("Token:");
      mTokenPnl.setLayout(mTokenPnlLayout);
      mParentsPnl.setLayout(mParentsPnlLayout);
      mParentsPnl.setBorder(new TitledBorder(new EtchedBorder(EtchedBorder.RAISED,Color.white,new Color(142, 142, 142)),"Parents"));
      mParentsListCtrlPnl.setLayout(new BoxLayout(mParentsListCtrlPnl, BoxLayout.X_AXIS));
      mParentsListRemoveBtn.setEnabled(false);
      mParentsListRemoveBtn.setToolTipText("Remove Parent");
      mParentsListRemoveBtn.setMargin(new Insets(0, 0, 0, 0));
      mParentsListNewBtn.setToolTipText("New Parent");
      mParentsListNewBtn.setMargin(new Insets(0, 0, 0, 0));
      mParentsList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
      mParentsList.setCellRenderer(new ConfigDefinitionCellRenderer());
      mParentsList.addListSelectionListener(
         new ListSelectionListener()
         {
            public void valueChanged(ListSelectionEvent e)
            {
               parentSelectionChanged(e);
            }
         }
      );
      mCategoriesPnl.setLayout(mCategoriesPnlLayout);
      mCategoriesPnl.setBorder(new TitledBorder(new EtchedBorder(EtchedBorder.RAISED,Color.white,new Color(142, 142, 142)),"Categories"));
      mCategoriesListCtrlPnl.setLayout(new BoxLayout(mCategoriesListCtrlPnl, BoxLayout.X_AXIS));
      mCategoriesListRemoveBtn.setToolTipText("Remove Category");
      mCategoriesListRemoveBtn.setMargin(new Insets(0, 0, 0, 0));
      mCategoriesListNewBtn.setToolTipText("New Category");
      mCategoriesListNewBtn.setMargin(new Insets(0, 0, 0, 0));
      mCategoriesList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
      mNameTxt.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            mDefinition.setName(mNameTxt.getText());
         }
      });
      // XXX: Remove this when renaming config definitions is allowed.
      mTokenTxt.setEditable(false);
      mTokenTxt.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            mDefinition.setToken(mTokenTxt.getText());
         }
      });
      mAbstractBox.setText("Is abstract?");
      mAbstractBox.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            mDefinition.setAbstract(mAbstractBox.isSelected());
         }
      });
      mParentsListNewBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            addParent();
         }
      });
      mParentsListRemoveBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            removeParent((ConfigDefinition) mParentsList.getSelectedValue());
         }
      });
      mCategoriesListNewBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            addCategory();
         }
      });
      mCategoriesListRemoveBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            removeCategory(mCategoriesList.getSelectedRow());
         }
      });
      this.add(mBasicPanel, BorderLayout.NORTH);
      mBasicPanel.add(mNameLbl,   new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 2, 2));
      mBasicPanel.add(mNamePnl,     new GridBagConstraints(1, 0, 1, 1, 1.0, 0.0
            ,GridBagConstraints.CENTER, GridBagConstraints.BOTH, new Insets(0, 0, 0, 0), 2, 2));
      mNamePnl.add(mNameTxt, BorderLayout.CENTER);
      mBasicPanel.add(mTokenLbl,   new GridBagConstraints(0, 1, 1, 1, 0.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 2, 2));
      mBasicPanel.add(mTokenPnl,  new GridBagConstraints(1, 1, 1, 1, 1.0, 0.0
            ,GridBagConstraints.CENTER, GridBagConstraints.BOTH, new Insets(0, 0, 0, 0), 2, 2));
      mBasicPanel.add(mAbstractBox,  new GridBagConstraints(0, 2, 2, 1, 0.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 2, 2));
      mTokenPnl.add(mTokenTxt,  BorderLayout.CENTER);
      mParentCategoryPanel.add(mParentsPnl,
                               new TableLayoutConstraints(0, 0, 0, 0,
                                                          TableLayoutConstraints.FULL,
                                                          TableLayoutConstraints.FULL));
      mParentCategoryPanel.add(mCategoriesPnl,
                               new TableLayoutConstraints(0, 1, 0, 1,
                                                          TableLayoutConstraints.FULL,
                                                          TableLayoutConstraints.FULL));
      this.add(mParentCategoryPanel, BorderLayout.CENTER);
      mParentsPnl.add(mParentsListSP, BorderLayout.CENTER);
      mParentsListSP.getViewport().add(mParentsList, null);
      mParentsPnl.add(mParentsListCtrlPnl, BorderLayout.NORTH);
      mParentsListCtrlPnl.add(Box.createHorizontalGlue(), null);
      mParentsListCtrlPnl.add(mParentsListNewBtn, null);
      mParentsListCtrlPnl.add(mParentsListRemoveBtn, null);
      mCategoriesPnl.add(mCategoriesListSP, BorderLayout.CENTER);
      mCategoriesListSP.getViewport().add(mCategoriesList, null);
      mCategoriesPnl.add(mCategoriesListCtrlPnl, BorderLayout.NORTH);
      mCategoriesListCtrlPnl.add(Box.createHorizontalGlue(), null);
      mCategoriesListCtrlPnl.add(mCategoriesListNewBtn, null);
      mCategoriesListCtrlPnl.add(mCategoriesListRemoveBtn, null);
   }

   // JBuilder GUI variables
   private BorderLayout mBaseLayout = new BorderLayout();
   private JPanel mBasicPanel = new JPanel();
   private GridBagLayout mBasicPanelLayout = new GridBagLayout();
   private JLabel mNameLbl = new JLabel();
   private JPanel mNamePnl = new JPanel();
   private BorderLayout mNamePnlLayout = new BorderLayout();
   private JTextField mNameTxt = new JTextField();
   private JLabel mTokenLbl = new JLabel();
   private JPanel mTokenPnl = new JPanel();
   private BorderLayout mTokenPnlLayout = new BorderLayout();
   private JCheckBox mAbstractBox = new JCheckBox();
   private JTextField mTokenTxt = new JTextField();
   private JPanel mParentCategoryPanel = new JPanel();
   private JPanel mParentsPnl = new JPanel();
   private BorderLayout mParentsPnlLayout = new BorderLayout();
   private JScrollPane mParentsListSP = new JScrollPane();
   private JPanel mParentsListCtrlPnl = new JPanel();
   private JButton mParentsListNewBtn = new JButton();
   private JButton mParentsListRemoveBtn = new JButton();
   private JList mParentsList = new JList();
   private JPanel mCategoriesPnl = new JPanel();
   private BorderLayout mCategoriesPnlLayout = new BorderLayout();
   private JScrollPane mCategoriesListSP = new JScrollPane();
   private JPanel mCategoriesListCtrlPnl = new JPanel();
   private JButton mCategoriesListNewBtn = new JButton();
   private JButton mCategoriesListRemoveBtn = new JButton();
   private JTable mCategoriesList = new JTable()
   {
      public Dimension getPreferredScrollableViewportSize()
      {
         Dimension pref_size = getPreferredSize();
         if (pref_size.height < getRowHeight()*4)
         {
            pref_size.height = getRowHeight()*4;
         }
         return pref_size;
      }
   };

   /**
    * The data model for the parents list.
    */
   private DefaultListModel mParentsModel = new DefaultListModel();

   /**
    * The data model for the categories list.
    */
   private CategoriesTableModel mCategoriesModel = new CategoriesTableModel();

   /**
    * The configuration definition that is being edited.
    */
   private ConfigDefinition mDefinition = null;

   /**
    * Listens for changes to the definition being edited.
    */
   private DefChangeListener mDefinitionListener = new DefChangeListener();

   /**
    * A specialized table model for the includes list (table).
    */
   private class CategoriesTableModel
      extends AbstractTableModel
   {
      private static final int MIN_ROWS = 1;

      public void add(Object elt)
      {
         mData.add(elt);
         int new_row = mData.size()-1;
         fireTableRowsInserted(new_row, new_row);
      }

      public void remove(Object elt)
      {
         int idx = mData.indexOf(elt);
         if (idx != -1)
         {
            remove(idx);
         }
      }

      public Object remove(int idx)
      {
         Object obj = mData.remove(idx);
         fireTableRowsDeleted(idx, idx);
         return obj;
      }

      public void clear()
      {
         int size = mData.size();
         mData.clear();

         if (size > 1)
         {
            fireTableRowsDeleted(1, size-1);
         }
         fireTableRowsUpdated(0, 0);
      }

      public int size()
      {
         return mData.size();
      }

      public void insert(Object elt, int idx)
      {
         mData.add(idx, elt);
      }

      public Object getValueAt(int row, int col)
      {
         if (row < mData.size())
         {
            return mData.get(row);
         }
         return null;
      }

      public void setValueAt(Object obj, int row, int col)
      {
         if (row <= mData.size())
         {
            Object old_val = getValueAt(row, col);

            // Modify the definition
            mDefinition.removeCategory((Category) old_val);
            mDefinition.addCategory((Category) obj);
         }
      }

      public boolean isCellEditable(int row, int col)
      {
         if (row < mData.size())
         {
            return true;
         }
         return false;
      }

      public Class getColumnClass(int row) { return String.class; }
      public String getColumnName(int col) { return null; }

      public int getRowCount()
      {
         int rows = mData.size();
         if (rows < MIN_ROWS)
         {
            rows = MIN_ROWS;
         }
         return rows;
      }

      public int getColumnCount() { return 1; }

      private java.util.List mData = new ArrayList();
   }

   private static class ConfigDefinitionCellRenderer
      extends JLabel
      implements ListCellRenderer
   {
      public Component getListCellRendererComponent(JList list, Object value,
                                                    int index,
                                                    boolean isSelected,
                                                    boolean hasFocus)
      {
         ConfigDefinition def = (ConfigDefinition) value;
         this.setText(def.getName() + " (" + def.getToken() + ")");

         if ( isSelected )
         {
            setForeground(list.getSelectionForeground());
            setBackground(list.getSelectionBackground());
         }
         else
         {
            setForeground(list.getForeground());
            setBackground(list.getBackground());
         }

         return this;
      }
   }

   /**
    * Specialized class for listening to changes to a config definition.
    */
   private class DefChangeListener
      extends ConfigDefinitionAdapter
   {
      public void nameChanged(ConfigDefinitionEvent evt)
      {
         ConfigDefinition def = (ConfigDefinition)evt.getSource();
         mNameTxt.setText(def.getName());
      }

      public void tokenChanged(ConfigDefinitionEvent evt)
      {
         ConfigDefinition def = (ConfigDefinition)evt.getSource();
         mTokenTxt.setText(def.getToken());
      }

      public void parentAdded(ConfigDefinitionEvent evt)
      {
         ConfigBroker broker = new ConfigBrokerProxy();
         ConfigDefinitionRepository repos = broker.getRepository();
         mParentsModel.addElement(repos.get((String) evt.getValue()));
      }

      public void parentRemoved(ConfigDefinitionEvent evt)
      {
         ConfigBroker broker = new ConfigBrokerProxy();
         ConfigDefinitionRepository repos = broker.getRepository();
         mParentsModel.removeElement(repos.get((String) evt.getValue()));
      }

      public void categoryAdded(ConfigDefinitionEvent evt)
      {
         mCategoriesModel.add((Category)evt.getValue());
      }

      public void categoryRemoved(ConfigDefinitionEvent evt)
      {
         mCategoriesModel.remove((Category)evt.getValue());
      }
   }
}
