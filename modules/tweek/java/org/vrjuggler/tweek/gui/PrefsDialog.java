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
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
      enableEvents(AWTEvent.WINDOW_EVENT_MASK);

      mPrefs = prefs;

      userLevel          = mPrefs.getUserLevel();
      lookAndFeel        = mPrefs.getLookAndFeel();
      beanViewer         = mPrefs.getBeanViewer();
      windowWidth        = mPrefs.getWindowWidth();
      windowHeight       = mPrefs.getWindowHeight();
      chooserStartDir    = mPrefs.getChooserStartDir();
      defaultCorbaHost   = mPrefs.getDefaultCorbaHost();
      defaultCorbaPort   = mPrefs.getDefaultCorbaPort();
      defaultIiopVersion = mPrefs.getDefaultIiopVersion();

      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      this.configComboBoxes();

      mCorbaHostField.setText(String.valueOf(defaultCorbaHost));
      mCorbaPortField.setText(String.valueOf(defaultCorbaPort));
      mIiopVerField.setText(String.valueOf(defaultIiopVersion));
      mWindowWidthField.setText(String.valueOf(windowWidth));
      mWindowHeightField.setText(String.valueOf(windowHeight));

      this.setModal(true);
      this.pack();
   }

   /**
    * Returns the current user level choice.  This may be different than
    * what is currently available through the Global Preferences Service,
    * depending on whether or not the user has chosen to apply changes.
    */
   public int getUserLevel()
   {
      return userLevel;
   }

   /**
    * Returns the current look and feel choice.  This may be different than
    * what is currently available through the Global Preferences Service,
    * depending on whether or not the user has chosen to apply changes.
    */
   public String getLookAndFeel()
   {
      return lookAndFeel;
   }

   /**
    * Returns the current Bean Viewer choice.  This may be different than
    * what is currently available through the Global Preferences Service,
    * depending on whether or not the user has chosen to apply changes.
    */
   public String getBeanViewer()
   {
      return beanViewer;
   }

   public void setBeanViewer(String v)
   {
      mPrefs.setBeanViewer(v);
   }

   /**
    * Returns the current GUI window size choice.  This may be different than
    * what is currently available through the Global Preferences Service,
    * depending on whether or not the user has chosen to apply changes.
    */
   public Dimension getWindowSize()
   {
      return new Dimension(windowWidth, windowHeight);
   }

   /**
    * Returns the current file chooser starting directory.  This may be
    * different than what is currently available through the Global
    * Preferences Service, depending on whether or not the user has chosen to
    * apply changes.
    */
   public String getChooserStartDir()
   {
      return chooserStartDir;
   }

   /**
    * Returns the current default CORBA host choice.  This may be different
    * than what is currently available through the Global Preferences Service,
    * depending on whether or not the user has chosen to apply changes.
    */
   public String getDefaultCorbaHost()
   {
      return defaultCorbaHost;
   }

   /**
    * Returns the current default CORBA port choice.  This may be different
    * than what is currently available through the Global Preferences Service,
    * depending on whether or not the user has chosen to apply changes.
    */
   public int getDefaultCorbaPort()
   {
      return defaultCorbaPort;
   }

   /**
    * Returns the current default IIOP version choice.  This may be different
    * than what is currently available through the Global Preferences Service,
    * depending on whether or not the user has chosen to apply changes.
    */
   public String getDefaultIiopVersion()
   {
      return defaultIiopVersion;
   }

   /**
    * Returns the "close status" of the window.  That is, it informs the
    * caller of the mechnism used to close the dialog.  There are three
    * possible ways this dialog may be closed: using the "OK" button, using
    * the "Cancel" button, or using the window manager.  These are reflected
    * through the integer constants OK_OPTION, CANCEL_OPTION, and CLOSE_OPTION
    * respectively.
    */
   public int getStatus()
   {
      return status;
   }

   public synchronized void addGlobalPrefsUpdateListener(GlobalPrefsUpdateListener l)
   {
      mUpdateListeners.add(l);
   }

   public synchronized void removeGlobalPrefsUpdateListener(GlobalPrefsUpdateListener l)
   {
      mUpdateListeners.remove(l);
   }

   public static final int OK_OPTION     = JOptionPane.OK_OPTION;
   public static final int CANCEL_OPTION = JOptionPane.CANCEL_OPTION;
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
      mOkButton.setMnemonic('O');
      mOkButton.setText("OK");
      mOkButton.addActionListener(new ActionListener()
      {
         public void actionPerformed (ActionEvent e)
         {
            okButtonAction(e);
         }
      });

      mApplyButton.setMnemonic('A');
      mApplyButton.setText("Apply");
      mApplyButton.setEnabled(false);
      mApplyButton.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            applyButtonAction(e);
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

      mFileChooserPanel.setLayout(mFileChooserLayout);
      mFcConfigPanel.setLayout(mFcConfigLayout);
      mFcStartDirLabel.setMaximumSize(new Dimension(24, 13));
      mFcStartDirLabel.setMinimumSize(new Dimension(24, 13));
      mFcStartDirLabel.setPreferredSize(new Dimension(24, 13));
      mFcStartDirLabel.setHorizontalAlignment(SwingConstants.RIGHT);
      mFcStartDirLabel.setText("Start Directory");
      mFcStartDirBox.setMinimumSize(new Dimension(126, 10));
      mFcStartDirBox.setPreferredSize(new Dimension(130, 10));
      mFcStartDirBox.setEditable(true);

      mLazyInstanceButton.setSelected(mPrefs.getLazyPanelBeanInstantiation());
      mLazyInstanceButton.setText("Lazy Panel Bean Instantiaion");
      mLevelBox.setMaximumSize(new Dimension(130, 26));
      mLevelBox.setMinimumSize(new Dimension(70, 26));
      mLevelBox.setPreferredSize(new Dimension(70, 26));
      mViewerBox.setMaximumSize(new Dimension(32767, 26));
      mViewerBox.setMinimumSize(new Dimension(126, 26));
      mViewerBox.setPreferredSize(new Dimension(130, 26));
      mGeneralPanel.setToolTipText("General Tweek interface configuration");
      mGeneralPanel.setLayout(mGenLayout);
      mLevelLabel.setMinimumSize(new Dimension(24, 13));
      mLevelLabel.setPreferredSize(new Dimension(24, 13));
      mLevelLabel.setHorizontalAlignment(SwingConstants.RIGHT);
      mLevelLabel.setLabelFor(mLevelBox);
      mLevelLabel.setText("User Level");
      mViewerLabel.setMaximumSize(new Dimension(24, 13));
      mViewerLabel.setMinimumSize(new Dimension(24, 13));
      mViewerLabel.setPreferredSize(new Dimension(24, 13));
      mViewerLabel.setHorizontalAlignment(SwingConstants.RIGHT);
      mViewerLabel.setLabelFor(mViewerBox);
      mViewerLabel.setText("Bean Viewer");
      mGenConfigPanel.setLayout(mGenConfigLayout);
      mLafBox.setMaximumSize(new Dimension(32767, 26));
      mLafBox.setMinimumSize(new Dimension(126, 26));
      mLafBox.setPreferredSize(new Dimension(130, 26));
      mLafLabel.setMaximumSize(new Dimension(74, 13));
      mLafLabel.setMinimumSize(new Dimension(24, 13));
      mLafLabel.setPreferredSize(new Dimension(24, 13));
      mLafLabel.setHorizontalAlignment(SwingConstants.RIGHT);
      mLafLabel.setLabelFor(mLafBox);
      mLafLabel.setText("Look and Feel");
      mCorbaPortField.setMinimumSize(new Dimension(50, 17));
      mCorbaPortField.setPreferredSize(new Dimension(50, 17));
      mCorbaPortField.setToolTipText("The port number of the CORBA Naming Service");
      mCorbaPortField.addFocusListener(new java.awt.event.FocusAdapter()
      {
         public void focusLost(FocusEvent e)
         {
            corbaPortFieldChanged();
         }
      });
      mCorbaPortField.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            corbaPortFieldChanged();
         }
      });
      mCorbaPortLabel.setHorizontalAlignment(SwingConstants.RIGHT);
      mCorbaPortLabel.setLabelFor(mCorbaPortField);
      mCorbaPortLabel.setText("Port Number");
      mCorbaPanel.setLayout(mCorbaLayout);
      mCorbaHostField.setToolTipText("The hostname for the CORBA Naming Service");
      mCorbaHostField.addFocusListener(new java.awt.event.FocusAdapter()
      {
         public void focusLost(FocusEvent e)
         {
            corbaHostFieldChanged();
         }
      });
      mCorbaHostField.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            corbaHostFieldChanged();
         }
      });
      mCorbaHostLabel.setHorizontalAlignment(SwingConstants.RIGHT);
      mCorbaHostLabel.setLabelFor(mCorbaHostField);
      mCorbaHostLabel.setText("Host Name");
      mIiopVerField.setMinimumSize(new Dimension(50, 17));
      mIiopVerField.setPreferredSize(new Dimension(50, 17));
      mIiopVerField.setToolTipText("The version number of the Internet Inter-ORB Protocol");
      mIiopVerField.addFocusListener(new java.awt.event.FocusAdapter()
      {
         public void focusLost(FocusEvent e)
         {
            iiopVersionFieldChanged();
         }
      });
      mIiopVerField.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            iiopVersionFieldChanged();
         }
      });
      mIiopVerLabel.setHorizontalAlignment(SwingConstants.RIGHT);
      mIiopVerLabel.setLabelFor(mIiopVerField);
      mIiopVerLabel.setText("IIOP Version");

      mWinSizeLabel.setHorizontalAlignment(SwingConstants.RIGHT);
      mWinSizeLabel.setText("Window Size");
      mWinSizePanel.setLayout(mWinSizeLayout);
      mWinSizePanel.setRequestFocusEnabled(true);
      mWinSizeLayout.setColumns(2);
      mWinSizeLayout.setRows(2);
      mWindowWidthLabel.setBorder(BorderFactory.createEtchedBorder());
      mWindowWidthLabel.setHorizontalAlignment(SwingConstants.CENTER);
      mWindowWidthLabel.setLabelFor(mWindowWidthField);
      mWindowWidthLabel.setText("Width");
      mWindowHeightLabel.setBorder(BorderFactory.createEtchedBorder());
      mWindowHeightLabel.setHorizontalAlignment(SwingConstants.CENTER);
      mWindowHeightLabel.setLabelFor(mWindowHeightField);
      mWindowHeightLabel.setText("Height");
      mWindowWidthField.setText("");
      mWindowWidthField.setHorizontalAlignment(SwingConstants.RIGHT);
      mWindowWidthField.addFocusListener(new java.awt.event.FocusAdapter()
      {
         public void focusLost(FocusEvent e)
         {
            windowWidthFieldChanged();
         }
      });
      mWindowWidthField.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            windowWidthFieldChanged();
         }
      });
      mWindowHeightField.setText("");
      mWindowHeightField.setHorizontalAlignment(SwingConstants.RIGHT);
      mWindowHeightField.addFocusListener(new java.awt.event.FocusAdapter()
      {
         public void focusLost(FocusEvent e)
         {
            windowHeightFieldChanged();
         }
      });
      mWindowHeightField.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            windowHeightFieldChanged();
         }
      });
      mButtonPanel.add(mOkButton, null);
      mButtonPanel.add(mApplyButton, null);
      mButtonPanel.add(mCancelButton, null);
      this.getContentPane().add(mContentPane,  BorderLayout.CENTER);
      mContentPane.add(mGeneralPanel,   "General");
      mGeneralPanel.add(mGenConfigPanel, BorderLayout.CENTER);
      mGenConfigPanel.add(mLevelLabel,  new GridBagConstraints(0, 1, 1, 1, 0.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(0, 0, 0, 3), 95, 23));
      mGenConfigPanel.add(mLevelBox,      new GridBagConstraints(1, 1, 1, 1, 1.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 0, 0));
      mGenConfigPanel.add(mLafLabel,  new GridBagConstraints(0, 2, 1, 1, 0.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(0, 0, 0, 3), 95, 23));
      mGenConfigPanel.add(mLafBox,    new GridBagConstraints(1, 2, 1, 1, 1.0, 0.0
            ,GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL, new Insets(0, 0, 0, 0), 0, 0));
      mGenConfigPanel.add(mViewerLabel,  new GridBagConstraints(0, 3, 1, 1, 0.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(0, 0, 0, 3), 95, 23));
      mGenConfigPanel.add(mViewerBox,   new GridBagConstraints(1, 3, 1, 1, 1.0, 0.0
            ,GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL, new Insets(0, 0, 0, 0), 64, 0));
      mGenConfigPanel.add(mLazyInstanceButton,  new GridBagConstraints(1, 0, 1, 1, 0.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 0, 0));
      mGenConfigPanel.add(mWinSizeLabel,    new GridBagConstraints(0, 4, 1, 1, 0.0, 0.0
            ,GridBagConstraints.EAST, GridBagConstraints.NONE, new Insets(0, 0, 0, 3), 0, 23));
      mGenConfigPanel.add(mWinSizePanel, new GridBagConstraints(1, 4, 1, 1, 0.0, 0.0
            ,GridBagConstraints.CENTER, GridBagConstraints.BOTH, new Insets(0, 0, 0, 0), 0, 0));
      mWinSizePanel.add(mWindowWidthLabel, null);
      mWinSizePanel.add(mWindowHeightLabel, null);
      mWinSizePanel.add(mWindowWidthField, null);
      mWinSizePanel.add(mWindowHeightField, null);
      mContentPane.add(mCorbaPanel,   "CORBA");
      mCorbaPanel.add(mCorbaHostLabel,              new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0
            ,GridBagConstraints.EAST, GridBagConstraints.NONE, new Insets(0, 0, 0, 2), 40, 0));
      mCorbaPanel.add(mCorbaHostField,              new GridBagConstraints(1, 0, 1, 1, 1.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, new Insets(0, 0, 0, 4), 0, 0));
      mCorbaPanel.add(mCorbaPortLabel,                   new GridBagConstraints(0, 1, 1, 1, 0.0, 0.0
            ,GridBagConstraints.EAST, GridBagConstraints.NONE, new Insets(3, 0, 0, 2), 0, 0));
      mCorbaPanel.add(mCorbaPortField,                   new GridBagConstraints(1, 1, 1, 1, 1.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(3, 0, 0, 0), 0, 0));
      mCorbaPanel.add(mIiopVerLabel,    new GridBagConstraints(0, 2, 1, 1, 0.0, 0.0
            ,GridBagConstraints.EAST, GridBagConstraints.NONE, new Insets(3, 0, 0, 2), 0, 0));
      mCorbaPanel.add(mIiopVerField,    new GridBagConstraints(1, 2, 1, 1, 1.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(3, 0, 0, 0), 0, 0));

      this.getContentPane().add(mButtonPanel, BorderLayout.SOUTH);
      mFileChooserPanel.add(mFcConfigPanel, BorderLayout.CENTER);
      mFcConfigPanel.add(mFcStartDirLabel,       new GridBagConstraints(0, 0, 1, 1, 0.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 95, 23));
      mFcConfigPanel.add(mFcStartDirBox,        new GridBagConstraints(1, 0, 1, 1, 1.0, 0.0
            ,GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL, new Insets(0, 0, 0, 0), 64, 14));
      mContentPane.add(mFileChooserPanel,  "File Chooser");
   }

   /**
    * Adds items to all the combo boxes.  In some cases, other modifications
    * to a combo box will be made.  These can include setting the default
    * selected value and/or adding action listeners.
    */
   private void configComboBoxes ()
   {
      // ----------------------------------------------------------------------
      // Add all the known Bean viewers to mViewerBox.
      java.util.Vector viewers = mPrefs.getBeanViewers();

      for ( int i = 0; i < viewers.size(); i++ )
      {
         mViewerBox.addItem(viewers.elementAt(i));
      }

      mViewerBox.setSelectedItem(mPrefs.getBeanViewer());
      mViewerBox.addActionListener(new ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               beanViewer = (String) mViewerBox.getSelectedItem();
               fireGlobalPrefsModified(GlobalPrefsUpdateEvent.BEAN_VIEWER);
            }
         });
      // ----------------------------------------------------------------------

      // ----------------------------------------------------------------------
      // Add user level options to mLevelBox.
      for ( int i = GlobalPreferencesService.MIN_USER_LEVEL;
            i <= GlobalPreferencesService.MAX_USER_LEVEL;
            i++ )
      {
         mLevelBox.addItem(String.valueOf(i));
      }

      mLevelBox.setSelectedIndex(mPrefs.getUserLevel() - 1);
      mLevelBox.addActionListener(new ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               userLevel = mLevelBox.getSelectedIndex() + 1;
               fireGlobalPrefsModified(GlobalPrefsUpdateEvent.USER_LEVEL);
            }
         });
      // ----------------------------------------------------------------------

      // ----------------------------------------------------------------------
      // Handle the Look-and-Feel box.
      UIManager.LookAndFeelInfo[] lafs = UIManager.getInstalledLookAndFeels();
      int selected_index = 0;
      String cur_laf = mPrefs.getLookAndFeel();

      // Add the available look-and-feel objects to the combo box.
      for ( int i = 0; i < lafs.length; ++i )
      {
         mLafBox.addItem(lafs[i]);

         if ( lafs[i].getClassName().equals(cur_laf) )
         {
            selected_index = i;
         }
      }

      mLafBox.setSelectedIndex(selected_index);
      mLafBox.setRenderer(new LAFBoxRenderer());
      mLafBox.addActionListener(new ActionListener()
         {
            public void actionPerformed(ActionEvent e)
            {
               UIManager.LookAndFeelInfo val =
                  (UIManager.LookAndFeelInfo) mLafBox.getSelectedItem();
               lookAndFeel = val.getClassName();
               fireGlobalPrefsModified(GlobalPrefsUpdateEvent.LOOK_AND_FEEL);
            }
         });
      // ----------------------------------------------------------------------

      // ----------------------------------------------------------------------
      // Handle the file chooser starting directory box stuff.
      Iterator i = GlobalPreferencesService.getStartDirList().iterator();

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
            public void actionPerformed(ActionEvent e)
            {
               chooserStartDir = (String) mFcStartDirBox.getSelectedItem();
               fireGlobalPrefsModified(GlobalPrefsUpdateEvent.CHOOSER_START_DIR);
            }
         });
      // ----------------------------------------------------------------------
   }

   private void okButtonAction (ActionEvent e)
   {
      status = OK_OPTION;
      commitAndSave();
      dispose();
   }

   private void applyButtonAction(ActionEvent e)
   {
      commitAndSave();
      mApplyButton.setEnabled(false);
   }

   private void cancelButtonAction (ActionEvent e)
   {
      status = CANCEL_OPTION;
      dispose();
   }

   private void commit()
   {
      mPrefs.setUserLevel(userLevel);
      mPrefs.setLookAndFeel(lookAndFeel);
      mPrefs.setBeanViewer(beanViewer);
      mPrefs.setWindowWidth(windowWidth);
      mPrefs.setWindowHeight(windowHeight);
      mPrefs.setChooserStartDir(chooserStartDir);
      mPrefs.setLazyPanelBeanInstantiation(mLazyInstanceButton.isSelected());
      mPrefs.setDefaultCorbaHost(defaultCorbaHost);
      mPrefs.setDefaultCorbaPort(defaultCorbaPort);
      mPrefs.setDefaultIiopVersion(defaultIiopVersion);
   }

   private void commitAndSave()
   {
      commit();
      mPrefs.save();
      fireGlobalPrefsSaved();
   }

   /**
    * Informs any objects implementing GlobalPrefsUpdateListener that one
    * element of the global preferences has been modified.  The element
    * modified is indicated by the given integer argument.
    *
    * @param updateType An identifier that defines which part of the global
    *                   preferences was changed.  This value must come from
    *                   the constants defined in GlobalPrefsUpdateEvent.
    */
   private void fireGlobalPrefsModified(int updateType)
   {
      mApplyButton.setEnabled(true);
      Vector listeners = null;

      synchronized (this)
      {
         listeners = (Vector) mUpdateListeners.clone();
      }

      GlobalPrefsUpdateEvent e = new GlobalPrefsUpdateEvent(this, updateType);
      GlobalPrefsUpdateListener l;

      for ( Iterator i = listeners.iterator(); i.hasNext(); )
      {
         l = (GlobalPrefsUpdateListener) i.next();
         l.globalPrefsModified(e);
      }
   }

   private void fireGlobalPrefsSaved()
   {
      Vector listeners = null;

      synchronized (this)
      {
         listeners = (Vector) mUpdateListeners.clone();
      }

      GlobalPrefsUpdateEvent e = new GlobalPrefsUpdateEvent(this);
      GlobalPrefsUpdateListener l;

      for ( Iterator i = listeners.iterator(); i.hasNext(); )
      {
         l = (GlobalPrefsUpdateListener) i.next();
         l.globalPrefsSaved(e);
      }
   }

   /**
    * Action taken when the user changes the text field containing the default
    * CORBA port.  This validates the entered hostname.
    */
   private void corbaHostFieldChanged()
   {
      defaultCorbaHost = mCorbaHostField.getText();
      fireGlobalPrefsModified(GlobalPrefsUpdateEvent.DEFAULT_CORBA_HOST);
   }

   /**
    * Action taken when the user changes the text field containing the default
    * CORBA port.  This validates the entered port number.
    */
   private void corbaPortFieldChanged()
   {
      try
      {
         int port = Integer.parseInt(mCorbaPortField.getText());

         if ( port > 0 && port < 65536 )
         {
            defaultCorbaPort = port;
            fireGlobalPrefsModified(GlobalPrefsUpdateEvent.DEFAULT_CORBA_PORT);
         }
         else
         {
            mCorbaPortField.setText(String.valueOf(defaultCorbaPort));
         }
      }
      catch (Exception ex)
      {
         mCorbaPortField.setText(String.valueOf(defaultCorbaPort));
      }
   }

   /**
    * Action taken when the user changes the text field containing the default
    * IIOP version number.
    */
   private void iiopVersionFieldChanged()
   {
      defaultIiopVersion = mIiopVerField.getText();
      fireGlobalPrefsModified(GlobalPrefsUpdateEvent.DEFAULT_IIOP_VERSION);
   }

   /**
    * Action taken when the user changes the text field containing the Tweek
    * window width.
    */
   private void windowWidthFieldChanged()
   {
      int old_width = windowWidth;
      windowWidth = Integer.parseInt(mWindowWidthField.getText());

      if ( old_width != windowWidth )
      {
         fireGlobalPrefsModified(GlobalPrefsUpdateEvent.WINDOW_SIZE);
      }
   }

   /**
    * Action taken when the user changes the text field containing the Tweek
    * window height.
    */
   private void windowHeightFieldChanged()
   {
      int old_height = windowHeight;
      windowHeight = Integer.parseInt(mWindowHeightField.getText());

      if ( old_height != windowHeight )
      {
         fireGlobalPrefsModified(GlobalPrefsUpdateEvent.WINDOW_SIZE);
      }
   }

   private static class LAFBoxRenderer extends JLabel
                                       implements ListCellRenderer
   {
      public LAFBoxRenderer()
      {
         setOpaque(true);
      }

      public Component getListCellRendererComponent(JList list, Object value,
                                                    int index,
                                                    boolean isSelected,
                                                    boolean cellHasFocus)
      {
         if ( isSelected )
         {
            setBackground(list.getSelectionBackground());
            setForeground(list.getSelectionForeground());
         }
         else
         {
            setBackground(list.getBackground());
            setForeground(list.getForeground());
         }

         UIManager.LookAndFeelInfo laf = (UIManager.LookAndFeelInfo) value;
         setText(laf.getName());
         return this;
      }
   }

   // =========================================================================
   // Private data members.
   // =========================================================================

   private int status;

   private int     userLevel          = 0;
   private String  lookAndFeel        = null;
   private String  beanViewer         = null;
   private int     windowWidth        = 1024;
   private int     windowHeight       = 768;
   private String  chooserStartDir    = GlobalPreferencesService.DEFAULT_START;
   private String  defaultCorbaHost   = "";
   private int     defaultCorbaPort   = 0;
   private String  defaultIiopVersion = "";

   private GlobalPreferencesService mPrefs = null;

   private Vector mUpdateListeners = new Vector();

   private JPanel       mFileChooserPanel = new JPanel();
   private BorderLayout mFileChooserLayout = new BorderLayout();

   private JPanel        mFcConfigPanel  = new JPanel();
   private GridBagLayout mFcConfigLayout = new GridBagLayout();

   private JLabel       mFcStartDirLabel         = new JLabel();
   private JComboBox    mFcStartDirBox           = new JComboBox();
   private ButtonGroup  mOpenStyleButtonGroup    = new ButtonGroup();

   private JPanel  mButtonPanel  = new JPanel();
   private JButton mCancelButton = new JButton();
   private JButton mApplyButton  = new JButton();
   private JButton mOkButton     = new JButton();

   private JTabbedPane mContentPane = new JTabbedPane();
   private JCheckBox mLazyInstanceButton = new JCheckBox();
   private JComboBox mLevelBox = new JComboBox();
   private JComboBox mViewerBox = new JComboBox();
   private JPanel mGeneralPanel = new JPanel();
   private JLabel mLevelLabel = new JLabel();
   private BorderLayout mGenLayout = new BorderLayout();
   private JLabel mViewerLabel = new JLabel();
   private GridBagLayout mGenConfigLayout = new GridBagLayout();
   private JPanel mGenConfigPanel = new JPanel();
   private JComboBox mLafBox = new JComboBox();
   private JLabel mLafLabel = new JLabel();
   private JTextField mCorbaPortField = new JTextField();
   private JLabel mCorbaPortLabel = new JLabel();
   private JTextField mIiopVerField = new JTextField();
   private JLabel mIiopVerLabel = new JLabel();
   private JPanel mCorbaPanel = new JPanel();
   private JTextField mCorbaHostField = new JTextField();
   private JLabel mCorbaHostLabel = new JLabel();
   private GridBagLayout mCorbaLayout = new GridBagLayout();
   private JLabel mWinSizeLabel = new JLabel();
   private JPanel mWinSizePanel = new JPanel();
   private GridLayout mWinSizeLayout = new GridLayout();
   private JLabel mWindowWidthLabel = new JLabel();
   private JTextField mWindowWidthField = new JTextField();
   private JLabel mWindowHeightLabel = new JLabel();
   private JTextField mWindowHeightField = new JTextField();
}
