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
import org.vrjuggler.tweek.services.GlobalPreferencesService;


/**
 * @since 1.0
 */
public class PrefsDialog extends JDialog
{
   public PrefsDialog (Frame owner, String title,
                       GlobalPreferencesService prefs)
   {
      super(owner, title);

      m_prefs = prefs;

      userLevel   = m_prefs.getUserLevel();
      lookAndFeel = m_prefs.getLookAndFeel();
      beanViewer  = m_prefs.getBeanViewer();

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

   public void setBeanViewer (String v)
   {
      m_prefs.setBeanViewer(v);
   }

   public int getStatus ()
   {
      return status;
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
      java.util.Vector viewers = m_prefs.getBeanViewers();

      for ( int i = 0; i < viewers.size(); i++ )
      {
         m_viewer_box.addItem(viewers.elementAt(i));
      }

      for ( int i = 1; i <= 10; i++ )
      {
         m_level_box.addItem(String.valueOf(i));
      }

      m_level_box.setSelectedIndex(m_prefs.getUserLevel() - 1);

      m_ok_button.setMnemonic('O');
      m_ok_button.setText("OK");
      m_ok_button.addActionListener(new ActionListener()
      {
         public void actionPerformed (ActionEvent e)
         {
            okButtonAction(e);
         }
      });

      m_save_button.setMnemonic('S');
      m_save_button.setText("Save");
      m_save_button.addActionListener(new ActionListener()
      {
         public void actionPerformed (ActionEvent e)
         {
            saveButtonAction(e);
         }
      });

      m_cancel_button.setMnemonic('C');
      m_cancel_button.setText("Cancel");
      m_cancel_button.addActionListener(new ActionListener()
      {
         public void actionPerformed (ActionEvent e)
         {
            cancelButtonAction(e);
         }
      });

      m_content_layout.setColumns(1);
      m_content_layout.setRows(0);
      m_content_panel.setLayout(m_content_layout);
      m_content_panel.add(m_level_panel, null);
      m_content_panel.add(m_laf_panel, null);
      m_content_panel.add(m_viewer_panel, null);

      m_level_label.setText("User Level");
      m_level_box.addActionListener(new ActionListener()
         {
            public void actionPerformed (ActionEvent e)
            {
               userLevel = m_level_box.getSelectedIndex() + 1;
            }
         });
      m_level_panel.add(m_level_label, null);
      m_level_panel.add(m_level_box, null);

      m_laf_label.setText("Look and Feel");
      m_laf_box.addItem("Metal");
      m_laf_box.addItem("Motif");
      m_laf_box.addItem("Windows");
      m_laf_box.addItem("Mac");
      m_laf_box.addItem("System");
      m_laf_box.addActionListener(new ActionListener()
         {
            public void actionPerformed (ActionEvent e)
            {
               String val = (String) m_laf_box.getSelectedItem();

               if ( val.equals("Metal") )
               {
                  lookAndFeel = "javax.swing.plaf.metal.MetalLookAndFeel";
               }
               else if ( val.equals("Motif") )
               {
                  lookAndFeel = "com.sun.java.swing.plaf.motif.MotifLookAndFeel";
               }
               else if ( val.equals("Windows") )
               {
                  lookAndFeel = "com.sun.java.swing.plaf.windows.WindowsLookAndFeel";
               }
               else if ( val.equals("Macintosh") )
               {
                  lookAndFeel = "com.sun.java.swing.plaf.mac.MacLookAndFeel";
               }
               else
               {
                  lookAndFeel = UIManager.getSystemLookAndFeelClassName();
               }
            }
         });
      m_laf_panel.add(m_laf_label, null);
      m_laf_panel.add(m_laf_box, null);

      m_viewer_label.setText("Bean Viewer");
      m_viewer_box.addActionListener(new ActionListener()
         {
            public void actionPerformed (ActionEvent e)
            {
               beanViewer = (String) m_viewer_box.getSelectedItem();
            }
         });
      m_viewer_panel.add(m_viewer_label, null);
      m_viewer_panel.add(m_viewer_box, null);

      m_button_panel.add(m_ok_button, null);
      m_button_panel.add(m_save_button, null);
      m_button_panel.add(m_cancel_button, null);

      this.getContentPane().add(m_content_panel, BorderLayout.CENTER);
      this.getContentPane().add(m_button_panel, BorderLayout.SOUTH);
   }

   private void okButtonAction (ActionEvent e)
   {
      status = OK_OPTION;
      commit();
      m_prefs.save(false);
      setVisible(false);
   }

   private void saveButtonAction (ActionEvent e)
   {
      commit();
      m_prefs.save(false);
   }

   private void cancelButtonAction (ActionEvent e)
   {
      status = CANCEL_OPTION;
      setVisible(false);
   }

   private void commit ()
   {
      m_prefs.setUserLevel(userLevel);
      m_prefs.setLookAndFeel(lookAndFeel);
      m_prefs.setBeanViewer(beanViewer);
   }

   private int status;

   private int    userLevel   = 0;
   private String lookAndFeel = null;
   private String beanViewer  = null;

   private GlobalPreferencesService m_prefs = null;

   private JPanel m_content_panel = new JPanel();
//   private JSplitPane m_content_panel = new JSplitPane();
   private GridLayout m_content_layout = new GridLayout();

   private JPanel m_level_panel = new JPanel();
   private JLabel m_level_label = new JLabel();
   private JComboBox m_level_box = new JComboBox();
   private JPanel m_laf_panel = new JPanel();
   private JLabel m_laf_label = new JLabel();
   private JComboBox m_laf_box = new JComboBox();
   private JLabel m_viewer_label = new JLabel();
   private JPanel m_viewer_panel = new JPanel();
   private JComboBox m_viewer_box = new JComboBox();

   private JPanel  m_button_panel  = new JPanel();
   private JButton m_cancel_button = new JButton();
   private JButton m_save_button   = new JButton();
   private JButton m_ok_button     = new JButton();
}
