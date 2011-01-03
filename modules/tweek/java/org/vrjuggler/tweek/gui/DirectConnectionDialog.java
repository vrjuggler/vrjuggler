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
import org.vrjuggler.tweek.beans.BeanRegistry;
import org.vrjuggler.tweek.beans.TweekBean;
import org.vrjuggler.tweek.services.*;
import org.vrjuggler.tweek.net.corba.CorbaService;
import tweek.SubjectManagerPackage.SubjectMgrInfoItem;
import org.vrjuggler.tweek.TweekCore;
import org.vrjuggler.tweek.services.EnvironmentServiceProxy;
import org.vrjuggler.tweek.text.MessageDocument;


/**
 * A modal dialog box used to make a direct connection with a remote ORB. The
 * Tweek CORBA Service created as a result of the connection is made available
 * to external code.
 *
 * @since 1.3.4
 */
public class DirectConnectionDialog extends JDialog
{
   public DirectConnectionDialog(Frame owner, String title)
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

      mHostField.getDocument().addDocumentListener(new AddressFieldValidator());
      mPortField.getDocument().addDocumentListener(new AddressFieldValidator());

      // Set defaults for the direct object connection.
      try
      {
         GlobalPreferencesService prefs = new GlobalPreferencesServiceProxy();

         mHostField.setText(prefs.getDefaultOrbAddress());
         mPortField.setText(String.valueOf(prefs.getDefaultOrbPort()));
      }
      // D'oh!  No defaults can be set.
      catch(Exception ex)
      {
         JOptionPane.showMessageDialog(this, "Could not set defaults '" +
                                       ex.getMessage() + "'",
                                       "Tweek GlobalPreferences Exception",
                                       JOptionPane.WARNING_MESSAGE);
      }

      // At this point, we may have valid object connection information, so we
      // should validate the network address.
      validateNetworkAddress();

