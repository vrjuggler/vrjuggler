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

   private void jbInit() throws Exception
   {
      titledBorder1 = new TitledBorder("");
      m_main_panel.setLayout(m_main_layout);
      m_orb_addr_label.setText("Address");
      m_addr_panel.setLayout(m_addr_layout);
      m_orb_addr_field.setMinimumSize(new Dimension(80, 17));
      m_orb_addr_field.setPreferredSize(new Dimension(180, 17));
      m_main_layout.setColumns(1);
      m_main_layout.setRows(-1);
      m_orb_panel.setLayout(m_orb_layout);
      m_orb_port_label.setText("Port Number");
      m_addr_panel.setBorder(BorderFactory.createLineBorder(Color.black));
      m_orb_panel.setBorder(BorderFactory.createLineBorder(Color.black));
      m_main_panel.setPreferredSize(new Dimension(240, 38));
      m_connected_orbs_panel.setLayout(m_connected_orbs_layout);
      m_connected_orbs.setToolTipText("Current connected ORBs");
      m_orb_panel.add(m_orb_port_label, BorderLayout.WEST);
      m_orb_panel.add(m_orb_port_field, BorderLayout.CENTER);
      this.getContentPane().add(m_main_panel,  BorderLayout.CENTER);
      m_addr_panel.add(m_orb_addr_label,  BorderLayout.WEST);
      m_addr_panel.add(m_orb_addr_field, BorderLayout.CENTER);
      m_main_panel.add(m_addr_panel, null);
      m_main_panel.add(m_orb_panel, null);
      m_main_panel.add(m_connected_orbs_panel, null);
      m_connected_orbs_panel.add(m_connected_orbs, BorderLayout.CENTER);
   }

   private JPanel m_main_panel = new JPanel();
   private JPanel m_addr_panel = new JPanel();
   private JLabel m_orb_addr_label = new JLabel();
   private JTextField m_orb_addr_field = new JTextField();
   private BorderLayout borderLayout1 = new BorderLayout();
   private BorderLayout m_addr_layout = new BorderLayout();
   private JPanel m_orb_panel = new JPanel();
   private GridLayout m_main_layout = new GridLayout();
   private BorderLayout m_orb_layout = new BorderLayout();
   private JLabel m_orb_port_label = new JLabel();
   private JTextField m_orb_port_field = new JTextField();
   private TitledBorder titledBorder1;
   private JPanel m_connected_orbs_panel = new JPanel();
   private BorderLayout m_connected_orbs_layout = new BorderLayout();
   private JComboBox m_connected_orbs = new JComboBox();
}