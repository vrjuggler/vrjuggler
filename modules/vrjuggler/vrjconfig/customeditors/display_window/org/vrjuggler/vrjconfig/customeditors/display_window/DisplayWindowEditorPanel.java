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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.customeditors.display_window;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;
import info.clearthought.layout.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editors.*;


public class DisplayWindowEditorPanel
   extends JPanel
   implements CustomEditor, InternalFrameListener
{
   private static String mBaseResourcePath = "org/vrjuggler/vrjconfig/customeditors/display_window";

   /**
    * For testing purposes only.
    *
    * @param args String[] Ignored arguments.
    */
   public static void main(String[] args)
   {
      JFrame frame = new JFrame();
      frame.getContentPane().add(new DisplayWindowEditorPanel());
      frame.pack();
      frame.show();
   }

   public DisplayWindowEditorPanel()
   {
      // Register ourselves with the CustomEditorRegistry.
      CustomEditorRegistry.registerEditor("display_window",
                                          DisplayWindowEditorPanel.class);

      try
      {
         jbInit();
         mWidthField.setText(String.valueOf(mCurrentResolution.width));
         mHeightField.setText(String.valueOf(mCurrentResolution.height));
      }
      catch (Exception e)
      {
         e.printStackTrace();
      }

      ClassLoader loader = getClass().getClassLoader();
      String root_path = mBaseResourcePath + "/images";

      try
      {
         ImageIcon window_icon =
            new ImageIcon(loader.getResource(root_path + "/vrjuggler-display-window-icon.png"));
         mDisplayCreateButton.setIcon(window_icon);
         ImageIcon surface_viewport_icon =
            new ImageIcon(loader.getResource(root_path + "/vrjuggler-surface-viewport-icon.png"));
         mSurfaceViewportButton.setIcon(surface_viewport_icon);
         ImageIcon sim_viewport_icon =
            new ImageIcon(loader.getResource(root_path + "/vrjuggler-sim-viewport-icon.png"));
         mSimViewportButton.setIcon(sim_viewport_icon);

         int width, height;
         int buffer = 6;
         width  = window_icon.getIconWidth() + buffer;
         height = window_icon.getIconHeight() + buffer;
         mDisplayCreateButton.setMinimumSize(new Dimension(width, height));
         mDisplayCreateButton.setPreferredSize(new Dimension(width, height));

         width  = surface_viewport_icon.getIconWidth() + buffer;
         height = surface_viewport_icon.getIconHeight() + buffer;
         mSurfaceViewportButton.setMinimumSize(new Dimension(width, height));
         mSurfaceViewportButton.setPreferredSize(new Dimension(width, height));

         width  = sim_viewport_icon.getIconWidth() + buffer;
         height = sim_viewport_icon.getIconHeight() + buffer;
         mSimViewportButton.setMinimumSize(new Dimension(width, height));
         mSimViewportButton.setPreferredSize(new Dimension(width, height));
      }
      // If any icon fails to load, we fall back on plain old text for the
      // button labels.
      catch (NullPointerException ex)
      {
         System.err.println("WARNING: Failed to load icons for display window editor");
         if ( ex.getMessage() != null )
         {
            System.err.println(ex.getMessage());
         }

         mDisplayCreateButton.setText("Add Display ...");
         mSurfaceViewportButton.setText("Add Surface Viewport");
         mSimViewportButton.setText("Add Simulator Viewport");
      }

      // Set a background image for the JDesktopPane that will display the
      // representations of display windows.
      try
      {
         mDesktopBackground.setImageFile(loader.getResource(root_path + "/empty-desktop.png"));
         mDesktopBackground.setOpaque(true);
         mDesktopBackground.setBounds(0, 0, 800, 600);
         mDesktopEditor.add(mDesktopBackground, JLayeredPane.FRAME_CONTENT_LAYER);
      }
      catch (NullPointerException ex)
      {
         System.err.println("WARNING: Failed to set up desktop background image");
         if ( ex.getMessage() != null )
         {
            System.err.println(ex.getMessage());
         }
      }
   }

   public void setConfigElement(ConfigElement elt)
   {
      mElement = elt;
      setEditorState();
      addDisplay(elt);
   }

   public void setContext(ConfigContext ctx)
   {
      mContext = ctx;
      setEditorState();

      // If we are acting as a desktop editor, then we need to pull out all
      // the existing display_window config elements.
      if ( mIsDesktopEditor )
      {
         ConfigBrokerProxy broker = new ConfigBrokerProxy();
         java.util.List all_elts = broker.getElements(mContext);

         for ( Iterator i = all_elts.iterator(); i.hasNext(); )
         {
            ConfigElement cur_elt = (ConfigElement) i.next();
            if ( cur_elt.getDefinition().getName().equals("display_window") )
            {
               addDisplay(cur_elt);
            }
         }
      }
   }

   /**
    * A configuration context without a configuration element indicates that
    * we should operate as a desktop editor rather than a single display
    * window editor.
    */
   private void setEditorState()
   {
      mIsDesktopEditor = (mContext != null && mElement == null);

      // The button for creating a new display window can only be active when
      // we are in desktop editor mode.
      mDisplayCreateButton.setEnabled(mIsDesktopEditor);
   }

   public Container getPanel()
   {
      return this;
   }

   public String getTitle()
   {
      return "Display Window Editor";
   }

   public void internalFrameActivated(InternalFrameEvent e)
   {
      setActiveDisplayWindow(e.getInternalFrame());
   }

   public void internalFrameClosed(InternalFrameEvent e)
   {
      DisplayWindowFrame win = (DisplayWindowFrame) e.getInternalFrame();

      // Only change the active window if the internal frame being closed is
      // the same as the active window.
      if ( mActiveWindow == win )
      {
         setActiveDisplayWindow(null);
      }

      ConfigElement elt = win.getConfiguration();
      mContext.remove(elt.getName());
      mAllWindows.remove(win);
   }

   public void internalFrameClosing(InternalFrameEvent e)
   {

   }

   public void internalFrameDeactivated(InternalFrameEvent e)
   {
      setActiveDisplayWindow(null);
   }

   public void internalFrameDeiconified(InternalFrameEvent e)
   {
      /* Nothing to do for this event. */ ;
   }

   public void internalFrameIconified(InternalFrameEvent e)
   {
      /* Nothing to do for this event. */ ;
   }

   public void internalFrameOpened(InternalFrameEvent e)
   {
      // This may not be necessary if internalFrameActivated() is called if a
      // newly opened window is automatically selected.
      if ( e.getInternalFrame().isSelected() )
      {
         if ( e.getInternalFrame() == mActiveWindow )
         {
            System.err.println("WARNING: Opened internal frame is already active");
         }

         setActiveDisplayWindow(e.getInternalFrame());
      }
   }

   private void jbInit() throws Exception
   {
      double button_size[][] =
         {{5, TableLayout.PREFERRED, 5},
          {TableLayout.FILL, TableLayout.PREFERRED, TableLayout.PREFERRED, 10,
           TableLayout.PREFERRED, TableLayout.PREFERRED, 10,
           TableLayout.PREFERRED, TableLayout.PREFERRED, TableLayout.FILL}};
      mButtonPanelLayout = new TableLayout(button_size);

      this.setLayout(mMainLayout);
      mMainEditorPanel.setLayout(mMainEditorLayout);
      mButtonPanel.setLayout(mButtonPanelLayout);
      mDisplayCreateButton.setToolTipText("Add a new display window to the desktop");
      mDisplayCreateButton.addActionListener(new DisplayWindowEditorPanel_mDisplayCreateButton_actionAdapter(this));
      mSurfaceViewportButton.setEnabled(false);
      mSurfaceViewportButton.setToolTipText("Add a new Surface Viewport to the display window ");
      mSimViewportButton.setEnabled(false);
      mSimViewportButton.setToolTipText("Add a new Simulator Viewport to the display window");
      mResolutionLabel.setLabelFor(mWidthField);
      mResolutionLabel.setText("Screen Resolution:");
      mWidthField.setMinimumSize(new Dimension(40, 21));
      mWidthField.setPreferredSize(new Dimension(40, 21));
      mWidthField.setToolTipText("Screen resolution width");
      mWidthField.setHorizontalAlignment(SwingConstants.TRAILING);
      mWidthField.addFocusListener(new DisplayWindowEditorPanel_Resolution_focusAdapter(this));
      mResolutionXLabel.setRequestFocusEnabled(true);
      mResolutionXLabel.setText("\u00D7");
      mHeightField.setMinimumSize(new Dimension(40, 21));
      mHeightField.setPreferredSize(new Dimension(40, 21));
      mHeightField.setToolTipText("Screen resolution height");
      mHeightField.addFocusListener(new DisplayWindowEditorPanel_Resolution_focusAdapter(this));
      mDesktopEditor.setBorder(BorderFactory.createLoweredBevelBorder());
      mDesktopEditor.setMinimumSize(new Dimension(800, 600));
      mDesktopEditor.setPreferredSize(new Dimension(800, 600));
      mDesktopEditor.setToolTipText("Desktop editor: add a new display to begin");
      mDesktopEditor.addMouseListener(new DisplayWindowEditorPanel_mDesktopEditor_mouseAdapter(this));
      mDesktopEditor.addComponentListener(new DisplayWindowEditorPanel_mDesktopEditor_componentAdapter(this));
      mButtonPanel.setBackground(Color.darkGray);
      mButtonPanel.setBorder(BorderFactory.createEtchedBorder());
      mEditorPanel.setLayout(mEditorPanelLayout);
      mDisplayCreateLabel.setFont(new java.awt.Font("Dialog", 0, 11));
      mDisplayCreateLabel.setForeground(Color.white);
      mDisplayCreateLabel.setMaximumSize(new Dimension(64, 28));
      mDisplayCreateLabel.setHorizontalAlignment(SwingConstants.CENTER);
      mDisplayCreateLabel.setLabelFor(mDisplayCreateButton);
      mDisplayCreateLabel.setText("Display");
      mSurfaceViewportLabel.setFont(new java.awt.Font("Dialog", 0, 11));
      mSurfaceViewportLabel.setForeground(Color.white);
      mSurfaceViewportLabel.setMaximumSize(new Dimension(64, 28));
      mSurfaceViewportLabel.setMinimumSize(new Dimension(64, 14));
      mSurfaceViewportLabel.setPreferredSize(new Dimension(64, 28));
      mSurfaceViewportLabel.setHorizontalAlignment(SwingConstants.CENTER);
      mSurfaceViewportLabel.setLabelFor(mSurfaceViewportButton);
      mSurfaceViewportLabel.setText("<html>Surface<br>Viewport</html>");
      mSimViewportLabel.setFont(new java.awt.Font("Dialog", 0, 11));
      mSimViewportLabel.setForeground(Color.white);
      mSimViewportLabel.setMaximumSize(new Dimension(64, 28));
      mSimViewportLabel.setMinimumSize(new Dimension(64, 14));
      mSimViewportLabel.setPreferredSize(new Dimension(64, 28));
      mSimViewportLabel.setHorizontalAlignment(SwingConstants.CENTER);
      mSimViewportLabel.setLabelFor(mSimViewportButton);
      mSimViewportLabel.setText("<html>Simulator<br>Viewport</html>");
      mMainPanel.setLayout(mMainPanelLayout);
      mAddDisplayItem.setToolTipText("Add a new display window to the desktop");
      mAddDisplayItem.setText("Add Display ...");
      mAddDisplayItem.addActionListener(new DisplayWindowEditorPanel_mAddDisplayItem_actionAdapter(this));
      mResolutionPanel.setToolTipText("Set the size of the managed area (display screen resolution)");
      mHelpButton.setEnabled(false);
      mHelpButton.setRequestFocusEnabled(false);
      mHelpButton.setMnemonic('0');
      mHelpButton.setText("Help");
      mCloseButton.setMnemonic('0');
      mCloseButton.setText("Close");
      mMainButtonPanel.setLayout(flowLayout1);
      flowLayout1.setAlignment(FlowLayout.RIGHT);
      this.add(mMainPanel, BorderLayout.CENTER);
      mMainPanel.add(mMainEditorPanel, BorderLayout.CENTER);
      mResolutionPanel.add(mResolutionLabel, null);
      mResolutionPanel.add(mWidthField, null);
      mResolutionPanel.add(mResolutionXLabel, null);
      mResolutionPanel.add(mHeightField, null);
      mMainEditorPanel.add(mEditorPanel, BorderLayout.CENTER);
      mEditorPanel.add(mButtonPanel, BorderLayout.WEST);
      mEditorPanel.add(mDesktopEditor, BorderLayout.CENTER);
      mMainEditorPanel.add(mResolutionPanel, BorderLayout.NORTH);
      mMainPanel.add(mExtraEditorPanel, BorderLayout.SOUTH);
      this.add(mMainButtonPanel,  BorderLayout.SOUTH);
      mMainButtonPanel.add(mCloseButton, null);
      mMainButtonPanel.add(mHelpButton, null);
      mButtonPanel.add(mDisplayCreateButton,
                       new TableLayoutConstraints(1, 1, 1, 1,
                                                  TableLayout.FULL,
                                                  TableLayout.CENTER));
      mButtonPanel.add(mDisplayCreateLabel,
                       new TableLayoutConstraints(1, 2, 1, 2,
                                                  TableLayout.FULL,
                                                  TableLayout.CENTER));
      mButtonPanel.add(mSurfaceViewportButton,
                       new TableLayoutConstraints(1, 4, 1, 4,
                                                  TableLayout.FULL,
                                                  TableLayout.CENTER));
      mButtonPanel.add(mSurfaceViewportLabel,
                       new TableLayoutConstraints(1, 5, 1, 5,
                                                  TableLayout.FULL,
                                                  TableLayout.CENTER));
      mButtonPanel.add(mSimViewportButton,
                       new TableLayoutConstraints(1, 7, 1, 7,
                                                  TableLayout.FULL,
                                                  TableLayout.CENTER));
      mButtonPanel.add(mSimViewportLabel,
                       new TableLayoutConstraints(1, 8, 1, 8,
                                                  TableLayout.FULL,
                                                  TableLayout.CENTER));
      mContextMenu.add(mAddDisplayItem);
   }

   private void setActiveDisplayWindow(JInternalFrame w)
   {
      mActiveWindow = w;
      boolean enabled = (w != null);
      mSimViewportButton.setEnabled(enabled);
      mSurfaceViewportButton.setEnabled(enabled);
   }

   private Dimension getResolution()
   {
      return mCurrentResolution;
   }

   private Frame getOwner()
   {
      return (Frame) SwingUtilities.getRoot(this);
   }

   /**
    * Positions the given Dialog object relative to this window frame.
    */
   private void positionDialog(Dialog dialog, Frame parent)
   {
      Dimension dlg_size   = dialog.getPreferredSize();
      Dimension frame_size = parent.getSize();
      Point loc            = parent.getLocation();

      // Set the location of the dialog so that it is centered with respect
      // to this frame.
      dialog.setLocation((frame_size.width - dlg_size.width) / 2 + loc.x,
                         (frame_size.height - dlg_size.height) / 2 + loc.y);
   }

   void createDisplayClicked(ActionEvent e)
   {
      Frame owner = getOwner();
      DisplayWindowStartDialog dlg =
         new DisplayWindowStartDialog(owner, mCurrentResolution);
      positionDialog(dlg, owner);
      dlg.show();

      if ( dlg.getStatus() == DisplayWindowStartDialog.OK_OPTION )
      {
         ConfigBrokerProxy broker = new ConfigBrokerProxy();
         ConfigDefinition window_def = broker.getRepository().get("display_window");
         ConfigElementFactory factory =
            new ConfigElementFactory(broker.getRepository().getAllLatest());

         // Validate the name chosen by the user to ensure that we do not
         // create conflicting config elements.
         java.util.List all_elts = broker.getElements(mContext);
         String title = checkElementName(dlg.getDisplayWindowTitle(), all_elts);

         // Create a new config element based on the initial information we have.
         ConfigElement elt = factory.create(title, window_def);

         Rectangle bounds  = dlg.getDisplayWindowBounds();
         elt.setProperty("origin", 0, new Integer(bounds.x));
         elt.setProperty("origin", 1, new Integer(bounds.y));
         elt.setProperty("size", 0, new Integer(bounds.width));
         elt.setProperty("size", 1, new Integer(bounds.height));

         ConfigDefinition fb_def = broker.getRepository().get("opengl_frame_buffer_config");
         ConfigElement fb_cfg = factory.create("OpenGL Frame Buffer Configuration",
                                               fb_def);
         fb_cfg.setProperty("visual_id", 0, dlg.getVisualID());
         fb_cfg.setProperty("red_size", 0, dlg.getRedDepth());
         fb_cfg.setProperty("green_size", 0, dlg.getGreenDepth());
         fb_cfg.setProperty("blue_size", 0, dlg.getBlueDepth());
         fb_cfg.setProperty("alpha_size", 0, dlg.getAlphaDepth());
         fb_cfg.setProperty("depth_buffer_size", 0, dlg.getDepthBufferSize());
         fb_cfg.setProperty("fsaa_enable", 0, dlg.getFSAAEnable());
         elt.setProperty("frame_buffer_config", 0, fb_cfg);

         elt.setProperty("stereo", 0, dlg.inStereo());
         elt.setProperty("border", 0, dlg.hasBorder());
         elt.setProperty("act_as_event_source", 0, dlg.isEventSource());

         addDisplay(elt);
      }
   }

   private void addDisplay(ConfigElement elt)
   {
      Dimension desktop_size = mDesktopEditor.getSize();

      if ( desktop_size.width == 0 || desktop_size.height == 0 )
      {
         desktop_size = mDesktopEditor.getPreferredSize();
      }

      JInternalFrame new_frame = new DisplayWindowFrame(elt.getName(),
                                                        getResolution(),
                                                        desktop_size, elt);

      // Attempt to set an icone for the newly created frame.
      try
      {
         ClassLoader loader = getClass().getClassLoader();
         ImageIcon icon =
            new ImageIcon(loader.getResource("org/vrjuggler/vrjconfig/customeditors/display_window/images/vrjuggler.gif"));

         new_frame.setFrameIcon(icon);
      }
      catch(NullPointerException ex)
      {
         /* Ignore the exception. */ ;
      }

      // XXX: How do we handle inactive windows?  Perhaps that could be left to
      // the rendering process for each indvidual window.
      new_frame.setVisible(true);
      new_frame.addInternalFrameListener(this);
      mDesktopEditor.add(new_frame);
      mAllWindows.add(new_frame);
   }

   private String checkElementName(String name, java.util.List allElts)
   {
      for ( Iterator i = allElts.iterator(); i.hasNext(); )
      {
         ConfigElement cur_elt = (ConfigElement) i.next();

         if ( cur_elt.getName().equals(name) )
         {
            String new_name =
               (String) JOptionPane.showInputDialog(this,
                                                    "A config element named '" +
                                                    name +
                                                       "' already exists in this context\n" +
                                                       "Please enter a new name:",
                                                    "Duplicated Element Name",
                                                    JOptionPane.ERROR_MESSAGE);
            return checkElementName(new_name, allElts);
         }
      }

      return name;
   }

   private ConfigElement mElement = null;
   private ConfigContext mContext = null;

   private JDesktopPane mDesktopEditor = new JDesktopPane();
   private DesktopBackgroundImage mDesktopBackground = new DesktopBackgroundImage();
   private JInternalFrame mActiveWindow = null;
   private java.util.List mAllWindows = new ArrayList();
   private boolean mIsDesktopEditor = false;
   private Dimension mCurrentResolution = new Dimension(1280, 1024);

   private JPopupMenu mContextMenu = new JPopupMenu();

   private BorderLayout mMainLayout = new BorderLayout();
   private JPanel mMainPanel = new JPanel();
   private BorderLayout mMainPanelLayout = new BorderLayout();
   private JPanel mMainEditorPanel = new JPanel();
   private BorderLayout mMainEditorLayout = new BorderLayout();
   private JPanel mResolutionPanel = new JPanel();
   private JPanel mEditorPanel = new JPanel();
   private JPanel mButtonPanel = new JPanel();
   private TableLayout mButtonPanelLayout = null;
   private JButton mDisplayCreateButton = new JButton();
   private JLabel mDisplayCreateLabel = new JLabel();
   private JButton mSurfaceViewportButton = new JButton();
   private JLabel mSurfaceViewportLabel = new JLabel();
   private JButton mSimViewportButton = new JButton();
   private JLabel mSimViewportLabel = new JLabel();
   private JLabel mResolutionLabel = new JLabel();
   private JTextField mWidthField = new JTextField();
   private JLabel mResolutionXLabel = new JLabel();
   private JTextField mHeightField = new JTextField();
   private JPanel mExtraEditorPanel = new JPanel();
   private BorderLayout mEditorPanelLayout = new BorderLayout();
   private JMenuItem mAddDisplayItem = new JMenuItem();
   private JPanel mMainButtonPanel = new JPanel();
   private JButton mHelpButton = new JButton();
   private JButton mCloseButton = new JButton();
   private FlowLayout flowLayout1 = new FlowLayout();

   void resolutionChanged(FocusEvent e)
   {
      // XXX: The size and/or aspect ration of mDesktopEditor should probably
      // be updated too.

      int new_width  = Integer.parseInt(mWidthField.getText());
      int new_height = Integer.parseInt(mHeightField.getText());

      if ( new_width != mCurrentResolution.width ||
           new_height != mCurrentResolution.height )
      {
         mCurrentResolution.setSize(new_width, new_height);

         for ( Iterator i = mAllWindows.iterator(); i.hasNext(); )
         {
            DisplayWindowFrame frame = (DisplayWindowFrame) i.next();
            frame.updateResolution(mCurrentResolution);
         }
      }
   }

   void desktopEditorResized(ComponentEvent e)
   {
      for ( Iterator i = mAllWindows.iterator(); i.hasNext(); )
      {
         DisplayWindowFrame frame = (DisplayWindowFrame) i.next();
         frame.updateDesktopSize(mDesktopEditor.getSize());
      }
   }

   void mDesktopEditor_mousePressed(MouseEvent e)
   {
      maybePopUp(e);
   }

   void mDesktopEditor_mouseReleased(MouseEvent e)
   {
      maybePopUp(e);
   }

   private void maybePopUp(MouseEvent e)
   {
      if ( e.isPopupTrigger() )
      {
         mContextMenu.show(e.getComponent(), e.getX(), e.getY());
      }
   }

   void mAddDisplayItem_actionPerformed(ActionEvent e)
   {
      createDisplayClicked(e);
   }

}