      // Add an input validator for the port number field.
      mPortField.setInputVerifier(new InputVerifier()
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

   public int getStatus()
   {
      return status;
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
      mConnectBorder = new TitledBorder(new EtchedBorder(EtchedBorder.RAISED,Color.white,new Color(142, 142, 142)),"CORBA Object Connection");
      mConnectPanel.setLayout(mConnectLayout);
      mConnectPanel.setBorder(mConnectBorder);
      mConnectPanel.setMinimumSize(new Dimension(450, 175));

      mHostLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mHostLabel.setLabelFor(mHostField);
      mHostLabel.setText("Host Address");
      mHostField.setMinimumSize(new Dimension(80, 17));
      mHostField.setPreferredSize(new Dimension(180, 17));
      mHostField.addFocusListener(new java.awt.event.FocusAdapter()
      {
         public void focusLost(FocusEvent e)
         {
            validateNetworkAddress();
         }
      });
      mHostField.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            validateNetworkAddress();
         }
      });
      mPortField.addFocusListener(new java.awt.event.FocusAdapter()
      {
         public void focusLost(FocusEvent e)
         {
            validateNetworkAddress();
         }
      });
      mPortField.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            validateNetworkAddress();
         }
      });
      mConnectButton.setEnabled(false);
      mConnectButton.setSelected(true);
      mConnectButton.setText("Connect");
      mConnectButton.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            objectConnectAction(e);
         }
      });
      mButtonPanel.setMinimumSize(new Dimension(450, 33));
      mButtonPanel.setPreferredSize(new Dimension(450, 33));
      mPortLabel.setHorizontalAlignment(SwingConstants.TRAILING);
      mPortLabel.setLabelFor(mPortField);
      mConnectPanel.add(mHostLabel, new GridBagConstraints(0, 0, 2, 1, 0.0, 0.0
            ,GridBagConstraints.EAST, GridBagConstraints.NONE, new Insets(0, 0, 0, 2), 47, 18));
      mConnectPanel.add(mHostField,        new GridBagConstraints(2, 0, 1, 1, 1.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, new Insets(0, 0, 0, 0), 14, 6));

      mPortLabel.setText("Port Number");
      mPortField.setMinimumSize(new Dimension(50, 17));
      mPortField.setPreferredSize(new Dimension(50, 17));
      mConnectPanel.add(mPortLabel,       new GridBagConstraints(0, 1, 2, 1, 0.0, 0.0
            ,GridBagConstraints.EAST, GridBagConstraints.NONE, new Insets(0, 0, 0, 2), 0, 18));
      mConnectPanel.add(mPortField,             new GridBagConstraints(2, 1, 1, 1, 1.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 10, 6));


      mCancelButton.setText("Cancel");
      mCancelButton.setMnemonic('C');
      mCancelButton.addActionListener(new ActionListener()
      {
         public void actionPerformed (ActionEvent e)
         {
            cancelButtonAction(e);
         }
      });

      this.getContentPane().add(mConnectPanel, BorderLayout.NORTH);

      mConnectPanel.add(mButtonPanel, new GridBagConstraints(1, 4, 2, 1, 0.0, 0.0
            ,GridBagConstraints.CENTER, GridBagConstraints.BOTH, new Insets(0, 0, 0, 0), 0, 0));

      this.getContentPane().add(mButtonPanel, BorderLayout.SOUTH);
      mButtonPanel.add(mConnectButton, null);
      mButtonPanel.add(mCancelButton, null);
   }

   /**
    * Handles the event generated by the user clicking the "Connect" button in
    * the connection panel.
    */
   private void objectConnectAction(ActionEvent e)
   {
      // If we have the Tweek Environment Service, and we should, initialize
      // the new CORBA service with the command line arguments passed when the
      // Tweek GUI was started.
      try
      {
         EnvironmentService env_service = new EnvironmentServiceProxy();

         // Create a new CORBA service using the information provided by
         // the user. This may throw several types of exceptions, all of
         // which are handled below.
         corbaService = new CorbaService(env_service.getCommandLineArgs());

         String host = mHostField.getText();
         int port    = Integer.parseInt(mPortField.getText());

         // Construct the corbaloc URI for the direct object connection.
         String uri = "corbaloc::" + host + ":" + port + "/SubjectManager";

         tweek.SubjectManager subj_mgr =
            tweek.SubjectManagerHelper.narrow(
               corbaService.getORB().string_to_object(uri)
            );

         if ( null != subj_mgr )
         {
            corbaService.setSubjectManager(subj_mgr);
            status = OK_OPTION;
            dispose();
         }
         else
         {
            JOptionPane.showConfirmDialog(this, "No object found at " + uri,
                                          "CORBA Object Not Found",
                                          JOptionPane.OK_OPTION,
                                          JOptionPane.ERROR_MESSAGE);
         }
      }
      catch (org.omg.CORBA.ORBPackage.InvalidName poaNameEx)
      {
         JOptionPane.showConfirmDialog(
            this, poaNameEx.getMessage(),
            "Root Portable Object Adapter Not Found",
            JOptionPane.OK_OPTION, JOptionPane.ERROR_MESSAGE
         );
      }
      catch (org.omg.PortableServer.POAManagerPackage.AdapterInactive poaEx)
      {
         JOptionPane.showConfirmDialog(
            this, poaEx.getMessage(),
            "Root Portable Object Adapter Inactive", JOptionPane.OK_OPTION,
            JOptionPane.ERROR_MESSAGE
         );
      }
      catch (org.omg.CORBA.UserException userEx)
      {
         JOptionPane.showConfirmDialog(this, userEx.getMessage(),
                                       "CORBA User Exception",
                                       JOptionPane.OK_OPTION,
                                       JOptionPane.ERROR_MESSAGE);
      }
      // Something went wrong with the CORBA service initialization.
      catch (org.omg.CORBA.SystemException sys_ex)
      {
         // Make sure the connection button is enabled.  We do not know what
         // caused the connection to fail, but failure should not disallow any
         // further attempts to connect.
         mConnectButton.setEnabled(true);

         // Pop up a dialog stating that the connection failed.
         JOptionPane.showMessageDialog(this, "CORBA System Exception: '" +
                                       sys_ex.getMessage() + "'",
                                       "CORBA System Exception",
                                       JOptionPane.ERROR_MESSAGE);

         // DEBUG
         sys_ex.printStackTrace();
      }
      catch (RuntimeException ex)
      {
         ex.printStackTrace();
      }
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

   /**
    * Validates that the network address (hostname and port number) entered
    * by the user.
    */
   private void validateNetworkAddress()
   {
      // Do not perform network address validation if there is an active
      // CorbaService object.
      if ( null == corbaService )
      {
         if ( ! mHostField.getText().equals("") &&
              validatePortNumber(mPortField.getText()) )
         {
            mConnectButton.setEnabled(true);
         }
         else
         {
            mConnectButton.setEnabled(false);
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
    * address entered for the CORBA object URI.
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

   // Attributes that may be queried by the class that instantiated us.
   private int          status;
   private CorbaService corbaService = null;

   // GUI elements for the connection panel.
   private TitledBorder  mConnectBorder;
   private JPanel        mConnectPanel  = new JPanel();
   private GridBagLayout mConnectLayout = new GridBagLayout();
   private JLabel        mHostLabel     = new JLabel();
   private JTextField    mHostField     = new JTextField();
   private JLabel        mPortLabel     = new JLabel();
   private JTextField    mPortField     = new JTextField();
   private JButton       mConnectButton = new JButton();

   // GUI elements for the OK/Cancel button panel.
   private JPanel  mButtonPanel  = new JPanel();
   private JButton mCancelButton = new JButton();
}
