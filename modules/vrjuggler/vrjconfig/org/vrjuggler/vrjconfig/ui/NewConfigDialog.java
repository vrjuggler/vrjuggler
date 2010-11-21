/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

package org.vrjuggler.vrjconfig.ui;

import java.awt.*;
import java.awt.event.*;
import java.io.File;
import java.util.*;
import javax.swing.*;
import javax.swing.border.*;
import javax.swing.event.*;
import javax.swing.filechooser.FileFilter;
import javax.swing.table.*;

import org.vrjuggler.tweek.beans.loader.BeanJarClassLoader;
import org.vrjuggler.tweek.services.EnvironmentService;
import org.vrjuggler.tweek.services.EnvironmentServiceProxy;
import org.vrjuggler.vrjconfig.VrjConfigConstants;

/**
 * A specialized toolbar that pays attention to the ConfigManager.
 */
public class NewConfigDialog
   extends JPanel
   implements VrjConfigConstants
{
   public static final int APPROVE_OPTION = 0;
   public static final int CANCEL_OPTION = 1;
   public static final int ERROR_OPTION = -1;

   public NewConfigDialog(File chooserCurDir)
   {
      try
      {
         jbInit();

         EnvironmentService env_svc = new EnvironmentServiceProxy();
         if ( env_svc.getOS() == EnvironmentService.Windows )
         {
            buttonPnl.setLayout(new FlowLayout(FlowLayout.CENTER));
            buttonPnl.add(okBtn);
            buttonPnl.add(cancelBtn);
         }
         else
         {
            buttonPnl.setLayout(new FlowLayout(FlowLayout.RIGHT));
            buttonPnl.add(cancelBtn);
            buttonPnl.add(okBtn);
         }
      }
      catch (Exception e)
      {
         e.printStackTrace();
      }

      fileChooser.setCurrentDirectory(chooserCurDir);

      /*
      // Try to get icons for the toolbar buttons
      try
      {
         ClassLoader loader = BeanJarClassLoader.instance();
         includesListNewBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/newinclude.gif")));
         includesListRemoveBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/removeinclude.gif")));
         includesListMoveUpBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/moveup.gif")));
         includesListMoveDownBtn.setIcon(new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/images/movedown.gif")));
      }
      catch (Exception e)
      {
         // Ack! No icons. Use text labels instead
         includesListNewBtn.setText("New");
         includesListRemoveBtn.setText("Remove");
         includesListMoveUpBtn.setText("Move Up");
         includesListMoveDownBtn.setText("Move Down");
      }
      */
      // Setup the default UI texts
      setDialogTitle("Create New Configuration");
      nameTxt.setText("untitled.jconf");
      
      /*
      // Init the includes list
      includesList.setModel(includesTableModel);
      includesList.getColumnModel().getColumn(0).setCellEditor(new FileCellEditor(fileChooser));
      */
      
      // Default to the user's home dir
      directoryTxt.setText(fileChooser.getCurrentDirectory().getAbsolutePath());
      
      /*
      // Fill in the definition path table with the values given.
      java.util.List dirs = getDefinitionPath();
      Iterator itr = dirs.iterator();
      while(itr.hasNext())
      {
         includesTableModel.add(itr.next());
      }
      */
   }

   public int showDialog(Component parent)
   {
      Frame frame = parent instanceof Frame ?
                    (Frame)parent :
                    (Frame)SwingUtilities.getAncestorOfClass(Frame.class, parent);

      // Create a dialog to hold this component
      dialog = new JDialog(frame, getDialogTitle(), true);
      dialog.addWindowListener(new WindowAdapter()
      {
         public void windowClosing(WindowEvent evt)
         {
            cancel();
         }
      });
      Container contentPane = dialog.getContentPane();
      contentPane.setLayout(new BorderLayout());
      contentPane.add(this, BorderLayout.CENTER);

      // Make sure our buttons are set up correctly
      validateData();

      // Size and position the dialog
      dialog.getRootPane().setDefaultButton(okBtn);
      dialog.pack();
      dialog.setLocationRelativeTo(parent);
      dialog.setVisible(true);

      return returnValue;
   }

   public void approve()
   {
      File dir = new File(directoryTxt.getText().trim());
      // If the file does not exist the user that they have entered an invalid
      // location.
      if (!dir.isDirectory())
      { 
         JOptionPane.showMessageDialog(null, directoryTxt.getText().trim()  + " is not a valid directory.");
         return;
      }
      
      File file = new File(getDirectory(), getFileName());
      // If the file does not exist the user that they have entered an invalid
      // location.
      // If file exists ask the user if we should overwrite(delete it now)
      if (file.exists())
      { 
         int result = JOptionPane.showConfirmDialog(null, 
               "File already exists, do you want to overwrite it?", "File exists!", 
               JOptionPane.YES_NO_OPTION, JOptionPane.WARNING_MESSAGE);
         if(JOptionPane.NO_OPTION == result)
         {
            return;
         }
         else
         {
            file.delete();
         }
      }

      returnValue = APPROVE_OPTION;
      if (dialog != null)
      {
         dialog.setVisible(false);
      }
   }

   public void cancel()
   {
      returnValue = CANCEL_OPTION;
      if (dialog != null)
      {
         dialog.setVisible(false);
      }
   }

   /**
    * Gets the name of the configuration file.
    */
   public String getFileName()
   {
      return nameTxt.getText().trim();
   }

   /**
    * Gets the directory in which to place the configuration file.
    */
   public String getDirectory()
   {
      return directoryTxt.getText().trim();
   }

   /**
    * Gets a list of the files to include in the context by default.
    */
   /*
   public java.util.List getIncludes()
   {
      java.util.List includes = new ArrayList();
      for (int i=0; i<includesTableModel.size(); ++i)
      {
         includes.add(includesTableModel.getValueAt(i, 0));
      }

      return includes;
   }
   */

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
    * Gets a list of the directories in which to look for configuration
    * definitions.
    */
   /*
   private java.util.List getDefinitionPath()
   {
      java.util.List dirs = new ArrayList();

      // Get the path from the environment
      EnvironmentService env_svc = new EnvironmentServiceProxy();
      String default_path = "${VJ_BASE_DIR}/share/vrjuggler/data/definitions";
      String path = env_svc.getenv("JCCL_DEFINITION_PATH", default_path);
      path = env_svc.expandEnvVars(path);

      // Split the path on the path separator
      StringTokenizer tokenizer = new StringTokenizer(path, File.pathSeparator);
      while (tokenizer.hasMoreTokens())
      {
         dirs.add(tokenizer.nextToken());
      }

      return dirs;
   }
   */
   private void validateData()
   {
      boolean valid = true;

      if (nameTxt.getText().trim().equals(""))
      {
         valid = false;
      }
      if (directoryTxt.getText().trim().equals(""))
      {
         valid = false;
      }

      okBtn.setEnabled(valid);
   }
   /*

   private void addInclude()
   {
      int new_row = includesTableModel.size();
      includesTableModel.add("");
      includesList.requestFocus();
      includesList.setRowSelectionInterval(new_row, new_row);
      includesList.editCellAt(new_row, 0);
   }

   private void removeInclude(int idx)
   {
      System.out.println("Removing: " + idx);
      includesTableModel.remove(idx);

      int sel_idx = idx;
      if (sel_idx >= includesTableModel.size())
      {
         sel_idx = includesTableModel.size() - 1;
      }

      // Assume that while the table's size is zero, it has a minimum number of
      // rows that is greater than 0 to fill out the preferred size.
      if (includesTableModel.size() == 0)
      {
         sel_idx = 0;
      }
      System.out.println("Selecting: " + sel_idx);
      includesList.setRowSelectionInterval(sel_idx, sel_idx);
   }

   private void moveIncludeUp(int idx)
   {
      if (idx > 0)
      {
         //includesList.clearSelection();
         if(includesList.isEditing() && includesList.getCellEditor().stopCellEditing())
         {
            Object elt = includesTableModel.remove(idx);
            includesTableModel.insert(elt, idx-1);
            includesList.setRowSelectionInterval(idx-1, idx-1);
         }
      }
   }

   private void moveIncludeDown(int idx)
   {
      if (idx < (includesTableModel.size()-1))
      {
         //includesList.clearSelection();
         if(includesList.isEditing() && includesList.getCellEditor().stopCellEditing())
         {
            Object elt = includesTableModel.remove(idx);
            includesTableModel.insert(elt, idx+1);
            includesList.setRowSelectionInterval(idx+1, idx+1);
         }
      }
   }
   */

   /**
    * Expands all known environment variables in the given string and returns
    * the result.
    */
   private String expandEnvVars(String str)
   {
      return mEnvService.expandEnvVars(str);
   }

   /**
    * JBuilder auto-generated GUI code.
    */
   private void jbInit()
      throws Exception
   {
      this.setLayout(new BorderLayout());
      configPnl.setLayout(configPnlLayout);
      nameLbl.setText("Name:");
      configPnl.setBorder(new TitledBorder(new EtchedBorder(EtchedBorder.RAISED,Color.white,new Color(142, 142, 142)),"Configuration"));
      directoryLbl.setText("Directory:");
      nameTxt.setAlignmentX((float) 0.0);
      namePnl.setLayout(namePnlLayout);
      directoryPnl.setLayout(directoryPnlLayout);
      directoryBrowseBtn.setMargin(new Insets(0, 2, 0, 2));
      directoryBrowseBtn.setText("...");
      /*
      includesPnl.setBorder(new TitledBorder(new EtchedBorder(EtchedBorder.RAISED,Color.white,new Color(142, 142, 142)),"Definition Path"));
      includesPnl.setLayout(includesPnlLayout);
      includesList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
      includesList.setShowGrid(false);
      includesListCtrlPnl.setLayout(new BoxLayout(includesListCtrlPnl, BoxLayout.X_AXIS));
      includesListMoveUpBtn.setToolTipText("Move Up Include");
      includesListMoveUpBtn.setMargin(new Insets(0, 0, 0, 0));
      includesListMoveDownBtn.setToolTipText("Move Down Include");
      includesListMoveDownBtn.setMargin(new Insets(0, 0, 0, 0));
      includesListRemoveBtn.setToolTipText("Remove Include");
      includesListRemoveBtn.setMargin(new Insets(0, 0, 0, 0));
      includesListNewBtn.setToolTipText("New Include");
      includesListNewBtn.setMargin(new Insets(0, 0, 0, 0));
      */
      cancelBtn.setText("Cancel");
      okBtn.setText("OK");
      nameTxt.getDocument().addDocumentListener(new DocumentListener()
      {
         public void changedUpdate(DocumentEvent evt)
         {
            validateData();
         }

         public void insertUpdate(DocumentEvent evt)
         {
            validateData();
         }

         public void removeUpdate(DocumentEvent evt)
         {
            validateData();
         }
      });
      directoryTxt.getDocument().addDocumentListener(new DocumentListener()
      {
         public void changedUpdate(DocumentEvent evt)
         {
            validateData();
         }

         public void insertUpdate(DocumentEvent evt)
         {
            validateData();
         }

         public void removeUpdate(DocumentEvent evt)
         {
            validateData();
         }
      });
      
      directoryBrowseBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            fileChooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
            fileChooser.setFileHidingEnabled(false);
            fileChooser.setDialogTitle("Select Directory");
            int result = fileChooser.showOpenDialog(NewConfigDialog.this);
            if (result == JFileChooser.APPROVE_OPTION)
            {
               File file = fileChooser.getSelectedFile();
               directoryTxt.setText(file.getAbsolutePath());
            }
         }
      });
      /*
      includesListNewBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            addInclude();
         }
      });
      includesListRemoveBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            removeInclude(includesList.getSelectedRow());
         }
      });
      includesListMoveUpBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            moveIncludeUp(includesList.getSelectedRow());
         }
      });
      includesListMoveDownBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            moveIncludeDown(includesList.getSelectedRow());
         }
      });
      */
      cancelBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            cancel();
         }
      });
      okBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            approve();
         }
      });
      nameTxt.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            approve();
         }
      });
      directoryTxt.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            approve();
         }
      });
      
      this.add(configPnl, BorderLayout.NORTH);
      configPnl.add(nameLbl,         new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 2, 2));
      configPnl.add(directoryLbl,         new GridBagConstraints(0, 1, 1, 1, 0.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 2, 2));
      configPnl.add(namePnl,      new GridBagConstraints(1, 0, 2, 1, 0.0, 0.0
            ,GridBagConstraints.CENTER, GridBagConstraints.BOTH, new Insets(0, 0, 0, 0), 0, 0));
      namePnl.add(nameTxt, BorderLayout.CENTER);
      configPnl.add(directoryPnl,     new GridBagConstraints(1, 1, 1, 1, 1.0, 0.0
            ,GridBagConstraints.CENTER, GridBagConstraints.BOTH, new Insets(0, 0, 0, 0), 0, 0));
      directoryPnl.add(directoryTxt,  BorderLayout.CENTER);
      configPnl.add(directoryBrowseBtn,      new GridBagConstraints(2, 1, 1, 1, 0.0, 0.0
            ,GridBagConstraints.CENTER, GridBagConstraints.BOTH, new Insets(0, 0, 0, 0), 0, 0));
      /*
      this.add(includesPnl, BorderLayout.CENTER);
      includesPnl.add(includesListSP, BorderLayout.CENTER);
      includesPnl.add(includesListCtrlPnl,  BorderLayout.NORTH);
      includesListCtrlPnl.add(Box.createHorizontalGlue());
      includesListCtrlPnl.add(includesListNewBtn, null);
      includesListCtrlPnl.add(includesListRemoveBtn, null);
      includesListCtrlPnl.add(includesListMoveUpBtn, null);
      includesListCtrlPnl.add(includesListMoveDownBtn, null);
      */
      this.add(buttonPnl,  BorderLayout.SOUTH);
      /*
      includesListSP.getViewport().add(includesList, null);
      */
   }

   // JBuilder GUI variables
   private JPanel configPnl = new JPanel();
   private GridBagLayout configPnlLayout = new GridBagLayout();
   private JLabel nameLbl = new JLabel();
   private JLabel directoryLbl = new JLabel();
   private JPanel namePnl = new JPanel();
   private JTextField nameTxt = new JTextField();
   private BorderLayout namePnlLayout = new BorderLayout();
   private JPanel directoryPnl = new JPanel();
   private BorderLayout directoryPnlLayout = new BorderLayout();
   private JTextField directoryTxt = new JTextField();
   private JButton directoryBrowseBtn = new JButton();
   private JPanel includesPnl = new JPanel();
   private BorderLayout includesPnlLayout = new BorderLayout();
   private JScrollPane includesListSP = new JScrollPane();
   /*
   private JTable includesList = new JTable()
   {
      public Dimension getPreferredScrollableViewportSize()
      {
         Dimension pref_size = getPreferredSize();
         if (pref_size.width < 450)
         {
            pref_size.width = 540;
         }
         if (pref_size.height < getRowHeight()*5)
         {
            pref_size.height = getRowHeight()*5;
         }
         return pref_size;
      }
   };
   private JPanel includesListCtrlPnl = new JPanel();
   private JButton includesListMoveUpBtn = new JButton();
   private JButton includesListMoveDownBtn = new JButton();
   private JButton includesListRemoveBtn = new JButton();
   private JButton includesListNewBtn = new JButton();
   */
   private JPanel buttonPnl = new JPanel();
   private JButton cancelBtn = new JButton();
   private JButton okBtn = new JButton();
   private JFileChooser fileChooser = new JFileChooser();

   private EnvironmentService mEnvService = new EnvironmentServiceProxy();

   /**
    * The return value from the last time the dialog was shown.
    */
   private int returnValue = ERROR_OPTION;

   /**
    * The title of the dialog.
    */
   private String dialogTitle;

   /**
    * The dialog that is currently showing this component.
    */
   private transient JDialog dialog;

   /**
    * The mutable data model for the list.
    */
   /*
    private IncludesTableModel includesTableModel = new IncludesTableModel();
   */

   /**
    * A specialized table model for the includes list (table).
    */
   /*
   private class IncludesTableModel
      extends AbstractTableModel
   {
      private static final int MIN_ROWS = 1;

      public void add(Object elt)
      {
         mData.add(elt);
         int new_row = mData.size()-1;
         this.fireTableRowsInserted(new_row, new_row);
      }

      public Object remove(int idx)
      {
         System.out.println("mData: " + mData);
         Object obj = mData.remove(idx);
         System.out.println("Removing: " + obj + " at " + idx);
         System.out.println("mData: " + mData);
         this.fireTableRowsDeleted(idx, idx);
         return obj;
      }

      public int size()
      {
         return mData.size();
      }

      public void insert(Object elt, int idx)
      {
         System.out.println("Inserting: " + elt + " at " + idx);
         System.out.println("mData: " + mData);
         mData.add(idx, elt);
         System.out.println("mData: " + mData);
         this.fireTableRowsUpdated(0, mData.size()-1);
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
            mData.set(row, obj);
            fireTableRowsUpdated(row, row);
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
*/
}