class DisplayWindowEditorPanel_mDisplayCreateButton_actionAdapter
   implements java.awt.event.ActionListener
{
   private DisplayWindowEditorPanel adaptee;

   DisplayWindowEditorPanel_mDisplayCreateButton_actionAdapter(DisplayWindowEditorPanel adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.createDisplayClicked(e);
   }
}

class DisplayWindowEditorPanel_Resolution_focusAdapter extends java.awt.event.FocusAdapter
{
   private DisplayWindowEditorPanel adaptee;

   DisplayWindowEditorPanel_Resolution_focusAdapter(DisplayWindowEditorPanel adaptee)
   {
      this.adaptee = adaptee;
   }
   public void focusLost(FocusEvent e)
   {
      adaptee.resolutionChanged(e);
   }
}

class DisplayWindowEditorPanel_mDesktopEditor_componentAdapter extends java.awt.event.ComponentAdapter
{
   private DisplayWindowEditorPanel adaptee;

   DisplayWindowEditorPanel_mDesktopEditor_componentAdapter(DisplayWindowEditorPanel adaptee)
   {
      this.adaptee = adaptee;
   }
   public void componentResized(ComponentEvent e)
   {
      adaptee.desktopEditorResized(e);
   }
}

class DisplayWindowEditorPanel_mDesktopEditor_mouseAdapter extends java.awt.event.MouseAdapter
{
   private DisplayWindowEditorPanel adaptee;

   DisplayWindowEditorPanel_mDesktopEditor_mouseAdapter(DisplayWindowEditorPanel adaptee)
   {
      this.adaptee = adaptee;
   }
   public void mousePressed(MouseEvent e)
   {
      adaptee.mDesktopEditor_mousePressed(e);
   }
   public void mouseReleased(MouseEvent e)
   {
      adaptee.mDesktopEditor_mouseReleased(e);
   }
}

class DisplayWindowEditorPanel_mAddDisplayItem_actionAdapter implements java.awt.event.ActionListener
{
   private DisplayWindowEditorPanel adaptee;

   DisplayWindowEditorPanel_mAddDisplayItem_actionAdapter(DisplayWindowEditorPanel adaptee)
   {
      this.adaptee = adaptee;
   }
   public void actionPerformed(ActionEvent e)
   {
      adaptee.mAddDisplayItem_actionPerformed(e);
   }
}
