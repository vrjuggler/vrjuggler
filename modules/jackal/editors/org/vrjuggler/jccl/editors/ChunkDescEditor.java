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
import org.vrjuggler.tweek.beans.loader.BeanJarClassLoader;

/**
 * A specialized panel that knows how to edit a single ChunkDesc.
 */
public class ChunkDescEditor
   extends JPanel
{
   public ChunkDescEditor()
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
         ClassLoader loader = BeanJarClassLoader.instance();
         categoriesListNewBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/jccl/editors/images/newcategory.gif")));
         categoriesListRemoveBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/jccl/editors/images/removecategory.gif")));
      }
      catch (Exception e)
      {
         // Ack! No icons. Use text labels instead
         categoriesListNewBtn.setText("New");
         categoriesListRemoveBtn.setText("Remove");
      }

      // Setup the categories list
      categoriesList.setModel(mCategoriesModel);
   }

   /**
    * Sets the configuration definition that this panel is editing.
    *
    * @param definition   the ChunkDesc to edit
    */
   public void setDefinition(ChunkDesc definition)
   {
      // Setup the listeners
      if (mDefinition != null)
      {
         mDefinition.removePropertyChangeListener(mDefinitionListener);
      }
      mDefinition = definition;
      if (mDefinition != null)
      {
         mDefinition.addPropertyChangeListener(mDefinitionListener);
      }

      String name = (mDefinition != null) ? mDefinition.getName() : "";
      String token = (mDefinition != null) ? mDefinition.getToken() : "";

      // Set the new data in the GUI
      nameTxt.setText(name);
      tokenTxt.setText(token);

      // Reset the categories list
      mCategoriesModel.clear();
      if (mDefinition != null)
      {
         for (Iterator itr = mDefinition.getCategories(); itr.hasNext(); )
         {
            mCategoriesModel.add(itr.next());
         }
      }
   }

   private void addCategory()
   {
      mDefinition.addCategory("Untitled");
   }

   private void removeCategory(int idx)
   {
      mDefinition.removeCategory((String)mCategoriesModel.getValueAt(idx, 0));
   }

   /**
    * JBuilder auto-generated GUI code.
    */
   private void jbInit()
      throws Exception
   {
      this.setLayout(baseLayout);
      basicPanel.setBorder(new TitledBorder(new EtchedBorder(EtchedBorder.RAISED,Color.white,new Color(142, 142, 142)),"Basic"));
      basicPanel.setLayout(basicPanelLayout);
      nameLbl.setText("Name:");
      namePnl.setLayout(namePnlLayout);
      tokenLbl.setText("Token:");
      tokenPnl.setLayout(tokenPnlLayout);
      categoriesPnl.setLayout(categoriesPnlLayout);
      categoriesPnl.setBorder(new TitledBorder(new EtchedBorder(EtchedBorder.RAISED,Color.white,new Color(142, 142, 142)),"Categories"));
      categoriesListCtrlPnl.setLayout(new BoxLayout(categoriesListCtrlPnl, BoxLayout.X_AXIS));
      categoriesListRemoveBtn.setToolTipText("Remove Category");
      categoriesListRemoveBtn.setMargin(new Insets(0, 0, 0, 0));
      categoriesListNewBtn.setToolTipText("New Category");
      categoriesListNewBtn.setMargin(new Insets(0, 0, 0, 0));
      categoriesList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
      nameTxt.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            mDefinition.setName(nameTxt.getText());
         }
      });
      tokenTxt.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            mDefinition.setToken(tokenTxt.getText());
         }
      });
      categoriesListNewBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            addCategory();
         }
      });
      categoriesListRemoveBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            removeCategory(categoriesList.getSelectedRow());
         }
      });
      this.add(basicPanel, BorderLayout.NORTH);
      basicPanel.add(nameLbl,   new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 2, 2));
      basicPanel.add(namePnl,     new GridBagConstraints(1, 0, 1, 1, 1.0, 0.0
            ,GridBagConstraints.CENTER, GridBagConstraints.BOTH, new Insets(0, 0, 0, 0), 2, 2));
      namePnl.add(nameTxt, BorderLayout.CENTER);
      basicPanel.add(tokenLbl,   new GridBagConstraints(0, 1, 1, 1, 0.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 2, 2));
      basicPanel.add(tokenPnl,  new GridBagConstraints(1, 1, 1, 1, 1.0, 0.0
            ,GridBagConstraints.CENTER, GridBagConstraints.BOTH, new Insets(0, 0, 0, 0), 2, 2));
      tokenPnl.add(tokenTxt,  BorderLayout.CENTER);
      this.add(categoriesPnl, BorderLayout.CENTER);
      categoriesPnl.add(categoriesListSP, BorderLayout.CENTER);
      categoriesListSP.getViewport().add(categoriesList, null);
      categoriesPnl.add(categoriesListCtrlPnl, BorderLayout.NORTH);
      categoriesListCtrlPnl.add(Box.createHorizontalGlue(), null);
      categoriesListCtrlPnl.add(categoriesListNewBtn, null);
      categoriesListCtrlPnl.add(categoriesListRemoveBtn, null);
   }

   // JBuilder GUI variables
   private BorderLayout baseLayout = new BorderLayout();
   private JPanel basicPanel = new JPanel();
   private GridBagLayout basicPanelLayout = new GridBagLayout();
   private JLabel nameLbl = new JLabel();
   private JPanel namePnl = new JPanel();
   private BorderLayout namePnlLayout = new BorderLayout();
   private JTextField nameTxt = new JTextField();
   private JLabel tokenLbl = new JLabel();
   private JPanel tokenPnl = new JPanel();
   private BorderLayout tokenPnlLayout = new BorderLayout();
   private JTextField tokenTxt = new JTextField();
   private JPanel categoriesPnl = new JPanel();
   private BorderLayout categoriesPnlLayout = new BorderLayout();
   private JScrollPane categoriesListSP = new JScrollPane();
   private JPanel categoriesListCtrlPnl = new JPanel();
   private JButton categoriesListNewBtn = new JButton();
   private JButton categoriesListRemoveBtn = new JButton();
   private JTable categoriesList = new JTable()
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
   private ChunkDesc mDefinition = null;

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

            // Modify the desc
            mDefinition.removeCategory((String)old_val);
            mDefinition.addCategory((String)obj);
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
      implements PropertyChangeListener
   {
      public void propertyChange(PropertyChangeEvent evt)
      {
         ChunkDesc desc = (ChunkDesc)evt.getSource();
         if (evt.getPropertyName().equals("name"))
         {
            nameTxt.setText(desc.getName());
         }
         else if (evt.getPropertyName().equals("token"))
         {
            tokenTxt.setText(desc.getToken());
         }
         else if (evt.getPropertyName().equals("category"))
         {
            mCategoriesModel.clear();
            for (Iterator itr = desc.getCategories(); itr.hasNext(); )
            {
               mCategoriesModel.add(itr.next());
            }
         }
      }
   }
}
