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

package org.vrjuggler.vrjconfig.controlpanel;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Font;
import java.awt.Frame;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.util.Stack;
import javax.swing.*;
import javax.swing.event.UndoableEditEvent;
import javax.swing.event.UndoableEditListener;

import org.vrjuggler.tweek.TweekCore;
import org.vrjuggler.tweek.beans.FileLoader;
import org.vrjuggler.tweek.beans.UndoHandler;
import org.vrjuggler.tweek.event.*;
import org.vrjuggler.tweek.services.GlobalPreferencesServiceProxy;
import org.vrjuggler.tweek.services.GlobalPreferencesService;
import org.vrjuggler.tweek.text.MessageDocument;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.rtrc.*;

import org.vrjuggler.vrjconfig.ui.ConnectionDialog;
import org.vrjuggler.vrjconfig.ui.ContextToolbar;


/**
 */
public class ControlPanelToolbar
   extends ContextToolbar
{
   public static final String NEW_ACTION      = "New";
   public static final String OPEN_ACTION     = "Open";
   public static final String RTRC_ACTION     = "RTRC";
   public static final String SAVE_ACTION     = "Save";
   public static final String SAVE_AS_ACTION  = "SaveAs";

   private static final String SAVED_STATE_TEXT   = "";
   private static final String UNSAVED_STATE_TEXT = "Unsaved Changes";

   public ControlPanelToolbar(ConfigContext ctx, FileLoader fileLoader,
                              UndoHandler undoHandler)
   {
      super(ctx, fileLoader, undoHandler);

      saveCurrentEditPoint();

      try
      {
         GlobalPreferencesService prefs = new GlobalPreferencesServiceProxy();

         // Using the global user preferences from Tweek, set the start
         // directory for fileChooser.
         File f = new File(prefs.getChooserStartDir());
         fileChooser.setCurrentDirectory(f);
      }
      catch(Exception ex)
      {
         MessageDocument doc = TweekCore.instance().getMessageDocument();
         doc.printWarningnl("[ControlPanelToolbar()] WARNING: Failed to set " +
                            "file chooser start directory: " +
                            ex.getMessage());
      }

      try
      {
         jbInit();
      }
      catch (Exception e)
      {
         e.printStackTrace();
      }

      // Try to get icons for the toolbar buttons
      try
      {
         ClassLoader loader = getClass().getClassLoader();
         String img_root = "org/vrjuggler/vrjconfig/images";
         rtrcBtn.setIcon(new ImageIcon(loader.getResource(img_root +
                                                          "/vrjuggler.gif")));
      }
      catch (Exception e)
      {
         // Ack! No icons. Use text labels instead
         rtrcBtn.setText("RTRC");
      }

      toolbar.add(newBtn, null);
      toolbar.add(openBtn, null);
      toolbar.add(rtrcBtn, null);
      toolbar.add(saveBtn, null);
      toolbar.add(saveAsBtn, null);
      toolbar.addSeparator();
      toolbar.add(undoBtn, null);
      toolbar.add(redoBtn, null);
      toolbar.addSeparator();
      toolbar.add(Box.createHorizontalGlue(), null);
      toolbar.add(expandBtn, null);
   }

   public void undoableEditHappened(UndoableEditEvent e)
   {
      super.undoableEditHappened(e);
      mRedoSavePoints.clear();
   }

   public void setRemoteReconfigEnabled(boolean enabled)
   {
      rtrcBtn.setEnabled(enabled);
   }

   public boolean doSave()
   {
      boolean saved = super.doSave();
      stateLabel.setText(saved ? SAVED_STATE_TEXT : UNSAVED_STATE_TEXT);

      if ( saved )
      {
         resetUndoRedoStacks();
      }

      return saved;
   }

   public boolean doSaveAs()
   {
      boolean saved = super.doSaveAs();
      stateLabel.setText(saved ? SAVED_STATE_TEXT : UNSAVED_STATE_TEXT);

      if ( saved )
      {
         resetUndoRedoStacks();
      }

      return saved;
   }

   public boolean doRTRC()
   {
      try
      {
         ConfigBroker broker = getBroker();
         RTRCDataSourceBroker RTRCBroker = new RTRCDataSourceBrokerProxy();
         ConnectionDialog dialog =
            new ConnectionDialog((Frame) getParentFrame(),
                                 "Remote Run-Time Reconfiguration Connections");
         dialog.setVisible(true);

         if ( dialog.getStatus() == ConnectionDialog.OK_OPTION )
         {
            RTRCDataSource data_src = dialog.getDataSource();

            broker.add(data_src.toString(), data_src);
            this.context.add(data_src.toString());

            fireAction(RTRC_ACTION);

            return true;
         }
      }
      catch (Exception ioe)
      {
         JOptionPane.showMessageDialog(getParentFrame(), ioe.getMessage(),
                                       "Error", JOptionPane.ERROR_MESSAGE);
         ioe.printStackTrace();
      }

      return false;
   }

   /**
    * Performs a multiple undo operation that stops at the current value on
    * top of the undo save point stack.
    *
    * @see #addSavePoint(int)
    * @see #saveCurrentEditPoint()
    */
   public void doUndo()
   {
      Integer next_save_point = (Integer) mUndoSavePoints.pop();
      doUndoUntil(next_save_point.intValue());
   }

   /**
    * Performs a multiple undo operation until the ConfigUndoManager for our
    * config context reaches the given save point.  The given save point is
    * pushed onto the redo save point stack for later use.
    *
    * @param savePoint  the edit offset point in the ConfigUndoManager at
    *                   which the undo operations will stop
    */
   public void doUndoUntil(int savePoint)
   {
      System.out.println("[ControlPanelToolbar.doUndoUntil()] " +
                         "Undoing until " + savePoint);
      ConfigUndoManager mgr = context.getConfigUndoManager();
      mgr.undoUntil(savePoint);
      mRedoSavePoints.push(new Integer(savePoint));
      undoBtn.setEnabled(mgr.canUndo());
      redoBtn.setEnabled(true);
      saveBtn.setEnabled(context.getConfigUndoManager().getUnsavedChanges());
   }

   /**
    * Performs a multiple redo operation that stops at the current value on
    * top of the redo save point stack.
    */
   public void doRedo()
   {
      Integer next_save_point = (Integer) mRedoSavePoints.pop();
      doRedoUntil(next_save_point.intValue());
   }

   /**
    * Performs a multiple redo operation until the ConfigUndoManager for our
    * config context reaches the given save point.  The given save point is
    * pushed onto the undo save point stack for later use.
    *
    * @param savePoint  the edit offset point in the ConfigUndoManager at
    *                   which the redo operations will stop
    */
   public void doRedoUntil(int savePoint)
   {
      System.out.println("[ControlPanelToolbar.doRedoUntil()] " +
                         "Re-doing until " + savePoint);
      ConfigUndoManager mgr = context.getConfigUndoManager();
      mgr.redoUntil(savePoint);
      mUndoSavePoints.push(new Integer(savePoint));
      undoBtn.setEnabled(true);
      redoBtn.setEnabled(mgr.canRedo());
   }

   /**
    * Adds the given save point to the undo save point stack.  After this
    * operation, any undoable edit events that occur can be undone with a
    * single click to the given save point.
    *
    * @param savePoint  the marker for the current state of the
    *                   ConfigUndoManager for this context
    *
    * @see #saveCurrentEditPoint()
    */
   public void addSavePoint(int savePoint)
   {
      System.out.println("[ControlPanelToolbar.addSavePoint()] " +
                         "Adding save point at " + savePoint);
      mUndoSavePoints.push(new Integer(savePoint));
   }

   /**
    * Adds the current edit-offset-from-save value in the ConfigUndoManager
    * for our context to the undo stack.  This is a helper function that
    * wraps addSavePoint().
    *
    * @see #addSavePoint(int)
    */
   public void saveCurrentEditPoint()
   {
      addSavePoint(context.getConfigUndoManager().getEditOffsetFromSave());
   }

   private void resetUndoRedoStacks()
   {
      mUndoSavePoints.clear();
      mUndoSavePoints.push(new Integer(0));
      mRedoSavePoints.clear();
   }

   /**
    * JBuilder auto-generated GUI code.
    */
   private void jbInit()
      throws Exception
   {
      titlePanel.setLayout(new BorderLayout());
      titlePanel.setBorder(BorderFactory.createRaisedBevelBorder());
      titlePanel.setBackground(UIManager.getColor("textHighlight"));
      stateLabel.setBackground(UIManager.getColor("textHighlight"));
      stateLabel.setFont(new Font("Serif", Font.ITALIC, 18));
      stateLabel.setForeground(UIManager.getColor("textHighlightText"));
      stateLabel.setOpaque(true);
      stateLabel.setHorizontalAlignment(SwingConstants.LEFT);
      stateLabel.setText(SAVED_STATE_TEXT);
      titleLabel.setBackground(UIManager.getColor("textHighlight"));
      titleLabel.setFont(new Font("Serif", Font.BOLD, 18));
      titleLabel.setForeground(UIManager.getColor("textHighlightText"));
      titleLabel.setOpaque(true);
      titleLabel.setHorizontalAlignment(SwingConstants.RIGHT);
      titleLabel.setText("VRJConfig Control Panel");
      titlePanel.add(stateLabel, BorderLayout.WEST);
      titlePanel.add(titleLabel, BorderLayout.EAST);
      this.add(titlePanel, BorderLayout.NORTH);
      rtrcBtn.setToolTipText("Acquire the config elements of a running " +
                             "VR Juggler application");
      rtrcBtn.setActionCommand(RTRC_ACTION);
      rtrcBtn.setFocusPainted(false);
      rtrcBtn.addActionListener(new ActionListener()
      {
         public void actionPerformed(ActionEvent evt)
         {
            doRTRC();
         }
      });
   }

   private Stack mUndoSavePoints = new Stack();
   private Stack mRedoSavePoints = new Stack();

   // JBuilder GUI variables
   private JPanel titlePanel = new JPanel();
   private JLabel stateLabel = new JLabel();
   private JLabel titleLabel = new JLabel();
   private JButton rtrcBtn = new JButton();
}
