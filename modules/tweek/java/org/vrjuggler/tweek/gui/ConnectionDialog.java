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
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.tweek.gui;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.table.DefaultTableModel;
import org.omg.CosNaming.NamingContextPackage.*;
import org.vrjuggler.tweek.beans.BeanRegistry;
import org.vrjuggler.tweek.beans.TweekBean;
import org.vrjuggler.tweek.services.*;
import org.vrjuggler.tweek.net.corba.CorbaService;
import tweek.SubjectManagerPackage.SubjectMgrInfoItem;
import org.vrjuggler.tweek.TweekCore;
import org.vrjuggler.tweek.services.EnvironmentServiceProxy;
import org.vrjuggler.tweek.text.MessageDocument;


/**
 * A modal dialog box used to make a connection with a remote ORB.  The Tweek
 * CORBA Service created as a result of the connection is made available to
 * external code.
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

      mNSHostField.getDocument().addDocumentListener(new AddressFieldValidator());
      mNSPortField.getDocument().addDocumentListener(new AddressFieldValidator());

      this.mSubjectMgrList.addListSelectionListener(new SubjectMgrListSelectionListener(this));

      // Set defaults for the Naming Service host and the port number.
      try
      {
         GlobalPreferencesService prefs = new GlobalPreferencesServiceProxy();

         mNSHostField.setText(prefs.getDefaultCorbaHost());
         mNSPortField.setText(String.valueOf(prefs.getDefaultCorbaPort()));
         mNSIiopVerField.setText(String.valueOf(prefs.getDefaultIiopVersion()));
      }
      // D'oh!  No defaults can be set.
      catch(Exception ex)
      {
         JOptionPane.showMessageDialog(this, "Could not set defaults '" +
                                       ex.getMessage() + "'",
                                       "Tweek GlobalPreferences Exception",
                                       JOptionPane.WARNING_MESSAGE);
      }

      // At this point, we may have valid Naming Service connection
      // information, so we should validate the network address.
      validateNetworkAddress();

      // Add an input validator for the port number field.
      mNSPortField.setInputVerifier(new InputVerifier()
         {
            public boolean verify(JComponent input)
            {
               JTextField tf = (JTextField) input;
               return validatePortNumber(tf.getText());
            }
         });

      this.setModal(true);
      this.pack();
      this.setLocationRelativeTo(owner);
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

   public String getNameServiceIiopVersion()
   {
      return nameServiceIiopVersion;
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
      mNSConnectBorder = new TitledBorder(new EtchedBorder(EtchedBorder.RAISED,Color.white,new Color(142, 142, 142)),"Naming Service Connection");
      mSubjectMgrBorder = new TitledBorder(new EtchedBorder(EtchedBorder.RAISED,Color.white,new Color(142, 142, 142)),"Subject Manager Choice");
      mNSConnectPanel.setLayout(mNSConnectLayout);
      mNSConnectPanel.setBorder(mNSConnectBorder);
      mNSConnectPanel.setMinimumSize(new Dimension(450, 175));

      mNSHostLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mNSHostLabel.setLabelFor(mNSHostField);
      mNSHostLabel.setText("Naming Service Host");
      mNSHostField.setMinimumSize(new Dimension(80, 17));
      mNSHostField.setPreferredSize(new Dimension(180, 17));
      mNSHostField.addFocusListener(new java.awt.event.FocusAdapter()
      {
         public void focusLost(FocusEvent e)
         {
            validateNetworkAddress();
         }
      });
      mNSHostField.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            validateNetworkAddress();
         }
      });
      mNSPortField.addFocusListener(new java.awt.event.FocusAdapter()
      {
         public void focusLost(FocusEvent e)
         {
            validateNetworkAddress();
         }
      });
      mNSPortField.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            validateNetworkAddress();
         }
      });
      mSubjectMgrPanel.setBorder(mSubjectMgrBorder);
      mSubjectMgrPanel.setMinimumSize(new Dimension(450, 200));
      mSubjectMgrPanel.setPreferredSize(new Dimension(450, 200));
      mSubjectMgrPanel.setLayout(mSubjectMgrLayout);
      mSubjectMgrSplitPane.setDividerSize(7);
      mNSConnectButton.setEnabled(false);
      mNSConnectButton.setSelected(true);
      mNSConnectButton.setText("Connect");
      mNSConnectButton.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            namingServiceConnectAction(e);
         }
      });
      mOkayButton.setEnabled(false);
      mSubjectMgrList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
      mButtonPanel.setMinimumSize(new Dimension(450, 33));
      mButtonPanel.setPreferredSize(new Dimension(450, 33));
      mSubjectMgrListPane.setMinimumSize(new Dimension(100, 90));
      mSubjectMgrListPane.setPreferredSize(new Dimension(180, 90));
      mSubjectMgrInfoPane.setMinimumSize(new Dimension(100, 90));
      mSubjectMgrInfoPane.setPreferredSize(new Dimension(180, 90));
      mNSPortLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mNSPortLabel.setLabelFor(mNSPortField);
      mNamingContextLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mNamingContextLabel.setLabelFor(mNamingContextField);
      mNSIiopVerLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mNSIiopVerLabel.setLabelFor(mNSIiopVerField);
      mNSIiopVerLabel.setText("IIOP Version");
      mNSIiopVerField.addFocusListener(new java.awt.event.FocusAdapter()
      {
         public void focusLost(FocusEvent e)
         {
            validateNetworkAddress();
         }
      });
      mNSIiopVerField.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            validateNetworkAddress();
         }
      });
      mNSRefreshButton.setEnabled(false);
      mNSRefreshButton.setText("Refresh");
      mNSRefreshButton.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            namingServiceRefreshAction();
         }
      });
      mNSConnectPanel.add(mNSHostLabel, new GridBagConstraints(0, 0, 2, 1, 0.0, 0.0
            ,GridBagConstraints.EAST, GridBagConstraints.NONE, new Insets(0, 0, 0, 2), 47, 18));
      mNSConnectPanel.add(mNSHostField,        new GridBagConstraints(2, 0, 1, 1, 1.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, new Insets(0, 0, 0, 0), 14, 6));

      mNSPortLabel.setText("Naming Service Port");
      mNSPortField.setMinimumSize(new Dimension(50, 17));
      mNSPortField.setPreferredSize(new Dimension(50, 17));
      mNSConnectPanel.add(mNSPortLabel,       new GridBagConstraints(0, 1, 2, 1, 0.0, 0.0
            ,GridBagConstraints.EAST, GridBagConstraints.NONE, new Insets(0, 0, 0, 2), 0, 18));
      mNSConnectPanel.add(mNSPortField,             new GridBagConstraints(2, 1, 1, 1, 1.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 10, 6));

      mNSIiopVerField.setMinimumSize(new Dimension(50, 17));
      mNSIiopVerField.setPreferredSize(new Dimension(50, 17));
      mNSConnectPanel.add(mNSIiopVerLabel,      new GridBagConstraints(0, 2, 2, 1, 0.0, 0.0
            ,GridBagConstraints.EAST, GridBagConstraints.NONE, new Insets(0, 0, 0, 2), 0, 18));
      mNSConnectPanel.add(mNSIiopVerField,  new GridBagConstraints(2, 2, 1, 1, 1.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 10, 6));

      mNamingContextLabel.setText("Naming Subcontext");
      mNamingContextField.setMinimumSize(new Dimension(80, 17));
      mNamingContextField.setPreferredSize(new Dimension(150, 17));

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

      this.getContentPane().add(mNSConnectPanel,  BorderLayout.NORTH);

      mNSConnectPanel.add(mNamingContextLabel,     new GridBagConstraints(0, 3, 2, 1, 0.0, 0.0
            ,GridBagConstraints.EAST, GridBagConstraints.NONE, new Insets(0, 0, 0, 2), 0, 18));
      mNSConnectPanel.add(mNamingContextField,     new GridBagConstraints(2, 3, 1, 1, 1.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, new Insets(0, 0, 0, 0), 44, 6));
      mNSConnectPanel.add(mNSButtonPanel, new GridBagConstraints(1, 4, 2, 1, 0.0, 0.0
            ,GridBagConstraints.CENTER, GridBagConstraints.BOTH, new Insets(0, 0, 0, 0), 0, 0));
      mNSButtonPanel.add(mNSConnectButton, null);
      mNSButtonPanel.add(mNSRefreshButton, null);

      this.getContentPane().add(mButtonPanel, BorderLayout.SOUTH);
      mButtonPanel.add(mOkayButton, null);
      mButtonPanel.add(mCancelButton, null);
      this.getContentPane().add(mSubjectMgrPanel, BorderLayout.CENTER);
      mSubjectMgrPanel.add(mSubjectMgrSplitPane,  BorderLayout.CENTER);
      mSubjectMgrSplitPane.add(mSubjectMgrListPane, JSplitPane.LEFT);
      mSubjectMgrSplitPane.add(mSubjectMgrInfoPane, JSplitPane.RIGHT);
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
                                              this.getNameServiceIiopVersion(),
                                              this.getNamingSubcontext());

      try
      {
         // If we have the Tweek Environment Service, and we should, initialize
         // the new CORBA service with the command line arguments passed when
         // the Tweek GUI was started.
         try
         {
            EnvironmentService env_service = new EnvironmentServiceProxy();

            // This may throw several types of exceptions, all of which are
            // handled below.
            new_orb.init(env_service.getCommandLineArgs());

            // We initialized the CorbaService object successfully and
            // connected to the remote Naming Service.
            corbaService = new_orb;

            // XXX: Should we allow the user to make multiple connections from
            // a single dialog box?  Probably not...
            mNSConnectButton.setEnabled(false);
            mNSRefreshButton.setEnabled(true);

            // Create a new list model for the fresh list of Subject Managers.
            DefaultListModel mgr_list_model = new DefaultListModel();

            // Get the list of Subject Managers.  There had better be at least
            // one!  The list returned is guaranteed to contain valid references
            // at the time of construction.
            java.util.Iterator i = new_orb.getSubjectManagerList().iterator();
            tweek.SubjectManager cur_mgr;

            while ( i.hasNext() )
            {
               cur_mgr = (tweek.SubjectManager) i.next();
               mgr_list_model.addElement(new SubjectManagerWrapper(cur_mgr));
            }

            mSubjectMgrList.setModel(mgr_list_model);
         }
         catch(org.omg.CosNaming.NamingContextPackage.NotFound nfex)
         {
            String reason = "";

            if ( nfex.why == NotFoundReason.not_context )
            {
               reason = " (naming context is an object)";
            }
            else if ( nfex.why == NotFoundReason.not_object )
            {
               reason = " (object is a naming context)";
            }
            else if ( nfex.why == NotFoundReason.missing_node )
            {
               reason = " (node in name path is missing)";
            }
            else
            {
               reason = " (" + nfex.getMessage() + ")";
            }

            int answer =
               JOptionPane.showConfirmDialog(this,
                                             "No naming context found" +
                                             reason +
                                             ".\nDisconnect from this Naming Service?",
                                             "Naming Context Lookup Failure",
                                             JOptionPane.YES_NO_OPTION,
                                             JOptionPane.QUESTION_MESSAGE);

            // Disable this button regardless of the user's answer.  A new
            // connection cannot be attempted until new naming service
            // connection is entered or the current connection is closed.
            mNSConnectButton.setEnabled(false);

            // If the user clicked "No," then we need to enable the refresh
            // button so that an attempt can be made to get a new list of
            // Subject Managers later.
            if ( answer == JOptionPane.NO_OPTION )
            {
               corbaService = new_orb;
               mNSRefreshButton.setEnabled(true);
            }
            // If the user clicked "Yes," then ensure that the refresh button
            // is disabled until the next connection attempt.
            else
            {
               mNSRefreshButton.setEnabled(false);
            }
         }
         catch(org.omg.CORBA.ORBPackage.InvalidName poaNameEx)
         {
            JOptionPane.showConfirmDialog(this, poaNameEx.getMessage(),
                                          "Root Portable Object Adapter Not Found",
                                          JOptionPane.OK_OPTION,
                                          JOptionPane.ERROR_MESSAGE);
         }
         catch(org.omg.PortableServer.POAManagerPackage.AdapterInactive poaEx)
         {
            JOptionPane.showConfirmDialog(this, poaEx.getMessage(),
                                          "Root Portable Object Adapter Inactive",
                                          JOptionPane.OK_OPTION,
                                          JOptionPane.ERROR_MESSAGE);
         }
         catch(org.omg.CORBA.UserException userEx)
         {
            JOptionPane.showConfirmDialog(this, userEx.getMessage(),
                                          "CORBA User Exception",
                                          JOptionPane.OK_OPTION,
                                          JOptionPane.ERROR_MESSAGE);
         }
         catch(RuntimeException ex)
         {
            ex.printStackTrace();
         }
      }
      // Something went wrong with the CORBA service initialization.
      catch (org.omg.CORBA.SystemException sys_ex)
      {
         // Make sure the connection button is enabled.  We do not know what
         // caused the connection to fail, but failure should not disallow any
         // further attempts to connect.
         mNSConnectButton.setEnabled(true);
         mNSRefreshButton.setEnabled(false);

         // Pop up a dialog stating that the connection failed.
         JOptionPane.showMessageDialog(this, "CORBA System Exception: '" +
                                       sys_ex.getMessage() + "'",
                                       "CORBA System Exception",
                                       JOptionPane.ERROR_MESSAGE);

         // DEBUG
         sys_ex.printStackTrace();
      }
   }

   private void namingServiceRefreshAction()
   {
      // Create a new list model for the fresh list of Subject Managers.
      DefaultListModel mgr_list_model = new DefaultListModel();

      // Get the list of Subject Managers.  There had better be at least
      // one!  The list returned is guaranteed to contain valid references
      // at the time of construction.
      java.util.Iterator i = corbaService.getSubjectManagerList().iterator();
      tweek.SubjectManager cur_mgr;

      while ( i.hasNext() )
      {
         cur_mgr = (tweek.SubjectManager) i.next();
         mgr_list_model.addElement(new SubjectManagerWrapper(cur_mgr));
      }

      mSubjectMgrList.setModel(mgr_list_model);
      mSubjectMgrList.invalidate();
   }

   /**
    * Commits the user-specified information from the text fields to this
    * object's properties.  After calling this method, those properties can be
    * considered up to date and usable.
    */
   private void commitConnectInfo()
   {
      nameServiceHost        = mNSHostField.getText();
      nameServicePort        = Integer.parseInt(mNSPortField.getText());
      nameServiceIiopVersion = mNSIiopVerField.getText();
      namingSubcontext       = mNamingContextField.getText();
   }

   private void okButtonAction(ActionEvent e)
   {
      status = OK_OPTION;
      commit();
      dispose();
   }

   /**
    * Sets the close status to CANCEL_OPTION and closes this dialog box.  If
    * an ORB is running, it is shut down.
    */
   private void cancelButtonAction(ActionEvent e)
   {
      // If we have an ORB running, we have to shut it down.
      if ( null != corbaService )
      {
         corbaService.shutdown(true);
         corbaService = null;
      }

      status = CANCEL_OPTION;
      dispose();
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
   private void validateNetworkAddress()
   {
      // Do not perform network address validation if there is an active
      // CorbaService object.
      if ( null == corbaService )
      {
         if ( ! mNSHostField.getText().equals("") &&
              validatePortNumber(mNSPortField.getText()) &&
              ! mNSIiopVerField.getText().equals("") )
         {
            mNSConnectButton.setEnabled(true);
         }
         else
         {
            mNSConnectButton.setEnabled(false);
         }
      }
   }

   /**
    * Verifies that the given string is a valid port number.
    */
   private boolean validatePortNumber(String port)
   {
      boolean valid = false;

      try
      {
         int port_num = Integer.parseInt(port);

         if ( port_num > 0 && port_num < 65536 )
         {
            valid = true;
         }
      }
      catch (Exception e)
      {
         MessageDocument doc = TweekCore.instance().getMessageDocument();
         doc.printErrornl("Invalid port number: " + port);
      }

      return valid;
   }

   /**
    * An implementation of DocumentListener used to validate the network
    * address entered for the CORBA Naming Service.
    */
   private class AddressFieldValidator implements DocumentListener
   {
      public void insertUpdate(DocumentEvent e)
      {
         validateNetworkAddress();
      }

      public void removeUpdate(DocumentEvent e)
      {
         validateNetworkAddress();
      }

      public void changedUpdate(DocumentEvent e)
      {
         validateNetworkAddress();
      }
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
      public SubjectMgrListSelectionListener(JDialog parent)
      {
         mParentDialog = parent;
      }

      public void valueChanged(ListSelectionEvent e)
      {
         // Pull the tweek.SubjectManager reference from the list.
         int index = mSubjectMgrList.getSelectedIndex();

         // No selection.
         if ( -1 == index )
         {
            mOkayButton.setEnabled(false);
         }
         // New selection.
         else
         {
            SubjectManagerWrapper mgr_wrapper =
               (SubjectManagerWrapper) mSubjectMgrList.getModel().getElementAt(index);
            mSubjectManager = mgr_wrapper.getSubjectManager();

            // Fill in the table model for the selected Subject Manager.
            Object[] column_names = new Object[]{"Property", "Value"};
            DefaultTableModel table_model = new DefaultTableModel();
            table_model.setColumnIdentifiers(column_names);

            try
            {
               SubjectMgrInfoItem[] subj_mgr_info = mSubjectManager.getInfo();

               for ( int i = 0; i < subj_mgr_info.length; ++i )
               {
                  table_model.addRow(new Object[]{subj_mgr_info[i].key,
                                                  subj_mgr_info[i].value});
               }

               mSubjectMgrInfo.setModel(table_model);
               mSubjectMgrInfo.setEnabled(false);
               mOkayButton.setEnabled(true);
            }
            catch (org.omg.CORBA.COMM_FAILURE commEx)
            {
               mOkayButton.setEnabled(false);
               String msg = "Invalid Subject Manager Selected";

               // If the exception has an error message, append it.
               if ( ! commEx.getMessage().equals("") )
               {
                  msg += ": '" + commEx.getMessage() + "'";
               }

               JOptionPane.showMessageDialog(mParentDialog, msg,
                                             "CORBA Communication Exception",
                                             JOptionPane.ERROR_MESSAGE);
            }
         }
      }

      private JDialog mParentDialog = null;
   }

   // Attributes that may be queried by the class that instantiated us.
   private int          status;
   private String       nameServiceHost        = null;
   private int          nameServicePort        = 2809;
   private String       nameServiceIiopVersion = "1.0";
   private String       namingSubcontext       = null;
   private CorbaService corbaService           = null;

   // Internal-use properties.
   private tweek.SubjectManager mSubjectManager = null;

   // GUI elements for the Naming Service connection panel.
   private TitledBorder  mNSConnectBorder;
   private JPanel        mNSConnectPanel     = new JPanel();
   private GridBagLayout mNSConnectLayout    = new GridBagLayout();
   private JLabel        mNSHostLabel        = new JLabel();
   private JTextField    mNSHostField        = new JTextField();
   private JLabel        mNSPortLabel        = new JLabel();
   private JTextField    mNSPortField        = new JTextField();
   private JLabel        mNSIiopVerLabel     = new JLabel();
   private JTextField    mNSIiopVerField     = new JTextField();
   private JLabel        mNamingContextLabel = new JLabel();
   private JTextField    mNamingContextField = new JTextField();
   private JPanel        mNSButtonPanel      = new JPanel();
   private JButton       mNSConnectButton    = new JButton();
   private JButton       mNSRefreshButton    = new JButton();

   // GUI elements for the Subject Manager panel.
   private TitledBorder mSubjectMgrBorder;
   private JPanel       mSubjectMgrPanel     = new JPanel();
   private BorderLayout mSubjectMgrLayout    = new BorderLayout();
   private JSplitPane   mSubjectMgrSplitPane = new JSplitPane();
   private JList        mSubjectMgrList      = new JList();
   private JScrollPane  mSubjectMgrListPane  = new JScrollPane(mSubjectMgrList);
   private JTable       mSubjectMgrInfo      = new JTable();
   private JScrollPane  mSubjectMgrInfoPane  = new JScrollPane(mSubjectMgrInfo);

   // GUI elements for the OK/Cancel button panel.
   private JPanel  mButtonPanel  = new JPanel();
   private JButton mOkayButton   = new JButton();
   private JButton mCancelButton = new JButton();
}
