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

package org.vrjuggler.tweek.gui;

import java.awt.*;
import java.awt.event.*;
import java.util.Iterator;
import java.util.List;
import java.util.Vector;
import javax.swing.*;
import javax.swing.border.*;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import java.awt.event.FocusListener;
import java.awt.event.FocusEvent;
import javax.swing.table.DefaultTableModel;
import javax.swing.table.AbstractTableModel;
import org.vrjuggler.tweek.services.GlobalPreferencesService;
import org.vrjuggler.tweek.beans.*;


/**
 * @since 0.16.0
 */
public class BeanStatusDialog extends JDialog
{
   public BeanStatusDialog (Frame owner, String title)
   {
      super(owner, title);
      enableEvents(AWTEvent.WINDOW_EVENT_MASK);

      try
      {
         jbInit();
         init();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      this.mActiveBeansList.addListSelectionListener(new BeanListSelectionListener());
      this.mActiveBeansList.addFocusListener(new BeanListSelectionListener());
      this.mPendingBeansList.addListSelectionListener(new BeanListSelectionListener());
      this.mPendingBeansList.addFocusListener(new BeanListSelectionListener());

      this.setModal(true);
      this.pack();
      this.setLocationRelativeTo(owner);
   }

   private void init()
   {
      // Create a new list model for the fresh list of Subject Managers.
      DefaultListModel active_beans_list_model = new DefaultListModel();

      java.util.Iterator i =
         BeanRegistry.instance().getBeans().values().iterator();

      TweekBean cur_bean;

      while ( i.hasNext() )
      {
         cur_bean = (TweekBean) i.next();
         active_beans_list_model.addElement(cur_bean);
      }

      mActiveBeansList.setModel(active_beans_list_model);

      // Create a new list model for the fresh list of Subject Managers.
      DefaultListModel pending_beans_list_model = new DefaultListModel();

      java.util.Iterator j =
         BeanDependencyManager.instance().getPendingBeans().iterator();

      TweekBean pending_bean;

      while ( j.hasNext() )
      {
         pending_bean = (TweekBean) j.next();
         pending_beans_list_model.addElement(pending_bean);
      }

      mPendingBeansList.setModel(pending_beans_list_model);



   }

   /**
    * An implementation of ListSelectionListener for use with the list of
    * known Tweek Subject Manager references.  When a Subject Manager is
    * selected in the list, the table mSubjectMgrInfo is updated to display
    * information queried from that Subject Manager.
    */
   private class BeanListSelectionListener
      implements ListSelectionListener, FocusListener
   {
      public void valueChanged(ListSelectionEvent e)
      {
         JList changed_list = (JList)e.getSource();
         // Pull the tweek.SubjectManager reference from the list.
         int index = changed_list.getSelectedIndex();

         // No selection.
         if ( -1 == index )
         {
            /* Do Nothing */
         }
         // New selection.
         else
         {
            TweekBean bean = (TweekBean) changed_list.getModel().getElementAt(index);

            // Fill in the table model for the selected Subject Manager.
            Object[] column_names = new Object[]{"Property", "Value"};
            DefaultTableModel table_model = new DefaultTableModel();
            table_model.setColumnIdentifiers(column_names);

            // Get attributes for this bean
            BeanAttributes attrib = bean.getAttributes();

            // Construct the info table
            table_model.addRow(new Object[] {"Name", attrib.getName()});
            table_model.addRow(new Object[] {"Jar URL", attrib.getJarURL()});
            table_model.addRow(new Object[] {"Class Name", attrib.getClassname()});

            // Add Bean Dependencies
            List bean_deps = attrib.getBeanDependencies();
            if(bean_deps.size() > 0)
            {
               Iterator itr = bean_deps.iterator();

               BeanDependency dep = (BeanDependency)itr.next();
               String value = dep.getBeanName();
               if (! BeanRegistry.instance().isRegistered(dep.getBeanName()))
               {
                  value = "X " + value;
               }
               table_model.addRow(new Object[] {"Bean Deps", value});

               while(itr.hasNext())
               {
                  dep = (BeanDependency)itr.next();
                  value = dep.getBeanName();
                  if (! BeanRegistry.instance().isRegistered(dep.getBeanName()))
                  {
                     value = "X " + value;
                  }

                  table_model.addRow(new Object[] {"", value});
               }
            }


            // Add Jar Dependencies
            List jar_deps = attrib.getJarDependencies();
            if(jar_deps.size() > 0)
            {
               Iterator itr = jar_deps.iterator();

               JarDependency dep = (JarDependency)itr.next();
               table_model.addRow(new Object[] {"Jar Deps", dep.getPath() + "/" + dep.getFile()});
               while(itr.hasNext())
               {
                  dep = (JarDependency)itr.next();
                  table_model.addRow(new Object[] {"", dep.getPath() + "/" + dep.getFile()});
               }
            }


            //table_model.addRow(new Object[] {"", attrib.getBeanDependencies()});
            //table_model.addRow(new Object[] {"", attrib.getJarDependencies()});

            mBeanInfoTable.setModel(table_model);
         }
      }
      public void focusGained(FocusEvent e)
      {
         valueChanged(new ListSelectionEvent(e.getSource(), 0, 0, false));
      }
      public void focusLost(FocusEvent e)
      {
         /* Do nothing*/
      }
   }

   /**
    * Returns the "close status" of the window.  That is, it informs the
    * caller of the mechnism used to close the dialog.  There are three
    * possible ways this dialog may be closed: using the "OK" button, using
    * the "Cancel" button, or using the window manager.  These are reflected
    * through the integer constants OK_OPTION, and CLOSE_OPTION
    * respectively.
    */
   public int getStatus()
   {
      return status;
   }

   public static final int OK_OPTION     = JOptionPane.OK_OPTION;
   public static final int CLOSED_OPTION = JOptionPane.CLOSED_OPTION;

   protected void processWindowEvent(WindowEvent e)
   {
      if (e.getID() == WindowEvent.WINDOW_CLOSING)
      {
         status = CLOSED_OPTION;
         dispose();
      }

      super.processWindowEvent(e);
   }

   private void jbInit() throws Exception
   {
      mOkayButton.addActionListener(new ActionListener()
      {
         public void actionPerformed (ActionEvent e)
         {
            okButtonAction(e);
         }
      });
    mOkayButton.setSelected(true);
    mOkayButton.setMnemonic('O');
    mOkayButton.setText("OK");
    mButtonPanel.setPreferredSize(new Dimension(450, 33));
    mButtonPanel.setMinimumSize(new Dimension(450, 33));
    this.getContentPane().setLayout(mFrameBorderLayout);
    mBeanInfoPane.setMinimumSize(new Dimension(100, 90));
    mBeanInfoPane.setPreferredSize(new Dimension(180, 90));
    mActiveBeansListPane.setMinimumSize(new Dimension(100, 90));
    mActiveBeansListPane.setPreferredSize(new Dimension(180, 90));
    mBeansSplitPane.setDividerSize(7);
    mMainSplitPane.setOrientation(JSplitPane.VERTICAL_SPLIT);
    mMainSplitPane.setDividerSize(7);
    mPendingBeansListPane.setPreferredSize(new Dimension(180, 90));
    mPendingBeansListPane.setMinimumSize(new Dimension(100, 90));
    mPendingBeansPanel.setLayout(mPendingBeansLayout);
    mPendingBeansPanel.setPreferredSize(new Dimension(450, 200));
    mPendingBeansPanel.setMinimumSize(new Dimension(450, 200));
    mPendingPanel.setLayout(mPendingPanelBorderLayout);
    mActivePanel.setLayout(mActivePanelBorderLayout);
    mActiveLabel.setText("Active Beans");
    mPendingLabel.setText("Pending Beans");
    this.getContentPane().add(mButtonPanel,  BorderLayout.SOUTH);
    mButtonPanel.add(mOkayButton, null);
    this.getContentPane().add(mMainSplitPane,  BorderLayout.CENTER);
    mBeansSplitPane.add(mPendingPanel, JSplitPane.LEFT);
    mPendingPanel.add(mPendingBeansListPane, BorderLayout.CENTER);
    mPendingPanel.add(mPendingLabel, BorderLayout.NORTH);
    mBeansSplitPane.add(mActivePanel, JSplitPane.RIGHT);
    mActivePanel.add(mActiveBeansListPane, BorderLayout.CENTER);
    mActivePanel.add(mActiveLabel, BorderLayout.NORTH);
    mMainSplitPane.add(mPendingBeansPanel,  JSplitPane.TOP);
    mPendingBeansPanel.add(mBeansSplitPane, BorderLayout.CENTER);
    mMainSplitPane.add(mBeanInfoPane, JSplitPane.BOTTOM);
   }

   private void okButtonAction (ActionEvent e)
   {
      status = OK_OPTION;
      dispose();
   }

   // =========================================================================
   // Private data members.
   // =========================================================================

   private int status;

   private JPanel       mActivePanel            = new JPanel();
   private JPanel       mPendingPanel           = new JPanel();
   private JButton      mOkayButton             = new JButton();
   private JPanel       mButtonPanel            = new JPanel();
   private BorderLayout mFrameBorderLayout      = new BorderLayout();
   private JSplitPane   mMainSplitPane          = new JSplitPane();
   private JTable       mBeanInfoTable          = new JTable();
   private JScrollPane  mBeanInfoPane           = new JScrollPane(mBeanInfoTable);
   private JList        mActiveBeansList        = new JList();
   private JScrollPane  mActiveBeansListPane    = new JScrollPane(mActiveBeansList);
   private JSplitPane   mBeansSplitPane         = new JSplitPane();
   private JList        mPendingBeansList       = new JList();
   private JScrollPane  mPendingBeansListPane   = new JScrollPane(mPendingBeansList);
   private JPanel       mPendingBeansPanel      = new JPanel();
   private BorderLayout mPendingBeansLayout     = new BorderLayout();
   private BorderLayout mPendingPanelBorderLayout  = new BorderLayout();
   private BorderLayout mActivePanelBorderLayout   = new BorderLayout();
   private JLabel       mActiveLabel            = new JLabel();
   private JLabel       mPendingLabel           = new JLabel();

   void mOkayButton_actionPerformed(ActionEvent e)
   {
   }
}
