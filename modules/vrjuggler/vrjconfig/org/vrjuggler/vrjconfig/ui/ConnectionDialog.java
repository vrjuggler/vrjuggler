/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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
package org.vrjuggler.vrjconfig.ui;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.table.DefaultTableModel;
import org.vrjuggler.tweek.beans.BeanRegistry;
import org.vrjuggler.tweek.services.GlobalPreferencesService;

import org.vrjuggler.jccl.rtrc.RTRCDataSource;
import org.vrjuggler.jccl.rtrc.RTRCDataSourceBroker;
import org.vrjuggler.jccl.rtrc.RTRCDataSourceBrokerProxy;

/**
 * A modal dialog box used to choose the RTRCDataSource that we want to use.
 *
 * @since 1.0
 */
public class ConnectionDialog extends JDialog
{
   public ConnectionDialog(Frame owner, String title)
   {
      super(owner, title);

      enableEvents(AWTEvent.WINDOW_EVENT_MASK);

      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      this.mSubjectMgrList.addListSelectionListener(new SubjectMgrListSelectionListener());

      // Set defaults for the Naming Service host and the port number.
      GlobalPreferencesService prefs =
         (GlobalPreferencesService) BeanRegistry.instance().getBean("GlobalPreferences");

      addDataSources();

      this.setModal(true);
      this.pack();
      this.setLocationRelativeTo(owner);
   }

   private void addDataSources()
   {
      // Create a new list model for the fresh list of Subject Managers.
      DefaultListModel mgr_list_model = new DefaultListModel();

      RTRCDataSourceBroker RTRCBroker = new RTRCDataSourceBrokerProxy();
      
      java.util.Iterator i = RTRCBroker.getDataSources().iterator();
      RTRCDataSource cur_mgr;

      while ( i.hasNext() )
      {
         cur_mgr = (RTRCDataSource) i.next();
         mgr_list_model.addElement(cur_mgr);
      }

      mSubjectMgrList.setModel(mgr_list_model);
   }

   public int getStatus()
   {
      return status;
   }
   
   public RTRCDataSource getDataSource()
   {
      return mRTRCDataSource;
   }

   public static final int OK_OPTION     = JOptionPane.OK_OPTION;
   public static final int CANCEL_OPTION = JOptionPane.CANCEL_OPTION;
   public static final int CLOSED_OPTION = JOptionPane.CLOSED_OPTION;

   protected void processWindowEvent(WindowEvent e)
   {
      if ( e.getID() == WindowEvent.WINDOW_CLOSING )
      {
         status = CLOSED_OPTION;
         dispose();
      }

      super.processWindowEvent(e);
   }

   private void jbInit() throws Exception
   {
      mSubjectMgrBorder = new TitledBorder(new EtchedBorder(EtchedBorder.RAISED,Color.white,new Color(142, 142, 142)),"Subject Manager Choice");
      mSubjectMgrPanel.setBorder(mSubjectMgrBorder);
      mSubjectMgrPanel.setMinimumSize(new Dimension(450, 200));
      mSubjectMgrPanel.setPreferredSize(new Dimension(450, 200));
      mSubjectMgrPanel.setLayout(mSubjectMgrLayout);
      mSubjectMgrSplitPane.setDividerSize(7);
      mOkayButton.setEnabled(false);
      mSubjectMgrList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
      mButtonPanel.setMinimumSize(new Dimension(450, 33));
      mButtonPanel.setPreferredSize(new Dimension(450, 33));
      mSubjectMgrListPane.setMinimumSize(new Dimension(100, 90));
      mSubjectMgrListPane.setPreferredSize(new Dimension(180, 90));
      mSubjectMgrInfoPane.setMinimumSize(new Dimension(100, 90));
      mSubjectMgrInfoPane.setPreferredSize(new Dimension(180, 90));

      mOkayButton.setText("OK");
      mOkayButton.setMnemonic('O');
      mOkayButton.setSelected(true);
      mOkayButton.addActionListener(new ActionListener()
      {
         public void actionPerformed (ActionEvent e)
         {
            okButtonAction(e);
         }
      });

      mCancelButton.setText("Cancel");
      mCancelButton.setMnemonic('C');
      mCancelButton.addActionListener(new ActionListener()
      {
         public void actionPerformed (ActionEvent e)
         {
            cancelButtonAction(e);
         }
      });
  
      this.getContentPane().add(mButtonPanel, BorderLayout.SOUTH);
      mButtonPanel.add(mOkayButton, null);
      mButtonPanel.add(mCancelButton, null);
      this.getContentPane().add(mSubjectMgrPanel, BorderLayout.CENTER);
      mSubjectMgrPanel.add(mSubjectMgrSplitPane,  BorderLayout.CENTER);
      mSubjectMgrSplitPane.add(mSubjectMgrListPane, JSplitPane.LEFT);
      mSubjectMgrSplitPane.add(mSubjectMgrInfoPane, JSplitPane.RIGHT);
   }

   private void okButtonAction(ActionEvent e)
   {
      status = OK_OPTION;
      dispose();
   }

   /**
    * Sets the close status to CANCEL_OPTION and closes this dialog box.  If
    * an ORB is running, it is shut down.
    */
   private void cancelButtonAction(ActionEvent e)
   {
      status = CANCEL_OPTION;
      dispose();
   }

   /**
    * An implementation of ListSelectionListener for use with the list of
    * known RTRCDataSources.  When a Data Source is
    * selected in the list, the table mSubjectMgrInfo is updated to display
    * information queried from that RTRCDataSource.
    */
   private class SubjectMgrListSelectionListener
      implements ListSelectionListener
   {
      public void valueChanged(ListSelectionEvent e)
      {
         // Pull the RTRCDataSource reference from the list.
         int index = mSubjectMgrList.getSelectedIndex();

         // No selection.
         if ( -1 == index )
         {
            mOkayButton.setEnabled(false);
         }
         // New selection.
         else
         {
            mOkayButton.setEnabled(true);
            mRTRCDataSource = (RTRCDataSource)mSubjectMgrList.getModel().getElementAt(index);
         }
      }
   }

   // Attributes that may be queried by the class that instantiated us.
   private int          status;

   private RTRCDataSource mRTRCDataSource = null;

   // GUI elements for the Subject Manager panel.
   private TitledBorder mSubjectMgrBorder;
   private JPanel       mSubjectMgrPanel     = new JPanel();
   private BorderLayout mSubjectMgrLayout    = new BorderLayout();
   private JSplitPane   mSubjectMgrSplitPane = new JSplitPane();
   private JList        mSubjectMgrList      = new JList();
   private JScrollPane  mSubjectMgrListPane  = new JScrollPane(mSubjectMgrList);
   private JTable       mSubjectMgrInfo      = new JTable();
   private JScrollPane  mSubjectMgrInfoPane  = new JScrollPane(mSubjectMgrInfo);
   private JButton      mNSConnectButton     = new JButton();

   // GUI elements for the OK/Cancel button panel.
   private JPanel  mButtonPanel  = new JPanel();
   private JButton mOkayButton   = new JButton();
   private JButton mCancelButton = new JButton();
}
