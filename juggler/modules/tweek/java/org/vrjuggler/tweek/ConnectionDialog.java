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

package org.vrjuggler.tweek;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.*;


/**
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

   public String getOrbHost ()
   {
      return orbHost;
   }

   public int getOrbPort ()
   {
      return orbPort;
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
      m_main_panel.setLayout(m_main_layout);
      m_main_panel.setMinimumSize(new Dimension(375, 96));
      m_main_panel.setPreferredSize(new Dimension(375, 96));
      m_main_layout.setRows(-1);
      m_main_layout.setColumns(1);

      m_addr_label.setText("Address");
      m_addr_field.setMinimumSize(new Dimension(80, 17));
      m_addr_field.setPreferredSize(new Dimension(250, 17));
      m_addr_panel.setLayout(m_addr_layout);
      m_addr_panel.add(m_addr_label, null);
      m_addr_panel.add(m_addr_field, null);

      m_port_label.setText("Port Number");
      m_port_field.setMinimumSize(new Dimension(80, 17));
      m_port_field.setPreferredSize(new Dimension(80, 17));
      m_port_panel.setLayout(m_port_layout);
      m_port_panel.add(m_port_label, null);
      m_port_panel.add(m_port_field, null);

      m_connected_orbs.setToolTipText("Current connected ORBs");
      m_connected_orbs_panel.setLayout(m_connected_orbs_layout);
      m_connected_orbs_label.setText("Currently Connected ORBs");
      m_connected_orbs_panel.add(m_connected_orbs_label, null);
      m_connected_orbs_panel.add(m_connected_orbs, null);

      m_ok_button.setText("OK");
      m_ok_button.setMnemonic('O');
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

      this.getContentPane().add(m_main_panel, BorderLayout.CENTER);

      m_main_panel.add(m_addr_panel, null);
      m_main_panel.add(m_port_panel, null);
      m_main_panel.add(m_connected_orbs_panel, null);

      this.getContentPane().add(m_button_panel, BorderLayout.SOUTH);
      m_button_panel.add(m_ok_button, null);
      m_button_panel.add(m_cancel_button, null);
   }

   private void okButtonAction (ActionEvent e)
   {
      status = OK_OPTION;
      commit();
      setVisible(false);
   }

   private void cancelButtonAction (ActionEvent e)
   {
      status = CANCEL_OPTION;
      setVisible(false);
   }

   private void commit ()
   {
      orbHost = m_addr_field.getText();
      orbPort = Integer.parseInt(m_port_field.getText());
   }

   private int status;

   private JPanel     m_main_panel  = new JPanel();
   private GridLayout m_main_layout = new GridLayout();

   private JPanel     m_addr_panel  = new JPanel();
   private FlowLayout m_addr_layout = new FlowLayout();
   private JLabel     m_addr_label  = new JLabel();
   private JTextField m_addr_field  = new JTextField();

   private JPanel     m_port_panel  = new JPanel();
   private FlowLayout m_port_layout = new FlowLayout();
   private JLabel     m_port_label  = new JLabel();
   private JTextField m_port_field  = new JTextField();

   private JPanel     m_connected_orbs_panel  = new JPanel();
   private FlowLayout m_connected_orbs_layout = new FlowLayout();
   private JLabel     m_connected_orbs_label  = new JLabel();
   private JComboBox  m_connected_orbs        = new JComboBox();

   private JPanel  m_button_panel  = new JPanel();
   private JButton m_ok_button     = new JButton();
   private JButton m_cancel_button = new JButton();

   private String orbHost = null;
   private int    orbPort = -1;
}