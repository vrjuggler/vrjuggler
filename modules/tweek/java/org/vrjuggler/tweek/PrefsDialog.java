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
import java.util.Iterator;
import java.util.List;
import javax.swing.*;
import org.vrjuggler.tweek.services.GlobalPreferencesService;
import javax.swing.border.*;


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

      userLevel        = m_prefs.getUserLevel();
      lookAndFeel      = m_prefs.getLookAndFeel();
      beanViewer       = m_prefs.getBeanViewer();
      chooserStartDir  = m_prefs.getChooserStartDir();
      chooserOpenStyle = m_prefs.getChooserOpenStyle();

      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      this.populateComboBoxes();

      switch ( chooserOpenStyle )
      {
         case GlobalPreferencesService.EMACS_CHOOSER:
            mEmacsStyleButton.setSelected(true);
            break;
         case GlobalPreferencesService.WINDOWS_CHOOSER:
         default:
            mWindowsStyleButton.setSelected(true);
            break;
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

      mFileChooserBorder = new TitledBorder(BorderFactory.createEtchedBorder(Color.white,new Color(142, 142, 142)),"File Chooser Configuration");
      mGenBorder = new TitledBorder(BorderFactory.createEtchedBorder(Color.white,new Color(142, 142, 142)),"General Configuration");
      m_viewer_box.setMinimumSize(new Dimension(126, 10));
      m_viewer_box.setPreferredSize(new Dimension(130, 10));
      m_viewer_box.setSelectedItem(m_prefs.getBeanViewer());

      for ( int i = 1; i <= 10; i++ )
      {
         m_level_box.addItem(String.valueOf(i));
      }

      m_level_box.setMinimumSize(new Dimension(126, 10));
      m_level_box.setPreferredSize(new Dimension(130, 10));
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
      mFileChooserPanel.setLayout(mFileChooserLayout);
      mGenConfigPanel.setLayout(mGenConfigLayout);
      mGeneralPanel.setBorder(mGenBorder);
      mGeneralPanel.setToolTipText("General Tweek interface configuration");
      mGeneralPanel.setLayout(mGenLayout);
      mFileChooserPanel.setBorder(mFileChooserBorder);
      mFcConfigPanel.setLayout(mFcConfigLayout);
      mFcStartDirLabel.setMaximumSize(new Dimension(24, 13));
      mFcStartDirLabel.setMinimumSize(new Dimension(24, 13));
      mFcStartDirLabel.setPreferredSize(new Dimension(24, 13));
      mFcStartDirLabel.setHorizontalAlignment(SwingConstants.RIGHT);
      mFcStartDirLabel.setText("Start Directory");
      mFcStartDirBox.setMinimumSize(new Dimension(126, 10));
      mFcStartDirBox.setPreferredSize(new Dimension(130, 10));
      mFcStartDirBox.setEditable(true);
      mFcOpenStyleTitleLabel.setMaximumSize(new Dimension(24, 13));
      mFcOpenStyleTitleLabel.setMinimumSize(new Dimension(24, 13));
      mFcOpenStyleTitleLabel.setPreferredSize(new Dimension(24, 13));
      mFcOpenStyleTitleLabel.setHorizontalAlignment(SwingConstants.RIGHT);
      mFcOpenStyleTitleLabel.setText("Open Style");
      mEmacsStyleButton.setMinimumSize(new Dimension(109, 15));
      mEmacsStyleButton.setPreferredSize(new Dimension(109, 15));
      mEmacsStyleButton.setText("Emacs Style");
      mEmacsStyleButton.addActionListener(new ActionListener()
         {
            public void actionPerformed (ActionEvent e)
            {
               chooserOpenStyle = GlobalPreferencesService.EMACS_CHOOSER;
            }
         });

      mWindowsStyleButton.setMinimumSize(new Dimension(128, 15));
      mWindowsStyleButton.setPreferredSize(new Dimension(128, 15));
      mWindowsStyleButton.setText("Windows Style");
      mWindowsStyleButton.addActionListener(new ActionListener()
         {
            public void actionPerformed (ActionEvent e)
            {
               chooserOpenStyle = GlobalPreferencesService.WINDOWS_CHOOSER;
            }
         });

      mFcOpenStyleButtonPanel.setLayout(mFcOpenStyleButtonLayout);
      mFcOpenStyleButtonLayout.setColumns(1);
      mFcOpenStyleButtonLayout.setRows(0);
      m_laf_label.setMaximumSize(new Dimension(74, 13));
      m_laf_label.setMinimumSize(new Dimension(24, 13));
      m_laf_label.setPreferredSize(new Dimension(24, 13));
      m_laf_label.setHorizontalAlignment(SwingConstants.RIGHT);
      m_level_label.setMinimumSize(new Dimension(24, 13));
      m_level_label.setPreferredSize(new Dimension(24, 13));
      m_level_label.setHorizontalAlignment(SwingConstants.RIGHT);
      m_viewer_label.setMaximumSize(new Dimension(24, 13));
      m_viewer_label.setMinimumSize(new Dimension(24, 13));
      m_viewer_label.setPreferredSize(new Dimension(24, 13));
      m_viewer_label.setHorizontalAlignment(SwingConstants.RIGHT);
      m_laf_box.setMinimumSize(new Dimension(126, 10));
      m_laf_box.setPreferredSize(new Dimension(130, 10));
      mFcOpenStyleButtonPanel.setMinimumSize(new Dimension(128, 50));
      mFcOpenStyleButtonPanel.setPreferredSize(new Dimension(128, 50));
      mGeneralPanel.add(mGenConfigPanel, BorderLayout.CENTER);
      m_content_panel.add(mGeneralPanel, null);
      m_content_panel.add(mFileChooserPanel, null);

      m_level_label.setText("User Level");
      m_level_box.addActionListener(new ActionListener()
         {
            public void actionPerformed (ActionEvent e)
            {
               userLevel = m_level_box.getSelectedIndex() + 1;
            }
         });
      mGenConfigPanel.add(m_level_label,    new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(0, 0, 0, 3), 95, 23));
      mGenConfigPanel.add(m_level_box,  new GridBagConstraints(1, 0, 1, 1, 1.0, 0.0
            ,GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL, new Insets(0, 0, 0, 0), 64, 14));

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
      mGenConfigPanel.add(m_laf_label,   new GridBagConstraints(0, 1, 1, 1, 0.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(0, 0, 0, 3), 95, 23));
      mGenConfigPanel.add(m_laf_box,  new GridBagConstraints(1, 1, 1, 1, 1.0, 0.0
            ,GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL, new Insets(0, 0, 0, 0), 64, 14));

      m_viewer_label.setText("Bean Viewer");
      m_viewer_box.addActionListener(new ActionListener()
         {
            public void actionPerformed (ActionEvent e)
            {
               beanViewer = (String) m_viewer_box.getSelectedItem();
            }
         });
      mGenConfigPanel.add(m_viewer_label,    new GridBagConstraints(0, 2, 1, 1, 0.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(0, 0, 0, 3), 95, 23));
      mGenConfigPanel.add(m_viewer_box,  new GridBagConstraints(1, 2, 1, 1, 1.0, 0.0
            ,GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL, new Insets(0, 0, 0, 0), 64, 14));

      m_button_panel.add(m_ok_button, null);
      m_button_panel.add(m_save_button, null);
      m_button_panel.add(m_cancel_button, null);

      this.getContentPane().add(m_content_panel, BorderLayout.CENTER);
      this.getContentPane().add(m_button_panel, BorderLayout.SOUTH);
      mFileChooserPanel.add(mFcConfigPanel, BorderLayout.CENTER);
      mFcConfigPanel.add(mFcStartDirLabel,       new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 95, 23));
      mFcConfigPanel.add(mFcStartDirBox,        new GridBagConstraints(1, 0, 1, 1, 1.0, 0.0
            ,GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL, new Insets(0, 0, 0, 0), 64, 14));
      mFcConfigPanel.add(mFcOpenStyleTitleLabel,             new GridBagConstraints(0, 1, 1, 1, 0.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(0, 0, 0, 3), 95, 23));
      mFcConfigPanel.add(mFcOpenStyleButtonPanel,           new GridBagConstraints(1, 1, 1, 1, 1.0, 1.0
            ,GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL, new Insets(0, 0, 0, 0), 66, 0));
      mFcOpenStyleButtonPanel.add(mWindowsStyleButton, null);
      mFcOpenStyleButtonPanel.add(mEmacsStyleButton, null);
      mOpenStyleButtonGroup.add(mWindowsStyleButton);
      mOpenStyleButtonGroup.add(mEmacsStyleButton);
   }

   /**
    * Adds items to all the combo boxes.  In some cases, other modifications
    * to a combo box will be made.  These can include setting the default
    * selected value and/or adding action listeners.
    */
   private void populateComboBoxes ()
   {
      Iterator i      = GlobalPreferencesService.getStartDirList().iterator();

      boolean has_start_dir = false;

      while ( i.hasNext() )
      {
         String dir = (String) i.next();

         if ( dir.equals(chooserStartDir) )
         {
            has_start_dir = true;
         }

         mFcStartDirBox.addItem(dir);
      }

      if ( ! has_start_dir )
      {
         mFcStartDirBox.addItem(chooserStartDir);
      }

      mFcStartDirBox.setSelectedItem(chooserStartDir);

      mFcStartDirBox.addActionListener(new ActionListener()
         {
            public void actionPerformed (ActionEvent e)
            {
               chooserStartDir = (String) mFcStartDirBox.getSelectedItem();
            }
         });
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
      m_prefs.setChooserStartDir(chooserStartDir);
      m_prefs.setChooserOpenStyle(chooserOpenStyle);
   }

   private int status;

   private int    userLevel        = 0;
   private String lookAndFeel      = null;
   private String beanViewer       = null;
   private String chooserStartDir  = GlobalPreferencesService.DEFAULT_START;
   private int    chooserOpenStyle = GlobalPreferencesService.DEFAULT_CHOOSER;

   private GlobalPreferencesService m_prefs = null;

   private JPanel m_content_panel = new JPanel();
   private GridLayout m_content_layout = new GridLayout();

   private JPanel       mGeneralPanel     = new JPanel();
   private TitledBorder mGenBorder;
   private BorderLayout mGenLayout        = new BorderLayout();

   private JPanel        mGenConfigPanel  = new JPanel();
   private GridBagLayout mGenConfigLayout = new GridBagLayout();

   private JLabel    m_level_label  = new JLabel();
   private JComboBox m_level_box    = new JComboBox();
   private JLabel    m_laf_label    = new JLabel();
   private JComboBox m_laf_box      = new JComboBox();
   private JLabel    m_viewer_label = new JLabel();
   private JComboBox m_viewer_box   = new JComboBox();

   private JPanel       mFileChooserPanel = new JPanel();
   private TitledBorder mFileChooserBorder;
   private BorderLayout mFileChooserLayout = new BorderLayout();

   private JPanel        mFcConfigPanel  = new JPanel();
   private GridBagLayout mFcConfigLayout = new GridBagLayout();

   private JLabel       mFcStartDirLabel         = new JLabel();
   private JComboBox    mFcStartDirBox           = new JComboBox();
   private JLabel       mFcOpenStyleTitleLabel   = new JLabel();
   private JPanel       mFcOpenStyleButtonPanel  = new JPanel();
   private GridLayout   mFcOpenStyleButtonLayout = new GridLayout();
   private ButtonGroup  mOpenStyleButtonGroup    = new ButtonGroup();
   private JRadioButton mWindowsStyleButton      = new JRadioButton();
   private JRadioButton mEmacsStyleButton        = new JRadioButton();

   private JPanel  m_button_panel  = new JPanel();
   private JButton m_cancel_button = new JButton();
   private JButton m_save_button   = new JButton();
   private JButton m_ok_button     = new JButton();
}
