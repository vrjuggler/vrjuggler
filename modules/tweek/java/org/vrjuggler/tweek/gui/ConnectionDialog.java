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
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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
import javax.swing.*;
import javax.swing.border.*;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.table.DefaultTableModel;
import org.vrjuggler.tweek.beans.BeanRegistry;
import org.vrjuggler.tweek.beans.TweekBean;
import org.vrjuggler.tweek.services.EnvironmentService;
import org.vrjuggler.tweek.services.GlobalPreferencesService;
import org.vrjuggler.tweek.net.corba.CorbaService;


/**
 * A modal dialog box used to make a connection with a remote ORB.  The Tweek
 * CORBA Service created as a result of the connection is made available to
 * external code.
 *
 * @since 1.0
 */
public class ConnectionDialog extends JDialog
{
   public ConnectionDialog (Frame owner, String title)
   {
      super(owner, title);

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
      m_ns_host_field.setText(prefs.getDefaultCorbaHost());
      m_ns_port_field.setText(String.valueOf(prefs.getDefaultCorbaPort()));

      // Add an input validator for the port number field.
      m_ns_port_field.setInputVerifier(new InputVerifier()
         {
            public boolean verify(JComponent input)
            {
               JTextField tf = (JTextField) input;
               return validatePortNumber(tf.getText());
            }
         });

      this.setModal(true);
      this.setLocationRelativeTo(owner);
   }

   public void display ()
   {
      this.pack();
      this.setVisible(true);
   }

   public int getStatus ()
   {
      return status;
   }

   public String getNameServiceHost ()
   {
      return nameServiceHost;
   }

   public int getNameServicePort ()
   {
      return nameServicePort;
   }

   public String getNamingSubcontext ()
   {
      return namingSubcontext;
   }

   public CorbaService getCorbaService()
   {
      return corbaService;
   }

   public static final int OK_OPTION     = JOptionPane.OK_OPTION;
   public static final int CANCEL_OPTION = JOptionPane.CANCEL_OPTION;
   public static final int CLOSED_OPTION = JOptionPane.CLOSED_OPTION;

   protected void processWindowEvent (WindowEvent e)
   {
      super.processWindowEvent(e);

      if (e.getID() == WindowEvent.WINDOW_CLOSING)
      {
         status = CLOSED_OPTION;
      }
   }

