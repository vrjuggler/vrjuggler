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

import java.awt.*;
import java.awt.event.*;
import java.beans.*;
import java.util.*;
import javax.swing.*;
import javax.swing.border.*;
import javax.swing.event.*;
import javax.swing.table.*;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.config.event.*;
import org.vrjuggler.jccl.config.event.ConfigDefinitionEvent;

/**
 * A specialized panel that knows how to edit a single configuration definition.
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
         mCategoriesListNewBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/jccl/editors/images/newcategory.gif")));
         mCategoriesListRemoveBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/jccl/editors/images/removecategory.gif")));
      }
      catch (Exception e)
      {
         // Ack! No icons. Use text labels instead
         mCategoriesListNewBtn.setText("New");
         mCategoriesListRemoveBtn.setText("Remove");
      }

      // Setup the categories list
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

      // Reset the categories list
      mCategoriesModel.clear();
      if (mDefinition != null)
      {
         for (Iterator itr = mDefinition.getCategories().iterator(); itr.hasNext(); )
         {
            mCategoriesModel.add(itr.next());
         }
      }
   }

   private void addCategory()
   {
      mDefinition.addCategory(new Category("/"));
   }

   private void removeCategory(int idx)
   {
      mDefinition.removeCategory((Category)mCategoriesModel.getValueAt(idx, 0));
   }

   /**
    * JBuilder auto-generated GUI code.
    */
   private void jbInit()
      throws Exception
   {
      this.setLayout(mBaseLayout);
      mBasicPanel.setBorder(new TitledBorder(new EtchedBorder(EtchedBorder.RAISED,Color.white,new Color(142, 142, 142)),"Basic"));
      mBasicPanel.setLayout(mBasicPanelLayout);
      mNameLbl.setText("Name:");
      mNamePnl.setLayout(mNamePnlLayout);
      mTokenLbl.setText("Token:");
      mTokenPnl.setLayout(mTokenPnlLayout);
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
      mTokenTxt.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            mDefinition.setToken(mTokenTxt.getText());
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
      mTokenPnl.add(mTokenTxt,  BorderLayout.CENTER);
      this.add(mCategoriesPnl, BorderLayout.CENTER);
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
   private JTextField mTokenTxt = new JTextField();
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
            mDefinition.removeCategory((Category)old_val);
            mDefinition.addCategory((Category)obj);
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

      public void categoryAdded(ConfigDefinitionEvent evt)
      {
         ConfigDefinition def = (ConfigDefinition)evt.getSource();
         mCategoriesModel.add((Category)evt.getValue());
      }

      public void categoryRemoved(ConfigDefinitionEvent evt)
      {
         mCategoriesModel.remove((Category)evt.getValue());
      }
   }
}