/**
 * Custom cell editor for editing a filename.
 */

//class FileCellEditor
//   extends AbstractCellEditor
//   implements TableCellEditor
//{
   /**
    * Creates a new file cell editor that will use a default file chooser when
    * the user chooses to select a file.
    */
//   public FileCellEditor()
//   {
//      this(new JFileChooser());
//   }

   /**
    * Creates a new file cell editor that will use the given file chooser when
    * the user chooses to select a file.
    */
/*
   public FileCellEditor(JFileChooser chooser)
   {
      mChooser = chooser;

      // Init the filename textfield
      mFilenameTxt.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            stopCellEditing();
         }
      });
*/
      /*
      mFilenameTxt.addFocusListener(new FocusAdapter()
      {
         public void focusLost(FocusEvent evt)
         {
            stopCellEditing();
         }
      });
      */
/*      
      // Init the browse button
      mBrowseBtn.setText("...");
      mBrowseBtn.setMargin(new Insets(2, 2, 2, 2));
      mBrowseBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            // Point the file chooser at the directory containing the file that
            // is the current value in the cell if it exists
            File cur_file = new File((String)getCellEditorValue());
            if (cur_file.exists())
            {
               if (! cur_file.isDirectory() && cur_file.getParentFile() != null)
               {
                  cur_file = cur_file.getParentFile();
               }
               mChooser.setCurrentDirectory(cur_file);
            }


            // Setup the file chooser options
            mChooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
            mChooser.setFileHidingEnabled(false);
            mChooser.setDialogTitle("Select Directory");

            int result = mChooser.showOpenDialog((Component)evt.getSource());
            if (result == JFileChooser.APPROVE_OPTION)
            {
               mFilenameTxt.setText(mChooser.getSelectedFile().getAbsolutePath());
            }
         }
      });
      mEditPnl.setLayout(new BorderLayout());
      mEditPnl.add(mFilenameTxt, BorderLayout.CENTER);
      mEditPnl.add(mBrowseBtn, BorderLayout.EAST);
   }

   public Component getTableCellEditorComponent(JTable table,
                                                Object value,
                                                boolean selected,
                                                int row,
                                                int col)
   {
      mFilenameTxt.setText((String)value);
      mFilenameTxt.selectAll();
      mFilenameTxt.requestFocus();
      mEditPnl.revalidate();
      return mEditPnl;
   }
*/

   /**
    * Editing is invalid if the file does not exist.
    */
   /*
   public boolean stopCellEditing()
   {
      File file = new File((String)getCellEditorValue());
      // If the file does not exist or it is not a directory inform the user
      // that they have entered an invalid location.
      if (!file.exists() || !file.isDirectory())
      {
         JOptionPane.showMessageDialog(null, getCellEditorValue() + " is not a valid directory");
         mFilenameTxt.selectAll();
         mFilenameTxt.requestFocus();
         return false;
      }
      return super.stopCellEditing();
   }

   public Object getCellEditorValue()
   {
      return mFilenameTxt.getText();
   }
   */

   /** The filename text field. */
   //private JTextField mFilenameTxt = new JTextField();

   /** The browse button. */
   //private JButton mBrowseBtn = new JButton();

   /** The panel containing the filename textfield and browse button. */
   //private JPanel mEditPnl = new JPanel();

   /** The file chooser to use with this editor. */
   //private JFileChooser mChooser;
//}