   private void jbInit() throws Exception
   {
      mNSConnectBorder = new TitledBorder(new EtchedBorder(EtchedBorder.RAISED,Color.white,new Color(142, 142, 142)),"Naming Service Connection");
      mSubjectMgrBorder = new TitledBorder(new EtchedBorder(EtchedBorder.RAISED,Color.white,new Color(142, 142, 142)),"Subject Manager Choice");
      mNSConnectPanel.setLayout(gridBagLayout1);
      mNSConnectPanel.setBorder(mNSConnectBorder);
      mNSConnectPanel.setMinimumSize(new Dimension(450, 150));
      mNSConnectPanel.setPreferredSize(new Dimension(450, 155));

      m_ns_host_label.setHorizontalAlignment(SwingConstants.TRAILING);
      m_ns_host_label.setText("Naming Service Host");
      m_ns_host_field.setMinimumSize(new Dimension(80, 17));
      m_ns_host_field.setPreferredSize(new Dimension(180, 17));
      m_ns_host_field.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            validateNetworkAddress(e);
         }
      });
      m_ns_port_field.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            validateNetworkAddress(e);
         }
      });
      mSubjectMgrPanel.setBorder(mSubjectMgrBorder);
      mSubjectMgrPanel.setMinimumSize(new Dimension(450, 200));
      mSubjectMgrPanel.setPreferredSize(new Dimension(450, 200));
      mSubjectMgrPanel.setLayout(mSubjectMgrLayout);
      mSubjectMgrSplitPane.setDividerSize(7);
      mNSConnectButton.setEnabled(false);
      mNSConnectButton.setText("Connect");
      mNSConnectButton.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            namingServiceConnectAction(e);
         }
      });
      m_ok_button.setEnabled(false);
      mSubjectMgrList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
      m_button_panel.setMinimumSize(new Dimension(450, 33));
      m_button_panel.setPreferredSize(new Dimension(450, 33));
      mSubjectMgrListPane.setMinimumSize(new Dimension(100, 90));
      mSubjectMgrListPane.setPreferredSize(new Dimension(180, 90));
      mSubjectMgrInfo.setMinimumSize(new Dimension(100, 90));
      mSubjectMgrInfo.setPreferredSize(new Dimension(180, 90));
      m_ns_port_label.setHorizontalAlignment(SwingConstants.TRAILING);
      m_naming_label.setHorizontalAlignment(SwingConstants.TRAILING);
      mNSConnectPanel.add(m_ns_host_label,         new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0
            ,GridBagConstraints.CENTER, GridBagConstraints.NONE, new Insets(0, 0, 0, 2), 47, 18));
      mNSConnectPanel.add(m_ns_host_field,       new GridBagConstraints(1, 0, 1, 1, 1.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, new Insets(0, 0, 0, 0), 14, 6));

      m_ns_port_label.setText("Naming Service Port");
      m_ns_port_field.setMinimumSize(new Dimension(50, 17));
      m_ns_port_field.setPreferredSize(new Dimension(50, 17));
      mNSConnectPanel.add(m_ns_port_label,    new GridBagConstraints(0, 1, 1, 1, 0.0, 0.0
            ,GridBagConstraints.CENTER, GridBagConstraints.NONE, new Insets(0, 0, 0, 2), 51, 18));
      mNSConnectPanel.add(m_ns_port_field,            new GridBagConstraints(1, 1, 1, 1, 1.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 10, 6));

      m_naming_label.setText("Naming Subcontext");
      m_naming_field.setMinimumSize(new Dimension(80, 17));
      m_naming_field.setPreferredSize(new Dimension(150, 17));


      m_ok_button.setText("OK");
      m_ok_button.setMnemonic('O');
      m_ok_button.setSelected(true);
      m_ok_button.addActionListener(new ActionListener()
      {
         public void actionPerformed (ActionEvent e)
         {
            okButtonAction(e);
         }
      });

      m_cancel_button.setText("Cancel");
      m_cancel_button.setMnemonic('C');
      m_cancel_button.addActionListener(new ActionListener()
      {
         public void actionPerformed (ActionEvent e)
         {
            cancelButtonAction(e);
         }
      });

      this.getContentPane().add(mNSConnectPanel,  BorderLayout.NORTH);

      mNSConnectPanel.add(m_naming_label,   new GridBagConstraints(0, 2, 1, 1, 0.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(0, 0, 0, 2), 56, 18));
      mNSConnectPanel.add(m_naming_field,    new GridBagConstraints(1, 2, 1, 1, 1.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, new Insets(0, 0, 0, 0), 44, 6));
      mNSConnectPanel.add(mNSConnectButton,          new GridBagConstraints(0, 3, 2, 1, 0.0, 0.0
            ,GridBagConstraints.CENTER, GridBagConstraints.NONE, new Insets(5, 0, 2, 0), 45, 5));

      this.getContentPane().add(m_button_panel, BorderLayout.SOUTH);
      m_button_panel.add(m_ok_button, null);
      m_button_panel.add(m_cancel_button, null);
      this.getContentPane().add(mSubjectMgrPanel, BorderLayout.CENTER);
      mSubjectMgrPanel.add(mSubjectMgrSplitPane,  BorderLayout.CENTER);
      mSubjectMgrSplitPane.add(mSubjectMgrListPane, JSplitPane.LEFT);
      mSubjectMgrSplitPane.add(mSubjectMgrInfo, JSplitPane.RIGHT);
   }

   /**
    * Handles the event generated by the user clicking the "Connect" button in
    * the Naming Service connection panel.
    */
   private void namingServiceConnectAction(ActionEvent e)
   {
      // Commit the information provided by the user in the data entry fields.
      commitConnectInfo();

      // Create a new CORBA service using the information provided by the user.
      CorbaService new_orb = new CorbaService(this.getNameServiceHost(),
                                              this.getNameServicePort(),
                                              this.getNamingSubcontext());

      TweekBean service = BeanRegistry.instance().getBean("Environment");

      try
      {
         // If we have the Tweek Environment Service, and we should, initialize
         // the new CORBA service with the command line arguments passed when
         // the Tweek GUI was started.
         if ( null != service )
         {
            EnvironmentService env_service = (EnvironmentService) service;
            new_orb.init(env_service.getCommandLineArgs());
         }
         else
         {
            new_orb.init(null);
         }

         // XXX: Is this the best time to do this?  We want to be sure that
         // anyone who calls getCorbaService() gets a valid reference back.  If
         // something goes wrong later with the Subject Manager choice, this
         // should be set to null.
         corbaService = new_orb;

         // XXX: Should we allow the user to make multiple connections from a
         // single dialog box?  Probably not...
         mNSConnectButton.setEnabled(false);

         // Create a new list model for the fresh list of Subject Managers.
         DefaultListModel mgr_list_model = new DefaultListModel();

         // Get the list of Subject Managers.  There had better be at least one!
         java.util.Iterator i = new_orb.getSubjectManagerList().iterator();
         tweek.SubjectManager cur_mgr;

         while ( i.hasNext() )
         {
            cur_mgr = (tweek.SubjectManager) i.next();
            mgr_list_model.addElement(new SubjectManagerWrapper(cur_mgr));
         }

         mSubjectMgrList.setModel(mgr_list_model);
      }
      // Something went wrong with the CORBA service initialization.
      catch (org.omg.CORBA.SystemException sys_ex)
      {
         // Disable the Naming Service connection button just to be safe.
         mNSConnectButton.setEnabled(false);
         JOptionPane.showMessageDialog(null, "CORBA System Exception: '" +
                                       sys_ex.getMessage() + "'",
                                       "CORBA System Exception",
                                       JOptionPane.ERROR_MESSAGE);

         // DEBUG
         sys_ex.printStackTrace();
      }
   }

   /**
    * Commits the user-specified information from the text fields to this
    * object's properties.  After calling this method, those properties can be
    * considered up to date and usable.
    */
   private void commitConnectInfo()
   {
      nameServiceHost  = m_ns_host_field.getText();
      nameServicePort  = Integer.parseInt(m_ns_port_field.getText());
      namingSubcontext = m_naming_field.getText();
   }

   private void okButtonAction (ActionEvent e)
   {
      status = OK_OPTION;
      commit();
      setVisible(false);
   }

   /**
    * Sets the close status to CANCEL_OPTION and closes this dialog box.  If
    * an ORB is running, it is shut down.
    */
   private void cancelButtonAction (ActionEvent e)
   {
      // If we have an ORB running, we have to shut it down.
      if ( null != corbaService )
      {
         corbaService.shutdown(true);
         corbaService = null;
      }

      status = CANCEL_OPTION;
      setVisible(false);
   }

   private void commit()
   {
      if ( null != mSubjectManager )
      {
         corbaService.setSubjectManager(mSubjectManager);
      }
   }

   /**
    * Validates that the network address (hostname and port number) entered
    * by the user.  If the network address is valid, then the Naming Service
    * connection button is enabled.  Otherwise, it is disabled.
    */
   private void validateNetworkAddress(ActionEvent e)
   {
      if ( ! m_ns_host_field.getText().equals("") &&
           ! m_ns_port_field.getText().equals("") )
      {
         mNSConnectButton.setEnabled(true);
      }
      else
      {
         mNSConnectButton.setEnabled(false);
      }
   }

   /**
    * Verifies that the given string is a valid port number.
    */
   private boolean validatePortNumber(String port)
   {
      System.out.println("Validating " + port);
      boolean valid = false;

      try
      {
         int port_num = Integer.getInteger(port).intValue();

         if ( port_num > 0 && port_num < 65536 )
         {
            valid = true;
         }
      }
      catch (Exception e)
      {
         // Invalid port number.
      }

      return valid;
   }

   /**
    * An implementation of ListSelectionListener for use with the list of
    * known Tweek Subject Manager references.  When a Subject Manager is
    * selected in the list, the table mSubjectMgrInfo is updated to display
    * information queried from that Subject Manager.
    */
   private class SubjectMgrListSelectionListener
      implements ListSelectionListener
   {
      public void valueChanged(ListSelectionEvent e)
      {
         // Pull the tweek.SubjectManager reference from the list.
         int index = mSubjectMgrList.getSelectedIndex();

         // No selection.
         if ( -1 == index )
         {
            m_ok_button.setEnabled(false);
         }
         // New selection.
         else
         {
            m_ok_button.setEnabled(true);

            SubjectManagerWrapper mgr_wrapper =
               (SubjectManagerWrapper) mSubjectMgrList.getModel().getElementAt(index);
            mSubjectManager = mgr_wrapper.getSubjectManager();

            // Fill in the table model for the selected Subject Manager.
            Object[] column_names = new Object[]{"Property", "Value"};
            DefaultTableModel table_model = new DefaultTableModel();
            table_model.setColumnIdentifiers(column_names);
            table_model.addRow(new Object[]{"Hostname",
                                            mSubjectManager.getHostName()});
            table_model.addRow(new Object[]{"Application",
                                            mSubjectManager.getApplicationName()});
            table_model.addRow(new Object[]{"User",
                                            mSubjectManager.getUserName()});

            mSubjectMgrInfo.setModel(table_model);
         }
      }
   }

   // Attributes that may be queried by the class that instantiated us.
   private int          status;
   private String       nameServiceHost  = null;
   private int          nameServicePort  = 2809;
   private String       namingSubcontext = null;
   private CorbaService corbaService     = null;

   // Internal-use properties.
   private tweek.SubjectManager mSubjectManager = null;

   // GUI elements for the Naming Service connection panel.
   private TitledBorder mNSConnectBorder;
   private JPanel       mNSConnectPanel = new JPanel();

   private JLabel     m_ns_host_label  = new JLabel();
   private JTextField m_ns_host_field  = new JTextField();

   private JLabel     m_ns_port_label  = new JLabel();
   private JTextField m_ns_port_field  = new JTextField();

   private JLabel     m_naming_label  = new JLabel();
   private JTextField m_naming_field  = new JTextField();

   // GUI elements for the Subject Manager panel.
   private TitledBorder mSubjectMgrBorder;
   private JPanel       mSubjectMgrPanel     = new JPanel();
   private BorderLayout mSubjectMgrLayout    = new BorderLayout();
   private JSplitPane   mSubjectMgrSplitPane = new JSplitPane();
   private JList        mSubjectMgrList      = new JList();
   private JScrollPane  mSubjectMgrListPane  = new JScrollPane(mSubjectMgrList);
   private JTable       mSubjectMgrInfo      = new JTable();
   private JButton      mNSConnectButton     = new JButton();

   // GUI elements for the OK/Cancel button panel.
   private JPanel  m_button_panel  = new JPanel();
   private JButton m_ok_button     = new JButton();
   private JButton m_cancel_button = new JButton();
   private GridBagLayout gridBagLayout1 = new GridBagLayout();
}
