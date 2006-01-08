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
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import javax.swing.*;
import javax.swing.border.*;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.table.DefaultTableModel;
import org.vrjuggler.tweek.net.corba.CorbaService;
import tweek.SubjectManagerPackage.SubjectMgrInfoItem;


public class DisconnectionDialog extends JDialog
{
   public DisconnectionDialog(Frame owner, String title, java.util.List ORBs)
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

      // Create a new list model for the list of CORBA Service instances.
      DefaultListModel service_list_model = new DefaultListModel();
      java.util.Iterator i = ORBs.iterator();

      while ( i.hasNext() )
      {
         service_list_model.addElement(i.next());
      }

      mCorbaServiceList.setModel(service_list_model);
      this.mCorbaServiceList.addListSelectionListener(new CorbaServiceListSelectionListener());

      this.setModal(true);
      this.pack();
      this.setLocationRelativeTo(owner);
   }

   public int getStatus ()
   {
      return status;
   }

   public CorbaService getSelectedCorbaService()
   {
      return selectedCorbaService;
   }

   public static final int DISCONNECT_OPTION = JOptionPane.OK_OPTION;
   public static final int CANCEL_OPTION     = JOptionPane.CANCEL_OPTION;
   public static final int CLOSED_OPTION     = JOptionPane.CLOSED_OPTION;

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
      mCorbaServiceBorder = new TitledBorder(new EtchedBorder(EtchedBorder.RAISED,Color.white,new Color(142, 142, 142)),"CORBA Service Choice");
      mDisconnectButton.setEnabled(false);
      mDisconnectButton.setMnemonic('D');
      mDisconnectButton.setText("Disconnect");
      mDisconnectButton.addActionListener(new ActionListener()
      {
         public void actionPerformed (ActionEvent e)
         {
            disconnectButtonAction(e);
         }
      });
      mCancelButton.setMnemonic('C');
      mCancelButton.setText("Cancel");
      mCancelButton.addActionListener(new ActionListener()
      {
         public void actionPerformed (ActionEvent e)
         {
            cancelButtonAction(e);
         }
      });
      mCorbaServiceSplitPane.setDividerSize(7);
      mCorbaServiceListPane.setMinimumSize(new Dimension(100, 90));
      mCorbaServiceListPane.setPreferredSize(new Dimension(180, 90));
      mSubjectMgrInfo.setMinimumSize(new Dimension(100, 90));
      mSubjectMgrInfo.setPreferredSize(new Dimension(180, 90));
      mCorbaServicePanel.setMinimumSize(new Dimension(450, 200));
      mCorbaServicePanel.setPreferredSize(new Dimension(450, 200));
      mCorbaServicePanel.setLayout(mCorbaServiceLayout);
      mDisconnectPanel.setLayout(mDisconnectLayout);
      mDisconnectPanel.setBorder(mCorbaServiceBorder);
      this.getContentPane().add(mDisconnectPanel,  BorderLayout.CENTER);
      this.getContentPane().add(mButtonPanel,  BorderLayout.SOUTH);
      mButtonPanel.add(mDisconnectButton, null);
      mButtonPanel.add(mCancelButton, null);
      mCorbaServiceSplitPane.add(mCorbaServiceListPane, JSplitPane.LEFT);
      mCorbaServiceSplitPane.add(mSubjectMgrInfo, JSplitPane.RIGHT);
      mDisconnectPanel.add(mCorbaServicePanel, BorderLayout.CENTER);
      mCorbaServicePanel.add(mCorbaServiceSplitPane, BorderLayout.CENTER);
   }

   private void disconnectButtonAction(ActionEvent e)
   {
      status = DISCONNECT_OPTION;
      dispose();
   }

   /**
    * Sets the close status to CANCEL_OPTION and closes this dialog box.  If
    * an ORB is running, it is shut down.
    */
   private void cancelButtonAction(ActionEvent e)
   {
      selectedCorbaService = null;
      status = CANCEL_OPTION;
      dispose();
   }

   private class CorbaServiceListSelectionListener
      implements ListSelectionListener
   {
      public void valueChanged(ListSelectionEvent e)
      {
         // Pull the tweek.SubjectManager reference from the list.
         int index = mCorbaServiceList.getSelectedIndex();

         // No selection.
         if ( -1 == index )
         {
            mDisconnectButton.setEnabled(false);
         }
         // New selection.
         else
         {
            selectedCorbaService =
               (CorbaService) mCorbaServiceList.getModel().getElementAt(index);
            tweek.SubjectManager subj_mgr = selectedCorbaService.getSubjectManager();

            // Fill in the table model for the selected Subject Manager.
            Object[] column_names = new Object[]{"Property", "Value"};
            DefaultTableModel table_model = new DefaultTableModel();
            table_model.setColumnIdentifiers(column_names);

            try
            {
               SubjectMgrInfoItem[] subj_mgr_info = subj_mgr.getInfo();

               for ( int i = 0; i < subj_mgr_info.length; ++i )
               {
                  table_model.addRow(new Object[]{subj_mgr_info[i].key,
                                                  subj_mgr_info[i].value});
               }

               mSubjectMgrInfo.setModel(table_model);
               mDisconnectButton.setEnabled(true);
            }
            catch (org.omg.CORBA.COMM_FAILURE commEx)
            {
               mDisconnectButton.setEnabled(false);
               String msg = "Invalid CORBA Service Selected";

               // If the exception has an error message, append it.
               if ( ! commEx.getMessage().equals("") )
               {
                  msg += ": '" + commEx.getMessage() + "'";
               }

               JOptionPane.showMessageDialog(null, msg,
                                             "CORBA Communication Exception",
                                             JOptionPane.ERROR_MESSAGE);
            }
         }
      }
   }

   private int          status;
   private CorbaService selectedCorbaService;

   private JPanel       mDisconnectPanel = new JPanel();
   private BorderLayout mDisconnectLayout = new BorderLayout();
   private TitledBorder mCorbaServiceBorder;

   private JSplitPane  mCorbaServiceSplitPane = new JSplitPane();
   private JList       mCorbaServiceList      = new JList();
   private JScrollPane mCorbaServiceListPane  = new JScrollPane(mCorbaServiceList);
   private JTable mSubjectMgrInfo = new JTable();
   private BorderLayout mCorbaServiceLayout = new BorderLayout();
   private JPanel mCorbaServicePanel = new JPanel();

   private JPanel mButtonPanel = new JPanel();
   private JButton mDisconnectButton = new JButton();
   private JButton mCancelButton = new JButton();
}
